// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int f_dia_sem(const char *fecha);
  // De una fecha con formato cadena, retorna el numero de d¡a de la semana con rango [0 - 7]
  // (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
  // (6) Viernes, (7) Sabado, (0) Error en la fecha juliana
  // Nombre del archivo: F_D_SEMC.CPP



#include "fechas.hpp"

unsigned int f_dia_sem(const char *fecha)
{
   unsigned long dia_juliano;
   f_fecha__juli(fecha,dia_juliano);
   unsigned int d_s =  f_dia_sem(dia_juliano);
   return d_s;
}
