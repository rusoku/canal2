#include <windows.h>
#include <stdio.h>
#include "CAN_interface_list.h"

struct CAN_DEV_LIST canDeviceList = {};

/*
struct CAN_DEV_INFO{
    BOOL    valid;
    UINT8   index;
    UINT16  vid;
    UINT16  pid;
    TCHAR   SerialNumber[64];
    UINT32  reserved;
};

struct  CAN_DEV_LIST{
    struct CAN_DEV_INFO canDevInfo[8];
    UINT8   canDevCount;
};
*/

int main() {
    CAN_interface_list(&canDeviceList);

    printf("\ncan_dev_info_cnt = %d\n", canDeviceList.canDevCount);
    printf("vid = %04X, pid = %04X\n", canDeviceList.canDevInfo[0].vid, canDeviceList.canDevInfo[0].pid);
    printf("serial = %s\n", canDeviceList.canDevInfo[0].SerialNumber);
}
