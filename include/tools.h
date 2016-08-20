#ifndef __TOOLS_H
#define __TOOLS_H
#include "diag/Trace.h"
void RandInit(void);
float RandomWithin(float small,float big);
#if defined(TRACE)
int LOG(const char* format, ...);
#else
#define LOG(...)
#endif
#endif
