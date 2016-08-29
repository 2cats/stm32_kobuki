#include "cbuf.h"
/*
Cbuf : 环形缓冲
	初始化：										void CBuf_Init(CBuf*cb)
	添加数据：									void CBuf_Add(CBuf*cb,unsigned char val)  					 //CBuf_AddM和CBuf_Add相同，只不过用宏定义实现，省去了子函数调用的开销
	取出数据(先入先出)：				unsigned char CBuf_Pop(CBuf*cb)
	取出数据(同上但不删除)：		unsigned char CBuf_Get(CBuf*cb,int start)						//start是数据位置的偏移量
	获得可用数据长度：					unsigned char CBuf_Ava(CBuf*cb)

Example:

	Cbuf  cirBuf;
	CBuf_Init(&cirBuf);
	CBUF_ADDM(cirBuf,'a');
	CBUF_ADDM(cirBuf,'b');
	CBUF_ADDM(cirBuf,'c');
	uchar x=CBuf_Pop(&cirBuf);				//x='a' ('a'最先被添加故最先被取出)
	int ava=CBuf_Ava(&cirBuf);				//ava=2
	x=CBuf_Get(&cirBuf,0);						//x='b' (ava=2)
	

	
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
