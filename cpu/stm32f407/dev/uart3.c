#include <stm32f4xx.h>
#include "dev/uart3.h"
#include <stm32f4xx_usart.h>

static int (*uart3_input_handler)(unsigned char c);

/*---------------------------------------------------------------------------*/
uint8_t
uart3_active(void)
{
	return 0;
}
/*---------------------------------------------------------------------------*/
void
uart3_set_input(int (*input)(unsigned char c))
{
  uart3_input_handler = input;
}
/*---------------------------------------------------------------------------*/
void
uart3_writeb(unsigned char c)
{
	USART_SendData(USART3,c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE)== RESET);
}
/*---------------------------------------------------------------------------*/
/**
 * Initalize the RS232 port.
 *
 */
void
uart3_init(unsigned long ubr)
{
//
//  Init structures
//
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USARTInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC
//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
//
// USART3 config
//
	// USART3 Pin config
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

  	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
  	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);

	USARTInitStructure.USART_BaudRate=115200;
	USARTInitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USARTInitStructure.USART_Parity=USART_Parity_No;
	USARTInitStructure.USART_StopBits=USART_StopBits_1;
	USARTInitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USARTInitStructure);
	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

//
//  NVIC config
//
	// USART3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*---------------------------------------------------------------------------*/
void USART3_IRQHandler(void)
{
	uint8_t c;

	if(USART_GetITStatus(USART3, USART_IT_RXNE))
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		c=USART_ReceiveData(USART3);
		if(uart3_input_handler != NULL)
		{
			uart3_input_handler(c);
		}
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	}
	if(USART_GetITStatus(USART3, USART_IT_ORE|USART_IT_IDLE))
	{
		USART_ClearITPendingBit(USART3,USART_IT_ORE|USART_IT_IDLE);
	}
}
