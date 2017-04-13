// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int f_n_max_dias_mes(const char *fecha);
  // Retorna el numero maximo de dias en el mes al que corresponde la fecha con formato cadena
  // Nombre del archivo: F_NMAXDC.CPP


#include "fechas.hpp"

unsigned int f_n_max_dias_mes(const char *fecha)
{
   unsigned int dia, mes, ano;
   f_fecha__greg(fecha,dia,mes,ano);
   unsigned int n_m_d = f_n_max_dias_mes(mes,ano);
   return n_m_d;
}

