#include "canal2.h"
#include <stdio.h>
#include "CAN_interface_list.h"
#include "debug.h"

//https://learn.microsoft.com/en-us/windows/win32/Dlls/dynamic-link-library-best-practices

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            DebugPrintf("TouCAN DLL Process Attach\n");
        case DLL_THREAD_ATTACH:
            DebugPrintf("TouCAN DLL Thread Attach\n");
        case DLL_THREAD_DETACH:
            DebugPrintf("TouCAN DLL Thread Detach\n");
        case DLL_PROCESS_DETACH:
            DebugPrintf("TouCAN DLL Process Detach\n");
            break;
    }
    return TRUE;
}

DllExport char* WINAPI DriverName(void) {
    DebugPrintf("TouCAN DriverName\n");
    return (char*)"TouCAN Marine";
}

