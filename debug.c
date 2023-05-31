//
// Created by gedsi on 31/05/2023.
//

#include "debug.h"

void DebugPrintf(char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    char dbg_out[8192];
    vsprintf_s(dbg_out, sizeof(dbg_out)/sizeof(dbg_out[0]), fmt, argp);
    va_end(argp);
    OutputDebugString(dbg_out);
}