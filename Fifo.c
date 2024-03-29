// Fifo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Created: 11/15/2017 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
// --UUU-- Declare state variables for Fifo
//        buffer, put and get indexes
#define FIFO_SIZE 9
int32_t static PutI;
int32_t static GetI;
int32_t static elements; 
int32_t static Fifo[FIFO_SIZE];


// *********** Fifo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void Fifo_Init(){
	PutI=FIFO_SIZE-1;
	GetI=FIFO_SIZE-1;
	elements=0; 
// --UUU-- Complete this

}

// *********** Fifo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t Fifo_Put(char data){
// --UUU-- Complete this routine
	
if(elements==FIFO_SIZE-1){ //buffer is full
		return 0;
	}
	elements++; 
	Fifo[PutI] = data;
	PutI--;
	if (PutI<0) {
		PutI=FIFO_SIZE-1;
	}
	return 1;
//Replace this
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t Fifo_Get(char *datapt){ 
//--UUU-- Complete this routine
	if(PutI==GetI) {
		return 0;
	}
	elements--;
	*datapt=Fifo[GetI];
	GetI--;
	if(GetI<0) {
		GetI=FIFO_SIZE-1;
	}
	return 1;	// Replace this
}



