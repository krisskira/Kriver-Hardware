#include <16F887.h>
//#device ADC=16
//#device ICD=TRUE
#include <stdio.h>


#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=ESP8266)
#use rs232(baud=115200,parity=N,xmit=PIN_B0,rcv=PIN_B1,bits=8,stream=console)

