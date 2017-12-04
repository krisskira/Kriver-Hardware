#define  CMD_MODE    1
#define  CMD_DO_CONN 2
#define  CMD_CONN    3

// Esta bandera indica que comando fue lanzado
int   CMD_RUN = 0x00;

/* Constantes de respuestas */
const char ESP8266_RESP_OK[4]             ={'O','K',0x0D,0x0A};
const char ESP8266_RESP_NoAP[5]           ={'N', 'o', ' ', 'A', 'P'};
const char ESP8266_RESP_CWJAP[7]          ={'+', 'C', 'W', 'J', 'A', 'P', ':'};
const char ESP8266_RESP_Connected[9]      ={'C','O','N','N','E','C','T','E','D'};
const char ESP8266_RESP_FAIL[4]           ={'F', 'A', 'I', 'L'};

/* Buffer de response ESP8266 */
char bufferRespOK[4];
/*char bufferRespNoAp[5];
char bufferRespCWJAP[7];
char bufferRespConneted[9];
char bufferRespFail[4];*/

/* Declaracion de Funciones de comandos */
void  reset(void);
int   setupMode(char modeVal[]);
int   doConnected();
int   setupConn(char SSIDAndKey[]);
int   setupServer(void);

/* Declaracion de Funciones de respuestas */
void  waitRespOK(int buffer);
void  waitRespNoAp(int buffer);
void  waitRespCWJAP(int buffer);
void  waitRespConnected(int buffer);
void  waitRespFail(int buffer);
int   waitResp(void);

/* Flag usadas para conocer si la respuesta del comando ejecutado esta completo */
int flag_Resp_Valid         = 0;
int flag_Resp_NoAp          = 0;
int flag_Resp_CWJAP         = 0;
int flag_Resp_Connected     = 0;
int flag_Resp_Fail          = 0;

/* Flag para contadores tamaño de la respuesta */
int flag_Pos_Resp           = 0;
int flag_Pos_Resp_NoAp      = 0;
int flag_Pos_Resp_CWJAP     = 0;
int flag_Pos_Resp_Connected = 0;
int flag_Pos_Resp_Fail      = 0;

/*@TODO: Variables que se setean con la data de la EEPROM*/
char  modeStar[]  = "1";
char  SSIDAndKey[]= {'"','U','N','E','_','C','0','6','E','"',
                     ',',
                     '"','0','0','9','8','6','3','4','4','2','0','0','7','7','1','"',
                     0x0D,0x0A
                    };

/* Inicializa el modulo*/
void esp8266_init()
{
   char  modeWithoutConnect[] = "3";
   int   resp;
   
   /*fprintf(console, "Inicializando ESP8266");
   fputc(0x0D,console);*/

   // @TODO: Usar este comando solo en produccion
   /**reset();
   delay_ms(2000);**/

   CMD_RUN =  setupMode(modeStar);
   waitResp();

   CMD_RUN =  doConnected();
   
   resp = waitResp();
   /*fprintf(console, "Respuesta: %u",resp);
   fputc(0x0D,console);*/

   // Si el modulo no hizo una conexion automatica la realiza de forma manual
   // si no logra conectarse a una red entonces se autoconfigura en modo AP
   if(resp==2){
   
      /*fprintf(console, "Iniciando Conexion");
      fputc(0x0D,console);*/
      
      // Lanza el comando para conectarce a la red
      CMD_RUN = setupConn(SSIDAndKey);
      resp    = waitResp();
      
      /*fprintf(console, "Segunda Respuesta: %u",resp);
      fputc(0x0D,console);*/
      
      // Si no se establece una conexion se configura en modo AP
      if(resp==4){
      
         /*fprintf(console, "Configurando Modo AP");
         fputc(0x0D,console);*/
         
         CMD_RUN =  setupMode(modeWithoutConnect);
         waitResp();
         
         /*fprintf(console, "Modo AP Activado"); 
         fputc(0x0D,console);*/
         
      }
   } // Fin del if(doConnect)
  
   /** @TODO: Rutina que se encarga de recibir datos desde ESP8266 **/
   setupServer();
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
                  // Evalua si la respuesta del ESP8266 
                  // es "No AP" o "CWJAP"  
                  waitRespNoAp(buffer);
                  waitRespCWJAP(buffer);
                  waitRespOK(buffer);
                  break;

            case CMD_CONN:
                  waitRespConnected(buffer);
                  waitRespFail(buffer);
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
   fprintf(ESP8266, "AT+RST\r\n");
} // fin de la funcion reset()

/**
 * 
 *  Configura el modo de funcionamiento
 *  1-Cliente, 2-AccessPoint, 3-AP+STA 
 * 
 **/

int setupMode(char modeVal[])
{
   fprintf(ESP8266, "AT+CWMODE=%s\r\n",modeVal );
   return CMD_MODE;
} // fin de la funcion setupMode()

/**
 * 
 *  lanza el comando para verificar si el modulo
 *  se encuentra conectado a la red wifi 
 * 
 **/

int doConnected()
{
   fprintf(ESP8266, "AT+CWJAP?\r\n");
   return CMD_DO_CONN;
}

/**
 * 
 *  @TODO: Conecta a la red wifi
 *         AT+CWJAP="UNE_C06E","00986344200771"
 * 
 **/

int setupConn(char SSID_And_Key[])
{
   fprintf(ESP8266, "AT+CWJAP=%s",SSID_And_Key );
   return CMD_CONN;
} // fin de la funcion setupConn()

/**
 * 
 *  Activa el modo multiconexi�n
 *       
 * 
 **/

int setupServer(void)
{
   fprintf(ESP8266, "AT+CIPMUX=1\r\n");
   delay_ms(1000);
   fprintf(ESP8266, "AT+CIPSERVER=1,80\r\n");
   delay_ms(1000);
   return CMD_CONN;
} // fin de la funcion setupConn()


/**
   Espera hasta que la respuesta sea completada, si la respuesta es completada
   retorna 1 de lo contrario siempre retornara 0, es llamada en cada interrupcion 
   del USART
**/

void waitRespOK(int buffer)
{
   int lenResp = 4;  // Tamaño del buffer de respuesta esperado
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia
   if(buffer == ESP8266_RESP_OK[flag_Pos_Resp] )
   {
      // Almacena el caracter en el buffer de respuesta
      bufferRespOK[flag_Pos_Resp] = buffer;
      // Aumenta el valor de la bandera hasta que alcance el tamaño del buffer 
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      flag_Pos_Resp++;
      
      if(flag_Pos_Resp==lenResp){
         // Respuesta es valida
         flag_Resp_Valid   = 1;
      }else{
         // Reinicia la bandera para asegurar la integridad de la respesta
         flag_Resp_Valid   = 0;
      }

   }else{
      // Reinicia la bandera para asegurar la integridad de la respesta y
      // asegura que la respuesta optenida no es valida, limpia el buffer de
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
   del USART cuando se lanza el comando "doConnected"
**/
void waitRespNoAp(int buffer)
{
   int lenResp = 5;  // Tamaño del buffer de respuesta esperado
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia
   if(buffer == ESP8266_RESP_NoAp[flag_Pos_Resp_NoAp] )
   {
      // Almacena el caracter en el buffer de respuesta
      ESP8266_RESP_NoAp[flag_Pos_Resp_NoAp] = buffer;
      
      // Aumenta el valor de la bandera hasta que alcance el tamaño del buffer
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      flag_Pos_Resp_NoAp++;
      
      if(flag_Pos_Resp_NoAp==lenResp){
         // Respuesta es valida
         flag_Resp_NoAp   = 1;
      }
   }else{
      // Reinicia la bandera para asegurar la integridad de la respesta y
      // asegura que la respuesta optenida no es valida, basea el buffer de
      // respuesta
      flag_Pos_Resp_NoAp  = 0;
   }
} // Fin de la funcion waitRespNoAp()

/**
   Espera hasta que la respuesta sea completada, si la respuesta es completada
   retorna 1 de lo contrario siempre retornara 0, debe ser llamada en cada interrupcion 
   del USART cuando se lanza el comando "doConnected"
**/
void waitRespCWJAP(int buffer)
{
   int lenResp = 7;  // Tamaño del buffer de respuesta esperado
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera
   if(buffer == ESP8266_RESP_CWJAP[flag_Pos_Resp_CWJAP] )
   {
      // Almacena el caracter en el buffer de respuesta
      ESP8266_RESP_CWJAP[flag_Pos_Resp_CWJAP] = buffer;
     
      // Aumenta el valor de la bandera hasta que alcance el tamaño del buffer
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      flag_Pos_Resp_CWJAP++;
      
      if(flag_Pos_Resp_CWJAP==lenResp){
         // Respuesta es valida
         flag_Resp_CWJAP   = 1;
      }
   }else {
      flag_Pos_Resp_CWJAP = 0;
   }
   
} // Fin de la funcion waitRespCWJAP()

/**
   Espera hasta que la respuesta sea completada, si la respuesta es completada
   retorna 1 de lo contrario siempre retornara 0, debe ser llamada en cada interrupcion 
   del USART cuando se lanza el comando "doConnected"
**/

void waitRespConnected(int buffer)
{
   int lenResp = 9;  // Tamaño del buffer de respuesta esperado
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia
   if(buffer == ESP8266_RESP_Connected[flag_Pos_Resp_Connected] )
   {
      // Almacena el caracter en el buffer de respuesta
      ESP8266_RESP_Connected[flag_Pos_Resp_Connected] = buffer;
      // Aumenta el valor de la bandera hasta que alcance el tamaño del buffer 
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      flag_Pos_Resp_Connected++;
      
      if(flag_Pos_Resp_Connected==lenResp){
         // Respuesta es valida
         flag_Resp_Connected   = 1;
      }

   }else{
      // Reinicia la bandera para asegurar la integridad de la respesta y
      // asegura que la respuesta optenida no es valida, limpia el buffer de
      // respuesta
      flag_Pos_Resp_Connected = 0;
   }
} // Fin de la funcion waitRespConnected()

/**
   Espera hasta que la respuesta sea completada, si la respuesta es completada
   retorna 1 de lo contrario siempre retornara 0, debe ser llamada en cada interrupcion 
   del USART cuando se lanza el comando "setupConn"
**/

void waitRespFail(int buffer)
{
   int lenResp = 4;  // Tamaño del buffer de respuesta esperado
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia
   if(buffer == ESP8266_RESP_Fail[flag_Pos_Resp_Fail] )
   {
      // Almacena el caracter en el buffer de respuesta
      ESP8266_RESP_Fail[flag_Pos_Resp_Fail] = buffer;
      // Aumenta el valor de la bandera hasta que alcance el tamaño del buffer 
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      flag_Pos_Resp_Fail++;
      
      if(flag_Pos_Resp_Fail==lenResp){
         // Respuesta es valida
         flag_Resp_Fail   = 1;
      }
   }else{
      // Reinicia la bandera para asegurar la integridad de la respesta y
      // asegura que la respuesta optenida no es valida, limpia el buffer de
      // respuesta
      flag_Pos_Resp_Fail  = 0;
   }
} // Fin de la funcion waitRespFail()

/* Bloquea el programa hasta que la bandera flag_Resp_Valid=1 utilizada para
   esperar por una respuesta valida */
int waitResp(void)
{
   int ret=0;
   while(flag_Resp_Valid==0){
   
      if(flag_Resp_CWJAP==1){
      
         /*fprintf(console, "Inicio Conectado en la red\n");
         fputc(0x0D,console);*/
         
         // Reinicia la bandera para asegurar la integridad de la respesta y
         // asegura que la respuesta optenida no es valida, limpia el buffer de
         // respuesta
         flag_Pos_Resp_CWJAP  = 0;
         flag_Resp_CWJAP      = 0;
         
         /*bufferRespCWJAP[0]   = 0x00;
         bufferRespCWJAP[1]   = 0x00;
         bufferRespCWJAP[2]   = 0x00;
         bufferRespCWJAP[3]   = 0x00;
         bufferRespCWJAP[4]   = 0x00;
         bufferRespCWJAP[5]   = 0x00;
         bufferRespCWJAP[6]   = 0x00;*/
         ret = 1;
      } else if(flag_Resp_NoAp==1){
         /*
         fprintf(console, "Preparando para conectar");
         fputc(0x0D,console);*/
         flag_Pos_Resp_NoAp  = 0;
         flag_Resp_NoAp      = 0;
      
         /*bufferRespNoAp[0]   = 0x00;
         bufferRespNoAp[1]   = 0x00;
         bufferRespNoAp[2]   = 0x00;
         bufferRespNoAp[3]   = 0x00;
         bufferRespNoAp[4]   = 0x00;*/
         ret = 2;
      } else if (flag_Resp_Connected==1){
      
         /*fprintf(console, "Conectado por comandos en la red");
         fputc(0x0D,console);*/
         
         flag_Pos_Resp_Connected = 0;
         flag_Resp_Connected     = 0;
         
         /*bufferRespConneted[0]   = 0x00;
         bufferRespConneted[1]   = 0x00;
         bufferRespConneted[2]   = 0x00;
         bufferRespConneted[3]   = 0x00;
         bufferRespConneted[4]   = 0x00;
         bufferRespConneted[5]   = 0x00;
         bufferRespConneted[6]   = 0x00;
         bufferRespConneted[7]   = 0x00;
         bufferRespConneted[8]   = 0x00;*/
         ret = 3;
      } else if(flag_Resp_Fail==1){
      
         /*fprintf(console, "No se pudo conectar a la red");
         fputc(0x0D,console);**/
         
         flag_Pos_Resp_Fail   = 0;
         flag_Resp_Fail       = 0;
         // Rompe el ciclo
         flag_Resp_Valid      = 1;
         
         /*bufferRespFail[0]    = 0x00;
         bufferRespFail[1]    = 0x00;
         bufferRespFail[2]    = 0x00;
         bufferRespFail[3]    = 0x00;*/
         ret = 4;
      }
   };
   flag_Resp_Valid = 0;
   return ret;
}
