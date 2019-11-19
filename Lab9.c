// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 9/2/2019

// Analog Input connected to P22=ADC0 channel 5
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// This U0Rx PC4 (in) is connected to other LaunchPad PC5 (out)
// This U0Tx PC5 (out) is connected to other LaunchPad PC4 (in)
// This ground is connected to other LaunchPad ground
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w Fifo on the receiver end 
//    (we suggest implementing and testing this first)
// Lab 9 will not run in the simulator

#include <stdint.h>

#include "ST7735.h"
#include "PLL.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"
#include "UART1.h"
#include "FiFo.h"

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia
void SysTick_Init (void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data, Ones, Tenths, Hundreths;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.01 cm
uint32_t TxCounter=0;
uint32_t ADCStatus=0; 
	char datapt;

// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
	volatile unsigned long delay; 
	SYSCTL_RCGCGPIO_R |= 0x20;  					// activate Port F
	delay = SYSCTL_RCGCGPIO_R;          // allow time for clock to stabilize
	GPIO_PORTF_DIR_R |= 0x0E;         // make PF1-3 out
	GPIO_PORTF_DEN_R |= 0x0E;         // enable digital I/O on PF1-3
// Intialize PortF for hearbeat
}




uint32_t Status[20];  // entries 0,7,12,19 should be false, others true
char GetData[10];  // entries 1 2 3 4 5 6 7 8 should be 1 2 3 4 5 6 7 8
int main1(void){ // Make this main to test FiFo
  Fifo_Init();   // Assuming a buffer of size 6
  for(;;){
    Status[0]  = Fifo_Get(&GetData[0]);  // should fail,    empty
    Status[1]  = Fifo_Put(1);            // should succeed, 1 
    Status[2]  = Fifo_Put(2);            // should succeed, 1 2
    Status[3]  = Fifo_Put(3);            // should succeed, 1 2 3
    Status[4]  = Fifo_Put(4);            // should succeed, 1 2 3 4
    Status[5]  = Fifo_Put(5);            // should succeed, 1 2 3 4 5
    Status[6]  = Fifo_Put(6);            // should succeed, 1 2 3 4 5 6
    Status[7]  = Fifo_Put(7);            // should fail,    1 2 3 4 5 6 
    Status[8]  = Fifo_Get(&GetData[1]);  // should succeed, 2 3 4 5 6
    Status[9]  = Fifo_Get(&GetData[2]);  // should succeed, 3 4 5 6
    Status[10] = Fifo_Put(7);            // should succeed, 3 4 5 6 7
    Status[11] = Fifo_Put(8);            // should succeed, 3 4 5 6 7 8
    Status[12] = Fifo_Put(9);            // should fail,    3 4 5 6 7 8 
    Status[13] = Fifo_Get(&GetData[3]);  // should succeed, 4 5 6 7 8
    Status[14] = Fifo_Get(&GetData[4]);  // should succeed, 5 6 7 8
    Status[15] = Fifo_Get(&GetData[5]);  // should succeed, 6 7 8
    Status[16] = Fifo_Get(&GetData[6]);  // should succeed, 7 8
    Status[17] = Fifo_Get(&GetData[7]);  // should succeed, 8
    Status[18] = Fifo_Get(&GetData[8]);  // should succeed, empty
    Status[19] = Fifo_Get(&GetData[9]);  // should fail,    empty
  }
}

// Get fit from excel and code the convert routine with the constants
// from the curve-fit
uint32_t Convert(uint32_t input){
   uint32_t mult = 0; 
	mult = input*200;
	return (mult/4096)+1;//replace with your calibration code from Lab 8
 //replace with your calibration code from Lab 8
}


// final main program for bidirectional communication
// Sender sends using SysTick Interrupt
// Receiver receives using RX
int main(void){ 
  DisableInterrupts(); // complete initialization first
  PLL_Init(Bus80MHz);     // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB);
  ADC_Init();    // initialize to sample ADC
  PortF_Init();
  UART1_Init();       // initialize UART
//Enable SysTick Interrupt by calling SysTick_Init()
  Fifo_Init();
	SysTick_Init();
	EnableInterrupts();
  while(1){
		//if (ADCStatus==1){
		PF1 ^= 0X01;
		while (datapt != 0x02){
			Fifo_Get(&datapt);// pc5 is the transmitter
		}
		Fifo_Get(&datapt);
		ST7735_OutChar(datapt);
		Fifo_Get(&datapt);
		ST7735_OutChar(datapt);
		Fifo_Get(&datapt);
		ST7735_OutChar(datapt);
		Fifo_Get(&datapt);
		ST7735_OutChar(datapt);
		Fifo_Get(&datapt);
		ST7735_OutChar(datapt);
		
		ST7735_OutString(" cm");
		ST7735_SetCursor(0, 0);
		ADCStatus=0;
		//}
    //--UUU--Complete this  - see lab manual
  }
}

/* SysTick ISR
*/
void SysTick_Handler(void){ // every 20 ms
 //Sample ADC, convert to distance, create 8-byte message, send message out UART1
PF2 ^= 0X04;
	Data = ADC_In();
	PF2 ^= 0x04;
	Position= Convert(Data);
	//Position=Position/10;
	//Hundreths= (Position%10)+0x30;
	//Position=Position/10;
	//Tenths=(Position%10)+0x30;
	//Position=Position/10;
	//Ones= (Position%10)+0x30;
	UART1_OutChar(0x02);
	UART1_OutChar(Position / 100 + 0x30);
	Position = Position % 100;
	UART1_OutChar(0x2E);
	UART1_OutChar(Position / 10 + 0x30);
	Position=Position%10;
	UART1_OutChar(Position + 0x30);
	UART1_OutChar(0x20);
	UART1_OutChar(0x0D);
	UART1_OutChar(0x03);
	TxCounter++;
	PF2 ^= 0x04;
	ADCStatus=1;
}
void SysTick_Init(void) {
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = 1333333; // reload value for 60Hz
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1
  NVIC_ST_CTRL_R = 0x0007; // enable SysTick with core clock and interrupts
}
