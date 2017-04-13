// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void s_sin_formato(const char *cad, char *xcad);
  // Rutina que quita el formato de comas y postrofes de una cadena de
  // digitos, retornando un puntero a ella
  // Nota: La cadena esta cargada hacia la izquierda


#include "cad_car.hpp"
extern "C" {
   #include <string.h>
}

void s_sin_formato(const char *cad, char *xcad)
{
   int i = 0, xi = 0;
   while(cad[xi]) {
      if(cad[xi] != 39 && cad[xi] != 44) xcad[i++] = cad[xi];
      xi++;
   }
   xcad[i] = 0;
   i = strlen(cad);
   // Ajusta la cadena X_CAD a la longitud de CAD
   for(xi = s_trim(xcad); xi < i; xi++) xcad[xi] = 32;
   xcad[i] = 0;
}

