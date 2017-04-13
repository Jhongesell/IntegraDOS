// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void Tabla_ascii(void);
   // Rutina que muestra una ventana en la cual se visualiza la tabla del codigo ASCII

#include <stdio.h>
#include "vis_ayud.hpp"



#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "TABLA ASCII"
   #define TXT2      "Codigo  Caracter   Codigo  Caracter"
#else
   #define TXT1      "ASCII TABLE"
   #define TXT2      "Code    Character  Code    Character"
#endif


void Tabla_ascii(void)
{
   char xcad[300];
   Vis_archivo *Ve = new Vis_archivo;
   Ve->Parametros(" ",TXT1,350,300,VISUALIZA);
   // Carga el sistema y las ecuaciones al visualizador
   Ve->Adiciona_linea(TXT2);
   for(int i = 0; i < 128; i++) {
       sprintf(xcad,    " %#03d       %c        %#03d      %c",i,(i == 0 ? 32: i),i+128,i+128);
       Ve->Adiciona_linea(xcad);
   }
   Ve->Loop();
   delete Ve;
}



