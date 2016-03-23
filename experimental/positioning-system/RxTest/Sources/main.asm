;*****************************************************************
;* This stationery serves as the framework for a                 *
;* user application (single file, absolute assembly application) *
;* For a more comprehensive program that                         *
;* demonstrates the more advanced functionality of this          *
;* processor, please see the demonstration applications          *
;* located in the examples subdirectory of the                   *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*****************************************************************
;ELEC 402 RF Receiver Test
;Written by Joshua M. Arakaki 

; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point

; Include derivative-specific definitions 
		        INCLUDE 'MC9S12C128.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data
; variable/data section
            ORG   RAMStart
            
; code section
            ORG   ROMStart            
Entry:
_Startup:
           
;Data        RMB   1
;Read byte
;This subroutine reads the byte that was transmitted.
;            LDX   #Data
            MOVW  #52, SCIBDH
            MOVB  #%00000000, SCICR1
            MOVB  #%00000100, SCICR2
NotYetRx    BRCLR SCISR1, %00100000, NotYetRx
            LDAA  SCIDRL
;            STAA  1, X+
            
            CMPA  #$0F
            BNE   DidntWork
            LDAA  #$08		              ;The LED will be connected to PB4
				    STAA  DDRB                  ;Light up the LED
				    BSET  PORTB, $08
Worked	    BRA   Worked
DidntWork   BRA   DidntWork             ;If the LED lights up, then everything works!!!


;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector