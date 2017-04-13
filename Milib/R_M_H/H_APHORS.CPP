// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void am_pm__hors(const char *hors, char *xhors);
  // Retorna de una cadena de hora formateada con [A.M. o P.M.] una cadena
  // de horas con formato HH:MM:SS, en caso de error regresa una cadena en
  // blanco

#include "hor_seg.hpp"
extern "C" {
  #include <stdlib.h>
  #include <string.h>
}

void am_pm__hors(const char *hors,char *xhors)
{
   if(val_hora(hors)) {
      strcpy(xhors,"        ");
      return;
   }
   strcpy(xhors,hors);
   if(hors[9] == 'P') {
      char n_hor[4];
      n_hor[0] = xhors[0];
      n_hor[1] = xhors[1];
      n_hor[2] = 0;
      int h_s = atoi(n_hor) + 12;
      itoa(h_s,n_hor,10);
      xhors[0] = n_hor[0];
      xhors[1] = n_hor[1];
   }
   xhors[8] = 0;
}
