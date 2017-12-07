/** VARIABLES EXTERNAL **/
#define BEEP  Sound(1209,20)

/** DIRECTIVAS **/
#define MENU_START  0
#define MENU_ON     1
#define MENU_OFF    2
#define MENU_SETUP  3

  /** DIRECTIVAS SUBMENU SETUP**/
  #define OPT_SHOW_IP     0
  #define OPT_SHOW_SSID   1
  #define OPT_SHOW_KEY    2
  #define OPT_MODE_AP     3
  #define OPT_BACKLIGH    4
  #define OPT_EXIT        5

    /** DIRECTIVAS SUBMENU CONF_ACCESS_RED **/
    #define MODE_CLIENT     0
    #define MODE_AP         1
    #define MODE_CLIENT_AP  2

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
                                        {"Conf Accesso Red"},
                                        {"On/Off Backlight"},
                                        {"SALIR           "}
                                      };

//Contiene las opciones del subMenu CONF_ACCESS_RED [pos][lenString]
const char  optMenuModo[5][14]    =   { {"Estacion     "},
                                        {"Access Point "},
                                        {"Estacion + AP"},
                                        {"SALIR        "}
                                      };   

/* Declaracion de Funciones */
int   getMenuStart(void);
void  pressExit(void);
void  showBottonMenu(void);
int   getMenuOn(void);
int   getMenuOff(void);
int   getMenuSetup(void);
void  executeSetup(int optSelect);
void  setMenuModo(void);

/** VARIABLES GENERALS **/
int8 countOptMenuStart  = 2;
int8 countOptMenuOn     = 8;
int8 countOptMenuOff    = 8;
int8 countOptMenuSetup  = 5;
// Numero de opciones SubMenu Modo
int8 countOptMenuModo = 3;

/***************************************************
Despliega el menu principal MENU_START
***************************************************/
int getMenuStart(void){
   int optSelected = 0;
   int exit = 0;

   printf(lcd_putc, "\f%s",optMenuStart[optSelected]);
   showBottonMenu();
   while(!exit){
      
      /********************************************
      *  INVOCA LA FUNCION RUN_COMMAND_WIFI       *
      *********************************************/
      run_command_wifi();
      /********************************************/
      
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
         showBottonMenu();
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
Pausa el programa hasta que se presione OK
***************************************************/
void pressExit(void){
  lcd_gotoxy(12,2);
  printf(lcd_putc, "SALIR");
  
  while(1){
      /********************************************
      *  INVOCA LA FUNCION RUN_COMMAND_WIFI       *
      *********************************************/
      run_command_wifi();
      /********************************************/
      
     if(input(KEY_OK)==0){
           // Elimina el rebote
           delay_ms(500);
           break;
     }
  }
}
/***************************************************
Dibuja en la parte inferior del Display los botones de
Siguiente y Entrar
***************************************************/
void showBottonMenu(void){
  lcd_gotoxy(1,2);
  printf(lcd_putc, "[SIG]      [ENT]");
}

/***************************************************
Despliega el menu MENU_ON
***************************************************/
int getMenuOn(void){
   int optSelected = 0;
   int exit = 0;
   
   // Muestra la primera opcion del menu
   printf(lcd_putc, "\f%s",optMenuOn[optSelected]);
   showBottonMenu();
   
   while(!exit){
   
      /********************************************
      *  INVOCA LA FUNCION RUN_COMMAND_WIFI       *
      *********************************************/
      run_command_wifi();
      /********************************************/
      
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
         showBottonMenu();
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
   showBottonMenu();
   
   while(!exit){
   
      /********************************************
      *  INVOCA LA FUNCION RUN_COMMAND_WIFI       *
      *********************************************/
      run_command_wifi();
      /********************************************/
      
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
         showBottonMenu();
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

   /********************************************
   *  INVOCA LA FUNCION RUN_COMMAND_WIFI       *
   *********************************************/
   run_command_wifi();
   /********************************************/
   
   int optSelected = 0;
   int exit = 0;
   
   // Muestra la primera opcion del menu
   printf(lcd_putc, "\f%s",optMenuSetup[optSelected]);
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
         if(optSelected > (countOptMenuSetup)){
            optSelected = 0;
         }
         
         // Muestra la opcion seleccionada
         printf(lcd_putc, "\f%s",optMenuSetup[optSelected]);
         showBottonMenu();
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
            showBottonMenu();
         }
      } // Fin KEY_OK
   } // FIN While Infinito

   return 0;
} // Fin funcion getMenuSetup

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
  
      /********************************************
      *  INVOCA LA FUNCION RUN_COMMAND_WIFI       *
      *********************************************/
      run_command_wifi();
      /********************************************/
      
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
                 fprintf(ESP8266,"AT+CWMODE_DEF=1\r\n");
                 printf(lcd_putc, "\fConfig Cliente");
                 break;
              case MODE_AP:
                 fprintf(ESP8266,"AT+CWMODE_DEF=2\r\n");
                 printf(lcd_putc, "\fConfig AP");
                 break;
              case MODE_CLIENT_AP:
                 fprintf(ESP8266,"AT+CWMODE_DEF=3\r\n");
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
