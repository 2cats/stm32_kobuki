/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#ifndef _CBUF_H
#define _CBUF_H


#define CBUF_BUFSIZE	512

#define CBUF_ADDM(cb,val) 	do{cb.buf[cb.ep%CBUF_BUFSIZE]=val;cb.ep++;}while(0)
typedef struct
{
	unsigned char buf[CBUF_BUFSIZE];
	unsigned int sp;
	unsigned int ep;
}CBuf;

void CBufInit(CBuf*cb);
int CBufAva(CBuf*cb);
unsigned char CBufPop(CBuf*cb);
//void CBufAdd(CBuf*cb,unsigned char val);
unsigned char CBufGet(CBuf*cb,int start);
#define  CBufClear CBufInit
#endif
