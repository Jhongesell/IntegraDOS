// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_poner_delim(const char *fecha, char *fecha_cd);
  // De una fecha del tipo CCYYMMDD, retorna una fecha en formato cadena
  // Nombre del archivo: F_PODELC.CPP


#include "fechas.hpp"
extern  "C" {
   #include <stdlib.h>
   #include <string.h>
}

void f_poner_delim(const char *fecha, char *fecha_cd)
{
   char x_a[6], x_m[4], x_d[4], xf[15];
   unsigned int dia, mes, ano;
   x_a[0] = fecha[0];
   x_a[1] = fecha[1];
   x_a[2] = fecha[2];
   x_a[3] = fecha[3];
   x_a[4] = 0;
   x_m[0] = fecha[4];
   x_m[1] = fecha[5];
   x_m[2] = 0;
   x_d[0] = fecha[6];
   x_d[1] = fecha[7];
   x_d[2] = 0;
   dia =  atoi(x_d);
   mes =  atoi(x_m);
   ano =  atoi(x_a);
   f_greg__fecha(dia,mes,ano,xf);
   strcpy(fecha_cd,xf);
}
