// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void f_f__ddmmmaa(const char *fecha, const unsigned int t_l, char *xfecha);
  // De una fecha con formato cadena, retorna una cadena del tipo DD/MMM/CCYY,
  // el mes (MMM) ser  en letra
  //     Si T_L es (0) ser  en ingles
  //               (1) ser  en espa¤ol
  // Si hay error en la fecha retornar  una cadena de espacios
  // Nombre del archivo: F_MES3LE.CPP


extern char DELIMIT_FECHA;  // Variable definida en el archivo VAR_GLOB.HPP


#include "fechas.hpp"
extern "C" {
  #include <string.h>
  #include <stdio.h>
}

void f_f__ddmmmaa(const char *fecha, const unsigned int t_l,char *xfecha)
{
    unsigned int dia, mes, ano;
    char m_l[40];
    f_fecha__greg(fecha,dia,mes,ano);
    f_mes_let(fecha,m_l,t_l);
    sprintf(xfecha,"%#02d%c%c%c%c%c%#02d",dia,DELIMIT_FECHA,m_l[0],m_l[1],m_l[2],DELIMIT_FECHA,ano);
}
