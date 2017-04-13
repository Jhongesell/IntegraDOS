// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


/*
#include <dos.h>
#include "hor_seg.hpp"
#include "fechas.hpp"

void mod_fecha_hora_sys(const char *n_fecha, const char *n_hora);
// Modifica la fecha y hora del sistema, segun los parametros pasados a la rutina


void mod_fecha_hora_sys(const char *n_fecha, const char *n_hora)
{
   unsigned int dia= 0, mes = 0, ano = 0;
   int hor = 0, min = 0, seg = 0;
   f_fecha__greg(n_fecha,dia,mes,ano);
   h_cad__hms(n_hora,hor, min, seg);
   if(hor < 0) return;
   struct date f_a;
   f_a.da_year = ano;
   f_a.da_mon = (char) mes;
   f_a.da_day = (char) dia;
   setdate(&f_a);
   struct time t_a;
   t_a.ti_hour = (unsigned char) hor;
   t_a.ti_min  = (unsigned char) min;
   t_a.ti_sec  = (unsigned char) seg;
   t_a.ti_hund = (unsigned char) 0;
   settime(&t_a);
}

*/
