//
// Created by gedsi on 03/05/2023.
//

#include <windows.h>
#include <stdio.h>
//#include <inttypes.h>
#include <initguid.h>
#include <cfgmgr32.h>
#include <heapapi.h>
#include "CAN_interface_list.h"

const GUID GUID_DEVINTERFACE_WinUsbF4FS1 = {0xFD361109, 0x858D, 0x4F6F, 0x81, 0xEE, 0xAA, 0xB5, 0xD6, 0xCB, 0xF0, 0x6B};

//struct CAN_DEV_INFO CanDeviceInfo[TOTAL_DEVICES_AVAILABLE+1] = {0};

CONFIGRET   cr = CR_SUCCESS;
ULONG   device_interface_list_length = 0;
TCHAR*  device_interface_list = NULL;
HANDLE  file_hd = NULL;
HRESULT hr = ERROR_SUCCESS;
//if (FAILED(hr))

size_t  DeviceStrLen = 0;
UINT16  TotalDevicesFound = 0;
TCHAR*  pCurrentIterfaceList;

/*
https://www.cs.helsinki.fi/group/boi2016/doc/cppreference/reference/en.cppreference.com/w/c/string/byte/strtok.html
*/

/***
 *
 * @return cr
 */
int CAN_interface_list(struct CAN_DEV_LIST* canDeviceList) {

    UINT8   canDeviceListIndex = 0;
    TCHAR   *token = NULL;
    TCHAR   *next_token = NULL;

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

    for(int x = 0; x < TOTAL_DEVICES_AVAILABLE; x++)
    {
        DeviceStrLen = strlen(pCurrentIterfaceList);

        if(DeviceStrLen == 0)
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
            printf("%s\n", pCurrentIterfaceList);
            TotalDevicesFound++;

            //strtok_s(
        }
        CloseHandle(file_hd);

        pCurrentIterfaceList = DeviceStrLen + pCurrentIterfaceList + sizeof(TCHAR);
    }

    clean0:
    HeapFree(GetProcessHeap(), 0, device_interface_list);
    printf("TotalDevicesFound = %d\n", TotalDevicesFound);

    return (HRESULT)hr;
}
