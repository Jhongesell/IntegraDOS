// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int f_n_mes(const char *fecha);
  // Regresa el n£mero de mes de una fecha en formato de cadena, retornando ‚sta
  // como un entero del rango [0 - 12]
  // (1) Enero, (2) Febrero, (3) Marzo,      (4) Abril,    (5) Mayo,       (6) Junio
  // (7) Julio, (8) Agosto,  (9) Septiembre, (10) Octubre, (11) Noviembre, (12) Diciembre
  // (0) Error en el formato de la fecha
  // Nombre del archivo: F_N_MES.CPP


#include "fechas.hpp"


unsigned int f_n_mes(const char *fecha)
{
   unsigned int dia, mes, ano;
   f_fecha__greg(fecha,dia,mes,ano);
   return mes;
}

