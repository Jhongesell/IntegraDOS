// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void hors__am_pm(const char *hora, char *xhora);
  // Retorna de una cadena de horas una cadena de hora con formato HH:MM:SS,
  // en caso de error regresa una cadena en blanco


#include "cad_car.hpp"
#include "hor_seg.hpp"
extern "C" {
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>
}

void hors__am_pm(const char *hora,char *xhora)
{
   if(val_hora(hora)) {
      strcpy(xhora,"        ");
      return;
   }
   char n_hor[4], *cad = " A.M.", x_hor[15];
   n_hor[0] = hora[0];
   n_hor[1] = hora[1];
   n_hor[2] = 0;
   s_substr(hora,2,6,x_hor);
   int n_h = atoi(n_hor);
   if(n_h > 12) {
      n_h -= 12;
      cad = " P.M.";
   }
   sprintf(xhora,"%#02d%s%s",n_h,n_hor,cad);
}
