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

#pragma once

#include <windows.h>

#define TOTAL_DEVICES_AVAILABLE 8

struct CAN_DEV_INFO{
    TCHAR   DeviceType[64];
    TCHAR   uuid[64];
    UINT16  vid;
    UINT16  pid;
    TCHAR   SerialNumber[64];
};

struct CAN_DEV_LIST{
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

#ifdef __cplusplus
extern "C"
{
#endif

DWORD CAN_interface_list(struct CAN_DEV_LIST* canDeviceList);

#ifdef __cplusplus
}
#endif

