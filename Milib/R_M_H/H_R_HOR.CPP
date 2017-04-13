// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int h_ret_horas(const char *hora);
  // Retorna de una cadena del tipo HH:MM;SS el numero de horas, minutos y segundos


#include "hor_seg.hpp"
extern "C" {
   #include <stdlib.h>
}


int h_ret_horas(const char *hora)
{
   int n_hor = -1;
   if(val_hora(hora) == 0) {
      char xc[4];
      xc[0] = hora[0];
      xc[1] = hora[1];
      xc[2] = 0;
      n_hor = atoi(xc);
   }
   return n_hor;
}

