;*****************************************************************
;* This stationery serves as the framework for a                 *
;* user application (single file, absolute assembly application) *
;* For a more comprehensive program that                         *
;* demonstrates the more advanced functionality of this          *
;* processor, please see the demonstration applications          *
;* located in the examples subdirectory of the                   *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*****************************************************************
;ELEC 402 RF Transmitter Test
;Written by Joshua M. Arakaki 

; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point

; Include derivative-specific definitions 
		        INCLUDE 'MC9S12C128.inc' 

ROMStart    EQU   $4000  ; absolute address to place my code/constant data
Number      EQU   $00

; variable/data section
            ORG   RAMStart
            
; code section
            ORG   ROMStart            
Entry:
_Startup:

            ;preapre PORTB0 to take input
            MOVB  #$00, DDRB
            MOVB  #$01, PWME
            MOVB  #$01, PWMPOL
            MOVB  #$01, PWMCLK
            MOVB  #$07, PWMPRCLK
            MOVB  #$00, PWMCAE
            MOVB  #$00, PWMCTL
            MOVB  #$40, PWMSCLA
            
Here        BRA   Here
            


 
            ldy   #20
delay       dey
            bne   delay
;notPressed  brset  PORTB, %00000001, notPressed
;pressed     brclr  PORTB, %00000001, pressed
          	BRA   Here
 
	          
;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector