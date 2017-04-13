// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

void s_conv_double_char(const double num, const unsigned int ent, const unsigned int dec, char *cad);
   // Convierte un double a cadena, con ENT entreos y DEC decimales redondeados


#include "cad_car.hpp"
extern "C" {
   #include <math.h>
   #include <stdlib.h>
}



void s_conv_double_char(const double num, const unsigned int ent, const unsigned int dec, char *cad)
{
   char x_cad[100];
   double xnum = num + (5.0 / pow10(dec + 1));
   gcvt(xnum,50,x_cad);
   s_for_num(x_cad,ent,dec,cad);
}

