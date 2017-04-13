// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_mes_let(const char *fecha, char *m_let, const unsigned int t_l);
  // Regresa el nombre del mes en letra, apartir de una fecha con formato cadena
  //     Si T_L es (0) ser  en ingles
  //               (1) ser  en espa¤ol
  // Si hay error en la fecha retornar  una cadena de espacios
  // Nombre del archivo: F_MESLEC.CPP


#include "fechas.hpp"
extern  "C" {
   #include <string.h>
}


void f_mes_let(const char *fecha, char *m_let, const unsigned int t_l)
{
   char xf[40];
   unsigned int n_m = f_n_mes(fecha);
   f_mes_let(n_m,xf,t_l);
   strcpy(m_let,xf);
}
