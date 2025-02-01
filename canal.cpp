/*
MIT License

Copyright (c) 2000-2023 Åke Hedman, Grodans Paradis AB
Copyright (c) 2005-2025 Gediminas Simanskis, Rusoku technologijos UAB (gediminas@rusoku.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "windows.h"
#include "include/canal.h"
#include "include/canal_macro.h"
#include "include/debug.h"
#include "include/CDllDrvObj.h"
#include "include/CAN_interface_list.h"
#include <strsafe.h>

//https://learn.microsoft.com/en-us/windows/win32/Dlls/dynamic-link-library-best-practices

static CDllDrvObj* theApp;
//CAN_DEV_LIST   CanDevList;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            theApp = new CDllDrvObj();
            theApp->InitInstance();
            break;
        }
        case DLL_THREAD_ATTACH: {
            break;
        }
        case DLL_THREAD_DETACH: {
            break;
        }
        case DLL_PROCESS_DETACH: {
            if (lpReserved != nullptr) {
                break; // do not do cleanup if process termination scenario
            }
            theApp->RemoveAllObjects();
            delete theApp;
            break;
        default:
            break;
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  CanalGetDeviceList
//
int CanalGetDeviceList(pcanal_dev_list canalDeviceList, int canalDeviceListSize){
    if(canalDeviceList == nullptr)
        return  CANAL_ERROR_MEMORY;

    CAN_DEV_LIST canDeviceList = {};
    unsigned int devcnt;

    CAN_interface_list(&canDeviceList);

    devcnt = canDeviceList.canDevCount % CANAL_DEVLIST_SIZE_MAX;
    canalDeviceList->canDevCount = devcnt;

    for(unsigned int x = 0; x < devcnt; x++){
        canalDeviceList->canDevInfo[x].DeviceId = 0;
        canalDeviceList->canDevInfo[x].pid = canDeviceList.canDevInfo[x].pid;
        canalDeviceList->canDevInfo[x].vid = canDeviceList.canDevInfo[x].vid;
        StringCbCopyA(canalDeviceList->canDevInfo[x].SerialNumber,
                      sizeof(canalDeviceList->canDevInfo[x].SerialNumber)/sizeof(canalDeviceList->canDevInfo[x].SerialNumber[0]),
                      canDeviceList.canDevInfo[x].SerialNumber);
    }
    return  CANAL_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//  CanalOpen
//
long CanalOpen(const char *pDevice, unsigned long flags)
{
    long h = 0;
    auto pDrvObj = new CTouCANObj();

        if((h = theApp->AddDriverObject(pDrvObj)) <= 0) {
            h = 0;
            delete pDrvObj;
        } else {
            if (pDrvObj->Open(pDevice, flags, TRUE) == FALSE) {
                theApp->RemoveDriverObject(h);
                h = 0;
            }
        }
    return h;
}

///////////////////////////////////////////////////////////////////////////////
//  CanalClose
//

long CanalClose(long handle)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);

    if (nullptr == pDrvObj)
        return CANAL_ERROR_MEMORY;

    pDrvObj->Close();
    theApp->RemoveDriverObject(handle);

    return CANAL_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//  CanalGetLevel
//

long CanalGetLevel(long handle)
{
    return CANAL_LEVEL_STANDARD;
}


///////////////////////////////////////////////////////////////////////////////
// CanalSend
//

int CanalSend(long handle, PCANALMSG pCanalMsg)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->WriteMsg(pCanalMsg));
}


///////////////////////////////////////////////////////////////////////////////
// CanalSend blocking
//

int CanalBlockingSend(long handle, PCANALMSG pCanalMsg, unsigned long timeout)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->WriteMsgBlocking(pCanalMsg, timeout));
}

///////////////////////////////////////////////////////////////////////////////
// CanalReceive
//

int CanalReceive(long handle, PCANALMSG pCanalMsg)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->ReadMsg(pCanalMsg));
}

///////////////////////////////////////////////////////////////////////////////
// CanalReceive blocking
//

int CanalBlockingReceive(long handle, PCANALMSG pCanalMsg, unsigned long timeout)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->ReadMsgBlocking(pCanalMsg, timeout));
}

///////////////////////////////////////////////////////////////////////////////
// CanalDataAvailable
//

int CanalDataAvailable(long handle)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return pDrvObj->DataAvailable();
}

///////////////////////////////////////////////////////////////////////////////
// CanalGetStatus
//
int CanalGetStatus(long handle, PCANALSTATUS pCanalStatus)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);

    if (nullptr == pDrvObj)
        return CANAL_ERROR_MEMORY;

    return (pDrvObj->GetStatus(pCanalStatus));
}

///////////////////////////////////////////////////////////////////////////////
// CanalGetStatistics
//
int CanalGetStatistics(long handle, PCANALSTATISTICS pCanalStatistics)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);

    if (nullptr == pDrvObj)
        return CANAL_ERROR_MEMORY;

    return (pDrvObj->GetStatistics(pCanalStatistics));
}

///////////////////////////////////////////////////////////////////////////////
// CanalSetFilter
//
int CanalSetFilter(long handle, unsigned long filter)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->SetFilter(filter) ? CANAL_ERROR_SUCCESS : CANAL_ERROR_GENERIC);
}

///////////////////////////////////////////////////////////////////////////////
// CanalSetMask
//
int CanalSetMask(long handle, unsigned long mask)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->SetMask(mask) ? CANAL_ERROR_SUCCESS : CANAL_ERROR_GENERIC);
}

///////////////////////////////////////////////////////////////////////////////
// CanalSetBaudrate
//
int CanalSetBaudrate(long handle, unsigned long baudrate)
{
    // Not supported in this DLL
    return CANAL_ERROR_NOT_SUPPORTED;
}

///////////////////////////////////////////////////////////////////////////////
// CanalGetVersion
//
unsigned long CanalGetVersion(void)
{
    unsigned long version;
    unsigned char* p = (unsigned char *)&version;

    *p = CANAL_MAIN_VERSION;
    *(p + 1) = CANAL_MINOR_VERSION;
    *(p + 2) = CANAL_SUB_VERSION;
    *(p + 3) = 0;
    return version;
}

///////////////////////////////////////////////////////////////////////////////
// CanalGetDllVersion
//
unsigned long CanalGetDllVersion(void)
{
    unsigned long version;
    unsigned char *p = (unsigned char *)&version;

    *p = DLL_MAIN_VERSION;
    *(p + 1) = DLL_MINOR_VERSION;
    *(p + 2) = DLL_SUB_VERSION;
    *(p + 3) = 0;
    return version;
}

///////////////////////////////////////////////////////////////////////////////
// CanalGetVendorString
//
const char* CanalGetVendorString(void)
{
    static char r_str[256];

    CTouCANObj *pDrvObj = theApp->GetDriverObject(1681);

    if (nullptr == pDrvObj)
        return nullptr;

    strcpy_s(r_str, 256, pDrvObj->GetVendorString());

    return r_str;
}

///////////////////////////////////////////////////////////////////////////////
//Get CANAL driver properties
//
const char* CanalGetDriverInfo(void)
{
    return "\n";
}

//---------------------------- A type CANAL API list ------------------------------

/*!
	Set the 11bit filter  ID, Mask for a CANAL channel

	@param handle Handle to open physical CANAL channel.
	@return zero on success or error-code on failure.
*/
int CanalSetFilter11bit(long handle, Filter_Type_TypeDef type, unsigned long list, unsigned long mask)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->SetFilter11bit(type, list, mask));
}

/*!
	Set the 29bit filter  ID, Mask for a CANAL channel

	@param handle Handle to open physical CANAL channel.
	@return zero on success or error-code on failure.
*/
int CanalSetFilter29bit(long handle, Filter_Type_TypeDef type, unsigned long list, unsigned long mask)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->SetFilter29bit(type, list, mask)); // ? CANAL_ERROR_SUCCESS : CANAL_ERROR_GENERIC);
}

/*!
	Get bootloader ver

	@param handle Handle to open physical CANAL channel.
	@return zero on success or error-code on failure.
*/
int CanalGetBootloaderVersion(long handle, unsigned long *bootloader_version)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->GetBootloaderVersion(bootloader_version));
}

int CanalGetHardwareVersion(long handle, unsigned long *hardware_version)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->GetHardwareVersion(hardware_version));
}

int CanalGetFirmwareVersion(long handle, unsigned long *firmware_version)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->GetFirmwareVersion(firmware_version));
}

int CanalGetSerialNumber(long handle, unsigned long *serial)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->GetSerialNumber(serial));
}

int CanalGetVidPid(long handle, unsigned long *vidpid)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->GetVidPid(vidpid));
}

int CanalGetDeviceId(long handle, unsigned long *deviceid)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return 0;
    return (pDrvObj->GetDeviceId(deviceid));
}

int CanalGetVendor(long handle, unsigned int size, char *vendor)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->GetVendor(size, vendor));
}

int CanalInterfaceStart(long handle)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->InterfaceStart());
}

int CanalInterfaceStop(long handle)
{
    CTouCANObj *pDrvObj = theApp->GetDriverObject(handle);
    if (nullptr == pDrvObj) return CANAL_ERROR_MEMORY;
    return (pDrvObj->InterfaceStop());
}
