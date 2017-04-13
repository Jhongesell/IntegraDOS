// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A



#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <string.h>
   #include <stdio.h>
#ifdef __PROG_DOS__
}
#endif
#include "d:\libreria\Fechora.hpp"
#pragma hdrstop




// Retorna la fecha y hora actuales escrita con el sigiente formato:
// Si T_P es (0) el formato es:
//    Si ID es (1) el formato es [DD DE MMMMMMMM DE CCAA HH:MM:SS] en español
//    Si ID es (0) el formato es [DD OF MMMMMMMM OF CCAA HH:MM:SS] en ingles
// Si T_P es (1) el formato es:
//    Si ID es (1) el formato es [DDDDDDDD DD DE MMMMMMMM DE CCAA HH:MM:SS P.M.] en español
//    Si ID es (0) el formato es [DDDDDDDD DD OF MMMMMMMM OF CCAA HH:MM:SS P.M.] en ingles
// Si T_P es (2) el formato es:
//    ID no importa el formato es [DD/MM/AA HH:MM:SS]    
void Fechas_Horas::Fecha_y_hora(char *cad, const unsigned int t_p, const unsigned int id)
{
   char fecha[20], tiempo[20], xcad[100], fec_esc[50];
   int xtp = t_p;
   if (xtp == 2) xtp = 0;
   
   Fecha_hoy(fecha);
   Fecha_escrita(fecha, fec_esc, xtp, id);
   Hora_actual(tiempo, xtp, id);
   Trim(tiempo);
   if (t_p != 2) sprintf(xcad,"%s %s",fec_esc,tiempo);
    else sprintf(xcad,"%s %s",fecha,tiempo);
   strcpy(cad,xcad);
}
