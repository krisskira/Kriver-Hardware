#define Speaker   PIN_B2      // Altavoz conectado a RB0
void Sound(int16 frecuencia, int16 duracion){
   
   int16 mS_Transcurridos=0;
   int16 CiclosL=0;
   int16 uS;
   int32 tmp;
   /** 
      si la frecuancia se encuentra entre los margenes de 20Hz y 20 KHz se ejecuta
      de los contrario no.
   **/
   if (frecuencia>=20&&frecuencia<=20000){ 
      tmp=100000;
      tmp/=frecuencia;       // convierte los Hz a microsegundos para la pausa
      tmp*=5;   
      uS=tmp;
      do{
         output_high(Speaker);   // Genera la frecuancia deseada
         delay_us(uS);           // con los retardos mientras
         CiclosL+=(uS);          // aumenta el contador de ciclos transcurridos
         output_low(Speaker);    // en dos partes para repartir el 
         delay_us(uS);           // trabajo entre estado alto y bajo.
         CiclosL+=(uS);          // 
         CiclosL+=25;            // Compensador.
         
         while(CiclosL>999){     // Se queda en el bucle mientras CiclosL sea
                                 // menor a 1000 (1 mS)
            CiclosL-=1000;       // Le resta 1000 a CiclosL 
            mS_Transcurridos++;  // y le suma 1 a mS_Transcurridos.
            CiclosL+=25;         // Compensador.
         }
      }while (duracion>mS_Transcurridos);// Repite el bucle hasta que haya pasado el
                                         // tiempo indicado.
   }
}

/*
#define nDO     0    // DO
#define nDO_    1    // DO#
#define nRE     2    // RE
#define nRE_    3    // RE#
#define nMI     4    // MI
#define nFA     5    // FA
#define nFA_    6    // FA#
#define nSOL    7    // SOL
#define nSOL_   8    // SOL#
#define nLA     9    // LA
#define nLA_    10   // LA#
#define nSI     11   // SI

int16 FreqNota[12]={  // retardos entre estado alto
                      // y bajo para generar las notas
15289, // DO
14430, // DO#
13620, // RE
12856, // RE#
12134, // MI
11453, // FA
10810, // FA#
10204, // SOL
9631,  // SOL#
9090,  // LA
8580,  // LA#
8099   // SI
};

void Play(int nota, int octava, int16 duracion){
 int16 fn;

 int16 mS_Transcurridos=0;  // Contadores necesarios
                            // para controlar la duraci�n
 int16 CiclosL=0;           // Contandor de uS

 fn=FreqNota[nota];         // Define los retardos para generar
                            // la frecuencia de cada nota
 fn>>=(octava);             // Adapta la frecuencia a la octava actual
                            // haciendo una rotaci�n
                            // a la derecha por octava
do{

    output_high(Speaker);   // Genera la frecuancia
    delay_us(fn);           // con los retardos mientras
    CiclosL+=(fn);          // aumenta el contador de 
                            // ciclos transcurridos
    output_low(Speaker);    // en dos partes para repartir el
    delay_us(fn);           // trabajo entre estado alto y bajo.
    CiclosL+=(fn);          //
    CiclosL+=25;            // Compensador.

    while(CiclosL>999){     // Se queda en el bucle mientras CiclosL
                            // sea menor a 1000 (1 mS)
       CiclosL-=1000;       // Le resta 1000 a CiclosL
       mS_Transcurridos++;  // y le suma 1 a mS_Transcurridos.
       CiclosL+=25;         // Compensador.
    }
 }while (duracion>mS_Transcurridos); // Repite el bucle hasta que haya
                                     // pasado el tiempo indicado.
}
*/

