// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void diff_hors_c(const char *hor1, const char *hor2, char *dif);
  // Realiza la diferencia entre dos cadenas de hora, con formato HH:MM:SS,
  // retornando una cadena de hora con formato HH:MM:SS, en caso de error
  // regresa una cadena en blanco


#include "hor_seg.hpp"
extern "C" {
   #include <string.h>
}


void diff_hors_c(const char *hor1, const char *hor2, char *dif)
{
   if(val_hora(hor1) || val_hora(hor2)) {
      strcpy(dif,"        ");
      return;
   }
   long n_seg_h1 = 0, n_seg_h2 = 0, diff = 0;
   n_seg_h1 = horas__n_seg(hor1);
   n_seg_h2 = horas__n_seg(hor2);
   if(n_seg_h1 >= n_seg_h2) diff = n_seg_h1 - n_seg_h2;
    else diff = n_seg_h2 - n_seg_h1;
   n_seg__horas(diff,dif);
}

