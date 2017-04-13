// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_poner_delim(const unsigned int dia, const unsigned int mes, const unsigned int ano, char *fecha_cd);
  // De una fecha con formato gregoriano, retorna una fecha con formato cadena
  // Nombre del archivo: F_PODELG.CPP

#include "fechas.hpp"
extern  "C" {
   #include <string.h>
}

void f_poner_delim(const unsigned int dia, const unsigned int mes, const unsigned int ano, char *fecha_cd)
{
   char xf[15];
   f_greg__fecha(dia,mes,ano,xf);
   strcpy(fecha_cd,xf);
}
