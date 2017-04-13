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



#ifndef __FECHAS_HPP__
#define __FECHAS_HPP__


#include "cadenas.hpp"

class Fechas: virtual public Cadenas 
{

    private:
    

//       const char *ACT_CENTURIA;         // Indica la actual centuria

      void Gregoriano_a_fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano, char  *fecha);
        // De una fecha con formato gregoriano, retorna una fecha con formato cadena
        // En caso de error en la fecha retornar  una cadena en blanco

      void Fecha_a_gregoriano(const char  *fecha, unsigned int &dia, unsigned int &mes, unsigned int &ano);
        // De una fecha con formato cadena, retorna el n£mero de D¡as, Meses, y A¤os
        //  Esta rutina modifica las variables DIA, MES, ANO en su valor
        //  En caso de error en la fecha retornar  (0) en las variables DIA, MES, ANO



    public:
    
      char  TIPO_FECHA;                 // Define el formato de la cadena fecha
                                        // si TIPO_FECHA es (0)  el formato es DD/MM/[CC]YY
                                        //                  (1)  el formato es MM/DD/[CC]YY
                                        //                  (2)  el formato es [CC]YY/MM/DD
                                        // [CC] existira si la variable CENTURIA es (1)
      
      char  DELIMIT_FECHA;              // Define el caracter delimitador de la fecha
                                        // Puede ser:  diagonal (/)
                                        //               punto    (.)
                                        //               guion    (-)
     
      char  CENTURIA;                   // Indica se esta activa la centuria en la cadena fecha
                                        // si CENTURIA es  (0)  no esta activa la centuria
                                        //                 (1)  si esta activa la centuria
      char  TMPCENTURIA[4];

             Fechas(void)
             {
                TIPO_FECHA     = 0;
                DELIMIT_FECHA  = '/';
                CENTURIA       = 1;   
//                ACT_CENTURIA   = "19";
             }

      void Fecha_hoy(char *fecha);
        // Regresa la fecha actual del sistema con formato cadena en la variable FECHA
        // Nota: Esta rutina modifica la variable FECHA en su valor

      void Fecha_hoy(unsigned int &dia, unsigned int &mes, unsigned int &ano);
        // Regresa la fecha actual del sistema con formato gregoriano en las variables DIA, MES, ANO
        // Nota: Esta rutina modifica las variables DIA, MES, ANO en su valor


      unsigned int Numero_max_dias_mes(const unsigned int mes, const unsigned int ano);
        // Retorna el numero maximo de dias por el mes (MES) y ano (ANO) pasados como parametros

      unsigned int Numero_max_dias_mes(const unsigned long dia_jul);
        // Retorna el numero maximo de dias en el mes al que corresponde la fecha con formato juliano

      unsigned int Numero_max_dias_mes(const char  *fecha);
        // Retorna el numero maximo de dias en el mes al que corresponde la fecha con formato cadena


      void Gregoriano_a_juliano(const unsigned int dia, const unsigned int mes, const unsigned int ano, unsigned long &dia_jul);
        // De una fecha con formato gregoriano, retorna una fecha con formato juliano
        //     La variable DIA es del rango [0 - 31]
        //                 MES es del rango [0 - 12]
        //                 ANO es del rango [0 - 2100]
        // Nota: En caso de error en la fecha con formato gregoriano retorna cero como dia juliano


      void Juliano_a_gregoriano(const unsigned long dia_jul, unsigned int &dia, unsigned int &mes, unsigned int &ano);
        // De una fecha con formato juliano, retorna una fecha con formato gregoriano
        //     La variable DIA es del rango [0 - 31]
        //                 MES es del rango [0 - 12]
        //                 ANO es del rango [0 - 2100]
        // Nota: Esta rutina modifica las variables DIA, MES, ANO en su valor
        //       en caso de error en la fecha con formato juliano retorna cero en las variables DIA, MES, ANO
/*



      void f_f_hoy(unsigned long &dia_jul);
        // Regresa la fecha actual del sistema con formato juliano en la variable DIA_JUL
        // Nota: Esta rutina modifica la variable DIA_JUL en su valor


      void f_f_hoy(unsigned int &dia, unsigned int &mes, unsigned int &ano);
        // Regresa la fecha actual del sistema con formato gregoriano en las variables DIA, MES, ANO
        // Nota: Esta rutina modifica las variables DIA, MES, ANO en su valor
*/

      unsigned int Dia_semana(const char  *fecha);
        // De una fecha con formato cadena, retorna el numero de d¡a de la semana con rango [0 - 7]
        // (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
        // (6) Viernes, (7) Sabado, (0) Error en la fecha juliana


      unsigned int Dia_semana(const unsigned int dia, const unsigned int mes, const unsigned int ano);
        // De una fecha con formato gregoriano, retorna el numero de d¡a de la semana con rango [0 - 7]
        // (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
        // (6) Viernes, (7) Sabado, (0) Error en la fecha juliana


      unsigned int Dia_semana(const unsigned long dia_jul);
        // De una fecha con formato juliano, retorna el numero de d¡a de la semana con rango [0 - 7]
        // (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
        // (6) Viernes, (7) Sabado, (0) Error en la fecha juliana


/*

        unsigned int f_n_dia_mes(const char  *fecha);
        // De una fecha con formato cadena, retorna el n£mero de d¡as del mes de con rango [0 - 31]
        // en caso de error de la fecha con formato cadena retorna 0.


      unsigned int f_n_dia_mes(const unsigned long dia_jul);
        // De una fecha con formato juliano, retorna el n£mero de d¡as del mes de con rango [0 - 31]
        // en caso de error de la fecha con formato juliano retorna 0.

*/
      unsigned int Numero_mes(const char  *fecha);
        // Regresa el n£mero de mes de una fecha en formato de cadena, retornando ‚sta
        // como un entero del rango [0 - 12]
        // (1) Enero, (2) Febrero, (3) Marzo,      (4) Abril,    (5) Mayo,       (6) Junio
        // (7) Julio, (8) Agosto,  (9) Septiembre, (10) Octubre, (11) Noviembre, (12) Diciembre
        // (0) Error en el formato de la fecha


      unsigned int Numero_mes(const unsigned long dia_jul);
        // Regresa el n£mero de mes de una fecha con formato juliano, retornando ‚sta
        // como un entero del rango [0 - 12]
        // (1) Enero, (2) Febrero, (3) Marzo,      (4) Abril,    (5) Mayo,       (6) Junio
        // (7) Julio, (8) Agosto,  (9) Septiembre, (10) Octubre, (11) Noviembre, (12) Diciembre
        // (0) Error en el formato de la fecha


      unsigned int Numero_ano(const char  *fecha);
        // Regresa el n£mero de a¤o en una fecha en formato cadena, retornando ‚sta
        // como un entero del rango [0 - 2100]
        // (0) Error en el formato de la fecha
/*

      unsigned int Numero_ano(const unsigned long dia_jul);
        // Regresa el n£mero de a¤o en una fecha con formato juliano, retornando esta
        // como un entero del rango [0 - 2100]
        // (0) Error en el formato de la fecha

*/
      void Dia_letra(const char  *fecha, char  *d_let, const unsigned int t_l = 1);
        // Regresa el nombre del d¡a de la semana en letra, apartir de una fecha con formato cadena
        //     Si T_L es (0) ser  en ingles
        //               (1) ser  en espa¤ol
        // Si hay error en la fecha retornar  una cadena de espacios


      void Dia_letra(const unsigned int n_dia, char  *d_let, const unsigned int t_l = 1);
        // Regresa el nombre del d¡a de la semana en letra, apartir de el numero de dia
        //     Si T_L es (0) ser  en ingles
        //               (1) ser  en espa¤ol
        // Si hay error en la fecha retornar  una cadena de espacios


      void Dia_letra(const unsigned long dia_jul, char  *d_let, const unsigned int t_l = 1);
        // Regresa el nombre del d¡a de la semana en letra, apartir de una fecha con formato juliano
        //     Si T_L es (0) ser  en ingles
        //               (1) ser  en español
        // Si hay error en la fecha retornar  una cadena de espacios


      void Mes_letra(const char  *fecha, char  *m_let, const unsigned int t_l = 1);
        // Regresa el nombre del mes en letra, apartir de una fecha con formato cadena
        //     Si T_L es (0) ser  en ingles
        //               (1) ser  en español
        // Si hay error en la fecha retornar  una cadena de espacios


      void Mes_letra(const unsigned int mes, char  *m_let, const unsigned int t_l = 1);
        // Regresa el nombre del mes en letra, apartir de el numero de mes
        //     Si T_L es (0) ser  en ingles
        //               (1) ser  en español
        // Si hay error en la fecha retornar  una cadena de espacios


      void Mes_letra(const unsigned long dia_jul, char  *m_let, const unsigned int t_l = 1);
        // Regresa el nombre del mes en letra, apartir de una fecha con formato juliano
        //     Si T_L es (0) ser  en ingles
        //               (1) ser  en espa¤ol
        // Si hay error en la fecha retornar  una cadena de espacios

      void Fecha_a_Juliano(const char  *fecha, unsigned long &dia_jul);
        // De una fecha con formato cadena, retorna una fecha con formato juliano
        // En caso de error en la fecha retornar  (0) en la variable DIA_JUL
/*


      void f_juli__fecha(const unsigned long dia_jul, char  *fecha);
        // De una fecha con formato juliano, regresa una fecha con formato cadena
        // En caso de error en la fecha retornar  una cadena en blanco

*/
      unsigned int Valida_fecha(const char  *fecha);
        // Valida una fecha con formato cadena, retornando:
        //     (0) Sin errores
        //     (1) Error en los D¡as
        //     (2) Error en los Mes
        //     (3) Error en los A¤os


      unsigned int Valida_fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano);
        // Valida una fecha con formato gregoriano, retornando:
        //     (0) Sin errores
        //     (1) Error en los D¡as
        //     (2) Error en los Mes
        //     (3) Error en los A¤os
/*
      void f_quitar_delim(const char  *fecha, char  *fecha_s_d);
        // De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
        // En caso de error retorna "00000000" en forma de cadena


      void f_quitar_delim(const unsigned int dia, const unsigned int mes, const unsigned int ano, char  *fecha_s_d);
        // De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
        // En caso de error retorna "00000000" en forma de cadena


      void f_quitar_delim(const unsigned long dia_jul, char  *fecha_s_d);
        // De una fecha con formato de cadena, retorna una fecha con formato CCYYMMDD
        // En caso de error retorna "00000000" en forma de cadena


      void f_poner_delim(const char  *fecha, char  *fecha_cd);
        // De una fecha del tipo CCYYMMDD, retorna una fecha en formato cadena


      void f_poner_delim(const unsigned int dia, const unsigned int mes, const unsigned int ano, char  *fecha_cd);
        // De una fecha con formato gregoriano, retorna una fecha con formato cadena


      void f_poner_delim(const unsigned long dia_jul, char  *fecha_cd);
        // De una fecha con formato juliano, retorna una fecha con formato cadena


      void f_f__ddmmmaa(const char  *fecha, const unsigned int t_l, char  *xfecha);
        // De una fecha con formato cadena, retorna una cadena del tipo DD/MMM/CCYY,
        // el mes (MMM) ser  en letra
        //     Si T_L es (0) ser  en ingles
        //               (1) ser  en espa¤ol
        // Si hay error en la fecha retornar  una cadena de espacios

*/

      void Fecha_escrita(const char  *fecha, char  *fec_esc, const unsigned int t_p, const unsigned int id = 1);
        // De una fecha con formato cadena, retorna la fecha escrita con el sigiente formato:
        // Si T_P es (0) el formato es:
        //    Si ID es (1) el formato es [DD DE MMMMMMMM DE CCAA] en español
        //    Si ID es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
        // Si T_P es (1) el formato es:
        //    Si ID es (1) el formato es [DDDDDDDD DD DE MMMMMMMM DE CCAA] en español
        //    Si ID es (0) el formato es [DDDDDDDD DD OF MMMMMMMM OF CCAA] en ingles


      void Fecha_escrita(const unsigned int dia, const unsigned int mes, const unsigned int ano, char  *fec_esc, const unsigned int t_p = 1);
        // De una fecha con formato gregoriano, retorna la fecha escrita con el sigiente formato:
        // Si T_P es (1) el formato es [DD DE MMMMMMMM DE CCAA] en español
        // Si T_P es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles


      void Fecha_escrita(const unsigned long dia_jul, char  *fec_esc, const unsigned int t_p = 1);
        // De una fecha con formato juliano, retorna la fecha escrita con el sigiente formato:
        // Si T_P es (1) el formato es [DD DE MMMMMMMM DE CCAA] en español
        // Si T_P es (0) el formato es [DD OF MMMMMMMM OF CCAA] en ingles
/*

      void cal_horas__fec_hor(const char  *cf1, const char  *ch1, const char  *cf2, const char  *ch2, int &hor, int &min);
        // Retorna la diferencia en horas y minutos de un par de fechas con la hora corespondiente
        // Retorna -1 en HOR y MIN si existe alguna inconsistencia en fechas o horas
        // Nota: Los segundos no son tomados en cuenta

*/
            // Retorna la centuria de acuerdo al año pasado como parametro en forma de cadena
      char *Centuria_caracter(const int ano);
            // Retorna la centuria de acuerdo al año pasado como parametro en foma numerica
      int   Centuria_numerica(const int ano);

};


#endif 

