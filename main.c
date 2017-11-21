#include "header/config.h"
#include <string.h>
#include <math.h>
#include <lcd.c>
#include "header/sound.h"
#include "header/func_menu_setup.h"
#include "header/menu.h"


/** VARIABLES GLOBALS **/

// Menu seleccionado, Inicia en el menuStart(0)
int menuActive = 0;
// Flag usada para almacenar las respuestas de las opciones de menu
int optSelect  = 0;

/** INTERRUPCIONES **/

//@hint: se dispara cuando se detectan datos de llegada al puerto serie portWF
/*
#INT_RDA
void  RDA_isr(void) 
{

}*/

/** INIT **/

   void main()
   {
      // Activa las resistencias de Pull-Up y configura el puerto
      port_b_pullups(0b00110000);
      set_tris_b(0b00110010);
      
      // Inicializa la lcd
      lcd_init();
      
      //Estabiliza el inicio del programa
      delay_ms(1000);
      
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

