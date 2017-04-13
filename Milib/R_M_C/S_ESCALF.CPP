// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



int s_es_car_alf_ace(const char car);
  // Retorna (1) si el carcter es alfabetico, en caso contrario retorna (0)
  // Acepta loa siguientes caracteres como alfabeticos [ABCDEFGHIJKLMNOPQRSTUVWXYZZabcdefghijklmnopqrstuvwxyz …‚Š¡¢¢£—¤¥ -_]

#include "cad_car.hpp"

int s_es_car_alf_ace(const char car)
{                
   if(s_es_car_acent(car) || (car > 64 && car < 91) || (car > 96 && car < 123) || car == 32 || car == 95 || car == 45) return 1;
   return 0;
}


int s_es_car_alf(const char car);
// Retorna (1) si el carcter es alfabetico, en caso contrario retorna (0)
// Acepta loa siguientes caracteres como alfabeticos [ABCDEFGHIJKLMNOPQRSTUVWXYZZabcdefghijklmnopqrstuvwxyz]


int s_es_car_alf(const char car)
{
   if((car > 64 && car < 91) || (car > 96 && car < 123)) return 1;
   return 0;
}



