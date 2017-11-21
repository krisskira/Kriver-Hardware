/** VARIABLES EXTERNAL **/
#define BEEP  Sound(1209,20)
/** DIRECTIVAS **/
#define OPT_SHOW_IP     0
#define OPT_SHOW_SSID   1
#define OPT_SHOW_KEY    2
#define OPT_MODE_AP     3
#define OPT_BACKLIGH    4
#define OPT_EXIT        5

void pressOk(void){
   while(1){
      if(input(KEY_OK)==0){
            // Elimina el rebote
            delay_ms(500);
            break;
      }
   }
}
/***************************************************
Dispacher function MENU_SETUP
***************************************************/

void executeSetup(int optSelect){
   switch(optSelect){
      
      case OPT_SHOW_IP:
         printf(lcd_putc, "\f192.168.0.1");
         lcd_gotoxy(1,2);
         printf(lcd_putc, "OK para SALIR");
         pressOk();
         break;
      case OPT_SHOW_SSID:
         lcd_gotoxy(1,2);
         printf(lcd_putc, "\fC0G3_UNE");
         lcd_gotoxy(1,2);
         printf(lcd_putc, "OK para SALIR");
         pressOk();         
         break;
      case OPT_SHOW_KEY:
         printf(lcd_putc, "\f1234Az!");
         lcd_gotoxy(1,2);
         printf(lcd_putc, "OK para SALIR");
         pressOk(); 
         break;
      case OPT_MODE_AP:
         lcd_gotoxy(1,2);
         printf(lcd_putc, "\fDisable Mode");
         lcd_gotoxy(1,2);
         printf(lcd_putc, "OK para SALIR");
         pressOk();
         break;
      case OPT_BACKLIGH:
         output_toggle(LCD_LIGHT_PIN);
         printf(lcd_putc, "\fOK para SALIR");
         pressOk();
         break;
      case OPT_EXIT:
         break;
         
   }

}
