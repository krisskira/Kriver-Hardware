#include "header/config.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <lcd.c>
#include "header/sound.h"
#include "header/func_menu_setup.h"
#include "header/menu.h"
#include "header/esp8266.h"
/** VARIABLES GLOBALS **/

// Menu seleccionado, Inicia en el menuStart(0)
int menuActive = 0;
// Flag usada para almacenar las respuestas de las opciones de menu
int optSelect  = 0;

/** INTERRUPCIONES **/

//@hint: se dispara cuando se detectan datos de llegada al puerto serie portWF
#INT_RDA
void  RDA_isr(void) 
{
   int buffer = getc();
   
   ESP8266_PROCCESS_RESPONSE(buffer);
}

/** INIT **/

   void main()
   {
      // Activa las resistencias de Pull-Up y configura el puerto
      port_b_pullups(0b00110000);
      set_tris_b(0b00110010);
      
      enable_interrupts(INT_RDA); 
      enable_interrupts(GLOBAL); 

      // Inicializa la lcd
      lcd_init();
      output_high(LCD_LIGHT_PIN);

      //Estabiliza el inicio del programa
      delay_ms(500);
      
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

