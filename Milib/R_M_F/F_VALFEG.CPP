// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int f_val_fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano);
  // Valida una fecha con formato gregoriano, retornando:
  //     (0) Sin errores
  //     (1) Error en los D¡as
  //     (2) Error en los Mes
  //     (3) Error en los A¤os
  // Nombre del archivo: F_VALFEG.CPP



#include "fechas.hpp"

unsigned int f_val_fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano)
{
   unsigned int t_r = 0, n_m_d = f_n_max_dias_mes(mes,ano);
   if(dia == 0 || dia > n_m_d) t_r = 1;
   if(mes == 0 || mes > 12)    t_r = 2;
   if(ano == 0)  t_r = 3;
   return t_r;
}
