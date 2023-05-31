//
// Created by gedsi on 03/05/2023.
//

#ifndef DEVICEINTERFACELIST_CAN_INTERFACE_LIST_H
#define DEVICEINTERFACELIST_CAN_INTERFACE_LIST_H

#define TOTAL_DEVICES_AVAILABLE 8

struct CAN_DEV_INFO{
    TCHAR   DeviceType[64];
    TCHAR   uuid[64];
    UINT16  vid;
    UINT16  pid;
    TCHAR   SerialNumber[64];
};

struct  CAN_DEV_LIST{
    struct CAN_DEV_INFO canDevInfo[TOTAL_DEVICES_AVAILABLE];
    UINT8   canDevCount;
};


/*
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
*/

//DEFINE_GUID(GUID_INTERFACE_WinUsbF4FS1,
//            0xFD361109, 0x858D, 0x4F6F, 0x81, 0xEE, 0xAA, 0xB5, 0xD6, 0xCB, 0xF0, 0x6B);

extern  DWORD CAN_interface_list(struct CAN_DEV_LIST* canDeviceList);

#endif //DEVICEINTERFACELIST_CAN_INTERFACE_LIST_H
