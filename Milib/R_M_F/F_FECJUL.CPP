// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void f_fecha__juli(const char *fecha, unsigned long &dia_jul);
  // De una fecha con formato cadena, retorna una fecha con formato juliano
  // En caso de error en la fecha retornar  (0) en la variable DIA_JUL
  // Nombre del archivo: F_FECJUL.CPP


#include "fechas.hpp"


void f_fecha__juli(const char *fecha, unsigned long &dia_jul)
{
   unsigned long d_j;
   unsigned int dia, mes, ano;
   f_fecha__greg(fecha,dia,mes,ano);
   gregorian_julian(dia,mes,ano,d_j);
   dia_jul = d_j;
}

