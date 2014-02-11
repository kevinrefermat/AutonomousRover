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
Number      EQU   $0f

; variable/data section
            ORG   RAMStart
            
; code section
            ORG   ROMStart            
Entry:
_Startup:
 	;The Goal here will be to transmit the number $55 to another board.
          	MOVW  #52, SCIBDH
          	MOVB  #%00000000, SCICR1
          	MOVB  #%00001000, SCICR2
          	LDAA  #Number
Here        BRCLR SCISR1, %10000000, Here
           	STAA  SCIDRL
            ;movb  #$ff,DDRB
            ;movb  #$ff,PORTB
            ;clr   PORTB
            ldy   #20
delay       dey
            bne   delay
          	BRA   Here
;NotYetTx    BRCLR SCISR1, %01000000, NotYetTx          	
;          	SWI
	          
;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector