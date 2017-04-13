// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

/*

char *f_quitar_delim(const char *fecha);
  // De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
  // En caso de error retorna "00000000" en forma de cadena
  // Nombre del archivo: F_QUIDEL.CPP


// Variable definida en el archivo VAR_GLOB.HPP
extern char ACT_CENTURIA[];


#include <stdlib.h>
#include <stdio.h>
#include "fechas.hpp"


char *f_quitar_delim(const char *fecha)
{
   char buff[15];
   unsigned int dia = 0, mes = 0, ano = 0;
   f_fecha__greg(fecha,dia,mes,ano);
   if(ano < 100) ano += (atoi(ACT_CENTURIA) * 100);
   sprintf(buff,"%#04d%#02d%#02d",ano,mes,dia);
   return buff;
}

*/
