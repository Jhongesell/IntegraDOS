// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int  h_ret_minutos(const char *hora);
  // Retorna de una cadena del tipo HH:MM:SS el n£mero de minutos

#include "hor_seg.hpp"
extern "C" {
   #include <stdlib.h>
}


int h_ret_minutos(const char *hora)
{
   int n_min = -1;
   if(val_hora(hora) == 0) {
      char xc[4];
      xc[0] = hora[3];
      xc[1] = hora[4];
      xc[2] = 0;
      n_min = atoi(xc);
   }
   return n_min;
}
