#include "contiki.h"
#include <stm32f4xx.h>
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	GPIO_ResetBits(GPIOD, GPIO_Pin_15);
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
  return	((GPIOD->ODR & ((1u)<<12)) ? LEDS_RED : 0) |
			((GPIOD->ODR & ((1u)<<13)) ? LEDS_GREEN : 0);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
	((leds & LEDS_RED) ? GPIO_SetBits(GPIOD, GPIO_Pin_12) : GPIO_ResetBits(GPIOD, GPIO_Pin_12));
    ((leds & LEDS_GREEN) ? GPIO_SetBits(GPIOD, GPIO_Pin_13) : GPIO_ResetBits(GPIOD, GPIO_Pin_13));
}
/*---------------------------------------------------------------------------*/
