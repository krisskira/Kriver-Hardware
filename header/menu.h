/*******************************************************

   Company Name:  KRIVER DEVICE 
   Product Name:  KRIVER SMART HOME
   
   Developer:     CRHISTIAN DAVID VERGARA
   Date:          21 Diciembre de 2017
   
   FILE:          menu.H: contiene las funciones que
                          muestran y ejecutan las
                          operaciones de los menus

*******************************************************/

/** VARIABLES EXTERNAL **/
#define BEEP  Sound(1209,20)

/** DIRECTIVAS **/
#define MENU_START  0
#define MENU_ON     1
#define MENU_OFF    2
#define MENU_SETUP  3

  /** DIRECTIVAS SUBMENU SETUP**/
  #define OPT_SHOW_STA_IP     0
  #define OPT_SHOW_STA_SSID   1
  #define OPT_SHOW_AP_SSID    2
  #define OPT_SHOW_AP_KEY     3
  #define OPT_SHOW_AP_IP      4
  #define OPT_MODE_AP         5
  #define OPT_RESET_ALL       6
  #define OPT_BACKLIGH        7
  #define OPT_EXIT            8

    /** DIRECTIVAS SUBMENU CONF_ACCESS_RED **/
    #define MODE_CLIENT     0
    #define MODE_AP         1
    #define MODE_CLIENT_AP  2
    #define MODE_EXIT       3

//Contiene las opciones del menu principal [pos][lenString]
const char  optMenuStart[4][16]    =  { {"Encender Manual"},
                                        {"Apagado Manual "},
                                        {"Configuracion  "}
                                      };
                                      
//Contiene las opciones del menu Encender [pos][lenString]
char  optMenuOn[10][11]            =   {{"Output 1  "},
                                        {"Output 2  "},
                                        {"Output 3  "},
                                        {"Output 4  "},
                                        {"Output 5  "},
                                        {"Output 6  "},
                                        {"Output 7  "},
                                        {"Output 8  "},
                                        {"SALIR     "}
                                      };

//Contiene las opciones del menu Configuracion [pos][lenString]
const char  optMenuSetup[9][17]    =  { {"ESTACION IP     "},
                                        {"ESTACION SSID   "},
                                        {"AP SSID         "},
                                        {"AP KEY          "},
                                        {"AP IP           "},
                                        {"Conf Accesso Red"},
                                        {"Borrar a Fabrica"},
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
int8 countOptMenuSetup  = 8;
// Numero de opciones SubMenu Modo
int8 countOptMenuModo   = 3;


/***************************************************
   Carga el nombre de las opciones del menu On y Off
***************************************************/
void getNameMenusOutput(){
   int posIni = 0;
   int hasData = 0xFF;
   posIni = read_eeprom(0x00);
   
   if(posIni!=0xFF){
      posIni =0;
      for(int idx=0;idx<8;idx++){
         
         posIni =( 10 * idx ) + 40;
         hasData = read_eeprom(posIni);
         delay_ms(10);
         
         if(hasData!=0xFF){
            for(int idxChar=0;idxChar<=9;idxChar++){
               optMenuOn[idx][idxChar]=read_eeprom(posIni+idxChar);
               delay_ms(10);
            }
         }
      }
      
      optMenuOn[8][0]='\f';
      optMenuOn[8][1]='S';
      optMenuOn[8][2]='A';
      optMenuOn[8][3]='L';
      optMenuOn[8][4]='I';
      optMenuOn[8][5]='R';
   }   
} // FIn de la funcion getNameMenusOutput

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
         if(optSelected > countOptMenuStart){
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
  int exit=0;
  lcd_gotoxy(12,2);
  printf(lcd_putc, "SALIR");
  
  while(exit==0){
      /********************************************
      *  INVOCA LA FUNCION RUN_COMMAND_WIFI       *
      *********************************************/
      run_command_wifi();
      /********************************************/
      
     if(input(KEY_OK)==0){
           // Elimina el rebote
           delay_ms(500);
           exit=1;
           //break;
     }
  }
} // Fin de la funcion pressExit
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
   printf(lcd_putc, "\fOn -> %s",optMenuOn[optSelected]);
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
         printf(lcd_putc, "\fOn -> %s",optMenuOn[optSelected]);
         showBottonMenu();
      } // Fin KEY_NEXT
      
      // Finaliza la funcion y retorna la opcion del menu seleccionada
      if(input(KEY_OK)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         
         // Si optSelected = countOptMenuOn entonces esta en la pos SALIR
         if(optSelected == countOptMenuOn){
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
   printf(lcd_putc, "\fOff-> %s",optMenuOn[optSelected]);
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
         printf(lcd_putc, "\fOff-> %s",optMenuOn[optSelected]);
         showBottonMenu();
      } // Fin KEY_NEXT
      
      // Finaliza la funcion y retorna la opcion del menu seleccionada
      if(input(KEY_OK)==0)
      { 
         // Elimina el rebote
         delay_ms(500);
         BEEP;
         
         // Si optSelected = countOptMenuOn entonces esta en la pos SALIR
         if(optSelected == countOptMenuOff){
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
         if(optSelected > countOptMenuSetup){
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
         
         // Si optSelected = countOptMenuSetup entonces esta en la pos SALIR
         if(optSelected == countOptMenuSetup){
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
  
     case OPT_SHOW_AP_IP:
        fprintf(ESP8266, "AT+CIFSR\r\n");
        CMD_RUN = CMD_GET_IP_FROM_MENU;
        printf(lcd_putc, "\fCargando...");
        waitResp();
        printf(lcd_putc, "\f%s",buffer_Resp_CMD[0]);
        flag_Resp_Get_IP_CMD = 0;
        break;
        
     case OPT_SHOW_STA_IP:
        fprintf(ESP8266, "AT+CIFSR\r\n");
        CMD_RUN = CMD_GET_IP_FROM_MENU;
        printf(lcd_putc, "\fCargando...");
        waitResp();
        printf(lcd_putc, "\f%s",buffer_Resp_CMD[1]);
        flag_Resp_Get_IP_CMD = 0;
        break;
        
     case OPT_SHOW_STA_SSID:
        if(modeStar!='2'){
           fprintf(ESP8266, "AT+CWJAP?\r\n");
           CMD_RUN = CMD_GET_STA_INFO_FROM_MENU;
           printf(lcd_putc, "\fCargando...");
           waitResp();
           printf(lcd_putc, "\f%s",buffer_Resp_CMD[4]);
           flag_Resp_Get_STA_Info_CMD=0;
        }else{
           printf(lcd_putc, "\fDeshabilitado");
        }
        break;
        
     case OPT_SHOW_AP_SSID:
        if(modeStar!='1'){
           fprintf(ESP8266, "AT+CWSAP?\r\n");
           CMD_RUN = CMD_GET_AP_INFO_FROM_MENU;
           printf(lcd_putc, "\fCargando...");
           waitResp();
           printf(lcd_putc, "\f%s",buffer_Resp_CMD[2]);
           flag_Resp_Get_AP_Info_CMD=0;
        }else{
           printf(lcd_putc, "\fDeshabilitado");
        }
        break;
        
     case OPT_SHOW_AP_KEY:
        if(modeStar!='1'){
           fprintf(ESP8266, "AT+CWSAP?\r\n");
           CMD_RUN = CMD_GET_AP_INFO_FROM_MENU;
           printf(lcd_putc, "\fCargando...");
           waitResp();
           printf(lcd_putc, "\f%s",buffer_Resp_CMD[3]);
           flag_Resp_Get_AP_Info_CMD=0;
        }else{
           printf(lcd_putc, "\fDeshabilitado");
        }
        break;
        
     case OPT_RESET_ALL:
         printf(lcd_putc, "\fBorrando EEEPROM");
         for(int idxNC=0x00;idxNC<=0x6E;idxNC++){
            write_eeprom(idxNC,0xFF);
            delay_ms(10);
         }
         printf(lcd_putc, "\fBorrando RED");
         fprintf(ESP8266, "AT+CWQAP\n\r");
         delay_ms(1000);
         fprintf(ESP8266, "AT+RST\n\r");
         delay_ms(2000);
         fprintf(ESP8266, "AT+CWSAP_DEF=\"Kriver SmartHome\",\"kriver56106\",1,4\r\n");
         delay_ms(2000);
         reset_cpu();
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
        break;
        
     case OPT_EXIT:
        break;
        
  }
  
  if(optSelect!=OPT_EXIT && optSelect!=OPT_MODE_AP){
   pressExit();
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
  
  while(exit==0){
  
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
                 write_eeprom(0,'1');
                 delay_ms(10);
                 printf(lcd_putc, "\fConfig Cliente");
                 break;
              case MODE_AP:
                 fprintf(ESP8266,"AT+CWMODE_DEF=2\r\n");
                 write_eeprom(0,'2');
                 delay_ms(10);
                 printf(lcd_putc, "\fConfig AP");
                 break;
              case MODE_CLIENT_AP:
                 fprintf(ESP8266,"AT+CWMODE_DEF=3\r\n");
                 write_eeprom(0,'3');
                 delay_ms(10);
                 printf(lcd_putc, "\fConfig ClienteAP");
                 break;
             case MODE_EXIT:
                 break;
                 
           }
           
           if(optSelected!=MODE_EXIT){
              pressExit();
              printf(lcd_putc, "\fReiniciando");
              delay_ms(3000);
              reset_cpu();
              // Muestra la ultima opcion del menu seleccionada
              //printf(lcd_putc, "\f%s",optMenuModo[optSelected]);
              //showBottonMenu();
           }
        }
     } // Fin KEY_OK
  } // FIN While Infinito
} // Fin funcion setMenuModo
