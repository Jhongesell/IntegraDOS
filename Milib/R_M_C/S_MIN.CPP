// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_conv_min(char *cad);
  // Los caracteres de la cadena CAD se combierten a minusculas


#include "cad_car.hpp"

void s_conv_min(char *cad)
{
   while(*cad){
      *cad = s_conv_may_min(*cad,2);
      cad++;
   }
}
