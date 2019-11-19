// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 11/14/2018
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){
		SYSCTL_RCGCGPIO_R |= 0x08;
		volatile int wait = 0;
		wait++;
		GPIO_PORTD_DIR_R &= ~0x04;      // 2) make PD2 input
		GPIO_PORTD_AFSEL_R |= 0x04;     // 3) enable alternate fun on PD2
		GPIO_PORTD_DEN_R &= ~0x04;      // 4) disable digital I/O on PD2
		GPIO_PORTD_AMSEL_R |= 0x04;     // 5) enable analog fun on PD2
		SYSCTL_RCGCADC_R |= 0x01;       // 6) activate ADC0 
		wait++;
		wait++; 
		wait++;
		wait++; 
		wait++;
		wait++; 
		ADC0_PC_R = 0x01;               // 7) configure for 125K 
		ADC0_SSPRI_R = 0x0123;          // 8) Seq 3 is highest priority
		ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
		ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
		ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0)+5;  // 11) Ain5 (PD2)
		ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
		ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
		ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
		//ADC0_SAC_R = 0X06; 
	// --UUU-- Complete this(copy from Lab8)
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
  	uint32_t result; 
		ADC0_PSSI_R = 0X0008; 		
		while((ADC0_RIS_R&0x08)==0) {}; 
		result=ADC0_SSFIFO3_R&0xFFF;
		ADC0_ISC_R=0x0008;
		return result;  // --UUU-- Complete this(copy from Lab8)
}


