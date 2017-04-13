// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void gregorian_julian(const unsigned int dia, const unsigned int mes, const unsigned int ano, unsigned long &dia_jul);
  // De una fecha con formato gregoriano, retorna una fecha con formato juliano
  //     La variable DIA es del rango [0 - 31]
  //                 MES es del rango [0 - 12]
  //                 ANO es del rango [0 - 2100]
  // Nombre del archivo: F_GREJUL.CPP

#include "fechas.hpp"


void gregorian_julian(const unsigned int dia, const unsigned int mes, const unsigned int ano, unsigned long &dia_jul)
{
   unsigned int n_m_d = f_n_max_dias_mes(mes,ano);
   long d_j = 0,lmes = (long) mes, ldia = (long) dia, lano = (long) ano;
   if(dia == 0 || dia > n_m_d || mes == 0 || mes > 12 || ano == 0 || ano > 2100) d_j = 0;
    else d_j = ldia - 32075l + 1461l * (lano + 4800l + (lmes - 14l) / 12l) / 4l + 367l * (lmes - 2l - (lmes -14l) / 12l * 12l) / 12l - 3l * ((lano + 4900l + (lmes - 14l) / 12l) / 100l) / 4l;
   dia_jul = (unsigned long) d_j;
}

