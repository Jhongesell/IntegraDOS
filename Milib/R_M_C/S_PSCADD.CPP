// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "cad_car.hpp"
extern "C" {
   #include <string.h>
}


int s_pos_sig_cad_dist(const char *cad,const char *xcad, const unsigned int posi, const unsigned int posf);
  // Retorna la posicion del siguiente caracter distinto al de la cadena XCAD en la cadena CAD
  // la busqueda inicia el POSI y termina en POSF. Si POSF es cero asume hasta el final de la cadena
  // Nombre del archivo: S_PSCADD.CPP

/*
int s_pos_sig_cad_dist(const char *cad,const char *xcad, const unsigned int posi, const unsigned int posf)
{
   int lg = strlen(cad), i = posi;
   unsigned int pos;
   if(posf && posf < lg) lg = posf + 1;
   for(; i < lg; i++) {
      if(!s_busca_caracter(xcad,cad[i],pos)) return i;
   }
   return lg;
}

*/
