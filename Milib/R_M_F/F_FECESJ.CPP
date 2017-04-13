// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

void f_fecha_escrita(const unsigned long dia_jul, char *fec_esc, const unsigned int t_p);
  // De una fecha con formato juliano, retorna la fecha escrita con el sigiente formato:
  // Si T_P es (1) el formato es [DD DE MMMMMMMM DE CCAA] en espa¤ol
  // Si T_P es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
  // Nombre del archivo: F_FECESJ.CPP


#include "fechas.hpp"
extern  "C" {
   #include <string.h>
}


void f_fecha_escrita(const unsigned long dia_jul, char *fec_esc, const unsigned int t_p)
{
   char xf[50];
   unsigned int dia, mes, ano;
   julian_gregorian(dia_jul,dia,mes,ano);
   f_fecha_escrita(dia,mes,ano,xf,t_p);
   strcpy(fec_esc,xf);
}
