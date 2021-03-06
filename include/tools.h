/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#ifndef __TOOLS_H
#define __TOOLS_H
void RandInit(void);
float RandomWithin(float small,float big);
#if defined(TRACE)
#include "diag/Trace.h"
int LOG(const char* format, ...);
#else
#define LOG(...)
#endif

int atollmod(const char* pstr,int mod);

#endif
