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

/* Key flag responses */
const int   Valid      = 0;
const int   CWJAP      = 1;
const int   NoAp       = 2;
const int   Connected  = 3;
const int   Fail       = 4;
const int   Error      = 5;

//#define  END_CMD_GET_IP       2

/* Declaracion de Funciones de respuestas */
int  waitResp(void);
//void waitRespGetIP(char buffer);
void waitRespMajor(int buffer,int flag_resp);
/** BANDERAS MODOS DE FUNCIONAMIENTO **/
int CMD_RUN    = 0x00;
int resp_Pos   = 1;
int resp_Flag  = 0;
int countResp  = 5;     //Conteo de Respuestas esperadas

char buffer_flag_Resp[2][6]   =  
{
   {0,0,0,0,0,0}, // Flag_Responses Command valid
   {0,0,0,0,0,0}  // Flag_Pos_Responses
};


/*buffer_flag_Resp[resp_Flag][1] = 5;*/
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
                  waitRespMajor(buffer,Valid);
                  break;

            case CMD_DO_CONN:
                  // Evalua si la respuesta del ESP8266 
                  // es "No AP" o "CWJAP"  
                  waitRespMajor(buffer,NoAp);
                  waitRespMajor(buffer,CWJAP);
                  waitRespMajor(buffer,Valid);
                  break;

            case CMD_CONN:
                  waitRespMajor(buffer,Connected);
                  waitRespMajor(buffer,Fail);
                  waitRespMajor(buffer,Valid);
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
   int ret=0,idx=0;
   // Si la bandera flag_Resp_Valid = 1 el_
   // fin del comando ESP8266 es OK 
   while(buffer_flag_Resp[resp_Flag][Valid]==0){
      idx++;
      if(buffer_flag_Resp[resp_Flag][idx]==1){
         buffer_flag_Resp[resp_Pos][idx]   = 0;
         buffer_flag_Resp[resp_Flag][idx]  = 0;
        
         if(idx == Fail || idx == Error){
            buffer_flag_Resp[resp_Flag][Valid]  = 1;
         }
         ret = idx;
      }
      // Reinicia el contador para no desbordar el array de flag_resp
      if(idx>=countResp){
         idx = 0;
      }
      
   }; // Fin del loop
   
   buffer_flag_Resp[resp_Flag][Valid] = 0;
   CMD_RUN = 0x00;
   delay_ms(10);
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
void waitRespMajor(int buffer,int flag_resp)
{
   
   char  strResp[10];
   int   lenResp = 0,restartFlag = 0;

   switch(flag_resp)
   {
      case Valid:
         strResp = "OK\r\n";
         lenResp = 4;
         // Como esta funcion es el ultimo caracter recibido no es necesario mantener
         // Activa la bandera flag_Resp_Valid
         restartFlag = 1;
         break;

      case NoAp:
         strResp = "No Ap";
         lenResp = 5;
         break;

      case CWJAP:
         strResp = "+CWJAP:";
         lenResp = 7;
         break;
         
      case Connected:
         strResp = "CONNECTED";
         lenResp = 9;
         break;
      
      case Fail:
         strResp = "FAIL";
         lenResp = 4;
         restartFlag = 1;
         break;
   }

   // Si el caracter pasado por argumento se encuentra en la posicion de la 
   // constante de respuesta entonces aumenta la bandera de lo contrario la
   // reinicia

   if(buffer == strResp[buffer_flag_Resp[resp_Pos][flag_resp]])
   {
      // Aumenta el valor de la bandera hasta que alcance el tamaño del buffer 
      // de respuesta, cuando esto suscede indica que la respuesta es valida
      buffer_flag_Resp[resp_Pos][flag_resp]++;
      
      if(buffer_flag_Resp[resp_Pos][flag_resp]==lenResp){
         // Respuesta es valida
         buffer_flag_Resp[resp_Flag][flag_resp]  = 1;
      }else if(flag_resp == Valid) {
         // Reinicia la bandera para indicar el final del comando
          //buffer_flag_Resp[resp_Flag][flag_resp] = 0;
      }
       
   }else{
      // Si el caracter no se encuentra dentro de la cadena de la constante
      // entonces reinicia las banderas de respuesta
      buffer_flag_Resp[resp_Pos][flag_resp]   = 0;

      // Banderas que se reinician
      if(restartFlag == 1){
         buffer_flag_Resp[resp_Flag][flag_resp] = 0;
      }
      
   }
}

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
