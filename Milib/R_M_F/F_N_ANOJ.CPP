// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int f_n_ano(const unsigned long dia_jul);
  // Regresa el n£mero de a¤o en una fecha con formato juliano, retornando esta
  // como un entero del rango [0 - 2100]
  // (0) Error en el formato de la fecha
  // Nombre del archivo: F_N_ANO.CPP


#include "fechas.hpp"

unsigned int f_n_ano(const unsigned long dia_jul)
{
   unsigned int dia, mes, ano;
   julian_gregorian(dia_jul,dia,mes,ano);
   return ano;
}
