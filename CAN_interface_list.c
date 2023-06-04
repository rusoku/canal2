/*
 * CANAL interface DLL for RUSOKU technologies for TouCAN, TouCAN Marine, TouCAN Duo USB to CAN bus converter
 *
 * Copyright (C) 2005-2023 Gediminas Simanskis (gediminas@rusoku.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3.0 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.
 *
 */

//#include "targetver.h"
//#include <windows.h>

/*
#include <stdlib.h>
#include <malloc.h>
#include "CDllList.h"
#include "CDllDrvObj.h"
#include "CTouCANobj.h"
#include "vscp_common/canal_macro.h"
#include "vscp_common/canal.h"
#include "vscp_common/canal_a.h"
#include <initguid.h>
#include <tchar.h>
#include <strsafe.h>
#include <winusb.h>
#include <usb.h>
#include <cfgmgr32.h>
#include <shlwapi.h>
#include "MyStrings.h"
#include "Threads.h"
*/

#include <windows.h>
#include <strsafe.h>
#include <string.h>
#include <initguid.h>
#include <cfgmgr32.h>
#include <heapapi.h>
#include "include/CAN_interface_list.h"
#include "include/canal.h"

//const GUID GUID_DEVINTERFACE_WinUsbF4FS1 = {0xFD361109, 0x858D, 0x4F6F, 0x81, 0xEE, 0xAA, 0xB5, 0xD6, 0xCB, 0xF0, 0x6B};

//struct CAN_DEV_INFO CanDeviceInfo[TOTAL_DEVICES_AVAILABLE+1] = {0};

CONFIGRET   cr = CR_SUCCESS;
ULONG   device_interface_list_length = 0;
TCHAR*  device_interface_list = NULL;
HANDLE  file_hd = NULL;
HRESULT hr = ERROR_SUCCESS;
size_t  DeviceStrLen = 0;
UINT16  CurrentDeviceIndex = 0;
TCHAR*  pCurrentIterfaceList;

/***
 *
 * @return cr
 */
DWORD CAN_interface_list(struct CAN_DEV_LIST* canDeviceList) {

//    UINT8   canDeviceListIndex = 0;
    TCHAR   *token1 = NULL;
    TCHAR   *next_token1 = NULL;
    TCHAR   *token2 = NULL;
    TCHAR   *next_token2 = NULL;
    TCHAR   tmp_string[64];

    if (canDeviceList == NULL)
        return  CR_INVALID_POINTER;

    cr = CM_Get_Device_Interface_List_Size(&device_interface_list_length,
                                           (LPGUID)&GUID_DEVINTERFACE_WinUsbF4FS1,
                                           NULL,
                                           CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

    if (cr != CR_SUCCESS){
        hr = (HRESULT)CM_MapCrToWin32Err(cr, CR_DEFAULT);
        goto clean0;
    }

    if (device_interface_list_length <= 1)
        goto clean0;

    device_interface_list = (TCHAR*)HeapAlloc(GetProcessHeap(),
                                              HEAP_ZERO_MEMORY,
                                              device_interface_list_length * sizeof(TCHAR));

    if (device_interface_list == NULL) {
        hr = (HRESULT)ERROR_OUTOFMEMORY;
        goto clean0;
    }

    cr = CM_Get_Device_Interface_List((LPGUID)&GUID_DEVINTERFACE_WinUsbF4FS1,
                                      NULL,
                                      device_interface_list,
                                      device_interface_list_length,
                                      CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

    if (cr != CR_SUCCESS){
        hr = (HRESULT)CM_MapCrToWin32Err(cr, CR_DEFAULT);
        goto clean0;
    }

    pCurrentIterfaceList = device_interface_list;
    CurrentDeviceIndex = 0;

    for (UINT16 x = 0; x < TOTAL_DEVICES_AVAILABLE; x++)
    {
        hr = StringCchLengthA(pCurrentIterfaceList, STRSAFE_MAX_LENGTH, &DeviceStrLen);
        if (FAILED(hr) || DeviceStrLen == 0)
            break;

        file_hd = CreateFile(pCurrentIterfaceList,
                             GENERIC_WRITE | GENERIC_READ,
                             FILE_SHARE_WRITE | FILE_SHARE_READ,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                             NULL);

        if (file_hd != INVALID_HANDLE_VALUE)
        {
            /* Device ID */
            token1 = strtok_s(pCurrentIterfaceList,"#", &next_token1);
            //printf("token = %s\n", token1);
            StringCbCopyA(canDeviceList->canDevInfo[CurrentDeviceIndex].DeviceType,
                          sizeof(canDeviceList->canDevInfo[CurrentDeviceIndex].DeviceType)/sizeof(canDeviceList->canDevInfo[CurrentDeviceIndex].DeviceType[0]),
                          token1);

            if(token1 == NULL)
                goto next_device;

            /* VID, PID */
            token1 = strtok_s(NULL,"#", &next_token1);
            //printf("token = %s\n", token1);
            lstrcpynA(tmp_string, token1, sizeof(tmp_string)/sizeof(tmp_string[0]));
            token2 = strtok_s(tmp_string,"&", &next_token2);
                //printf("VID = %s\n", token2);

            canDeviceList->canDevInfo[CurrentDeviceIndex].vid = (UINT16) strtoul(token2+4, NULL, 16);

            if(token2 == NULL)
                goto next_device;;

            token2 = strtok_s(NULL,"&", &next_token2);
                //printf("PID = %s\n", token2);

            canDeviceList->canDevInfo[CurrentDeviceIndex].pid = (UINT16) strtoul(token2+4, NULL, 16);

            if(token1 == NULL)
                goto next_device;;

            /* Serial */
            token1 = strtok_s(NULL,"#", &next_token1);
            //printf("token = %s\n", token1);
            StringCbCopyA(canDeviceList->canDevInfo[CurrentDeviceIndex].SerialNumber,
                          sizeof(canDeviceList->canDevInfo[CurrentDeviceIndex].SerialNumber)/sizeof(canDeviceList->canDevInfo[CurrentDeviceIndex].SerialNumber[0]),
                          token1);

            if(token1 == NULL)
                goto next_device;;

            /* UUID */
            token1 = strtok_s(NULL,"#", &next_token1);
            //printf("token = %s\n", token1);

            StringCbCopyA(canDeviceList->canDevInfo[CurrentDeviceIndex].uuid,
                          sizeof(canDeviceList->canDevInfo[CurrentDeviceIndex].uuid)/sizeof(canDeviceList->canDevInfo[CurrentDeviceIndex].uuid[0]),
                          token1);

            CurrentDeviceIndex++;
        }

        next_device:
        CloseHandle(file_hd);
        pCurrentIterfaceList = DeviceStrLen + pCurrentIterfaceList + sizeof(TCHAR);
    }

    clean0:
    canDeviceList->canDevCount = CurrentDeviceIndex;
    HeapFree(GetProcessHeap(), 0, device_interface_list);
    //printf("TotalDevicesFound = %d\n", CurrentDeviceIndex);

    return (HRESULT)hr;
}
