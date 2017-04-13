// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_fecha_escrita(const unsigned int dia, const unsigned int mes, const unsigned int ano, char *fec_esc, const unsigned int t_p);
  // De una fecha con formato gregoriano, retorna la fecha escrita con el sigiente formato:
  // Si T_P es (1) el formato es [DD DE MMMMMMMM DE CCAA] en espa¤ol
  // Si T_P es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
  // Nombre del archivo: F_FECESG.CPP


#include "fechas.hpp"
extern  "C" {
   #include <conio.h>
   #include <stdio.h>
}


void f_fecha_escrita(const unsigned int dia, const unsigned int mes, const unsigned int ano, char *fec_esc, const unsigned int t_p)
{
   char m_l[40];
   f_mes_let(mes,m_l,t_p);
   if(t_p) sprintf(fec_esc,"%u DE %s DE %u",dia,m_l,ano);
    else sprintf(fec_esc,"%u OF %s OF %u",dia,m_l,ano);
}
