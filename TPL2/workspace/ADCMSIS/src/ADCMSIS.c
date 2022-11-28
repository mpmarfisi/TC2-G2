/*
===============================================================================
 Name        : ADCMSIS.c
 Author      : Israel Pavelek, Cesar Fuoco
 Version     : 1.0
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <filter.h>
#include "adc.h"
#include "dac.h"
#include "arm_math.h"

#define TALKTHROUGH 1
#define FIRLP 		2
#define BIQUAD1 	3


extern uint8_t procesar;
extern uint8_t estado;

arm_fir_instance_q31 SLP;
arm_biquad_casd_df1_inst_q31  SBIQUAD1;

q31_t lowpass_state[LOWPASS_TAP_NUM+SAMPLES_PER_BLOCK-1];
q31_t biquad1_state[BIQUAD1_TAP_NUM+SAMPLES_PER_BLOCK-1];

//Ping Pong Buffers
q31_t InputA[SAMPLES_PER_BLOCK];
q31_t InputB[SAMPLES_PER_BLOCK];
q31_t OutputA[SAMPLES_PER_BLOCK];
q31_t OutputB[SAMPLES_PER_BLOCK];


//int state = BIQUAD1;  //Función que ejecuta
int state = FIRLP;
//int state = TALKTHROUGH;

int i;

int main(void) {

    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);

    // Convierto coeficientes float a punto fijo
    arm_float_to_q31(lowpass_taps_float, lowpass_taps, LOWPASS_TAP_NUM);
    arm_float_to_q31(biquad1_taps_float, biquad1_taps, LOWPASS_TAP_NUM);

    // Inicializa los filtros
    arm_fir_init_q31(&SLP,LOWPASS_TAP_NUM,lowpass_taps,lowpass_state,SAMPLES_PER_BLOCK);
	arm_biquad_cascade_df1_init_q31	(&SBIQUAD1,1,biquad1_taps,biquad1_state,0);

	// Inicializa ADC y DAC
    adcInit();
	dacInit();

	while(1){
		if(procesar==1){
			procesar=0;

			if(state==TALKTHROUGH)
			{
				for(i=0;i<SAMPLES_PER_BLOCK;i++)
				{
					if(estado)OutputA[i]=InputA[i];
					else OutputB[i]=InputB[i];
				}
			}

			if(state==FIRLP) // arm_fir_q31
			{
				if(estado)
					arm_fir_q31(&SLP, InputA, OutputA, SAMPLES_PER_BLOCK);
				else
					arm_fir_q31(&SLP, InputB, OutputB, SAMPLES_PER_BLOCK);
			}

			if(state==BIQUAD1) // arm_biquad_cascade_df1_q31
			{

				if(estado)
					arm_biquad_cascade_df1_q31(&SBIQUAD1, InputA, OutputA, SAMPLES_PER_BLOCK);
				else
					arm_biquad_cascade_df1_q31(&SBIQUAD1, InputB, OutputB, SAMPLES_PER_BLOCK);
			}
		}
	}
}
