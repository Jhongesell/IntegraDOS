// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_f_hoy(unsigned long &dia_jul);
  // Regresa la fecha actual del sistema con formato juliano en la variable DIA_JUL
  // Nota: Esta rutina modifica la variable DIA_JUL en su valor
  // Nombre del archivo: F_F_HOYJ.CPP

#include "fechas.hpp"

void f_f_hoy(unsigned long &dia_jul)
{
   unsigned long d_j;
   unsigned int dia, mes, ano;
   f_f_hoy(dia,mes,ano);
   gregorian_julian(dia,mes,ano,d_j);
   dia_jul = d_j;
}
