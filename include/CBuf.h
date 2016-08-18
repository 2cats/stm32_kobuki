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

void CBuf_Init(CBuf*cb);
int CBuf_Ava(CBuf*cb);
unsigned char CBuf_Pop(CBuf*cb);
//void CBuf_Add(CBuf*cb,unsigned char val);
unsigned char CBuf_Get(CBuf*cb,int start);
#endif
