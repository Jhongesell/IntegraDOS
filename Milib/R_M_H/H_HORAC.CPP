// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

/*
void hora_act(char *hora, const unsigned int t_h);
  // Retorna una cadena con la hora actual del sistema, T_H indica si visualiza
  // en formato  (0) HH:MM:SS                [HH rango 00 - 23]
  //	           (1) HH:MM:SS [A.M o P.M]    [HH rango 00 - 12]
  //             (2) HH:MM                   [HH rango 00 - 23]
  //             (3) HH:MM [A.M o P.M]       [HH rango 00 - 12]



#include "cad_car.hpp"
#include "hor_seg.hpp"

void hora_act(char *hora, const unsigned int t_h)
{
   char buff[50];
   hora_act(buff,t_h);
   return buff;
}
*/
