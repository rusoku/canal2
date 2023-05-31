//https://learn.microsoft.com/en-us/cpp/build/exporting-from-a-dll-using-declspec-dllexport?view=msvc-170
//https://learn.microsoft.com/en-us/cpp/build/exporting-functions-from-a-dll-by-ordinal-rather-than-by-name?view=msvc-170
//https://learn.microsoft.com/en-us/cpp/build/linking-an-executable-to-a-dll?view=msvc-170#determining-which-linking-method-to-use
//https://learn.microsoft.com/en-us/cpp/build/run-time-library-behavior?view=msvc-170#initializing-a-dll
//https://stackoverflow.com/questions/39330898/c-winapi-call-exported-function-via-getprocaddress

#ifndef CANAL2_H
#define CANAL2_H

#include <windows.h>

#define DllExport __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

DllExport char* WINAPI DriverName(void);
//DllExport char* DriverVersion(void);
//DllExport char* ManufacturerName(void);
//DllExport int OpenAdapter(void);
//DllExport int CloseAdapter(void);
//DllExport int ReadAdapter(byte* frame);
//DllExport int WriteAdapter(const unsigned int id, const int dataLength, byte* data);

#ifdef __cplusplus
}
#endif

//DWORD WINAPI ReadThread(LPVOID lParam);

#endif //CANAL2_H
