; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 
Digit0 EQU 0
Digit1 EQU 4
Digit2 EQU 8
Digit3 EQU 12
Digit4 EQU 16
Digit5 EQU 20
Digit6 EQU 24
Digit7 EQU 28
Digit8 EQU 32
Digit9 EQU 36
    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
		PUSH {R3-R11, LR};
		MOV R2, #10;
		MOV R4, #0;
		
		SUB SP, SP, #40;
		
		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;;
		STR R1, [SP, #Digit0];
		ADD R4, R4, #1;Counter for digits in stack
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata;
		
		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit1];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit2];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit3];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit4];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit5];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit6];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit7];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;	
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit8];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

		UDIV R3, R0, R2;
		MUL R1, R3, R2;
		SUB R1, R0, R1;
		STR R1, [SP, #Digit9];
		ADD R4, R4, #1;
		MOV R0, R3;
		CMP R0, #0;
		BEQ popdata

popdata
		SUB R6, R4, #1;
startpopdec
		MOV R5, #4;
		MUL R5, R6, R5; 
		LDR R0, [SP, R5]; 
		ADD R0, R0, #0x30;
		BL ST7735_OutChar;
		SUB R6, R6, #1;
		SUB R4, R4 , #1;
		CMP R4, #0; 
		BNE startpopdec; 
		
	
return
		ADD SP, SP, #40;
		POP {R3-R11, LR};


	
;; --UUU-- Complete this (copy from Lab7-8)
    BX LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
;; --UUU-- Complete this (copy from Lab7-8)
     PUSH {R3-R11, LR}
	MOV R5, #100; 
	MOV R4, #0;

	SUB SP, SP, #16; 
	MOV R4, R0;
	UDIV R4, R4, R5;
	CMP R4, #9;
	BGT error

	MOV R2, #10; 
	UDIV R3, R0, R2;
	MUL R1, R3, R2;
	SUB R1, R0, R1;
	STR R1, [SP, #Digit2];
	MOV R0, R3;

	UDIV R3, R0, R2;
	MUL R1, R3, R2;
	SUB R1, R0, R1;
	STR R1, [SP, #Digit1];
	MOV R0, R3;

	UDIV R3, R0, R2;
	MUL R1, R3, R2;
	SUB R1, R0, R1;
	STR R1, [SP, #Digit0];
	MOV R0, R3;

popfp
	LDR R0, [SP, #Digit0]; 
	ADD R0, R0, #0x30;	
	BL ST7735_OutChar;

	MOV R0, #0X2E;
	BL ST7735_OutChar;

	LDR R0, [SP, #Digit1]; 
	ADD R0, R0, #0x30;	
	BL ST7735_OutChar;

	LDR R0, [SP, #Digit2]; 
	ADD R0, R0, #0x30;	
	BL ST7735_OutChar;

	LDR R0, [SP, #Digit3]; 
	ADD R0, R0, #0x30;	
	;BL ST7735_OutChar;
	B returnfp

error
	MOV R0, #0X2A;
	BL ST7735_OutChar;
	MOV R0, #0X2E;
	BL ST7735_OutChar;
	MOV R0, #0X2A;
	BL ST7735_OutChar;
	MOV R0, #0X2A;
	BL ST7735_OutChar;


returnfp
	ADD SP, SP, #16;
	POP {R3-R11, LR}
	 BX LR
;* * * * * * * * End of LCD_OutFix * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
