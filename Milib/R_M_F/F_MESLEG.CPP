// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_mes_let(const unsigned int mes, char *m_let, const unsigned int t_l);
  // Regresa el nombre del mes en letra, apartir de el numero de mes
  //     Si T_L es (0) ser  en ingles
  //               (1) ser  en espa¤ol
  // Si hay error en la fecha retornar  una cadena de espacios
  // Nombre del archivo: F_MESLEG.CPP



#include "fechas.hpp"
extern  "C" {
   #include <string.h>
}
static char *mes_del_ano[] = {
   "          ",
   "ENERO",
   "FEBRERO",
   "MARZO",
   "ABRIL",
   "MAYO",
   "JUNIO",
   "JULIO",
   "AGOSTO",
   "SEPTIEMBRE",
   "OCTUBRE",
   "NOVIEMBRE",
   "DICIEMBRE"
};

static char *month_of_year[] =  {
   "          ",
   "JUNUARY",
   "FEBRUARY",
   "MARCH",
   "APRIL",
   "MAY",
   "JUNE",
   "JULY",
   "AUGUST",
   "SEPTEMBER",
   "OCTUBER",
   "NOVEMBER",
   "DECEMBER"
};


void f_mes_let(const unsigned int mes, char *m_let, const unsigned int t_l)
{
   unsigned int xm = mes;
   if(mes == 0 || mes > 12) xm = 0;
   if(t_l) strcpy(m_let,mes_del_ano[xm]);    // Retorna el mes en espa¤ol
    else strcpy(m_let,month_of_year[xm]);    // Retorna el mes en ingles 
}

