// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int  val_hora(const char *hora, const unsigned int t_p);
  // Valida una cadena de hora (HH:MM:SS)
  // Si T_P es (1) Valida solamente la hora
  //           (2) Valida solamente la hora y los minutos
  //           (3) Valida solamente la hora, minutos y segundos
  // Retornando un valor de rango [0 - 4]
  //                              (0) No hay error
  //                              (1) Error en horas
  //                              (2) Error en minutos
  //                              (3) Error en segundos
  //                              (4) Error en delimitadores

#include "cad_car.hpp"
extern "C" {
   #include <stdlib.h>
}

int val_hora(const char *hora, const unsigned int t_p)
{
   int st = 0;
   char n_hor[4];
   n_hor[0] = hora[0];
   n_hor[1] = hora[1];
   n_hor[2] = 0;
   if(!s_es_car_dig(n_hor[0]) || !s_es_car_dig(n_hor[0])) return 1;
   if(atoi(n_hor) > 23) st = 1;                            // Error en horas
   if(t_p == 1)  return st;                                // Validaci¢n de horas
   n_hor[0] = hora[3];
   n_hor[1] = hora[4];
   n_hor[2] = 0;
   if(!s_es_car_dig(n_hor[0]) || !s_es_car_dig(n_hor[0])) return 2;
   if(atoi(n_hor) > 59) st = 2;                            // Error en minutos
   if(hora[2] != ':') st = 4;                              // Error en delimitador
   if(t_p == 2) return st;                                 // Validaci¢n de horas y minutos
   n_hor[0] = hora[6];
   n_hor[1] = hora[7];
   n_hor[2] = 0;
   if(!s_es_car_dig(n_hor[0]) || !s_es_car_dig(n_hor[0])) return 3;
   if(atoi(n_hor) > 59) st = 3;                            // Error en segundos
   if(hora[5] != ':') st = 4;                              // Error en delimitador
   return st;
}

