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

/******************************************\
*   Formato de comando en la respuesta:    *
*   INICIO:COMANDO:DATOS:<-(FIN)           *
*   e.g. CMD:CMD_SSID_AND_PASS,DATA:       *
\******************************************/

/*
+-----------------------------------------------+ 
|      ****  CMD:OP,[op][port]:                 |
|-----------------------------------------------|     
|> Realiaza Operaciones en el Puerto de Salida  |
|                                               |
|   @Sintax:                                    |
|      CMD:OP,[1|0][0-7]:                       |
|                                               |
|   @Param:                                     |
|      op:    1   -> Encender                   |
|             0   -> Apagar                     |
|      port:  0-7 -> No. del puerto.            |
|                                               |
|** @e.g. CMD:OP,10:  Enciende el PORT 0        | 
+-----------------------------------------------|
*/

/** VARIABLES DATOS GENERALES **/
char  ID_CONNETION[2];
char  COMMAND[2];
char  DATA_COMMAND[50];
int16 LEN_DATA_COMMAND     = 0;

/* Flag usadas para conocer si la respuesta del comando ejecutado esta completo */
int flag_Resp_CMD          = 0x00;
int flag_Resp_IPD          = 0x00;
int flag_Resp_StartGetData = 0x00;
int flag_Resp_Ready        = 0x00;

/* Flag para contadores tamaño de la respuesta */
int flag_Pos_Resp_CMD   = 0x00;
int flag_Pos_Resp_IPD   = 0x00;
int flag_Pos_Resp_Data  = 0x00;

/********************************************************
Funcion Busca la cadena de inicio recepcion de request, 
y retorna el ID de la conexion
*********************************************************/
void ESP8266_Get_IPD(char buffer[]){
   
   const char ESP8266_RESP_IPD[5] ={ '+','I', 'P', 'D', ','};
   int lenResp = 5;
  
   // Si el inicio la bandera de recepcion de comando esta
   // activa, almacena el siguiente caracter como identificador
   // de la conxion hasta que sea leido el caracter ":"
   if(flag_Resp_IPD==1){
      
      if(buffer!=','){
         ID_CONNETION[flag_Pos_Resp_IPD] = buffer;
         flag_Pos_Resp_IPD++;
      }else{
         flag_Pos_Resp_IPD       = 0x00;
         flag_Resp_IPD           = 0x00;
      }
   }else{
      // Verifica si los datos que se estan recibiendo son
      // son la secuencia recepcion de comando
      if(buffer == ESP8266_RESP_IPD[flag_Pos_Resp_IPD])
      {
         flag_Pos_Resp_IPD++;
         if(flag_Pos_Resp_IPD==lenResp){
            // Activa la bandera de inicio recepcion de 
            // comando y reinicia el contador
            flag_Resp_IPD     = 1;
            flag_Pos_Resp_IPD = 0;
         }
      }else{
         // Reinicia el contador
         flag_Pos_Resp_IPD  = 0;
      }
   } // Fin del if(flag_Resp_IPD)
} // Fin de la funcion ESP8266_Get_IPD()

/********************************************************
Funcion Busca la cadena de inicio comando, 
y retorna el Comando enviado, Activa la bandera
de recepcion de datos
*********************************************************/

void ESP8266_Get_CMD(char buffer[]){
   
   const char ESP8266_RESP_CMD[4] ={'C', 'M', 'D', ':'};
   int lenResp = 4;
  
   // Si el inicio la bandera de recepcion de comando esta
   // activa, almacena el siguiente caracter como identificador
   // de la conxion hasta que sea leido el caracter ":"
   if(flag_Resp_CMD==1){
   
      if(buffer!=','){
         COMMAND[flag_Pos_Resp_CMD] = buffer;
         flag_Pos_Resp_CMD++;
      }else{        
         flag_Pos_Resp_CMD = 0x00;
         flag_Resp_CMD     = 0x00;
         
         // Activa la recepcion de datos del comando
         flag_Resp_StartGetData  = 1;
      }
      
   }else{
      // Verifica si los datos que se estan recibiendo son
      // son la secuencia recepcion de comando
      if(buffer == ESP8266_RESP_CMD[flag_Pos_Resp_CMD])
      {
         flag_Pos_Resp_CMD++;
         
         if(flag_Pos_Resp_CMD==lenResp){
            // Activa la bandera de inicio recepcion de 
            // comando y reinicia el contador
            flag_Resp_CMD     = 1;
            flag_Pos_Resp_CMD = 0;
         }
      }else{
         // Reinicia el contador
         flag_Pos_Resp_CMD  = 0;
      }
   } // Fin del if(flag_Resp_IPD)
}

/********************************************************
Funcion Verifica si la bandera de recepcion de datos esta
activa y almacena los datos entrantes en el arreglo
DATA_COMMAND, Finaliza la captura de caracteres cuando
encuentra el caracter ":".
Envia al ESP8266 el fin de la conexion
*********************************************************/
void ESP8266_Get_Data(char buffer[]){
   
   // Si el inicio la bandera de recepcion de comando esta
   // activa, almacena lo que entra por el buffer como Data
   // hasta que sea leido el caracter ":"
   if(flag_Resp_StartGetData==1){
      if(buffer!=':'){
        
         DATA_COMMAND[flag_Pos_Resp_Data] = buffer;
         LEN_DATA_COMMAND = flag_Pos_Resp_Data;
         
         if(flag_Pos_Resp_Data==0 && buffer==','){
            flag_Pos_Resp_Data=0;
         }else{
            flag_Pos_Resp_Data++;
         }
         
      }else{
         flag_Pos_Resp_Data      = 0x00;
         flag_Resp_StartGetData  = 0x00;
         flag_Resp_Ready         = 1;
      }
   } // Fin del if(flag_Resp_StartGetData)
}

int run_command_wifi(void){
   int8 ret = 0, optSelected = 0;
   char ESP8266_Resp[13] = {'{','"','c','m','d','"',':','"','o','k','"','}'};
   // Si hay un comando y datos listos para ejecutar
   if(flag_Resp_Ready==1){
      
      // Accede a las Salidas 
      /** CMD:OP,[0|1][0-7]: **/
      if(COMMAND[0]=='O' &&
         COMMAND[1]=='P')
      {
         optSelected =(int) ( DATA_COMMAND[1] - 48);
         
         switch(DATA_COMMAND[0]){
            case '0':
               output_low(PIN_OUT[optSelected]);
               break;
            case '1':
               output_high(PIN_OUT[optSelected]);
               break;
         }
         
         ret = 1;
      } // Fin comando OP
      
      // Reinicia el buffer
      flag_Resp_Ready  = 0x00;
      LEN_DATA_COMMAND = 0x00;
      
      // Responde la solicitud
      delay_ms(20);
      fprintf(ESP8266, "AT+CIPSEND=%c,12\r\n",ID_CONNETION[0]);
      delay_ms(50);
      fprintf(ESP8266, "%s\r\n",ESP8266_Resp);
      delay_ms(100);
      fprintf(ESP8266, "AT+CIPCLOSE=%c\r\n",ID_CONNETION[0]);
      
      // Limpia el buffer
      for(int idx=0;idx<=LEN_DATA_COMMAND;idx++){
         DATA_COMMAND[idx] ==0x00;
      }
      COMMAND[0]        ==0x00;
      COMMAND[1]        ==0x00;
      ID_CONNETION[0]   ==0x00;
   } // Fin del if(flag_Resp_Ready)
   
   return ret;
}
