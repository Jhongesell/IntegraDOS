// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_r_izq(const char *cad, const unsigned int lg, const char car);
  // Regresa en la cadena CAD un a una cadena rellena con el caracter CAR por la
  // izquierda de longitud LG, si esta es mayor no le hace nada
  // Nombre del archivo: S_R_IZP.CPP

#include "cad_car.hpp"
extern "C" {
   #include <string.h>
}


void s_r_izq(char *cad, const unsigned int lg, const char car)
{
   unsigned int i = s_trim(cad);
   if(i < lg) {
     strrev(cad);
     for(; i < lg; i ++) cad[i] = car;
     cad[lg] = 0;
     strrev(cad);
   }
}
