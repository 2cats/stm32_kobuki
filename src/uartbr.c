#include "uartbr.h"
//串口1环型缓存器
/*************!WARNING!******************/
/*
	当一次性接收太多数据时（多但间隔长没问题），buf数据很有可能在使用前被覆盖，导致数据错误现象，解决方法：增大buf容量
*/

/************INTERFACE*****************/
/*
2014/4/3
	new Function : uart_init_BR
	Example:
		USARTBufReceieverTypeDef USARTBufReceiever;
		uart_init_BR(115200,&USARTBufReceiever);
		if(USARTBufReceiever.getAvaliableNum()>0)
		{
			tmp=USARTBufReceiever.getByte();
			//printf("%c",tmp);
		}
*/
//variables
/*********************************/
unsigned char __defaultBuf[USART_BUFSIZE];
static unsigned  int __start;
//Functions
/*********************************/
u8 UartbrGetByte(void)
{
	 u8 tmp=__defaultBuf[__start++];
	 __start=__start%USART_BUFSIZE;
	return tmp;
}
void UartbrGetBytes(u8 *dat,unsigned int num)
{
	unsigned int i,end;
	if(UartbrGetAvaliableNum()<num)return;
	end=USART_BUFSIZE-DMA1_Channel5->CNDTR;
	end=end>__start?end:end+USART_BUFSIZE;
	for(i=__start;i<end;i++)
	{
		dat[i]=__defaultBuf[i%USART_BUFSIZE];
	}
}

unsigned int UartbrGetAvaliableNum(void)
{
	unsigned int end=USART_BUFSIZE-DMA1_Channel5->CNDTR;
	if(end==__start)
		return 0;
	end=end>__start?end:end+USART_BUFSIZE;
	return end-__start;
}
void __uartbrEnableReceieve(void)
{
	DMA_InitTypeDef DMA_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel5);

	DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t) & (USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)__defaultBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USART_BUFSIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure); 

	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE );
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	DMA_Cmd(DMA1_Channel5,ENABLE);//??DMA
}


void UartbrInit(u32 bound)
{
	uart_init(bound);
	__start=0;
	__uartbrEnableReceieve();
}
//IQR
/*********************************/
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5))
	{
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}
}
