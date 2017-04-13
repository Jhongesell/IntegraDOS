// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int f_dia_sem(const unsigned int dia, const unsigned int mes, const unsigned int ano);
  // De una fecha con formato gregoriano, retorna el numero de d¡a de la semana con rango [0 - 7]
  // (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
  // (6) Viernes, (7) Sabado, (0) Error en la fecha juliana
  // Nombre del archivo: F_D_SEMG.CPP

#include "fechas.hpp"


unsigned int f_dia_sem(const unsigned int dia, const unsigned int mes, const unsigned int ano)
{
   unsigned long d_j;
   gregorian_julian(dia,mes,ano,d_j);
   unsigned int d_s = f_dia_sem(d_j);
   return d_s;
}
