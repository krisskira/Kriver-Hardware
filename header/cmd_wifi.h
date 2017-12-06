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
*   e.g. CMD:CMD_SSID_AND_PASS:DATA:       *
\******************************************/

/** VARIABLES DATOS GENERALES **/
char  ID_CONNETION[];
char  COMMAND[5];
char  DATA_COMMAND[];

/* Flag usadas para conocer si la respuesta del comando ejecutado esta completo */
int flag_Resp_CMD          = 0x00;
int flag_Resp_IPD          = 0x00;
int flag_Resp_StartGetData = 0x00;

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
      
      switch(buffer){
         case '@':
            flag_Pos_Resp_CMD = 0x00;
            flag_Resp_CMD     = 0x00;
            fprintf(console,"%s",COMMAND);
            
            // Activa la recepcion de datos del comando
            flag_Resp_StartGetData  = 1;
            break;
         default:
            COMMAND[flag_Pos_Resp_CMD] = buffer;
            flag_Pos_Resp_CMD++;
            break;
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
         flag_Pos_Resp_Data++;
      }else{
         fprintf(console,"Comando: %s \n",DATA_COMMAND);
         flag_Pos_Resp_Data      = 0x00;
         flag_Resp_StartGetData  = 0x00;
      }
   } // Fin del if(flag_Resp_StartGetData)
}