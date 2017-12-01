#define  CMD_MODE    1
#define  CMD_DO_CONN 2
#define  CMD_CONN    3

/* Constantes de respuestas */
const char ESP8266_RESP_OK[4]           ={'O','K',0x0D,0x0A};
const char ESP8266_RESP_NoAP[5]         ={'N', 'o', ' ', 'A', 'P'};
const char ESP8266_RESP_CONNECTED[9]    ={'C','O','N','N','E','C','T','E','D'};

int   CMD_RUN = 0x00;

/* Buffer de response ESP8266 */
char bufferRespOK[4];
char bufferRespNoAp[5];

/* Declaracion de Funciones */
int   setupMode(char modeVal[]);
int   setupConn(char ssid[], char key[]);
void  waitRespOK(int buffer);
void  waitRespNoAp(int buffer);
void  waitRespConnected(int buffer);
void  waitResp(void);
void  reset(void);

/* Variables Globales */
int flag_Resp_Valid         = 0;
int flag_Resp_NoAp          = 0;
int flag_Resp_Connected     = 0;
int flag_Pos_Resp           = 0;
int flag_Pos_Resp_NoAp      = 0;
int flag_Pos_Resp_Connected = 0;

/* Inicializa el modulo*/
void esp8266_init()
{
   printf(lcd_putc, "\fInicializando...");
   
   /*@TODO: Variables que se setean con la data de la EEPROM*/
   char  modoVal[]   = "1";
   char  ssid[]      = {'"','U','N','E','_','C','0','6','E','"'};
   char  key[]       = {'"','0','0','9','8','6','3','4','4','2','0','0','7','7','1','"'};
   int   lenSSID     = 10;
   int   lendKey     = 16;

   // @TODO: Usar este comando solo en produccion
   /**reset();
   delay_ms(2000);**/
   
   CMD_RUN =  setupMode(modoVal);
   waitResp();
   delay_ms(500);
   CMD_RUN =  doConnected();
   waitResp();
   delay_ms(500);
   
}
/**
 * 
 * Funcion encangada de procesar las respuestas del modulo, debe ser llamada
 * durante la interrupcion del USART
 * 
 **/
void ESP8266_PROCCESS_RESPONSE(int buffer){
    // Seleciona la funcion de confirmacion de la respuesta
    switch(CMD_RUN){
        
                 case CMD_MODE:
                    waitRespOK(buffer);
                    break;
     
                 case CMD_DO_CONN:
                       waitRespNoAp(buffer);
                       waitRespConnected(buffer);
                       waitRespOK(buffer);
                       break;

                 case CMD_CONN:
                       waitRespOK(buffer);
                       break;
              
                 default:
                       CMD_RUN = 0x00;
                       break;
           }
}

/* reinicia el modulo para tener sincronia */
void reset(void)
{
   char  mode[]   = "AT+RST";   // Comando AT
   int   lenCMD   = 6;          // Tama�o del string del comando
   
   // Lanza el comando AT
   for(int idx=0;idx<lenCMD;idx++){
      fputc(mode[idx], ESP8266);
   }
   
   // Finaliza el comando con \r\n
   fputc(0x0D, ESP8266);
   fputc(0x0A, ESP8266);
} // fin de la funcion reset()

/**
 * 
 *  Configura el modo de funcionamiento
 *  1-Cliente, 2-AccessPoint, 3-AP+STA 
 * 
 **/

int setupMode(char modeVal[])
{
   char  mode[]   = "AT+CWMODE=";   // Comando AT
   int   lenCMD   = 11;             // Tama�o del string del comando
   
   // Concatena el comando junto con el valor a setear
   strcat(mode,modeVal);
   
   // Lanza el comando AT
   for(int idx=0;idx<lenCMD;idx++){
      fputc(mode[idx], ESP8266);
   }
   
   // Finaliza el comando con \r\n
   fputc(0x0D, ESP8266);
   fputc(0x0A, ESP8266);
   
   // Retorna Una constante que hace referencia al comando ejecutado
   return CMD_MODE;
} // fin de la funcion setupMode()

/**
 * 
 *  lanza el comando para verificar si el modulo
 *  se encuentra conectado a la red wifi 
 * 
 **/

int doConnected(){
   char  command[]   = "AT+CWJAP?";   // Comando AT
   int   lenCMD      = 9;             // Tama�o del string del comando
   //AT+CWJAP="UNE_C06E","00986344200771"

   // Lanza el comando AT
   for(int idx=0;idx<lenCMD;idx++){
      fputc(command[idx], ESP8266);
   }
   
   // Finaliza el comando con \r\n
   fputc(0x0D, ESP8266);
   fputc(0x0A, ESP8266);
   
   // Retorna Una constante que hace referencia al comando ejecutado
   return CMD_DO_CONN;
}

/**
 * 
 *  Conecta a la red wifi
 * 
 **/

int setupConn(char ssid[], char key[])
{
   char  command[]   = "AT+CWMODE=";   // Comando AT
   int   lenCMD      = 11;             // Tama�o del string del comando
   
   // Concatena el comando junto con el valor a setear
   strcat(command,modeVal);
   
   // Lanza el comando AT
   for(int idx=0;idx<lenCMD;idx++){
      fputc(command[idx], ESP8266);
   }
   
   // Finaliza el comando con \r\n
   fputc(0x0D, ESP8266);
   fputc(0x0A, ESP8266);
   
   // Retorna Una constante que hace referencia al comando ejecutado
   return CMD_CONN;
} // fin de la funcion setupConn()


/* 
   Espera hasta que la respuesta sea completada, si la respuesta es completada
   retorna 1 de lo contrario siempre retornara 0, es llamada en cada interrupcion 
   del usart
*/

void waitRespOK(int buffer)
{
   int lenResp = 4;  // Tama�o del buffer de respuesta esperado
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia
   if(buffer == ESP8266_RESP_OK[flag_Pos_Resp] )
   {
      // Almacena el caracter en el buffer de respuesta
      bufferRespOK[flag_Pos_Resp] = buffer;
      // Aumenta el valor de la bandera hasta que alcance el tama�o del buffer 
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      flag_Pos_Resp++;
      
      if(flag_Pos_Resp==lenResp){
         // Respuesta es valida
         flag_Resp_Valid   = TRUE;
      }else{
         // Reinicia la bandera para asegurar la integridad de la respesta
         flag_Resp_Valid   = FALSE;
      }

   }else{
      // Reinicia la bandera para asegurar la integridad de la respesta y
      // asegura que la respuesta optenida no es valida, basea el buffer de
      // respuesta
      flag_Pos_Resp     = 0;
      flag_Resp_Valid   = 0;
      bufferRespOK[0]   = 0x00;
      bufferRespOK[1]   = 0x00;
      bufferRespOK[2]   = 0x00;
      bufferRespOK[3]   = 0x00;
   }
} // Fin de la funcion waitRespOK()

/**
   Espera hasta que la respuesta sea completada, si la respuesta es completada
   retorna 1 de lo contrario siempre retornara 0, debe ser llamada en cada interrupcion 
   del usart cuando se lanza el comando "doConnected"
**/
void waitRespNoAp(int buffer)
{
   int lenResp = 5;  // Tama�o del buffer de respuesta esperado
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia
   if(buffer == ESP8266_RESP_NoAp[flag_Pos_Resp_NoAp] )
   {
      // Almacena el caracter en el buffer de respuesta
      ESP8266_RESP_NoAp[flag_Pos_Resp_NoAp] = buffer;
      // Aumenta el valor de la bandera hasta que alcance el tama�o del buffer 
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      flag_Pos_Resp_NoAp++;
      
      if(flag_Pos_Resp_NoAp==lenResp){
         // Respuesta es valida
         flag_Resp_NoAp   = TRUE;
      }else{
         // Reinicia la bandera para asegurar la integridad de la respesta
         flag_Resp_NoAp   = FALSE;
      }

   }else{
      // Reinicia la bandera para asegurar la integridad de la respesta y
      // asegura que la respuesta optenida no es valida, basea el buffer de
      // respuesta
      flag_Pos_Resp_NoAp     = 0;
      flag_Resp_NoAp         = 0;
      ESP8266_RESP_NoAp[0]   = 0x00;
      ESP8266_RESP_NoAp[1]   = 0x00;
      ESP8266_RESP_NoAp[2]   = 0x00;
      ESP8266_RESP_NoAp[3]   = 0x00;
      ESP8266_RESP_NoAp[4]   = 0x00;
   }
} // Fin de la funcion waitRespNoAp()

/* Bloque el programa hasta que la bandera flag_Resp_Valid=1 utilizada para
   esperar por una respuesta valida */
void waitResp(void){
   while(!flag_Resp_Valid);
   flag_Resp_Valid = FALSE;
}

