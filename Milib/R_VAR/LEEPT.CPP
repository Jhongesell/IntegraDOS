// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.0-A

extern "C" {
   #include <conio.h>
}

#include "man_arch.hpp"

#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "LEE PANTALLA"
#else
   #define TXT1 "READ WINDOW"
#endif



void lee_pantalla_texto(const int x1, const int y1, const int x2, const int y2, const char *archivo);
  // Lee una pantalla previamente grabada y la pone en las coordenadas indicadas



void lee_pantalla_texto(const int x1, const int y1, const int x2, const int y2, const char *archivo)
{
   unsigned int long_vent;
   Manipulador_archivos *Lee_pant  = new Manipulador_archivos;
   Lee_pant->Parametros(archivo,LEE_ARCHIVO,0,TXT1);
   Lee_pant->Lee_entero(long_vent);
   char *buff = new char [(unsigned int)long_vent+10];
   Lee_pant->Lee_caracteres(buff,long_vent);
   puttext(x1,y1,x2,y2,buff);
   delete []buff;
   delete Lee_pant;
}


