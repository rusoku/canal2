//
// Created by gedsi on 03/05/2023.
//

#include <windows.h>
#include <stdio.h>
#include <inttypes.h>
#include <initguid.h>
#include <cfgmgr32.h>
#include <heapapi.h>

/*
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;
*/

//DEFINE_GUID(GUID_INTERFACE_WinUsbF4FS1,
//            0xFD361109, 0x858D, 0x4F6F, 0x81, 0xEE, 0xAA, 0xB5, 0xD6, 0xCB, 0xF0, 0x6B);

struct CAN_DEV_INFO{
    int index;
    UINT16 vid;
    UINT16 pid;
    TCHAR SerialNumber[8];
};

struct CAN_DEV_INFO CanDeviceInfo = {};

const GUID GUID_DEVINTERFACE_WinUsbF4FS1 = {0xFD361109, 0x858D, 0x4F6F, 0x81, 0xEE, 0xAA, 0xB5, 0xD6, 0xCB, 0xF0, 0x6B};

CONFIGRET cr = CR_SUCCESS;
ULONG device_interface_list_length = 0;
TCHAR* device_interface_list = NULL;
//HANDLE file_hd = NULL;
HRESULT hr = ERROR_SUCCESS;
//if (FAILED(hr))

int CAN_interface_list(void) {

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
/*
    file_hd = CreateFile(device_interface_list,
                         GENERIC_WRITE | GENERIC_READ,
                         FILE_SHARE_WRITE | FILE_SHARE_READ,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                         NULL);

    if (file_hd == INVALID_HANDLE_VALUE) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto clean0;
    }
*/
    printf("List size = %lu\n", device_interface_list_length);
    printf("%s\n", device_interface_list);


    clean0:
    //CloseHandle(file_hd);
    HeapFree(GetProcessHeap(), 0, device_interface_list);

    printf("hr = %ld\n", hr);

    return (HRESULT)hr;
}
