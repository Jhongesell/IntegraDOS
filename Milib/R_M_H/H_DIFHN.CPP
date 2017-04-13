// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


long diff_hors_n(const char *hor1, const char *hor2);
  // Realiza la diferencias entre dos cadenas de hora, con formato HH:MM:SS,
  // retornando el numero de segundos rango [-1 - 86400], en caso de error
  // regresa -1l


#include "hor_seg.hpp"


long diff_hors_n(const char *hor1, const char *hor2)
{
   if(val_hora(hor1) || val_hora(hor2)) return -1l;
   long n_seg_h1 = 0, n_seg_h2 = 0, diff = 0;
   n_seg_h1 = horas__n_seg(hor1);
   n_seg_h2 = horas__n_seg(hor2);
   if(n_seg_h1 >= n_seg_h2) diff = n_seg_h1 - n_seg_h2;
    else diff = n_seg_h2 - n_seg_h1;
   return diff;
}

