#include "header/config.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <lcd.c>
#include "header/sound.h"
#include "header/cmd_wifi.h"
#include "header/esp8266.h"
#include "header/menu.h"
#ZERO_RAM
/********************************************
***         VARIABLES GLOBALS             ***
/*******************************************/

// Menu seleccionado, Inicia en el menuStart(0)
int menuActive = 0;

/********************************************
*****          INTERRUPCIONES            ****
se dispara cuando se detectan datos de
llegada al puerto serie portWF
********************************************/
#INT_RDA
void  RDA_isr(void) 
{
   int buffer = getc();
   ESP8266_PROCCESS_RESPONSE(buffer);
}
/********************************************
***                INIT                   ***
*********************************************/

   void main()
   {      
      /*for(int idxNC=0;idxNC<=255;idxNC++){
         write_eeprom(idxNC,0xFF);
         delay_ms(10);
      }*/
         
      // Estabiliza el inicio del programa para evitar_
      // posibles errores al energizar el equipo
      delay_ms(500);
      
      // Activa las resistencias de Pull-Up y configura el puerto
      port_b_pullups(0b00110000);
      set_tris_b(0b00110010);
      enable_interrupts(INT_RDA); 
      enable_interrupts(GLOBAL); 

      // Inicializa la LCD
      lcd_init();
      printf(lcd_putc, "\fConfigurando...");
      
      // Enciende el backligh del display
      output_high(LCD_LIGHT_PIN);

      // Inicializa el esp8266 y lo deja en modo lectura de comandos
      esp8266_init();
                
      // Ciclo infinito para mantener el programa activo
      while(1)
      {
         switch(menuActive){
            // Menu principal
            case MENU_START:
               menuActive = getMenuStart();
               break;
 
            case MENU_ON:
               menuActive = getMenuOn();
               break;
 
            case MENU_OFF:
               menuActive = getMenuOff();
               break;

            case MENU_SETUP:
               menuActive = getMenuSetup();
               break;
         } // Fin del Switch(menuActive)
      } // End loop while -> true
   } // Fin del main

