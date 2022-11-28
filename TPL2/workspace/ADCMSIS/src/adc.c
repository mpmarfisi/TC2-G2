/*
===============================================================================
 Name        : adc.c
 Authors     : Israel Pavelek, Cesar Fuoco
 Version     : 1.0
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "board.h"
#include "filter.h"
#include "arm_math.h"
#include "dac.h"


uint8_t procesar=0;
uint8_t estado=0;
uint8_t estado_OUT=0;

extern q31_t InputA[SAMPLES_PER_BLOCK];
extern q31_t InputB[SAMPLES_PER_BLOCK];
extern q31_t OutputA[SAMPLES_PER_BLOCK];
extern q31_t OutputB[SAMPLES_PER_BLOCK];

#define SAMPLE_RATE 44100

void adcInit(void)
{
	ADC_CLOCK_SETUP_T adc;

	Chip_ADC_Init(LPC_ADC, &adc);
	Chip_ADC_SetSampleRate(LPC_ADC, &adc, SAMPLE_RATE);

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);
	Chip_ADC_SetBurstCmd(LPC_ADC, ENABLE);

	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(void)
{
	static uint16_t index=0;
	static uint16_t indexOUT=0;

	static uint16_t data;

	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &data);
	if(!estado)InputA[index++]=data>>2;
	else InputB[index++]=data>>2;
	if(index==SAMPLES_PER_BLOCK){
			index=0;
			estado^=1;
			procesar=1;

	}

	if(!estado_OUT)dacWrite(OutputB[indexOUT++]);
	else dacWrite(OutputA[indexOUT++]);
	if(indexOUT==SAMPLES_PER_BLOCK){
			indexOUT=0;
			estado_OUT^=1;
	}

	//si hubiera que saturarlo para que no pegue el salto
	/*	if(OutputA>0x3FF) OutputA = 0x3FF;
	else if(OutputA<0) firOutputA = 0;*/

}

