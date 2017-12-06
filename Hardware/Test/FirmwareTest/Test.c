#include <Test.h>
#include <string.h>

char END_CMD_OK []={0x4F, 0x4B, 0x0D, 0x0A};
char response[4];
int pos=0,jpg_valido=0;
/** ESTRUCTURA DE CONFIGURACION ESP8266 **/

#INT_RDA
void  RDA_isr(void) 
{
   char dato[];
 dato=getc();
 fputc(dato,console);
 char dato[];
 dato=getc();
 char *retorno;
 retorno = strstr(response,END_CMD_OK);
 if(strcmp(END_CMD_OK[pos],dato) == 0){
   response[pos] = dato;
   pos++;
   
   if(strstr(END_CMD_OK,response)==0x00) {
      jpg_valido=0;
   }else{
      jpg_valido=1;
   }
 }else{
   pos=0;
   response="";
 }
 
}

void main()
{
   set_tris_a(0x00);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   
   char cmd[] = "1";
   char modo[] = "AT+CWMODE=";
   strcat(modo,cmd);
   fprintf(ESP8266,"%s\r\n",modo);
   char modo[] = "AT";
   char texto = "Start: ";
   fputc(texto,console);
   while(TRUE)
   {
      fprintf(ESP8266,"%s\r\n",modo);
      delay_ms(5000);
      
      if(input(PIN_A0)==0){
         delay_ms(500);
         fprintf(VIRTUAL,"Cadena: %s",response);
      }
      
      if(input(PIN_A1)==0){
         delay_ms(500);
         //fputc(CWMODE, VIRTUAL);
         for(int idx=0;idx<11;idx++){
            fputc(modo[idx], ESP8266);
         }
         fputc(0x0D, ESP8266);
         fputc(0x0A, ESP8266);
      }
   }

}