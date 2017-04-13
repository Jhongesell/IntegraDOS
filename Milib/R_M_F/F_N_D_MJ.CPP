// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int f_n_dia_mes(const unsigned long dia_jul);
  // De una fecha con formato juliano, retorna el n£mero de d¡as del mes de con rango [0 - 31]
  // en caso de error de la fecha con formato juliano retorna 0.
  // Nombre del archivo: F_N_D_MJ.CPP



#include "fechas.hpp"


unsigned int f_n_dia_mes(const unsigned long dia_jul)
{
   unsigned int dia, mes, ano;
   julian_gregorian(dia_jul,dia,mes,ano);
   return dia;
}

