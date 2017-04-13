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




#ifndef __TIEMPO_HPP__
#define __TIEMPO_HPP__

#include "cadenas.hpp"


class Tiempo: virtual public Cadenas
{


	private:
		const char *CAD_PM;
		const char *CAD_AM;
      const char *PTR_CAD;



   public:

                 // Constructor de la clase
                 Tiempo(void)
                 {
                     CAD_PM = "P.M.";
                     CAD_AM = "A.M.";
                 }


                 // Retorna la hora actual del sistema modificando la cadena CAD, T_H indica si visualiza
                 // en formato  (0) HH:MM:SS                  [HH rango 00 - 23]
                 //             (1) HH:MM:SS [A.M. o P.M.]    [HH rango 00 - 12]
                 //             (2) HH:MM                     [HH rango 00 - 23]
                 //             (3) HH:MM [A.M. o P.M.]       [HH rango 00 - 12]
                 // T_P indica si se remplaza por blanco en primer cero de la hora 
      void       Hora_actual(char *cad, const char t_h, const char t_p);

                 // Retorna de una cadena de horas con formato HH:MM:SS una cadena de hora con 
                 // formato HH:MM:SS [A.M.|P.M.], en caso de error regresa una cadena en blanco
      void       Hrs__am_pm(const char *hora,char *xhora);
/*
                 // Retorna de una cadena de horas, una cadena de horas formateada con [A.M. o P.M.]
                 // en caso de error regresa una cadena en blanco 
      int        Hrs__am_pm(const char *hora, char *xhora);
           
*/
                 // Regresa el numero de segundos desde el comienzo del dia, rango de [0 - 86400]
      long       Numero_segundos(void);


      void Nseg__horas(const long n_s, char *horas);
           // Retorna una cadena de horas a partir de el numero de segundos,
           // la hora es con el formato HH:MM:SS


      long Horas__n_seg(const char *hors);
        // Retorna el numero de segundos que contiene una cadena de hora con
        // formato HH:MM:SS, el rango es de [-1 - 86400], el caso de error (-1L)
/*

      long diff_hors_n(const char *hor1, const char *hor2);
        // Realiza la diferencia entre dos cadenas de hora, con formato HH:MM:SS,
        // retornando el numero de segundos rango [-1 - 86400], en caso de error
        // regresa -1l


      void diff_hors_c(const char *hor1, const char *hor2, char *dif);
        // Realiza la diferencia entre dos cadenas de hora, con formato HH:MM:SS,
        // retornando una cadena de hora con formato HH:MM:SS, en caso de error
        // regresa una cadena en blanco

*/
      void AmPm__hors(const char *hors, char *xhors);
        // Retorna de una cadena de hora formateada con [A.M. o P.M.] una cadena
        // de horas con formato HH:MM:SS, en caso de error regresa una cadena en blanco
/*
      void h_cad__hms(const char *hora, int &n_hor, int &n_min, int &n_seg);
        // Retorna de una cadena del tipo HH:MM:SS el numero de horas, minutos y segundos
        // En caso de error en la cadena de horas retornara (-1) en N_HOR, N_MIN, N_SEG
        // Nombre del archivo: H_HORHMS.CPP

      int  h_ret_horas(const char *hora);
        // Retorna de una cadena del tipo HH:MM:SS el n£mero de horas
        // En caso de error en la cadena de horas retornara (-1)
        // Nombre del archivo: H_R_HOR.CPP

      int  h_ret_minutos(const char *hora);
        // Retorna de una cadena del tipo HH:MM:SS el n£mero de minutos
        // En caso de error en la cadena de horas retornara (-1)
        // Nombre del archivo: H_R_MIN.CPP

      int  h_ret_segundos(const char *hora);
        // Retorna de una cadena del tipo HH:MM:SS el n£mero de segundos
        // En caso de error en la cadena de horas retornara (-1)
        // Nombre del archivo: H_R_SEG.CPP
*/
      int  Valida_hora(const char *hora, const unsigned int t_p = 3);
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
        
};



#endif

