//
// Created by gedsi on 03/05/2023.
//

#ifndef DEVICEINTERFACELIST_CAN_INTERFACE_LIST_H
#define DEVICEINTERFACELIST_CAN_INTERFACE_LIST_H

struct CAN_DEV_INFO{
    BOOL    valid;
    UINT8   index;
    UINT16  vid;
    UINT16  pid;
    TCHAR   SerialNumber[8];
};

#define TOTAL_DEVICES_AVAILABLE 8

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

extern  int CAN_interface_list(void);

#endif //DEVICEINTERFACELIST_CAN_INTERFACE_LIST_H
