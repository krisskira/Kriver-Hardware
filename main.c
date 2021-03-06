/*******************************************************

   Company Name:  KRIVER DEVICE 
   Product Name:  KRIVER SMART HOME
   
   Developer:     CRHISTIAN DAVID VERGARA
   Date:          21 Diciembre de 2017
   
   FILE:          MAIN.C: Funcion principal

*******************************************************/

#include "header/config.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <lcd.c>
#include "header/sound.h"
#include "header/cmd_wifi.h"
#include "header/esp8266.h"
#include "header/menu.h"

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
   /************************
   ***      INIT         ***
   *************************/

   void main()
   {      
      // Activa las resistencias de Pull-Up y configura el puerto
      port_b_pullups(0b00110000);
      set_tris_b(0b00110010);
      
      // Configura el Estado inicial de los Puertos
      output_low(CHPD_ESP8266);
      output_low(Speaker);
      output_high(LCD_LIGHT_PIN);
      
      for(int idxOff=0;idxOff<8;idxOff++){
         output_low(PIN_OUT[idxOff]);
      }
      
      // Estabiliza el inicio del programa para evitar_
      // posibles errores al energizar el equipo
      clear_interrupt(INT_RDA); 
      enable_interrupts(INT_RDA);
      enable_interrupts(GLOBAL); 
      delay_ms(1000);
      
      output_high(CHPD_ESP8266);
      // @TODO: Usar este comando solo en produccion
      //        comentarlo durante etapa de desarrollo
      //        Configurar el USART a 9600bps
      fprintf(ESP8266, "AT+RST\r\n");
            
      // Inicializa la LCD
      lcd_init();
      printf(lcd_putc, "\fIniciando...");

      delay_ms(5000);

      // Inicializa el esp8266 y lo deja en modo lectura de comandos
      esp8266_init();

      // Carga el nombre personalizado del puerto desde la eeprom
      getNameMenusOutput();
      
      // Ciclo infinito para mantener el programa activo
      int loop=1;
      while(loop==1)
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

