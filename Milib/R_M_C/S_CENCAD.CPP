// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void centra_cadena(const char *cad, char *x_cad);
   // Centra la cadena CAD en la longitud de la cadena


#include "cad_car.hpp"
extern "C" {
   #include <string.h>
}



void centra_cadena(const char *cad, char *x_cad)
{
   int len = 0, xlen = 0, n_esp = 0, i = 0;
   // Se optiene la longitud total de la cadena
   len = strlen(cad);
   // Se copia la cadena sin espacios iniciales y finales a una cadena temporal
   xlen = s_trim(cad,x_cad);
   // Se optiene la longitud real de la cadena
   n_esp = len - xlen;
   // Agrega los espacios iniciales
   if(n_esp > 1) {
      strrev(x_cad);
      n_esp = (n_esp + 1) / 2;
      for(i = 0; i < n_esp ; i++) x_cad[xlen + i] = 32;
      x_cad[n_esp + xlen] = 0;
      strrev(x_cad);
   }
   // Agrega los espacios finales
   for(i = n_esp + xlen; i < len; i++) x_cad[i] = 32;
   x_cad[len] = 0;
}


