// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_fecha_escrita(const char *fecha, char *fec_esc, const unsigned int t_p, const unsigned int id);
  // De una fecha con formato cadena, retorna la fecha escrita con el sigiente formato:
  // Si T_P es (0) el formato es:
  //    Si ID es (1) el formato es [DD DE MMMMMMMM DE CCAA] en espa¤ol
  //    Si ID es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
  // Si T_P es (1) el formato es:
  //    Si ID es (1) el formato es [DDDDDDDD DD DE MMMMMMMM DE CCAA] en espa¤ol
  //    Si ID es (0) el formato es [DDDDDDDD DD OF MMMMMMMM OF CCAA] en ingles
  // Nombre del archivo: F_FECESC.CPP


#include "fechas.hpp";
extern  "C" {
   #include <string.h>
   #include <stdio.h>
}


void f_fecha_escrita(const char *fecha, char *fec_esc, const unsigned int t_p, const unsigned int id)
{
   char xf[50], xcad[20];
   unsigned int dia, mes, ano;
   f_fecha__greg(fecha,dia,mes,ano);
   f_fecha_escrita(dia,mes,ano,xf,id);
   if(t_p) {
      f_dia_let(fecha,xcad,id);
      sprintf(fec_esc,"%s %s",xcad,xf);
   } else strcpy(fec_esc,xf);
}
