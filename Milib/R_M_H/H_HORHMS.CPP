// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void h_cad__hms(const char *hora, int &n_hor, int &n_min, int &n_seg);
  // Retorna de una cadena del tipo HH:MM;SS el numero de horas, minutos y segundos

#include "hor_seg.hpp"
extern "C" {
   #include <stdlib.h>
}


void h_cad__hms(const char *hora, int &n_hor, int &n_min, int &n_seg)
{
   n_hor = -1, n_min = -1, n_seg = -1;
   if(!val_hora(hora)) {
      char xc[8];
      xc[0] = hora[0];
      xc[1] = hora[1];
      xc[2] = 0;
      n_hor = atoi(xc);
      xc[0] = hora[3];
      xc[1] = hora[4];
      xc[2] = 0;
      n_min = atoi(xc);
      xc[0] = hora[6];
      xc[1] = hora[7];
      xc[2] = 0;
      n_seg = atoi(xc);
   }
}

