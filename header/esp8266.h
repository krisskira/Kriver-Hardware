/****************************************************************\
* @NOTA:   Funcionamiento de las funciones de Busqueda y         *
*         comparacion de String.                                 *
*                                                                *
* Cuenta el numero de aciertos de manera secuencial cuando       *
* compara los caracteres que recibe por el usart vs una cadena   *
* constante. Si el numero de aciertos es igual a la cadena       *
* entonces las cadenas son iguales y almacena en una estructura  *
* los siguientes caracteres recibidos hasta que encuentre el     *
* caracter del fin de la cadena (":")                            *
\****************************************************************/

#define  CMD_MODE             1
#define  CMD_DO_CONN          2
#define  CMD_CONN             3
#define  READY_RESPONSE_WIFI  4
//#define  END_CMD_GET_IP       2

/* Declaracion de Funciones de respuestas */
void waitRespOK(int buffer);
void waitRespNoAp(int buffer);
void waitRespCWJAP(int buffer);
void waitRespConnected(int buffer);
void waitRespFail(int buffer);
int  waitResp(void);
void waitRespGetIP(char buffer);

/** BANDERAS MODOS DE FUNCIONAMIENTO **/
int CMD_RUN             = 0x00;

/* Flag usadas para conocer si la respuesta del comando ejecutado esta completo */
//int flag_Ready_Resp         = 0;
int flag_Resp_Valid         = 0;
int flag_Resp_NoAp          = 0;
int flag_Resp_CWJAP         = 0;
int flag_Resp_Connected     = 0;
int flag_Resp_Fail          = 0;
int flag_Resp_Error         = 0;

/* Flag para contadores tamaño de la respuesta */
int flag_Pos_Resp           = 0;
int flag_Pos_Resp_NoAp      = 0;
int flag_Pos_Resp_CWJAP     = 0;
int flag_Pos_Resp_Connected = 0;
int flag_Pos_Resp_Fail      = 0;
int flag_Pos_Resp_Error     = 0;

/*@TODO: Variables que se setean con la data de la EEPROM*/
char  modeStar;
char  SSIDAndKey[50];

/* Inicializa el modulo*/
void esp8266_init()
{
   int   resp;
  
   // @TODO: Usar este comando solo en produccion
   /**fprintf(ESP8266, "AT+RST\r\n");
   delay_ms(2000);**/
   
   // Lee de la EEPROM el modo de conexion
   // Si no ha sido configurado lo setea en '3'
   modeStar = read_eeprom(0x00);
   delay_ms(10);
   switch(modeStar){
      case '1':
      case '2':
      case '3':
         break;
      default:
         write_eeprom(0x00,'3');
         delay_ms(10);
         modeStar = '3';
         break;
   }
   // Lanza el comando para configurar el modo de conexion
   fprintf(ESP8266, "AT+CWMODE=%c\r\n",modeStar);
   CMD_RUN =  CMD_MODE;
   
   // Espera por la respuesta OK del modulo
   delay_ms(50);
   
   // Lanza el comando para verificar si elmodulo ya se encuentra conectado
   fprintf(ESP8266, "AT+CWJAP?\r\n");
   CMD_RUN =  CMD_DO_CONN;
   
   resp = waitResp();

   // Si el modulo no hizo una conexion automatica la realiza de forma manual
   // si no logra conectarse a una red entonces se autoconfigura en modo AP
   if(resp==2){
         
      // Recupera el SSIDAndKey desde la EEPROM
      int idxRE = 0;
      while(SSIDAndKey[idxRE]!=0x0A){
         SSIDAndKey[idxRE] = read_eeprom(idxRE+1);
         delay_ms(10);
         if(SSIDAndKey[idxRE]==0xFF){
            resp = 5;
            break;
         }
         if(SSIDAndKey[idxRE]==0x0A){
            break;
         }
         idxRE++;
      }
      // Lanza el comando para conectarce a la red
      if(resp!=5){
         fprintf(ESP8266, "AT+CWJAP=%s",SSIDAndKey);     /** AT+CWJAP="UNE_C06E","00986344200771" **/
         CMD_RUN = CMD_CONN;
         resp    = waitResp();
      }

      // Si no se establece una conexion se configura en modo AP
      if(resp==4 || resp==5){
         fprintf(ESP8266, "AT+CWMODE=%c\r\n","3");
         CMD_RUN = CMD_MODE;
         waitResp();
      }
   } // Fin del if(doConnect)
   
   // Configura el modulo como server
   fprintf(ESP8266, "AT+CIPMUX=1\r\n");
   delay_ms(1000);
   fprintf(ESP8266, "AT+CIPSERVER=1,80\r\n");
   delay_ms(1000);
   
   CMD_RUN = READY_RESPONSE_WIFI;
}

/********************************************************
Funcion encangada de procesar las respuestas del modulo, 
debe ser llamada durante la interrupcion del USART
*********************************************************/
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
                  
            case READY_RESPONSE_WIFI:
            default:
                  ESP8266_Get_IPD(buffer);
                  ESP8266_Get_CMD(buffer);
                  ESP8266_Get_Data(buffer);
            
                  CMD_RUN = 0x00;
                  break;
      } // Fin del switch (CMD_RUN)
} // Fin Funcion ESP8266_PROCCESS_RESPONSE()

/*********************************************************************
 *    Bloquea el programa en un while(1) esperando la interrupcion
 *    USART hasta que la bandera flag_Resp_Valid=1 que indica el fin
 *    del comando. retorna el valor de la constante de respuesta para
 *    el comando lanzado.
**********************************************************************/
int waitResp(void)
{
   int ret=0;
   // Si la bandera flag_Resp_Valid = 1 el_
   // fin del comando ESP8266 es OK 
   while(flag_Resp_Valid==0){
   
      if(flag_Resp_CWJAP==1){
      // Comando doConnected Conectado a la red
         flag_Pos_Resp_CWJAP  = 0;
         flag_Resp_CWJAP      = 0;
         ret = 1;

      } else if(flag_Resp_NoAp==1){
      // Comando doConnected No se ha asignado una red
         flag_Pos_Resp_NoAp  = 0;
         flag_Resp_NoAp      = 0;
         ret = 2;

      } else if (flag_Resp_Connected==1){
      // Comando Conexion establecida
         flag_Pos_Resp_Connected = 0;
         flag_Resp_Connected     = 0;
         ret = 3;

      } else if(flag_Resp_Fail==1){
      // Comando Conexion Fallo al intentar la Conexion
      // Esta respuesta da como resultado un ERROR entonces
      // Se rompe el ciclo de forma manual flag_Resp_Valid=1
         flag_Pos_Resp_Fail   = 0;
         flag_Resp_Fail       = 0;
         flag_Resp_Valid      = 1;
         ret = 4;
      } else if(flag_Resp_Error==1){
      // Comando Fallo
      // Esta respuesta da como resultado un ERROR entonces
      // Se rompe el ciclo de forma manual flag_Resp_Valid=1
         flag_Pos_Resp_Error  = 0;
         flag_Resp_Error      = 0;
         flag_Resp_Valid      = 1;
         ret = 5;
      }
   }; // Fin del loop
   flag_Resp_Valid = 0;
   CMD_RUN = 0x00;
   return ret;
} // Fin de la funcion waitResp

/******************************************************************
 * 
 *          ***    FUNCIONES DE VERIFICACION DE RESPUESTAS

El valor que es recibido como parametro lo compara con la
constante de final de comando ó de respuesta esperada del modulo
ESP8266 si este caracter es encontrado aumenta la posicion del 
siguiente caracter a leer, si el siguiente caracter recibido como 
parametro no coincide con el caracter esperado reinicia el contador
y vuelve a empezar la comparacion desde el inicio de la constante 
de final de comando o de respuesta esperada

POSCICION DE CARACTER:                        0   1   2    3
RESPUESTA ESPERADA:    ESP8266_RESP_OK[4] = {'O','K',0x0D,0x0A}

*******************************************************************/

void waitRespOK(int buffer)
{
   const char ESP8266_RESP_OK[4] ={'O','K',0x0D,0x0A};
   int lenResp = 4;
   
   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia
   if(buffer == ESP8266_RESP_OK[flag_Pos_Resp])
   {
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
      // Si el caracter no se encuentra dentro de la cadena de la constante
      // entonces reinicia las banderas de respuesta
      flag_Pos_Resp     = 0;
      // Como esta funcion es el ultimo caracter recibido no es necesario mantener 
      // Activa la bandera flag_Resp_Valid
      flag_Resp_Valid   = 0;
   }
} // Fin de la funcion waitRespOK()

/********************************************************
 *    Espera por la respuesta del comando "doConnected"
 *    ESP8266_RESP_NoAp (Sin conexion)
*********************************************************/
void waitRespNoAp(int buffer)
{
   const char ESP8266_RESP_NoAP[5] ={'N', 'o', ' ', 'A', 'P'};
   int lenResp = 5;
   
   if(buffer == ESP8266_RESP_NoAp[flag_Pos_Resp_NoAp] )
   {
      flag_Pos_Resp_NoAp++;
      
      if(flag_Pos_Resp_NoAp==lenResp){
         // Respuesta es valida
         flag_Resp_NoAp   = 1;
      }
   }else{
      // Reinicia el contador
      flag_Pos_Resp_NoAp  = 0;
   }
} // Fin de la funcion waitRespNoAp()

/********************************************************
 *    Espera por la respuesta del comando "doConnected"
 *    ESP8266_RESP_CWJAP (Conectado)
*********************************************************/
void waitRespCWJAP(int buffer)
{
   const char ESP8266_RESP_CWJAP[7] ={'+', 'C', 'W', 'J', 'A', 'P', ':'};
   int lenResp = 7;

   if(buffer == ESP8266_RESP_CWJAP[flag_Pos_Resp_CWJAP] )
   {
      flag_Pos_Resp_CWJAP++;
      
      if(flag_Pos_Resp_CWJAP==lenResp){
         // Respuesta es valida
         flag_Resp_CWJAP   = 1;
      }
   }else {
      // Reinicia el contador
      flag_Pos_Resp_CWJAP = 0;
   }
   
} // Fin de la funcion waitRespCWJAP()

/********************************************************
 *    Espera por la respuesta del comando Conexion
 *    ESP8266_RESP_Connected (Conectado)
*********************************************************/
void waitRespConnected(int buffer)
{
   const char ESP8266_RESP_Connected[9] ={'C','O','N','N','E','C','T','E','D'};
   int lenResp = 9;
 
   if(buffer == ESP8266_RESP_Connected[flag_Pos_Resp_Connected] )
   {
      flag_Pos_Resp_Connected++;
      
      if(flag_Pos_Resp_Connected==lenResp){
         // Respuesta es valida
         flag_Resp_Connected   = 1;
      }
   }else{
      // Reinicia el contador
      flag_Pos_Resp_Connected = 0;
   }
} // Fin de la funcion waitRespConnected()

/********************************************************
 *    Espera por la respuesta del comando Conexion
 *    ESP8266_RESP_FAIL (Fallo al intentar conectarse)
*********************************************************/
void waitRespFail(int buffer)
{
   const char ESP8266_RESP_FAIL[4] ={'F', 'A', 'I', 'L'};
   int lenResp = 4;
 
   if(buffer == ESP8266_RESP_FAIL[flag_Pos_Resp_Fail] )
   {
      flag_Pos_Resp_Fail++;
      
      if(flag_Pos_Resp_Fail==lenResp){
         // Respuesta es valida
         flag_Resp_Fail   = 1;
      }
   }else{
      // Reinicia el contador
      flag_Pos_Resp_Fail  = 0;
   }
} // Fin de la funcion waitRespFail()

/*********************************************************
 *    Espera por la respuesta del comando enviado desde
 *    el menu.
 *********************************************************/
/*int waitRespCmdFromMenu(void)
{
   int ret=0;
   // Si la bandera flag_Resp_Valid = 1 el_
   // fin del comando ESP8266 es OK 
   while(flag_Resp_Valid==0){
   
      if(ESP8266_CMD_FROM_MENU==END_CMD_GET_IP){
         flag_Pos_Resp_GetIP  = 0;
         flag_Resp_GetIP      = 0;
         ret = 1;
      } 
      
   }
   
   return ret;
}*/
/*********************************************************
 *    Espera por la respuesta del comando enviado desde
 *    el menu.
 *********************************************************/
/*void waitRespGetIP(char buffer)
{

   const char ESP8266_RESP_GET_IP[7] ={'+', 'C', 'W', 'J', 'A', 'P', ':'};
   int lenResp = 7;
 
   if(flag_Resp_GetIP == 1){
      if(buffer!=','){
         DATA_CMD_GET_IP[flag_Pos_Resp_GetIP] = buffer;
         flag_Pos_Resp_GetIP++;
      }else{        
         flag_Pos_Resp_GetIP = 0x00;
         flag_Resp_GetIP     = 0x00;
         
         // Reinicia la bandera para liberar la captura de datos
         // en la interrupcion
         ESP8266_CMD_FROM_MENU=END_CMD_GET_IP;
      }
   }else{
      if(buffer == ESP8266_RESP_GET_IP[flag_Pos_Resp_GetIP] )
      {
         flag_Pos_Resp_GetIP++;
         
         if(flag_Pos_Resp_GetIP==lenResp){
            // Respuesta es valida
            flag_Resp_GetIP   = 1;
         }
      }else{
         // Reinicia el contador
         flag_Pos_Resp_GetIP  = 0;
      }
   }  
}*/
