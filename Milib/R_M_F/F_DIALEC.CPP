// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_dia_let(const char *fecha, char *d_let, const unsigned int t_l);
  // Regresa el nombre del d¡a de la semana en letra, apartir de una fecha con formato cadena
  //     Si T_L es (0) ser  en ingles
  //               (1) ser  en espa¤ol
  // Si hay error en la fecha retornar  una cadena de espacios
  // Nombre del archivo: F_DIALEC.CPP



#include "fechas.hpp"
extern  "C" {
   #include <string.h>
}

void f_dia_let(const char *fecha, char *d_let, const unsigned int t_l)
{
   char xf[15];
   unsigned int n_d = f_dia_sem(fecha);
   f_dia_let(n_d,xf,t_l);
   strcpy(d_let,xf);
}

