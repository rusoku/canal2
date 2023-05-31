#ifndef CANAL2_H
#define CANAL2_H

#include <windows.h>

#define DllExport __declspec( dllexport )

#ifdef __cplusplus
extern "C" {
#endif

DllExport char* DriverName(void);
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
