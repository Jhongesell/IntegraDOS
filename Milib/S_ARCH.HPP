// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.01-D


// Clase que muestra una ventana en la cual se visualizan los archivos, directorios y unidades
// existentes para que el usuario seleccione un archivo de los mostrados o tecle el nombre de este


#ifndef __S_ARCH_HPP__
#define __S_ARCH_HPP__



#include "v_iconos.hpp"
#include "barr_ind.hpp"
#include "menu.hpp"
#include "get.hpp"
#include "r_var.hpp"
#include "cad_car.hpp"
#include "teclado.hpp"


extern "C" {
   #include <string.h>
}

class Selecciona_muestra_archivos {

   private:

      unsigned int      st:                  1;  // Indica el resultado del ciclo
      int               Num_files;               // Numero de archivos
      int               Num_direc;               // Numero de directorios
      int               Num_unidades;            // Numero de unidades
      char             *Archivo;                 // Nombre del archivo
      char             *Trayectoria;             // Puntero a la cadena que contiene la trayectoria
      const char       *Trayectoria_original;    // Putero a la trayectoria original
      char             *Archivos;                // Puntero a los archivos
      char             *Directorios;             // Puntero a los directorios
      char             *Unidades;                // Puntero a las unidades de disco
      const char      **Arr_menu1;
      const char      **Arr_menu2;
      const char      **Arr_menu3;
      Ventana_iconos   *VT;
      Traza_indicador  *Ia;
      Menu             *Ma;
      Traza_indicador  *Id;
      Menu             *Md;
      Traza_indicador  *Iu;
      Menu             *Mu;
      Get              *Gt;
      Icono            *Iconos;


      // Fija selecion de archivo
      void  Fija_seleccion(const char *arch);
      
   public:
            // Constructo de la clase
            Selecciona_muestra_archivos(void);
            // Destructor de la clase
           ~Selecciona_muestra_archivos();

            // Parametros para la operacion de la clase
      void  Parametros(const char *arch, const char *tit);
            // Controla el ciclo de seleccion del archivo
      int   Ciclo(const unsigned int tp);
            // Cambia el path
      void  Cambia_path(const char *path);
            // Retorna el nombre de archivo seleccionado
      void  Retorna_archivo(char *arch)  const
            {if(st) strcpy(arch,Archivo);}
            // Dibuja la ventana de seleccion de archivos
      void  Dibuja(void);
};



#endif

