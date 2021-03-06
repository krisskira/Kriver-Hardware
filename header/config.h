#include <18F4620.h>

#FUSES XT,BROWNOUT
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES BORV21                   //Brownout reset at 2.1V

#use delay(crystal=20M)
/* RS232 */
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=ESP8266)
#use rs232(baud=9600,parity=N,xmit=PIN_B0,rcv=PIN_B1,bits=8,stream=console)

// Puertos de salida
#use FIXED_IO( A_outputs=PIN_A0,PIN_A1,PIN_A2,PIN_A3,PIN_A4)
#use FIXED_IO( E_outputs=PIN_E0,PIN_E1,PIN_E2)

#define PIN_OUT_1       PIN_E2
#define PIN_OUT_2       PIN_E1
#define PIN_OUT_3       PIN_E0
#define PIN_OUT_4       PIN_A4
#define PIN_OUT_5       PIN_A0
#define PIN_OUT_6       PIN_A1
#define PIN_OUT_7       PIN_A2
#define PIN_OUT_8       PIN_A3

#define KEY_NEXT        PIN_B4
#define KEY_OK          PIN_B5

const int16 PIN_OUT[8]={
                        PIN_E2,
                        PIN_E1,
                        PIN_E0,
                        PIN_A4,
                        PIN_A0,
                        PIN_A1,
                        PIN_A2,
                        PIN_A3
                        };

/* LCD 2x16 */
#define LCD_LIGHT_PIN   PIN_D3
#define LCD_RS_PIN      PIN_D2
#define LCD_RW_PIN      PIN_D1
#define LCD_ENABLE_PIN  PIN_D0
#define LCD_DATA4       PIN_D4
#define LCD_DATA5       PIN_D5
#define LCD_DATA6       PIN_D6
#define LCD_DATA7       PIN_D7
