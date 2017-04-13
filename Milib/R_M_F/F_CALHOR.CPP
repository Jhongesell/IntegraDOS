// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void cal_horas__fec_hor(const char *cf1, const char *ch1, const char *cf2, const char *ch2, int &hor, int &min);
  // Retorna la diferencia en horas y minutos de un par de fechas con la hora corespondiente
  // Retorna -1 en HOR y MIN si existe alguna inconsistencia en fechas o horas
  // Nota: Los segundos no son tomados en cuenta

#include "fechas.hpp"
#include "hor_seg.hpp"
extern "C" {
   #include <string.h>
   #include <stdlib.h>
}

void cal_horas__fec_hor(const char *cf1, const char *ch1, const char *cf2, const char *ch2, int &hor, int &min)
{
   hor = -1, min = -1;
   if(f_val_fecha(cf1) || f_val_fecha(cf2)) return;
   char xc[40];
   long f1 = 0, f2 = 0, h1 = 0, h2 = 0, diff = 0;
   int  n_d = 0;
   f_quitar_delim(cf1,xc);
   f1 = atol(xc);
   f_quitar_delim(cf2,xc);
   f2 = atol(xc);
   strcpy(xc,ch1);
   xc[5] = ':';
   xc[6] = '0';
   xc[7] = '0';
   xc[8] = 0;
   h1 = horas__n_seg(xc);
   strcpy(xc,ch2);
   xc[5] = ':';
   xc[6] = '0';
   xc[7] = '0';
   xc[8] = 0;
   h2 = horas__n_seg(xc);
   if((f1 == f2 && h1 > h2) || f1 > f2 || h1 == -1 || h2 == -1) return;
   if(f1 <= f2 && h1 <= h2)  diff = h2 - h1, n_d = int (f2 - f1);
    else  diff = (86400L + h2) - h1, n_d = int (f2 - (f1 + 1));
   n_seg__horas(diff,xc);
   xc[2] = 0;
   hor = atoi(xc) + (n_d * 24);
   xc[0] = 32;
   xc[1] = 32;
   xc[2] = 32;
   xc[5] = 0;
   min = atoi(xc);
}

