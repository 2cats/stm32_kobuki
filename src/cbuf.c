/*******************************************************************************
 * Copyright (C) 2016 Ence Wang
 *
 * MIT License
 *******************************************************************************/
#include "cbuf.h"
/*
Cbuf : 环形缓冲
*/
void CBufInit(CBuf*cb)//初始化
{
	cb->sp=cb->ep=0;
}
int CBufAva(CBuf*cb)//获取缓存区可用数据长度
{
	return cb->ep-cb->sp;
}
unsigned char CBufPop(CBuf*cb)//弹出缓存区里的一个字节（该数据会被移出缓存区）
{
	unsigned char val;
	if(cb->ep<=cb->sp) return 0;//check at least one available
	val=CBufGet(cb,0);
	cb->sp++;
	return val;
}
unsigned char CBufGet(CBuf*cb,int start)//获取缓存区里的一个字节，并且可带偏移（该数据不会被移出缓存区）
{
	unsigned char val;
	if(cb->ep-cb->sp>CBUF_BUFSIZE) cb->sp=cb->ep-CBUF_BUFSIZE;//check overflow
	if(cb->ep-cb->sp<=start) return 0;//check available
	val=cb->buf[(cb->sp+start)%CBUF_BUFSIZE];
	if(cb->ep>4*CBUF_BUFSIZE)//avoid ep too large
	{
		cb->ep=cb->ep-CBUF_BUFSIZE;
		cb->sp=cb->sp-CBUF_BUFSIZE; //at here,must have : cb->ep-cb->sp<=CBUF_BUFSIZE
	}
	return val;
}

//CBuf_ADDM和CBuf_Add相同，只不过用宏定义实现，省去了子函数调用的开销
//void CBufAdd(CBuf*cb,unsigned char val)
//{
//	cb->buf[cb->ep%CBUF_BUFSIZE]=val;
//	cb->ep++;
//	return ;
//}
