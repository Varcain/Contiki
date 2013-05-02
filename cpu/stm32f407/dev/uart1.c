#include <stm32f4xx.h>
#include "dev/uart1.h"
#include <stm32f4xx_usart.h>

static int (*uart1_input_handler)(unsigned char c);

/*---------------------------------------------------------------------------*/
uint8_t
uart1_active(void)
{
	return 0;
}
/*---------------------------------------------------------------------------*/
void
uart1_set_input(int (*input)(unsigned char c))
{
  uart1_input_handler = input;
}
/*---------------------------------------------------------------------------*/
void
uart1_writeb(unsigned char c)
{
	USART_SendData(USART1,c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)== RESET);
}
/*---------------------------------------------------------------------------*/
/**
 * Initalize the RS232 port.
 *
 */
void
uart1_init(unsigned long ubr)
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//
// USART1 config
//
	// USART1 Pin config
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

  	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
  	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);

	USARTInitStructure.USART_BaudRate=9600;
	USARTInitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USARTInitStructure.USART_Parity=USART_Parity_No;
	USARTInitStructure.USART_StopBits=USART_StopBits_1;
	USARTInitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USARTInitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//
//  NVIC config
//
	// USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*---------------------------------------------------------------------------*/
void USART1_IRQHandler(void)
{
	uint8_t c;

	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		c=USART_ReceiveData(USART1);
		if(uart1_input_handler != NULL)
		{
			uart1_input_handler(c);
		}
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
	if(USART_GetITStatus(USART1, USART_IT_ORE|USART_IT_IDLE))
	{
		USART_ClearITPendingBit(USART1,USART_IT_ORE|USART_IT_IDLE);
	}
}
