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
#include "stm32f4xx_adc.h"
#include "math.h"

const struct sensors_sensor temperature_sensor;
static int active;

static ADC_CommonInitTypeDef ADC_CommonInitStruct;
static ADC_InitTypeDef ADC_InitStruct;

/*---------------------------------------------------------------------------*/
static int
value(int type)
{
	int temp = ADC_GetConversionValue(ADC1);
	float voltage;
	voltage = (temp * 3000.0f)/4095;
	voltage = (((voltage - 760)/2.5f) + 25) * 10;
	temp = roundf(voltage);
	return temp;
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int value)
{
	switch(type) {
	case SENSORS_HW_INIT:
		active = 0;
		ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
		ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div2;
		ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInit(&ADC_CommonInitStruct);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

		ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;
		ADC_InitStruct.ADC_ScanConvMode=DISABLE;
		ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;
		ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
		ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStruct.ADC_ExternalTrigConv = 0;
		ADC_InitStruct.ADC_NbrOfConversion=1;
		ADC_Init(ADC1,&ADC_InitStruct);

		ADC_TempSensorVrefintCmd(ENABLE);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);

		ADC_Cmd(ADC1, ENABLE);
		ADC_SoftwareStartConv(ADC1);
		return 1;
	case SENSORS_ACTIVE:
		if(value)
		{
			if(!active)
			{
				active = 1;
				//ADC_Cmd(ADC1, ENABLE);
				//ADC_SoftwareStartConv(ADC1);
			}
		}
		else
		{
			//ADC_Cmd(ADC1, DISABLE);
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
