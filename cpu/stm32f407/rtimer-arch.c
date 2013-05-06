/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: rtimer-arch.c,v 1.3 2008/02/09 14:02:12 oliverschmidt Exp $
 */

/**
 * \file
 *         Native (non-specific) code for the Contiki real-time module rt
 * \author
 *         Adam Dunkels <adam@sics.se>
 */
#include <stm32f4xx.h>
#include "sys/rtimer.h"
#include "sys/clock.h"

static volatile uint16_t counter;
static volatile uint8_t scheduled;
/*---------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
	if(counter != 0)
	{
		counter--;
	}
	/* Scheduled Timeout */
	else
	if(counter == 0 && scheduled == 1)
	{
		scheduled = 0;
		rtimer_run_next();
	}
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_init(void)
{
	scheduled = 0;
	/* Setup SysTick Timer for 1 msec interrupts  */
 	if (SysTick_Config((SystemCoreClock / 8) / 1000))
    { 
    	/* Capture error */ 
    	//while (1);
    }
}
/*---------------------------------------------------------------------------*/
void
rtimer_arch_schedule(rtimer_clock_t t)
{
	counter = t;
	scheduled = 1;
}
/*---------------------------------------------------------------------------*/
