/**
 * FILE:		ADC.h
 * DESCRIPTION:	ADC interface for DSP
 * AUTHOR: 		Sharan ***REMOVED***
 * COPYRIGHT:	Copyright (c) 2014 Sharan ***REMOVED***
 * LICENSE:		MIT License
 */

#ifndef __ADC_H
#define __ADC_H

#include <stdlib.h>
#include "stm32f4xx.h"

void GPIO_Configuration(void);
void ADC_Configuration(void);
uint16_t analogRead(void);

#endif
