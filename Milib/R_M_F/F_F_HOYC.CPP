// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_f_hoy(char *fecha);
  // Regresa la fecha actual del sistema con formato cadena en la variable FECHA
  // Nota: Esta rutina modifica la variable FECHA en su valor
  // Nombre del archivo: F_F_HOYC.CPP


#include "fechas.hpp"

void f_f_hoy(char *fecha)
{
   unsigned int dia, mes, ano;
   f_f_hoy(dia,mes,ano);
   f_greg__fecha(dia,mes,ano,fecha);
}
