// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void f_fecha__greg(const char *fecha, unsigned int &dia, unsigned int &mes, unsigned int &ano);
  // De una fecha con formato cadena, retorna el n£mero de D¡as, Meses, y A¤os
  //     La variable DIA es del rango [0 - 31]
  //                 MES es del rango [0 - 12]
  //                 ANO es del rango [0 - 2100]
  //  Esta rutina modifica las variables DIA, MES, ANO en su valor
  //  En caso de error en la fecha retornar  (0) en las variables DIA, MES, ANO
  // Nombre del archivo: F_FECGRE.CPP


// Variables definidas en el archivo VAR_GLOB.HPP
extern int CENTURIA;
extern int TIPO_FECHA;


#include "fechas.hpp"
extern  "C" {
   #include <stdlib.h>
}


void f_fecha__greg(const char *fecha, unsigned int &dia, unsigned int &mes, unsigned int &ano)
{
   char x_m[4], x_d[4] , x_a[6];
   unsigned int i = 0;
   if(TIPO_FECHA == 0) {
      x_d[0] = fecha[0];
      x_d[1] = fecha[1];
      x_d[2] = 0;
      x_m[0] = fecha[3];
      x_m[1] = fecha[4];
      x_m[2] = 0;
      if(CENTURIA) {
         x_a[0] = fecha[6];
         x_a[1] = fecha[7];
         i = 2;
      }
      x_a[0 + i] = fecha[6 + i];
      x_a[1 + i] = fecha[7 + i];
      x_a[2 + i] = 0;
   }
   if(TIPO_FECHA == 1) {
      x_m[0] = fecha[0];
      x_m[1] = fecha[1];
      x_m[2] = 0;
      x_d[0] = fecha[3];
      x_d[1] = fecha[4];
      x_d[2] = 0;
      if(CENTURIA) {
         x_a[0] = fecha[6];
         x_a[1] = fecha[7];
         i = 2;
      }
      x_a[0 + i] = fecha[6 + i];
      x_a[1 + i] = fecha[7 + i];
      x_a[2 + i] = 0;
   }
   if(TIPO_FECHA == 2) {
      if(CENTURIA) {
         i = 2;
         x_a[0] = fecha[0];
         x_a[1] = fecha[1];
      }
      x_a[i] = fecha[i];
      x_a[1 + i] = fecha[i + 1];
      x_a[2 + i] = 0;
      x_m[0] = fecha[i + 3];
      x_m[1] = fecha[i + 4];
      x_m[2] = 0;
      x_d[0] = fecha[i + 6];
      x_d[1] = fecha[i + 7];
      x_d[2] = 0;
   }
   unsigned int xd, xm, xa, n_m_d;
   xd = atoi(x_d);
   xm = atoi(x_m);
   xa = atoi(x_a);
   n_m_d = f_n_max_dias_mes(xm,xa);
   if(xd > n_m_d || xm > 12) xd = 0, xm = 0, xa = 0;
   dia = xd, mes = xm, ano = xa;
}

