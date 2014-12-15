/**
 * FILE:		ADC.c
 * DESCRIPTION:	ADC interface for DSP
 * AUTHOR:		Sharan ***REMOVED***
 * COPYRIGHT:	Copyright (c) 2014 Sharan ***REMOVED***
 * LICENSE: 	MIT License
 */

#include "adc.h"

void GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;					// init struct
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// start clock port A

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				// choose pin 0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			// analog mode
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 		// no push/pull resistor
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// init function
}

void ADC_Configuration(void)
{
	ADC_InitTypeDef			ADC_InitStructure;
	ADC_CommonInitTypeDef	ADC_CommonInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_DeInit();
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_144Cycles);

	ADC_Cmd(ADC1, ENABLE);
}

uint16_t analogRead(void)
{
	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	return ADC_GetConversionValue(ADC1);
}
