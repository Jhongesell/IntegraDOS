// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void julian_gregorian(const unsigned long dia_jul, unsigned int &dia, unsigned int &mes, unsigned int &ano);
  // De una fecha con formato juliano, retorna una fecha con formato gregoriano
  //     La variable DIA es del rango [0 - 31]
  //                 MES es del rango [0 - 12]
  //                 ANO es del rango [0 - 2100]
  // Nota: Esta rutina modifica las variables DIA, MES, ANO en su valor
  // Nombre del archivo: F_JULGRE.CPP

#include "fechas.hpp"



void julian_gregorian(const unsigned long dia_jul, unsigned int &dia, unsigned int &mes, unsigned int &ano)
{
   unsigned int xd = 0, xm = 0, xa = 0;
   if(dia_jul) {
      long t1 = 0, t2 = 0, an = 0, ms = 0;
      t1 = dia_jul + 68569l;
      t2 = 4l * t1 / 146097l;
      t1 = t1 - (146097l * t2 + 3l) / 4l;
      an = 4000l * (t1 + 1) / 1461001l;
      t1 = t1 - 1461l * an / 4l + 31;
      ms = 80l * t1 / 2447l;
      xd = (unsigned int) (t1 - 2447l * ms / 80l);
      t1 = ms / 11l;
      xm = (unsigned int) (ms + 2l - 12l * t1);
      xa = (unsigned int) (100l * (t2 - 49l) + an + t1);
   }
   if(f_val_fecha(xd,xm,xa)) xd = 0, xm = 0, xa = 0;
   dia = xd, mes = xm, ano = xa;
}
