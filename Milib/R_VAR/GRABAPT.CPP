// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.0-A


#include "man_arch.hpp"

extern "C" {
   #include <conio.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "GRABA PANTALLA"
#else
   #define TXT1 "SAVE WINDOW"
#endif


void graba_pantalla_texto(const int x1, const int y1, const int x2, const int y2, const char *archivo);
  // Rutina que graba el contenido de un recuadro de la pantalla en el archivo ARCHIVO


void graba_pantalla_texto(const int x1, const int y1, const int x2, const int y2, const char *archivo)
{
  unsigned int long_vent = (((y2 - y1) + 1) * ((x2 - x1) + 1)) * 2;
  char *buff = new char[long_vent+10];
  Manipulador_archivos *Grab_pant = new Manipulador_archivos;
  Grab_pant->Parametros(archivo,GRABA_ARCHIVO,0,TXT1);
  gettext(x1,y1,x2,y2,buff);
  Grab_pant->Graba_entero(long_vent);
  Grab_pant->Graba_caracteres(buff,long_vent);
  delete []buff;
  delete Grab_pant;
}



