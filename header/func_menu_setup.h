/** VARIABLES EXTERNAL **/
#define BEEP  Sound(1209,20)
/** DIRECTIVAS **/
#define OPT_SHOW_IP     0
#define OPT_SHOW_SSID   1
#define OPT_SHOW_KEY    2
#define OPT_MODE_AP     3
#define OPT_BACKLIGH    4
#define OPT_EXIT        5

//Flag, Modo Cliente(1), Access Point(2), Cliente + AP (3)
#define MODE_CLIENT     0
#define MODE_AP         1
#define MODE_CLIENT_AP  2

int8 countOptMenuModo = 3;

//Contiene las opciones del menu optMenuModo[pos][lenString]
const char  optMenuModo[5][13]    =  { {"Cliente     "},
                                       {"Access Point"},
                                       {"Cliente + AP"},
                                       {"SALIR       "}
                                     };

/***************************************************
Pausa el programa hasta que se presione OK
***************************************************/
void pressExit(void){
   lcd_gotoxy(12,2);
   printf(lcd_putc, "SALIR");
   while(1){
      if(input(KEY_OK)==0){
            // Elimina el rebote
            delay_ms(500);
            break;
      }
   }
}

void showBottonMenu(void){
   lcd_gotoxy(1,2);
   printf(lcd_putc, "[SIG]      [ENT]");
}
/***************************************************
Despliega el menu MODE
***************************************************/
void setMenuModo(void){
   int optSelected = 0;
   int exit = 0;

   // Muestra la primera opcion del menu
   printf(lcd_putc, "\f%s",optMenuModo[optSelected]);
   showBottonMenu();
   
   while(!exit){
      // TECLA SIGUIENTE: hace la navegacion entre las opciones del menu
      if(input(KEY_NEXT)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         // incrementa la opcion seleccionada para mover el menu
         optSelected++;
         // Si la opcion seleccionada esta en el limite de las opciones 
         // entonces regresa a la primera posicion del menu
         if(optSelected > countOptMenuModo){
            optSelected = 0;
         }
         // Muestra la opcion seleccionada
         printf(lcd_putc, "\f%s",optMenuModo[optSelected]);
         showBottonMenu();
      } // Fin KEY_NEXT
      
      // Finaliza la funcion y retorna la opcion del menu seleccionada
      if(input(KEY_OK)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         
         // Si optSelected = countOptMenuModo entonces esta en la pos SALIR
         if(optSelected == countOptMenuModo){
            exit = 1;
         }else{
            switch(optSelected){
               case MODE_CLIENT:
                  //fprintf(ESP8266,"AT+CWMODE_DEF=1\r\n");
                  printf(lcd_putc, "\fConfig Cliente");
                  break;
               case MODE_AP:
                  //fprintf(ESP8266,"AT+CWMODE_DEF=2\r\n");
                  printf(lcd_putc, "\fConfig AP");
                  break;
               case MODE_CLIENT_AP:
                  //fprintf(ESP8266,"AT+CWMODE_DEF=3\r\n");
                  printf(lcd_putc, "\fConfig ClienteAP");
                  break;
            }
            pressExit();
            // Muestra la ultima opcion del menu seleccionada
            printf(lcd_putc, "\f%s",optMenuModo[optSelected]);
            showBottonMenu();
         }
      } // Fin KEY_OK
   } // FIN While Infinito
} // Fin funcion setMenuModo

/***************************************************
Dispacher function MENU_SETUP
***************************************************/

void executeSetup(int optSelect){
   switch(optSelect){
      
      case OPT_SHOW_IP:
         printf(lcd_putc, "\f192.168.0.1");
         pressExit();
         break;
      case OPT_SHOW_SSID:
         printf(lcd_putc, "\fC0G3_UNE");
         pressExit();         
         break;
      case OPT_SHOW_KEY:
         printf(lcd_putc, "\f1234Az!");
         pressExit();
         break;
      case OPT_MODE_AP:
         setMenuModo();
         break;
      case OPT_BACKLIGH:
         output_toggle(LCD_LIGHT_PIN);
         if(input_state(LCD_LIGHT_PIN)==1){
            printf(lcd_putc, "\fBacklight On");
         }else{
            printf(lcd_putc, "\fBacklight Off");
         }
         pressExit();
         break;
      case OPT_EXIT:
         break;
         
   }

}
