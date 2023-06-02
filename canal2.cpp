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

#include "canal2.h"
#include <stdio.h>
#include "CAN_interface_list.h"
#include "debug.h"
#include "CDllDrvObj.h"

//https://learn.microsoft.com/en-us/windows/win32/Dlls/dynamic-link-library-best-practices

static HANDLE hThisInstDll = NULL;
static CDllDrvObj *theApp = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            hThisInstDll = hModule;
            theApp = new CDllDrvObj();
            theApp->InitInstance();
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

