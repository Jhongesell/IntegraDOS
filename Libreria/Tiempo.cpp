// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Rutinas de manupulacion de horas, segundos y conversion entre ellas      //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#ifdef __PROG_DOS__
extern "C" {
#endif
  #include <stdlib.h>
  #include <string.h>
  #include <conio.h>
  #include <stdio.h>
  #include <dos.h>
#ifdef __PROG_DOS__
}
#endif
#include "tiempo.hpp"
#pragma hdrstop



// Retorna la hora actual del sistema modificando la cadena CAD, T_H indica si visualiza
// en formato  (0) HH:MM:SS                  [HH rango 00 - 23]
//             (1) HH:MM:SS [A.M. o P.M.]    [HH rango 01 - 12]
//             (2) HH:MM                     [HH rango 00 - 23]
//             (3) HH:MM [A.M. o P.M.]       [HH rango 01 - 12]
// T_P indica si se remplaza por blanco en primer cero de la hora 
void Tiempo::Hora_actual(char *cad, const char t_h, const char t_p)
{
   char buff[40];
   struct time t;

   gettime(&t);
   PTR_CAD = CAD_PM;
   if (t_h == 1 || t_h == 3) {
      if (t.ti_hour > 12) t.ti_hour -= 12;
       else {
         if (t.ti_hour != 12) PTR_CAD = CAD_AM;
         if (t.ti_hour == 0) t.ti_hour = 12;
      }
   }
   sprintf(buff,"%#02d:%#02d:%#02d %s",t.ti_hour,t.ti_min,t.ti_sec,PTR_CAD);
   if (!t_h) buff[8] = 0;
   if (t_h == 2 || t_h == 3) {
      buff[5] = buff[8];
      buff[6] = buff[9];
      buff[7] = buff[10];
      buff[8] = buff[11];
      buff[9] = buff[12];
      buff[10] = buff[13];
   }
   if (t_p) {
      if (buff[0] == '0' && buff[1] != '0') buff[0] = 32;
   }
   strcpy(cad,buff);
}



// Retorna de una cadena de horas con formato HH:MM:SS una cadena de hora con 
// formato HH:MM:SS [A.M.|P.M.], en caso de error regresa una cadena en blanco
void Tiempo::Hrs__am_pm(const char *hora, char *xhora)
{
   if (Valida_hora(hora,3)) {
      strcpy(xhora,"        ");
      return;
   }
   char n_hor[4], x_hor[15], *cad = " A.M.";
   n_hor[0] = hora[0];
   n_hor[1] = hora[1];
   n_hor[2] = 0;
   Substr(hora,2,6,x_hor);
   int n_h = atoi(n_hor);
   if (n_h > 12) {
      n_h -= 12;
      cad = " P.M.";
    } else {
      if (n_h == 12) {
         cad = " P.M.";
       } else {
         if (n_h == 0) n_h = 12;
      }
   }
   sprintf(xhora,"%#02d%s%s",n_h,x_hor,cad);
}

/*
// Retorna de una cadena del tipo HH:MM;SS el numero de horas, minutos y segundos
void h_cad__hms(const char *hora, int &n_hor, int &n_min, int &n_seg)
{
   n_hor = -1, n_min = -1, n_seg = -1;
   if (!val_hora(hora)) {
      char xc[8];
      xc[0] = hora[0];
      xc[1] = hora[1];
      xc[2] = 0;
      n_hor = atoi(xc);
      xc[0] = hora[3];
      xc[1] = hora[4];
      xc[2] = 0;
      n_min = atoi(xc);
      xc[0] = hora[6];
      xc[1] = hora[7];
      xc[2] = 0;
      n_seg = atoi(xc);
   }
}

*/
// Retorna el numero de segundos que contiene una cadena de hora con
// formato HH:MM:SS, el rango es de [-1 - 86400], el caso de error (-1L)
long Tiempo::Horas__n_seg(const char *hora)
{
   if (Valida_hora(hora,3)) return -1l;
   char n_hor[4], n_min[4], n_seg[4];
   n_hor[0] = hora[0];
   n_hor[1] = hora[1];
   n_hor[2] = 0;
   n_min[0] = hora[3];
   n_min[1] = hora[4];
   n_min[2] = 0;
   n_seg[0] = hora[6];
   n_seg[1] = hora[7];
   n_seg[2] = 0;
   long n_s = (atol(n_hor) * 3600l) + (atol(n_min) * 60) + atol(n_seg);
   return n_s;
}



// Regresa el numero de segundos desde el comienzo del dia, rango de [0 - 86400]
long Tiempo::Numero_segundos(void)
{
  long n_s = 0;
  struct time t;
  gettime(&t);
  n_s = long (t.ti_hour * 3600L) + long (t.ti_min * 60L) + long (t.ti_sec);
  return n_s;
}

/*

// Retorna de una cadena del tipo HH:MM;SS el numero de horas, minutos y segundos
int h_ret_horas(const char *hora)
{
   int n_hor = -1;
   if (val_hora(hora) == 0) {
      char xc[4];
      xc[0] = hora[0];
      xc[1] = hora[1];
      xc[2] = 0;
      n_hor = atoi(xc);
   }
   return n_hor;
}

// Retorna de una cadena del tipo HH:MM:SS el n£mero de minutos
int h_ret_minutos(const char *hora)
{
   int n_min = -1;
   if (val_hora(hora) == 0) {
      char xc[4];
      xc[0] = hora[3];
      xc[1] = hora[4];
      xc[2] = 0;
      n_min = atoi(xc);
   }
   return n_min;
}


// Retorna de una cadena del tipo HH:MM:SS el n£mero de segundos
int h_ret_segundos(const char *hora)
{
   int n_seg = -1;
   if (val_hora(hora) == 0) {
      char xc[4];
      xc[0] = hora[6];
      xc[1] = hora[7];
      xc[2] = 0;
      n_seg = atoi(xc);
   }
   return n_seg;
}
*/


// Retorna una cadena de horas a partir de el numero de segundos con el formato HH:MM:SS
void Tiempo::Nseg__horas(const long n_seg, char *horas)
{
   long n_s = n_seg;
   int n_h = 0, n_m = 0;
   if (n_s < 0 || n_s > 86400l) n_s = 0;
   while (n_s >= 3600) n_s -= 3600, n_h++;
   while (n_s >= 60)   n_s -= 60,   n_m++;
   sprintf(horas,"%#02d:%#02d:%#02d", n_h, n_m, n_s);
}



/*
// Realiza la diferencias entre dos cadenas de hora, con formato HH:MM:SS,
// retornando el numero de segundos rango [-1 - 86400], en caso de error
// regresa -1l
long diff_hors_n(const char *hor1, const char *hor2)
{
   if (val_hora(hor1) || val_hora(hor2)) return -1l;
   long n_seg_h1 = 0, n_seg_h2 = 0, diff = 0;
   n_seg_h1 = horas__n_seg(hor1);
   n_seg_h2 = horas__n_seg(hor2);
   if (n_seg_h1 >= n_seg_h2) diff = n_seg_h1 - n_seg_h2;
    else diff = n_seg_h2 - n_seg_h1;
   return diff;
}


// Realiza la diferencia entre dos cadenas de hora, con formato HH:MM:SS,
// retornando una cadena de hora con formato HH:MM:SS, en caso de error
// regresa una cadena en blanco
void diff_hors_c(const char *hor1, const char *hor2, char *dif)
{
   if (val_hora(hor1) || val_hora(hor2)) {
      strcpy(dif,"        ");
      return;
   }
   long n_seg_h1 = 0, n_seg_h2 = 0, diff = 0;
   n_seg_h1 = horas__n_seg(hor1);
   n_seg_h2 = horas__n_seg(hor2);
   if (n_seg_h1 >= n_seg_h2) diff = n_seg_h1 - n_seg_h2;
    else diff = n_seg_h2 - n_seg_h1;
   n_seg__horas(diff,dif);
}
*/

// Retorna de una cadena de hora formateada HH:MM:SS [A.M.|P.M.] una cadena
// de horas con formato HH:MM:SS, en caso de error regresa una cadena en blanco
void Tiempo::AmPm__hors(const char *hors,char *xhors)
{
   if (Valida_hora(hors,3)) {
      strcpy(xhors,"        ");
      return;
   }
   strcpy(xhors,hors);
   if (hors[9] == 'P') {
      char n_hor[4];
      n_hor[0] = xhors[0];
      n_hor[1] = xhors[1];
      n_hor[2] = 0;
      int h_s = atoi(n_hor);
      if (h_s < 12) h_s += 12;
      itoa(h_s,n_hor,10);
      xhors[0] = n_hor[0];
      xhors[1] = n_hor[1];
    } else {
      if(xhors[0] == '1' && xhors[1] == '2') xhors[0] = '0', xhors[1] = '0';
   }
   xhors[8] = 0;
}


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
int Tiempo::Valida_hora(const char *hora, const unsigned int t_p)
{
   int st = 0;
   char n_hor[4];
   n_hor[0] = hora[0];
   n_hor[1] = hora[1];
   n_hor[2] = 0;
   if (!Es_caracter_digito(n_hor[0]) || !Es_caracter_digito(n_hor[0])) return 1;
   if (atoi(n_hor) > 23) st = 1;                            // Error en horas
   if (t_p == 1)  return st;                                // Validaci¢n de horas
   n_hor[0] = hora[3];
   n_hor[1] = hora[4];
   n_hor[2] = 0;
   if (!Es_caracter_digito(n_hor[0]) || !Es_caracter_digito(n_hor[0])) return 2;
   if (atoi(n_hor) > 59) st = 2;                            // Error en minutos
   if (hora[2] != ':') st = 4;                              // Error en delimitador
   if (t_p == 2) return st;                                 // Validaci¢n de horas y minutos
   n_hor[0] = hora[6];
   n_hor[1] = hora[7];
   n_hor[2] = 0;
   if (!Es_caracter_digito(n_hor[0]) || !Es_caracter_digito(n_hor[0])) return 3;
   if (atoi(n_hor) > 59) st = 3;                            // Error en segundos
   if (hora[5] != ':') st = 4;                              // Error en delimitador
   return st;
}

