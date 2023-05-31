#include <windows.h>
#include <stdio.h>
#include "CAN_interface_list.h"

struct CAN_DEV_LIST canDeviceList = {0};

int main()
{
    CAN_interface_list(&canDeviceList);

    printf("\nTotal devices found: = %d\n", canDeviceList.canDevCount);
    printf("----------------------------------\n");

    for(int x = 0; x < canDeviceList.canDevCount; x++ )
    {
        printf("Serial = %s\n", canDeviceList.canDevInfo[x].SerialNumber);
        printf("VID = 0x%04X, PID = 0x%04X\n", canDeviceList.canDevInfo[x].vid, canDeviceList.canDevInfo[x].pid);
        printf("UUID = %s\n", canDeviceList.canDevInfo[x].uuid);
        printf("DeviceType = %s\n", canDeviceList.canDevInfo[x].DeviceType);
        printf("\n");
    }
}
