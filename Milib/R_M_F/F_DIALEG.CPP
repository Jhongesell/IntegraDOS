// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_dia_let(const unsigned int n_dia, char *d_let, const unsigned int t_l);
  // Regresa el nombre del d¡a de la semana en letra, apartir de el numero de dia
  //     Si T_L es (0) ser  en ingles
  //               (1) ser  en espa¤ol
  // Si hay error en la fecha retornar  una cadena de espacios
  // Nombre del archivo: F_DIALEG.CPP




#include "fechas.hpp"
extern  "C" {
   #include <string.h>
}

static char *dia_semana[] = {
   "          ",
   "DOMINGO",
   "LUNES",
   "MARTES",
   "MIERCOLES",
   "JUEVES",
   "VIERNES",
   "SABADO"
};

static char *day_of_week[] = {
   "          ",
   "SUNDAY",
   "MONDAY",
   "TUESDAY",
   "WEDNESDAY",
   "THURSDAY",
   "FRIDAY",
   "SATURDAY"
};


void f_dia_let(const unsigned int n_dia, char *d_let, const unsigned int t_l)
{

   unsigned int n_d = n_dia;
   if(n_d > 7) n_d = 0;
   if(t_l) strcpy(d_let,dia_semana[n_d]);
    else strcpy(d_let,day_of_week[n_d]);
}