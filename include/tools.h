#ifndef __TOOLS_H
#define __TOOLS_H
#include "diag/Trace.h"
#include "millis.h"

void RandInit(void);
float RandomWithin(float small,float big);
#if defined(TRACE)
int LOG(const char* format, ...);
#else
#define LOG(x)
#define	LOG_S(x)
#endif
#endif
