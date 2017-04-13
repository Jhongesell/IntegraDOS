// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int f_val_fecha(const char *fecha);
  // Valida una fecha con formato cadena, retornando:
  //     (0) Sin errores
  //     (1) Error en los D¡as
  //     (2) Error en los Mes
  //     (3) Error en los A¤os
  //     (4) Error en el delimitador
  // Nombre del archivo: F_VALFEC.CPP

extern char DELIMIT_FECHA;
extern int  CENTURIA;
extern int  TIPO_FECHA;

#include "fechas.hpp"
#include "cad_car.hpp"


unsigned int f_val_fecha(const char *fecha)
{
   unsigned int mes, dia, ano, t_r = 0, n_m_d;

   if(TIPO_FECHA == 0 || TIPO_FECHA == 1) {
      if(fecha[2] != DELIMIT_FECHA || fecha[5] != DELIMIT_FECHA) t_r = 4;
      if(!s_es_car_dig(fecha[0]) || !s_es_car_dig(fecha[1])) t_r = 1;
      if(!s_es_car_dig(fecha[3]) || !s_es_car_dig(fecha[4])) t_r = 2;
      if(!s_es_car_dig(fecha[6]) || !s_es_car_dig(fecha[7])) t_r = 3;
      if(CENTURIA) {
         if(!s_es_car_dig(fecha[8]) || !s_es_car_dig(fecha[9])) t_r = 3;
      }
   }
   if(TIPO_FECHA == 2) {
      if(CENTURIA) {
         if(fecha[4] != DELIMIT_FECHA || fecha[7] != DELIMIT_FECHA) t_r = 4;
         if(!s_es_car_dig(fecha[0]) || !s_es_car_dig(fecha[1]) || !s_es_car_dig(fecha[2]) || !s_es_car_dig(fecha[3])) t_r = 3;
         if(!s_es_car_dig(fecha[5]) || !s_es_car_dig(fecha[6])) t_r = 2;
         if(!s_es_car_dig(fecha[8]) || !s_es_car_dig(fecha[9])) t_r = 1;
       } else {
         if(fecha[2] != DELIMIT_FECHA || fecha[5] != DELIMIT_FECHA) t_r = 4;
         if(!s_es_car_dig(fecha[0]) || !s_es_car_dig(fecha[1])) t_r = 3;
         if(!s_es_car_dig(fecha[3]) || !s_es_car_dig(fecha[4])) t_r = 2;
         if(!s_es_car_dig(fecha[6]) || !s_es_car_dig(fecha[7])) t_r = 1;
      }
   }
   f_fecha__greg(fecha,dia,mes,ano);
   n_m_d = f_n_max_dias_mes(mes,ano);
   if(dia == 0 || dia > n_m_d) t_r = 1;
   if(mes == 0 || mes > 12)    t_r = 2;
   if(ano == 0)  t_r = 3;
   return t_r;
}
