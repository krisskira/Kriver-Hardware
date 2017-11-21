/** DIRECTIVAS **/
#define MENU_START  0
#define MENU_ON     1
#define MENU_OFF    2
#define MENU_SETUP  3

/** VARIABLES GENERALS **/
int8 countOptMenuStart  = 2;
int8 countOptMenuSetup  = 5;
int8 countOptMenuOn     = 8;
int8 countOptMenuOff    = 8;
//Contiene las opciones del menu principal [pos][lenString]
const char  optMenuStart[4][16]    =  { {"Encender Manual"},
                                        {"Apagado Manual "},
                                        {"Configuracion  "}
                                      };
                                      
//Contiene las opciones del menu Encender [pos][lenString]
const char  optMenuOn[10][15]      =  {{"On -> Salida 1"},
                                       {"On -> Salida 2"},
                                       {"On -> Salida 3"},
                                       {"On -> Salida 4"},
                                       {"On -> Salida 5"},
                                       {"On -> Salida 6"},
                                       {"On -> Salida 7"},
                                       {"On -> Salida 8"},
                                       {"SALIR         "}
                                      };

//Contiene las opciones del menu Apagar [pos][lenString]
const char  optMenuOff[10][16]     =  {{"Off -> Salida 1"},
                                       {"Off -> Salida 2"},
                                       {"Off -> Salida 3"},
                                       {"Off -> Salida 4"},
                                       {"Off -> Salida 5"},
                                       {"Off -> Salida 6"},
                                       {"Off -> Salida 7"},
                                       {"Off -> Salida 8"},
                                       {"SALIR          "}
                                      };

//Contiene las opciones del menu Configuracion [pos][lenString]
const char  optMenuSetup[7][17]    =  { {"VER IP          "},
                                        {"VER SSID        "},
                                        {"VER KEY         "},
                                        {"Modo AccessPoint"},
                                        {"On/Off Backlight"},
                                        {"SALIR           "}
                                      };

/***************************************************
Despliega el menu principal MENU_START
***************************************************/
int getMenuStart(void){
   int optSelected = 0;
   int exit = 0;

   printf(lcd_putc, "\f%s",optMenuStart[optSelected]);

   while(!exit){
      // TECLA SIGUIENTE: hace la navegacion entre las opciones del menu
      if(input(KEY_NEXT)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         // incrementa la opcion seleccionada para mover el menu
         optSelected++;
         
         // Si la opcion seleccionada esta en el limite menos uno de las
         // opciones entonces regresa a la primera posicion del menu
         if(optSelected > (countOptMenuStart)){
            optSelected = 0;
         }
         
         // Muestra la opcion seleccionada
         printf(lcd_putc, "\f%s",optMenuStart[optSelected]);
      } // Fin KEY_NEXT
      
      // Finaliza la funcion y retorna la opcion del menu seleccionada
      if(input(KEY_OK)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         // Rompe el Loop
         exit = 1;
      } // Fin KEY_OK
   } // FIN While Infinito

   return optSelected+1;
} // Fin funcion getMenuStart

/***************************************************
Despliega el menu MENU_ON
***************************************************/
int getMenuOn(void){
   int optSelected = 0;
   int exit = 0;
   
   // Muestra la primera opcion del menu
   printf(lcd_putc, "\f%s",optMenuOn[optSelected]);
   
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
         if(optSelected > countOptMenuOn){
            optSelected = 0;
         }
         // Muestra la opcion seleccionada
         printf(lcd_putc, "\f%s",optMenuOn[optSelected]);
      } // Fin KEY_NEXT
      
      // Finaliza la funcion y retorna la opcion del menu seleccionada
      if(input(KEY_OK)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         
         // Si optSelected = countOptMenuOn entonces esta en la pos SALIR
         if((optSelected) == countOptMenuOn){
            exit = 1;
         }else{
            output_high(PIN_OUT[optSelected]);
         }
         // Rompe el Loop
      } // Fin KEY_OK
   } // FIN While Infinito

   return 0;
} // Fin funcion getMenuOn

/***************************************************
Despliega el menu MENU_OFF
***************************************************/
int getMenuOff(void){
   int optSelected = 0;
   int exit = 0;

   // Muestra la primera opcion del menu
   printf(lcd_putc, "\f%s",optMenuOff[optSelected]);
   
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
         if(optSelected > countOptMenuOff){
            optSelected = 0;
         }
         // Muestra la opcion seleccionada
         printf(lcd_putc, "\f%s",optMenuOff[optSelected]);
      } // Fin KEY_NEXT
      
      // Finaliza la funcion y retorna la opcion del menu seleccionada
      if(input(KEY_OK)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         
         // Si optSelected = countOptMenuOn entonces esta en la pos SALIR
         if((optSelected) == countOptMenuOff){
            exit = 1;
         }else{
            output_low(PIN_OUT[optSelected]);
         }
         // Rompe el Loop
      } // Fin KEY_OK
   } // FIN While Infinito

   return 0;
} // Fin funcion getMenuMain

/***************************************************
Despliega el menu principal MENU_SETUP
***************************************************/
int getMenuSetup(void){
   int optSelected = 0;
   int exit = 0;
   
   // Muestra la primera opcion del menu
   printf(lcd_putc, "\f%s",optMenuSetup[optSelected]);
   
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
         if(optSelected > (countOptMenuSetup)){
            optSelected = 0;
         }
         
         // Muestra la opcion seleccionada
         printf(lcd_putc, "\f%s",optMenuSetup[optSelected]);
      } // Fin KEY_NEXT
      
      // Finaliza la funcion y retorna la opcion del menu seleccionada
      if(input(KEY_OK)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         
         // Si optSelected = countOptMenuOn entonces esta en la pos SALIR
         if((optSelected) == countOptMenuSetup){
            exit = 1;
         }else{
            executeSetup(optSelected);
            // Muestra la opcion seleccionada
            printf(lcd_putc, "\f%s",optMenuSetup[optSelected]);
         }
      } // Fin KEY_OK
   } // FIN While Infinito

   return 0;
} // Fin funcion getMenuSetup
