#include "contiki.h"
#include <stm32f10x.h>
#include "sys/clock.h"
#include "sys/etimer.h"
#include "rtimer-arch.h"
#include "stm32f10x_tim.h"

static volatile clock_time_t count;
static volatile unsigned long current_seconds = 0;
static unsigned int second_countdown = CLOCK_SECOND;
/*---------------------------------------------------------------------------*/
void TIM4_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  count++;
  if(etimer_pending()) {
    etimer_request_poll();
  }

  if (--second_countdown == 0) {
    current_seconds++;
    second_countdown = CLOCK_SECOND;
  }	
}
/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  return count;
}
/*---------------------------------------------------------------------------*/
void
clock_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* RCC TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/* TIM4 config for 1ms*INTERVAL interrupts */
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_Period = (uint32_t)((24000000/1000));
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	
	/* NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	count = 0;
}
/*---------------------------------------------------------------------------*/
/**
 * Delay the CPU for a multiple of 2.83 us.
 */
void
clock_delay(unsigned int i)
{
	do 
	{
		--i;
	} 
	while(i > 0);
}
/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of 10 ms.
 *
 */
void
clock_wait(clock_time_t t)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)t);
}
/*---------------------------------------------------------------------------*/
void
clock_set_seconds(unsigned long sec)
{

}
/*---------------------------------------------------------------------------*/
unsigned long
clock_seconds(void)
{
  return current_seconds;
}
/*---------------------------------------------------------------------------*/
rtimer_clock_t
clock_counter(void)
{
  return TIM4->CNT;
}
/*---------------------------------------------------------------------------*/
