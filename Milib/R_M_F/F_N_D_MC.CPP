// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int f_n_dia_mes(const char *fecha);
  // De una fecha con formato cadena, retorna el n£mero de d¡as del mes de con rango [0 - 31]
  // en caso de error de la fecha con formato cadena retorna 0.
  // Nombre del archivo: F_N_D_MC.CPP



#include "fechas.hpp"


unsigned int f_n_dia_mes(const char *fecha)
{
   unsigned int dia, mes, ano;
   f_fecha__greg(fecha,dia,mes,ano);
   return dia;
}

