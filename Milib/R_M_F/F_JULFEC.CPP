// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_juli__fecha(const unsigned long dia_jul, char *fecha);
  // De una fecha con formato juliano, regresa una fecha con formato cadena
  // En caso de error en la fecha retornar  una cadena en blanco
  // Nombre del archivo: F_JULFEC.CPP


#include "fechas.hpp"
extern  "C" {
   #include <string.h>
}

void f_juli__fecha(const unsigned long dia_jul, char *fecha)
{
   char xf[15];
   unsigned int dia, mes, ano;
   julian_gregorian(dia_jul,dia,mes,ano);
   f_greg__fecha(dia,mes,ano,xf);
   strcpy(fecha,xf);
}

