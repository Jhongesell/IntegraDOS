// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void f_quitar_delim(const char *fecha, char *fecha_s_d);
  // De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
  // En caso de error retorna "00000000" en forma de cadena
  // Nombre del archivo: F_QUIDEL.CPP


// Variable definida en el archivo VAR_GLOB.HPP
extern char ACT_CENTURIA[];


#include <stdlib.h>
#include <stdio.h>
#include "fechas.hpp"


void f_quitar_delim(const char *fecha, char *fecha_s_d)
{
   unsigned int dia, mes, ano;
   f_fecha__greg(fecha,dia,mes,ano);
   if(ano < 100) ano += (atoi(ACT_CENTURIA) * 100);
   sprintf(fecha_s_d,"%#04d%#02d%#02d",ano,mes,dia);
}
