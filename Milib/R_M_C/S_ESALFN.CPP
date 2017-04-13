// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// TelÇfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr†mite
// Revisi¢n  1.1-A


int s_es_alfanumerica(const char *cad, const unsigned int posi, const unsigned posf);
  // Rutina que retorna verdadero si no la cadena solo contiene caracteres alfabeticos, numericos, espacios, guion o subrrayado
  // en caso contrario retorna (0)
  // Soporta caracteres como †ÖÇä°¢ï£óÅ§•ç


#include "cad_car.hpp"

/*
int s_es_alfanumerica(const char *cad)
{
   while(*cad) {
      if(!s_es_car_alfnum(*cad)) return 0;
      cad++;
   }
   return 1;
}

*/
