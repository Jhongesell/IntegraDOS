// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int s_es_minusculas(const char *cad);
  // Rutina que retorna verdadero si no contiene caracteres alfabeticos en mayusculas
  // en caso contrario retorna (0)
  // Soporta caracteres como  …‚Š¡¢•£—¤¥
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena

#include "cad_car.hpp"

int s_es_minusculas(const char *cad)
{
   while(*cad) {
     if(!((*cad > 96 && *cad < 123) || *cad == 160 || *cad == 133 || *cad == 130 || *cad == 138 || *cad == 141 || *cad == 161 || *cad == 162 || *cad == 149 || *cad == 163 || *cad == 151 || *cad == 129 || *cad == 164 || s_es_car_acent(*cad))) return 0;
   }
   return 1;
}
