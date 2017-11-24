/*

   ****  AT+CWLAP\r\n            : Este comando nos proporciona una lista con las redes WiFi disponibles e informaci�n adicional de estas.
   ****  41542B43574C41500D0A    : hexadecimal
   
Respuesta del comando AT+CWLAP
==============================================================
AT+CWLAP
+CWLAP:(3,"HOME-C4C9",-79,"cc:35:40:55:c4:c9",1)
+CWLAP:(4,"UNE_C06E",-42,"c4:27:95:89:60:cf",1)
+CWLAP:(3,"SAMUEL",-89,"80:c6:ab:5f:97:72",11)
+CWLAP:(3,"UNE4GREVOLUTION703147",-88,"24:db:ac:58:47:95",2)
+CWLAP:(1,"60172707",-87,"90:00:4e:2b:f7:74",11)
+CWLAP:(3,"DTVNET_D61792",-59,"e4:3e:d7:d6:17:92",4)
+CWLAP:(3,".FLIA-SOLARTE",-94,"14:ab:f0:1d:48:f0",11)
+CWLAP:(4,"UNE_62D5",-93,"e0:88:5d:51:9f:81",6)
+CWLAP:(4,"UNE_HFC_3B30",-70,"ac:20:2e:3a:3b:38",6)
+CWLAP:(3,"FLIA NAZARI",-93,"44:32:c8:35:0e:3f",6)

OK
===============================================================
El comando responde con una cadena de caracteres que incluye:
+CWLAP: <ecn>, <ssid>, <rssi>, <mac>, <canal>  OK
 <ecn>:
0 - Red abierta (sin contrase�a).
1 - Red con contrase�a WEP.
2 - Red con contrase�a WPA_PSK.
3 - Red con contrase�a WPA2_PSK
4 - Red con contrase�a WPA_WPA2_PSK
<ssid>:
String con el nombre de la Red.
<rssi>:  
 String con el nivel (potencia) de la se�al.
<mac>:
String con la direcci�n MAC de la Red. 

*/
char *CWLAP[10]={                       
65, 84, 43, 67, 87, 76, 65, 80, 10, 13
};

char *CWLAP[12]={                       
65, 84, 43, 67, 73, 80, 83, 84, 65, 63, 10, 13
};

