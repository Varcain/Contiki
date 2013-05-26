/*
 * Copyright (c) 2005, Swedish Institute of Computer Science.
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
 * $Id: temperature-sensor.c,v 1.3 2010/02/08 00:00:45 nifi Exp $
 *
 * -----------------------------------------------------------------
 *
 * Author  : Adam Dunkels, Joakim Eriksson, Niclas Finne
 * Created : 2005-11-01
 * Updated : $Date: 2010/02/08 00:00:45 $
 *           $Revision: 1.3 $
 */

#include "dev/temperature-sensor.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "math.h"
#include "clock.h"

const struct sensors_sensor temperature_sensor;
static int active;

/*---------------------------------------------------------------------------*/
static int
value(int type)
{
	uint32_t pui32ADC0Value[1];
	float f32TempValueC;
	ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
	// TEMP = 147.5 - ((75 * (VREFP - VREFN) × ADCCODE) / 4096)
	f32TempValueC = 147.5f - ((75 * (3.3f - 0.0f) * pui32ADC0Value[0]) / 4096);
	return (int)(f32TempValueC*10);
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int value)
{
	switch(type) {
	case SENSORS_HW_INIT:
		active = 0;
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
		// delay to prevent bus fault
		clock_delay(5);
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_ALWAYS, 0);
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS |  ADC_CTL_END);
		ADCSequenceEnable(ADC0_BASE, 3);
		return 1;
	case SENSORS_ACTIVE:
		if(value)
		{
			if(!active)
			{
				active = 1;
			}
		}
		else
		{
			active = 0;
		}
		return 1;
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
	switch(type) {
	case SENSORS_ACTIVE:
	case SENSORS_READY:
		return active;
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(temperature_sensor, TEMPERATURE_SENSOR,
		value, configure, status);
