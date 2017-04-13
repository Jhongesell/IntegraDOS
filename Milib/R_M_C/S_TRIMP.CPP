// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int s_trim(char *cad);
  // Quita los espacios que existan al inicio y fin de la cadena CAD, retornando la longitud de ella


#include "r_m_c.hpp"
extern "C" {
   #include <string.h>
}

unsigned int s_trim(char *cad)
{
   unsigned int ind = 0, i = 0, pos = 0;
   // Salta los espacios en blanco al inicio de la cadena
   while(cad[i] == 32) i++;
   if(!cad[i]) {
      cad[0] = 0;
      return 0;
   }
   char *x_cad = new char[strlen(cad)+1];
   // Copia hasta el final de la cadena
   while(cad[i]) {
      x_cad[ind] = cad[i];
      if(x_cad[ind] != 32) pos = ind;
      ind++,i++;
   }
   pos++;
   x_cad[pos] = 0;
   // Copia la cadena de trabajo a la cadena original
   strcpy(cad,x_cad);
   delete x_cad;
   return (pos);
}
