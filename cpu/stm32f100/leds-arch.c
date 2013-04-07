#include "contiki.h"
#include <stm32f10x.h>
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
  return	((GPIOC->ODR & ((1u)<<8)) ? LEDS_RED : 0) |
			((GPIOC->ODR & ((1u)<<9)) ? LEDS_GREEN : 0);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
	((leds & LEDS_RED) ? GPIO_SetBits(GPIOC, GPIO_Pin_8) : GPIO_ResetBits(GPIOC, GPIO_Pin_8));
    ((leds & LEDS_GREEN) ? GPIO_SetBits(GPIOC, GPIO_Pin_9) : GPIO_ResetBits(GPIOC, GPIO_Pin_9));
}
/*---------------------------------------------------------------------------*/
