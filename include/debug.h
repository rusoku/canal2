
#ifndef CANAL2_DEBUG_H
#define CANAL2_DEBUG_H

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

void DebugPrintf(char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif //CANAL2_DEBUG_H
