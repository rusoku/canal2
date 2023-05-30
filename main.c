#include <windows.h>
#include <stdio.h>
#include "CAN_interface_list.h"

struct CAN_DEV_LIST canDeviceList = {0};

int main()
{
    CAN_interface_list(&canDeviceList);

    printf("\ncan_dev_info_cnt = %d\n\n", canDeviceList.canDevCount);

    for(int x = 0; x < canDeviceList.canDevCount; x++ )
    {
        printf("serial = %s\n", canDeviceList.canDevInfo[x].SerialNumber);
        printf("vid = 0x%04X, pid = 0x%04X\n", canDeviceList.canDevInfo[x].vid, canDeviceList.canDevInfo[x].pid);
        printf("uuid = %s\n", canDeviceList.canDevInfo[x].uuid);
        printf("DeviceType = %s\n", canDeviceList.canDevInfo[x].DeviceType);
        printf("\n");
    }
}
