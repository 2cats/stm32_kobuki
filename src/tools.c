/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tools.h>
#include "millis.h"
void RandInit(void) {
	srand(millis());
}
float RandomWithin(float small, float big) {
	return rand() * 1.0 / RAND_MAX * (big - small) + small;
}

#if defined(TRACE)
#ifndef OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE
#define OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE (128)
#endif
int LOG(const char* format, ...) {
	int ret;
	va_list ap;

	va_start(ap, format);

	// TODO: rewrite it to no longer use newlib, it is way too heavy

	static char buf[OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE];

	ret = snprintf(buf, sizeof(buf), "[%08lu %s]", millis(), __FILE__);
	trace_write(buf, ret);
	// Print to the local buffer
	ret = vsnprintf(buf, sizeof(buf), format, ap);
	if (ret > 0) {
		// Transfer the buffer to the device
		ret = trace_write(buf, (size_t) ret);
	}
	trace_write("\n", 1);
	va_end(ap);
	return ret;
}
#endif
int atollmod(const char* pstr,int mod) {
	long long ll = 0;
	int Integer_sign = 1;
	if ( NULL == pstr) {
		return 0;
	}
	//check sign
	if (*pstr == '-') {
		pstr++;
		Integer_sign = -1;
	}
	if (*pstr == '+') {
		pstr++;
	}
	while (*pstr >= '0' && *pstr <= '9') {
		ll = ll * 10 + *pstr - '0';
		pstr++;
	}
	ll = Integer_sign *  ll;
	return ll%mod;
}

