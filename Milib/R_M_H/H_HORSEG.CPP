// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


long horas__n_seg(const char *hors);
  // Retorna el numero de segundos que contiene una cadena de hora con
  // formato HH:MM:SS, el rango es de [-1 - 86400], el caso de error (-1L)

#include "hor_seg.hpp"
extern "C" {
   #include <stdlib.h>
}

long horas__n_seg(const char *hora)
{
   if(val_hora(hora,3)) return -1l;
   char n_hor[4], n_min[4], n_seg[4];
   n_hor[0] = hora[0];
   n_hor[1] = hora[1];
   n_hor[2] = 0;
   n_min[0] = hora[3];
   n_min[1] = hora[4];
   n_min[2] = 0;
   n_seg[0] = hora[6];
   n_seg[1] = hora[7];
   n_seg[2] = 0;
   long n_s = (atol(n_hor) * 3600l) + (atol(n_min) * 60) + atol(n_seg);
   return n_s;
}
