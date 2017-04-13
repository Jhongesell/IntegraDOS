// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Rutinas de manipulaci¢n de fechas. Trabajando como base las conversiones //
// juliana y/o gregoriana, las fecha en forma de cadena estar n de acuerdo  //
// al tipo de formato y delimitador definidos.                              //
//                                                                          //
// La fecha gregoriana tiene por formato:                                   //
//    unsigned int DIA, rango [0 - 31]                                      //
//    unsigned int MES, rango [0 - 12]                                      //
//    unsigned int ANO, rango [1900 - 2100]                                 //
//                                                                          //
// La fecha juliana tiene por formato:                                      //
//    long DIA_JUL, rango [0 - ì)                                           //
//                                                                          //
// La fecha con formato cadena tiene por formato:                           //
//    Si TIPO_FECHA es (0)  el formato es DD/MM/[CC]YY                      //
//                     (1)  el formato es MM/DD/[CC]YY                      //
//                     (2)  el formato es [CC]YY/MM/DD                      //
//    Si la variable CENTURIA es (1) [CC] existira                          //
//    La variable DELIMIT_FECHA contendra el delimitador de la fecha:       //
//                     Diagonal (/)                                         //
//                     Punto    (.)                                         //
//                     Guion    (-)                                         //
//    Nota: estas variables son definidas en el archivo VAR_GLOB.HPP        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <string.h>
   #include <stdlib.h>
   #include <conio.h>
   #include <stdio.h>
   #include <dos.h>
#ifdef __PROG_DOS__
}
#endif
#include "fechas.hpp"
#pragma hdrstop





static char  *dia_semana[] = {
   "          ",
   "DOMINGO",
   "LUNES",
   "MARTES",
   "MIERCOLES",
   "JUEVES",
   "VIERNES",
   "SABADO"
};

static char  *day_of_week[] = {
   "          ",
   "SUNDAY",
   "MONDAY",
   "TUESDAY",
   "WEDNESDAY",
   "THURSDAY",
   "FRIDAY",
   "SATURDAY"
};

static char  *mes_del_ano[] = {
   "          ",
   "ENERO",
   "FEBRERO",
   "MARZO",
   "ABRIL",
   "MAYO",
   "JUNIO",
   "JULIO",
   "AGOSTO",
   "SEPTIEMBRE",
   "OCTUBRE",
   "NOVIEMBRE",
   "DICIEMBRE"
};

static char  *month_of_year[] =  {
	"          ",
   "JUNUARY",
   "FEBRUARY",
   "MARCH",
   "APRIL",
   "MAY",
   "JUNE",
   "JULY",
   "AUGUST",
   "SEPTEMBER",
   "OCTUBER",
   "NOVEMBER",
   "DECEMBER"
};







// Regresa la fecha actual del sistema con formato cadena en la variable FECHA
// Nota: Esta rutina modifica la variable FECHA en su valor
void Fechas::Fecha_hoy(char *fecha)
{
   char xfecha[30];
   unsigned int dia, mes, ano;
   Fecha_hoy(dia,mes,ano);
   Gregoriano_a_fecha(dia,mes,ano,xfecha);
   strcpy(fecha,xfecha);
}

// Regresa la fecha actual del sistema con formato gregoriano en las variables DIA, MES, ANO
// Nota: Esta rutina modifica las variables DIA, MES, ANO en su valor
void Fechas::Fecha_hoy(unsigned int &dia, unsigned int &mes, unsigned int &ano)
{
   struct date e_f;
   getdate(&e_f);
   dia = (unsigned int) e_f.da_day;
   mes = (unsigned int) e_f.da_mon;
   ano = (unsigned int) e_f.da_year;
}

// De una fecha con formato gregoriano, retorna una fecha con formato cadena
// En caso de error en la fecha retornar  una cadena en blanco
void Fechas::Gregoriano_a_fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano, char *fecha)
{
    unsigned int n_m_d, i = 0, xa = 0;
    n_m_d = Numero_max_dias_mes(mes,ano);
    if (dia == 0 || dia > n_m_d || mes == 0 || mes > 12 || ano == 0 || ano > 2100) {
       strcpy(fecha,"        ");
       return;
    }

    char  xf[30], ldia[30], lmes[30], x_cad[100];
    itoa(dia,x_cad,10);
    if (dia > 9) {
       ldia[0] = x_cad[0];
       ldia[1] = x_cad[1];
     } else {
       ldia[0] = 48;
       ldia[1] = x_cad[0];
    }

    itoa(mes,x_cad,10);
    if (mes > 9) {
       lmes[0] = x_cad[0];
       lmes[1] = x_cad[1];
     } else {
       lmes[0] = 48;
       lmes[1] = x_cad[0];
    }

    xa = ano;
    if (ano < 100) xa +=  (Centuria_numerica(ano) * 100);
    itoa(xa,x_cad,10);

    if (TIPO_FECHA != 1 && TIPO_FECHA != 2) {
       xf[0] = ldia[0];
       xf[1] = ldia[1];
       xf[2] = DELIMIT_FECHA;
       xf[3] = lmes[0];
       xf[4] = lmes[1];
       xf[5] = DELIMIT_FECHA;
       if (CENTURIA) {
          xf[6] = x_cad[0];
          xf[7] = x_cad[1];
          i = 2;
       }
       xf[6 + i] = x_cad[2];
       xf[7 + i] = x_cad[3];
       xf[8 + i] = 0;
    }


    if (TIPO_FECHA == 1) {
       xf[0] = lmes[0];
       xf[1] = lmes[1];
       xf[2] = DELIMIT_FECHA;
       xf[3] = ldia[0];
       xf[4] = ldia[1];
       xf[5] = DELIMIT_FECHA;
       if (CENTURIA) {
          xf[6] = x_cad[0];
          xf[7] = x_cad[1];
          i = 2;
       }
       xf[6 + i] = x_cad[2];
       xf[7 + i] = x_cad[3];
       xf[8 + i] = 0;
    }

    if (TIPO_FECHA == 2) {
       if (CENTURIA) {
          xf[0] = x_cad[0];
          xf[1] = x_cad[1];
          i = 2;
       }
       xf[0 + i] = x_cad[2];
       xf[1 + i] = x_cad[3];
       xf[2 + i] = DELIMIT_FECHA;
       xf[3 + i] = lmes[0];
       xf[4 + i] = lmes[1];
       xf[5 + i] = DELIMIT_FECHA;
       xf[6 + i] = ldia[0];
       xf[7 + i] = ldia[1];
       xf[8 + i] = 0;
    }
    strcpy(fecha,xf);
}


// Retorna el numero maximo de dias en el mes al que corresponde la fecha con formato cadena
// Nombre del archivo: F_NMAXDC.CPP
unsigned int Fechas::Numero_max_dias_mes(const char  *fecha)
{
   unsigned int dia, mes, ano;
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   unsigned int n_m_d = Numero_max_dias_mes(mes,ano);
   return n_m_d;
}

// Retorna el numero maximo de dias por el mes (MES) y ano (ANO) pasados como parametros
// Nombre del archivo: F_NMAXDG.CPP
unsigned int Fechas::Numero_max_dias_mes(const unsigned int mes, const unsigned int ano)
{
   unsigned int n_m_d = 0, n_md[15] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
   if (mes == 0 || mes > 12) n_m_d = 0;
    else {
      if (mes == 2 && (ano % 4  == 0 && (ano % 100 != 0 || ano % 400 == 0))) n_m_d = 29;
       else n_m_d = n_md[mes];
   }
   return n_m_d;
}


// De una fecha con formato cadena, retorna el n£mero de D¡as, Meses, y A¤os
//     La variable DIA es del rango [0 - 31]
//                 MES es del rango [0 - 12]
//                 ANO es del rango [0 - 2100]
//  Esta rutina modifica las variables DIA, MES, ANO en su valor
//  En caso de error en la fecha retornar  (0) en las variables DIA, MES, ANO
void Fechas::Fecha_a_gregoriano(const char  *fecha, unsigned int &dia, unsigned int &mes, unsigned int &ano)
{
   char  x_m[4], x_d[4] , x_a[6];
   unsigned int i = 0;
   if (TIPO_FECHA == 0) {
      x_d[0] = fecha[0];
      x_d[1] = fecha[1];
      x_d[2] = 0;
      x_m[0] = fecha[3];
      x_m[1] = fecha[4];
      x_m[2] = 0;
      if (CENTURIA) {
         x_a[0] = fecha[6];
         x_a[1] = fecha[7];
         i = 2;
      }
      x_a[0 + i] = fecha[6 + i];
      x_a[1 + i] = fecha[7 + i];
      x_a[2 + i] = 0;
   }
   if (TIPO_FECHA == 1) {
      x_m[0] = fecha[0];
      x_m[1] = fecha[1];
      x_m[2] = 0;
      x_d[0] = fecha[3];
      x_d[1] = fecha[4];
      x_d[2] = 0;
      if (CENTURIA) {
         x_a[0] = fecha[6];
         x_a[1] = fecha[7];
         i = 2;
      }
      x_a[0 + i] = fecha[6 + i];
      x_a[1 + i] = fecha[7 + i];
      x_a[2 + i] = 0;
   }
   if (TIPO_FECHA == 2) {
      if (CENTURIA) {
         i = 2;
         x_a[0] = fecha[0];
         x_a[1] = fecha[1];
      }
      x_a[i] = fecha[i];
      x_a[1 + i] = fecha[i + 1];
      x_a[2 + i] = 0;
      x_m[0] = fecha[i + 3];
      x_m[1] = fecha[i + 4];
      x_m[2] = 0;
      x_d[0] = fecha[i + 6];
      x_d[1] = fecha[i + 7];
      x_d[2] = 0;
   }
   unsigned int xd, xm, xa, n_m_d;
   xd = atoi(x_d);
   xm = atoi(x_m);
   xa = atoi(x_a);
   n_m_d = Numero_max_dias_mes(xm,xa);
   if (xd > n_m_d || xm > 12) xd = 0, xm = 0, xa = 0;
   dia = xd, mes = xm, ano = xa;
}



// De una fecha con formato cadena, retorna el numero de d¡a de la semana con rango [0 - 7]
// (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
// (6) Viernes, (7) Sabado, (0) Error en la fecha juliana
// Nombre del archivo: F_D_SEMC.CPP
unsigned int Fechas::Dia_semana(const char  *fecha)
{
   unsigned long dia_juliano;
   Fecha_a_Juliano(fecha,dia_juliano);
   unsigned int d_s =  Dia_semana(dia_juliano);
   return d_s;
}


// De una fecha con formato gregoriano, retorna el numero de d¡a de la semana con rango [0 - 7]
// (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
// (6) Viernes, (7) Sabado, (0) Error en la fecha juliana
unsigned int Fechas::Dia_semana(const unsigned int dia, const unsigned int mes, const unsigned int ano)
{
   unsigned long d_j;
   Gregoriano_a_juliano(dia,mes,ano,d_j);
   unsigned int d_s = Dia_semana(d_j);
   return d_s;
}


// De una fecha con formato juliano, retorna el numero de d¡a de la semana con rango [0 - 7]
// (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
// (6) Viernes, (7) Sabado, (0) Error en la fecha juliana
unsigned int Fechas::Dia_semana(const unsigned long dia_juliano)
{
   unsigned int d_s;
   if (dia_juliano > 0) d_s = (unsigned int)  ((( dia_juliano + 1) % 7) + 1);
   return d_s;
}



// Regresa el nombre del d¡a de la semana en letra, apartir de una fecha con formato cadena
//     Si T_L es (0) ser  en ingles
//               (1) ser  en espa¤ol
// Si hay error en la fecha retornar  una cadena de espacios
// Nombre del archivo: F_DIALEC.CPP
void Fechas::Dia_letra(const char  *fecha, char  *d_let, const unsigned int t_l)
{
   char  xf[15];
   unsigned int n_d = Dia_semana(fecha);
   Dia_letra(n_d,xf,t_l);
   strcpy(d_let,xf);
}

// Regresa el nombre del d¡a de la semana en letra, apartir de el numero de dia
//     Si T_L es (0) ser  en ingles
//               (1) ser  en espa¤ol
// Si hay error en la fecha retornar  una cadena de espacios
// Nombre del archivo: F_DIALEG.CPP
void Fechas::Dia_letra(const unsigned int n_dia, char  *d_let, const unsigned int t_l)
{

   unsigned int n_d = n_dia;
   if (n_d > 7) n_d = 0;
   if (t_l) strcpy(d_let,dia_semana[n_d]);
    else strcpy(d_let,day_of_week[n_d]);
}

// Regresa el nombre del d¡a de la semana en letra, apartir de una fecha con formato juliano
//     Si T_L es (0) ser  en ingles
//               (1) ser  en espa¤ol
// Si hay error en la fecha retornar  una cadena de espacios
// Nombre del archivo: F_DIALEJ.CPP
void Fechas::Dia_letra(const unsigned long dia_jul, char  *d_let, const unsigned int t_l)
{
   char  xf[15];
   unsigned int n_d = Dia_semana(dia_jul);
   Dia_letra(n_d,xf,t_l);
   strcpy(d_let,xf);
}
/*

// Regresa la fecha actual del sistema con formato juliano en la variable DIA_JUL
// Nota: Esta rutina modifica la variable DIA_JUL en su valor
void f_f_hoy(unsigned long &dia_jul)
{
   unsigned long d_j;
   unsigned int dia, mes, ano;
   f_f_hoy(dia,mes,ano);
   Gregoriano_a_juliano(dia,mes,ano,d_j);
   dia_jul = d_j;
}
*/

// De una fecha con formato cadena, retorna la fecha escrita con el sigiente formato:
// Si T_P es (0) el formato es:
//    Si ID es (1) el formato es [DD DE MMMMMMMM DE CCAA] en español
//    Si ID es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
// Si T_P es (1) el formato es:
//    Si ID es (1) el formato es [DDDDDDDD DD DE MMMMMMMM DE CCAA] en español
//    Si ID es (0) el formato es [DDDDDDDD DD OF MMMMMMMM OF CCAA] en ingles
void Fechas::Fecha_escrita(const char  *fecha, char  *fec_esc, const unsigned int t_p, const unsigned int id)
{
   char  xf[50], xcad[20];
   unsigned int dia, mes, ano;
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   Fecha_escrita(dia,mes,ano,xf,id);
   if (t_p) {
      Dia_letra(fecha,xcad,id);
      sprintf(fec_esc,"%s %s",xcad,xf);
   } else strcpy(fec_esc,xf);
}

// De una fecha con formato gregoriano, retorna la fecha escrita con el sigiente formato:
// Si T_P es (1) el formato es [DD DE MMMMMMMM DE CCAA] en español
// Si T_P es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
void Fechas::Fecha_escrita(const unsigned int dia, const unsigned int mes, const unsigned int ano, char  *fec_esc, const unsigned int t_p)
{
   char  m_l[40];
   Mes_letra(mes,m_l,t_p);
   if (t_p) sprintf(fec_esc,"%u DE %s DE %u",dia,m_l,ano);
    else sprintf(fec_esc,"%u OF %s OF %u",dia,m_l,ano);
}

// De una fecha con formato juliano, retorna la fecha escrita con el sigiente formato:
// Si T_P es (1) el formato es [DD DE MMMMMMMM DE CCAA] en español
// Si T_P es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
// Nombre del archivo: F_FECESJ.CPP
void Fechas::Fecha_escrita(const unsigned long dia_jul, char  *fec_esc, const unsigned int t_p)
{
   char  xf[50];
   unsigned int dia, mes, ano;
   Juliano_a_gregoriano(dia_jul,dia,mes,ano);
   Fecha_escrita(dia,mes,ano,xf,t_p);
   strcpy(fec_esc,xf);
}


// De una fecha con formato cadena, retorna una fecha con formato juliano
// En caso de error en la fecha retornar  (0) en la variable DIA_JUL
// Nombre del archivo: F_FECJUL.CPP
void Fechas::Fecha_a_Juliano(const char  *fecha, unsigned long &dia_jul)
{
   unsigned long d_j;
   unsigned int dia, mes, ano;
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   Gregoriano_a_juliano(dia,mes,ano,d_j);
   dia_jul = d_j;
}


// De una fecha con formato gregoriano, retorna una fecha con formato juliano
//     La variable DIA es del rango [0 - 31]
//                 MES es del rango [0 - 12]
//                 ANO es del rango [0 - 2100]
void Fechas::Gregoriano_a_juliano(const unsigned int dia, const unsigned int mes, const unsigned int ano, unsigned long &dia_jul)
{
   unsigned int n_m_d = Numero_max_dias_mes(mes,ano);
   long d_j = 0,lmes = (long) mes, ldia = (long) dia, lano = (long) ano;
   if (dia == 0 || dia > n_m_d || mes == 0 || mes > 12 || ano == 0 || ano > 2100) d_j = 0;
    else d_j = ldia - 32075l + 1461l * (lano + 4800l + (lmes - 14l) / 12l) / 4l + 367l * (lmes - 2l - (lmes -14l) / 12l * 12l) / 12l - 3l * ((lano + 4900l + (lmes - 14l) / 12l) / 100l) / 4l;
   dia_jul = (unsigned long) d_j;
}


/*
// De una fecha con formato juliano, regresa una fecha con formato cadena
// En caso de error en la fecha retornar  una cadena en blanco
void f_juli__fecha(const unsigned long dia_jul, char  *fecha)
{
   char  xf[15];
   unsigned int dia, mes, ano;
   Juliano_a_gregoriano(dia_jul,dia,mes,ano);
   Gregoriano_a_fecha(dia,mes,ano,xf);
   strcpy(fecha,xf);
}
*/

// De una fecha con formato juliano, retorna una fecha con formato gregoriano
//     La variable DIA es del rango [0 - 31]
//                 MES es del rango [0 - 12]
//                 ANO es del rango [0 - 2100]
// Nota: Esta rutina modifica las variables DIA, MES, ANO en su valor
// Nombre del archivo: F_JULGRE.CPP
void Fechas::Juliano_a_gregoriano(const unsigned long dia_jul, unsigned int &dia, unsigned int &mes, unsigned int &ano)
{
   unsigned int xd = 0, xm = 0, xa = 0;
   if (dia_jul) {
      long t1 = 0, t2 = 0, an = 0, ms = 0;
      t1 = dia_jul + 68569l;
      t2 = 4l * t1 / 146097l;
      t1 = t1 - (146097l * t2 + 3l) / 4l;
      an = 4000l * (t1 + 1) / 1461001l;
      t1 = t1 - 1461l * an / 4l + 31;
      ms = 80l * t1 / 2447l;
      xd = (unsigned int) (t1 - 2447l * ms / 80l);
      t1 = ms / 11l;
      xm = (unsigned int) (ms + 2l - 12l * t1);
      xa = (unsigned int) (100l * (t2 - 49l) + an + t1);
   }
   if (Valida_fecha(xd,xm,xa)) xd = 0, xm = 0, xa = 0;
   dia = xd, mes = xm, ano = xa;
}


/*
// De una fecha con formato cadena, retorna una cadena del tipo DD/MMM/CCYY,
// el mes (MMM) ser  en letra
//     Si T_L es (0) ser  en ingles
//               (1) ser  en espa¤ol
// Si hay error en la fecha retornar  una cadena de espacios
// Nombre del archivo: F_MES3LE.CPP
void f_f__ddmmmaa(const char  *fecha, const unsigned int t_l,char  *xfecha)
{
    unsigned int dia, mes, ano;
    char  m_l[40];
    Fecha_a_gregoriano(fecha,dia,mes,ano);
    Mes_letra(fecha,m_l,t_l);
    sprintf(xfecha,"%#02d%c%c%c%c%c%#02d",dia,DELIMIT_FECHA,m_l[0],m_l[1],m_l[2],DELIMIT_FECHA,ano);
}
*/

// Regresa el nombre del mes en letra, apartir de una fecha con formato cadena
//     Si T_L es (0) ser  en ingles
//               (1) ser  en español
// Si hay error en la fecha retornar  una cadena de espacios
// Nombre del archivo: F_MESLEC.CPP
void Fechas::Mes_letra(const char  *fecha, char  *m_let, const unsigned int t_l)
{
   char  xf[40];
   unsigned int n_m = Numero_mes(fecha);
   Mes_letra(n_m,xf,t_l);
   strcpy(m_let,xf);
}



// Regresa el nombre del mes en letra, apartir de el numero de mes
//     Si T_L es (0) ser  en ingles
//               (1) ser  en espa¤ol
// Si hay error en la fecha retornar  una cadena de espacios
// Nombre del archivo: F_MESLEG.CPP
void Fechas::Mes_letra(const unsigned int mes, char  *m_let, const unsigned int t_l)
{
   unsigned int xm = mes;
   if (mes == 0 || mes > 12) xm = 0;
   if (t_l) strcpy(m_let,mes_del_ano[xm]);    // Retorna el mes en espa¤ol
    else strcpy(m_let,month_of_year[xm]);    // Retorna el mes en ingles 
}

// Regresa el nombre del mes en letra, apartir de una fecha con formato juliano
//     Si T_L es (0) ser  en ingles
//               (1) ser  en espa¤ol
// Si hay error en la fecha retornar  una cadena de espacios
// Nombre del archivo: F_MESLEJ.CPP
void Fechas::Mes_letra(const unsigned long dia_jul, char  *m_let, const unsigned int t_l)
{
   char  xf[40];
   unsigned int n_m = Numero_mes(dia_jul);
   Mes_letra(n_m,xf,t_l);
   strcpy(m_let,xf);
}


// Regresa el n£mero de a¤o en una fecha en formato cadena, retornando ‚sta
// como un entero del rango [0 - 2100]
// (0) Error en el formato de la fecha
unsigned int Fechas::Numero_ano(const char  *fecha)
{
   unsigned int dia, mes, ano;
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   return ano;
}
/*

// Regresa el n£mero de a¤o en una fecha con formato juliano, retornando esta
// como un entero del rango [0 - 2100]
// (0) Error en el formato de la fecha
// Nombre del archivo: Numero_ano.CPP
unsigned int Numero_ano(const unsigned long dia_jul)
{
   unsigned int dia, mes, ano;
   Juliano_a_gregoriano(dia_jul,dia,mes,ano);
   return ano;
}

// De una fecha con formato cadena, retorna el n£mero de d¡as del mes de con rango [0 - 31]
// en caso de error de la fecha con formato cadena retorna 0.
// Nombre del archivo: F_N_D_MC.CPP
unsigned int f_n_dia_mes(const char  *fecha)
{
   unsigned int dia, mes, ano;
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   return dia;
}

// De una fecha con formato juliano, retorna el n£mero de d¡as del mes de con rango [0 - 31]
// en caso de error de la fecha con formato juliano retorna 0.
// Nombre del archivo: F_N_D_MJ.CPP
unsigned int f_n_dia_mes(const unsigned long dia_jul)
{
   unsigned int dia, mes, ano;
   Juliano_a_gregoriano(dia_jul,dia,mes,ano);
   return dia;
}

*/
// Regresa el n£mero de mes de una fecha en formato de cadena, retornando ‚sta
// como un entero del rango [0 - 12]
// (1) Enero, (2) Febrero, (3) Marzo,      (4) Abril,    (5) Mayo,       (6) Junio
// (7) Julio, (8) Agosto,  (9) Septiembre, (10) Octubre, (11) Noviembre, (12) Diciembre
// (0) Error en el formato de la fecha
unsigned int Fechas::Numero_mes(const char  *fecha)
{
   unsigned int dia, mes, ano;
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   return mes;
}



// Regresa el n£mero de mes de una fecha con formato juliano, retornando ‚sta
// como un entero del rango [0 - 12]
// (1) Enero, (2) Febrero, (3) Marzo,      (4) Abril,    (5) Mayo,       (6) Junio
// (7) Julio, (8) Agosto,  (9) Septiembre, (10) Octubre, (11) Noviembre, (12) Diciembre
// (0) Error en el formato de la fecha
// Nombre del archivo: Numero_mes.CPP
unsigned int Fechas::Numero_mes(const unsigned long dia_jul)
{
   unsigned int dia, mes, ano;
   Juliano_a_gregoriano(dia_jul,dia,mes,ano);
   return mes;
}




// Retorna el numero maximo de dias en el mes al que corresponde la fecha con formato juliano
// Nombre del archivo: F_NMAXDJ.CPP
unsigned int Fechas::Numero_max_dias_mes(const unsigned long dia_jul)
{
   unsigned int dia, mes, ano;
   Juliano_a_gregoriano(dia_jul,dia,mes,ano);
   unsigned int n_m_d = Numero_max_dias_mes(mes,ano);
   return n_m_d;
}

/*
// De una fecha del tipo CCYYMMDD, retorna una fecha en formato cadena
// Nombre del archivo: F_PODELC.CPP
void f_poner_delim(const char  *fecha, char  *fecha_cd)
{
   char  x_a[6], x_m[4], x_d[4], xf[15];
   unsigned int dia, mes, ano;
   x_a[0] = fecha[0];
   x_a[1] = fecha[1];
   x_a[2] = fecha[2];
   x_a[3] = fecha[3];
   x_a[4] = 0;
   x_m[0] = fecha[4];
   x_m[1] = fecha[5];
   x_m[2] = 0;
   x_d[0] = fecha[6];
   x_d[1] = fecha[7];
   x_d[2] = 0;
   dia =  atoi(x_d);
   mes =  atoi(x_m);
   ano =  atoi(x_a);
   Gregoriano_a_fecha(dia,mes,ano,xf);
   strcpy(fecha_cd,xf);
}


// De una fecha con formato gregoriano, retorna una fecha con formato cadena
// Nombre del archivo: F_PODELG.CPP
void f_poner_delim(const unsigned int dia, const unsigned int mes, const unsigned int ano, char  *fecha_cd)
{
   char  xf[15];
   Gregoriano_a_fecha(dia,mes,ano,xf);
   strcpy(fecha_cd,xf);
}

// De una fecha con formato juliano, retorna una fecha con formato cadena
// Nombre del archivo: F_PODELJ.CPP
void f_poner_delim(const unsigned long dia_jul, char  *fecha_cd)
{
   char  xf[15];
   f_juli__fecha(dia_jul,xf);
   strcpy(fecha_cd,xf);
}

// De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
// En caso de error retorna "00000000" en forma de cadena
// Nombre del archivo: F_QUIDEL.CPP
void f_quitar_delim(const char  *fecha, char  *fecha_s_d)
{
   unsigned int dia, mes, ano;
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   if (ano < 100) ano += (Centuria_numerica(ano) * 100);
   sprintf(fecha_s_d,"%#04d%#02d%#02d",ano,mes,dia);
}


// De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
// En caso de error retorna "00000000" en forma de cadena
// Nombre del archivo: F_QUIDEL.CPP
void f_quitar_delim(const unsigned int dia, const unsigned int mes, const unsigned int ano, char  *fecha_s_d)
{
   unsigned int xa = ano;
   if (xa < 100) xa += (Centuria_numerica(xa) * 100);
   sprintf(fecha_s_d,"%#04d%#02d%#02d",xa,mes,dia);
}


// De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
// En caso de error retorna "00000000" en forma de cadena
// Nombre del archivo: F_QUIDEL.CPP
void f_quitar_delim(const unsigned long dia_jul, char  *fecha_s_d)
{
   unsigned int dia, mes, ano;
   Juliano_a_gregoriano(dia_jul,dia,mes,ano);
   if (ano < 100) ano += (Centuria_numerica(ano) * 100);
   sprintf(fecha_s_d,"%#04d%#02d%#02d",ano,mes,dia);
}
*/


// Valida una fecha con formato cadena, retornando:
//     (0) Sin errores
//     (1) Error en los D¡as
//     (2) Error en los Mes
//     (3) Error en los A¤os
//     (4) Error en el delimitador
unsigned int Fechas::Valida_fecha(const char  *fecha)
{
   unsigned int mes, dia, ano, t_r = 0, n_m_d;

   if (TIPO_FECHA == 0 || TIPO_FECHA == 1) {
      if (fecha[2] != DELIMIT_FECHA || fecha[5] != DELIMIT_FECHA) t_r = 4;
      if (!Es_caracter_digito(fecha[0]) || !Es_caracter_digito(fecha[1])) t_r = 1;
      if (!Es_caracter_digito(fecha[3]) || !Es_caracter_digito(fecha[4])) t_r = 2;
      if (!Es_caracter_digito(fecha[6]) || !Es_caracter_digito(fecha[7])) t_r = 3;
      if (CENTURIA) {
         if (!Es_caracter_digito(fecha[8]) || !Es_caracter_digito(fecha[9])) t_r = 3;
      }
   }
   if (TIPO_FECHA == 2) {
      if (CENTURIA) {
         if (fecha[4] != DELIMIT_FECHA || fecha[7] != DELIMIT_FECHA) t_r = 4;
         if (!Es_caracter_digito(fecha[0]) || !Es_caracter_digito(fecha[1]) || !Es_caracter_digito(fecha[2]) || !Es_caracter_digito(fecha[3])) t_r = 3;
         if (!Es_caracter_digito(fecha[5]) || !Es_caracter_digito(fecha[6])) t_r = 2;
         if (!Es_caracter_digito(fecha[8]) || !Es_caracter_digito(fecha[9])) t_r = 1;
       } else {
         if (fecha[2] != DELIMIT_FECHA || fecha[5] != DELIMIT_FECHA) t_r = 4;
         if (!Es_caracter_digito(fecha[0]) || !Es_caracter_digito(fecha[1])) t_r = 3;
         if (!Es_caracter_digito(fecha[3]) || !Es_caracter_digito(fecha[4])) t_r = 2;
         if (!Es_caracter_digito(fecha[6]) || !Es_caracter_digito(fecha[7])) t_r = 1;
      }
   }
   Fecha_a_gregoriano(fecha,dia,mes,ano);
   n_m_d = Numero_max_dias_mes(mes,ano);
   if (dia == 0 || dia > n_m_d) t_r = 1;
   if (mes == 0 || mes > 12)    t_r = 2;
   if (ano == 0)  t_r = 3;
   return t_r;
}


// Valida una fecha con formato gregoriano, retornando:
//     (0) Sin errores
//     (1) Error en los D¡as
//     (2) Error en los Mes
//     (3) Error en los A¤os
// Nombre del archivo: F_VALFEG.CPP
unsigned int Fechas::Valida_fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano)
{
   unsigned int t_r = 0, n_m_d = Numero_max_dias_mes(mes,ano);
   if (dia == 0 || dia > n_m_d) t_r = 1;
   if (mes == 0 || mes > 12)    t_r = 2;
   if (ano == 0)  t_r = 3;
   return t_r;
}






/*
// Retorna la diferencia en horas y minutos de un par de fechas con la hora corespondiente
// Retorna -1 en HOR y MIN si existe alguna inconsistencia en fechas o horas
// Nota: Los segundos no son tomados en cuenta
void cal_horas__fec_hor(const char  *cf1, const char  *ch1, const char  *cf2, const char  *ch2, int &hor, int &min)
{
   hor = -1, min = -1;
   if (Valida_fecha(cf1) || Valida_fecha(cf2)) return;
   char  xc[40];
   long f1 = 0, f2 = 0, h1 = 0, h2 = 0, diff = 0;
   int  n_d = 0;
   f_quitar_delim(cf1,xc);
   f1 = atol(xc);
   f_quitar_delim(cf2,xc);
   f2 = atol(xc);
   strcpy(xc,ch1);
   xc[5] = ':';
   xc[6] = '0';
   xc[7] = '0';
   xc[8] = 0;
   h1 = horas__n_seg(xc);
   strcpy(xc,ch2);
   xc[5] = ':';
   xc[6] = '0';
   xc[7] = '0';
   xc[8] = 0;
   h2 = horas__n_seg(xc);
   if ((f1 == f2 && h1 > h2) || f1 > f2 || h1 == -1 || h2 == -1) return;
   if (f1 <= f2 && h1 <= h2)  diff = h2 - h1, n_d = int (f2 - f1);
    else  diff = (86400L + h2) - h1, n_d = int (f2 - (f1 + 1));
   n_seg__horas(diff,xc);
   xc[2] = 0;
   hor = atoi(xc) + (n_d * 24);
   xc[0] = 32;
   xc[1] = 32;
   xc[2] = 32;
   xc[5] = 0;
   min = atoi(xc);
}

*/

// Retorna la centuria de acuerdo al año pasado como parametro en forma de cadena
char *Fechas::Centuria_caracter(const int ano)
{  
   TMPCENTURIA[0] = 0;
   if (ano < 100) {
      if (ano < 85) {
         TMPCENTURIA[0] = '2';
         TMPCENTURIA[1] = '0';
         TMPCENTURIA[2] = 0;
       } else {
         TMPCENTURIA[0] = '1';
         TMPCENTURIA[1] = '9';
         TMPCENTURIA[2] = 0;
      }
   }
   return TMPCENTURIA;
}

// Retorna la centuria de acuerdo al año pasado como parametro en foma numerica
int Fechas::Centuria_numerica(const int ano)
{  
   if (ano < 100) {
      if (ano < 85) return 20;
       else return 19;
   }
   return 0;
}
