// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#ifndef __CTRL_EST_HPP__
#define __CTRL_EST_HPP__



#include "C:\integra\sys\include\user_def.hpp"
#include "menu.hpp"
#include "barr_ind.hpp"
#include "v_iconos.hpp"
#include "vis_arch.hpp"
#include "\libreria\cadenas.hpp"


// Controla el tipo de versi¢n del sistema

typedef struct Inf_sistema {
   char  *Nombre;                     // Nombre del sistema
   int    N_e_Descripcion;            // Numero de lineas de la descipcion del sistema
   char **Descripcion;                // Descripcion del sistema
   int    N_e_Ecuaciones;             // Numero de lineas de las ecuaciones del sistema
   char **Ecuaciones;                 // Ecuaciones del sistema
   int    N_e_Parametros;             // Numero de lineas de los parametros del sistema
   char **Parametros;                 // Parametros del sistema
   int    N_e_Func_aux;               // Numero de lineas de las funciones auxiliares del sistema
   char **Func_aux;                   // Funciones auxiliares
} Informacion_sistema;


// Estructura a la informacion del proyecto
typedef struct Inf_proyecto {
   int                    Modificada;         // Indica si la estructura fue modificada
   char                  *Nombre;             // Nombre del proyecto
   int                    N_e_Descripcion;    // Numero de elementos de la descripcion
   char                 **Descripcion;        // Contienen la descripcion del proyecto
   int                    Numero_sistemas;    // Indica el numero de sistemas en el proyecto
   Informacion_sistema  **Inf_sistema;        // Contiene a los sistemas del proyecto
} Informacion_proyecto;


// Clase Ctrl_estructura
class Ctrl_estructura: public Cadenas 
{

   private:

           // Genera los archivos ejecutables
      void Genera_cpp(void);
           // Genera los archivos de descripcion
      void Genera_descripcion(void);
           // Remplaza las funciones auxiliares
      void Remplaza_funciones_auxiliares(const char *exp, char *xexp, const unsigned int i);
           // Remplaza las parametros
      void Remplaza_parametros(const char *exp, char *xexp, const unsigned int i);


   protected:
      // Indica el path de los proyectos
      char  PATH_PROYECTOS[200];            

      // Definicion de la estructura de informacion del proyecto
      Inf_proyecto *Informacion;

      // Primera ventana
      Ventana_titulo    *VMA;
      Traza_indicador   *VMAID;
      Menu              *M1;

      // Segunda ventana
      Ventana_titulo    *VDC;
      Traza_indicador   *VDCID;
      Visualiza_archivo *Va;

      int           Niconos;             // Indica el numero de iconos a manejar
      int           Ipresionado;         // Indica el icono presionado
      int           Opcion_actual_menu;  // Indica la opcion actual dentrol del menu
      unsigned int  selec;               // Indica la seleccion del menu de acceso rapido a funciones
      C_2I          vent;                // Cordenadas del menu de acceso rapido
      Icono        *Iconos;              // Puntero a iconos

           // Indica si el nombre del sistema es unico o no
      int  Valida_nombre_sistema(const char *sist);
           // Indica si el nombre del proyecto es unico o no
      int  Valida_nombre_proyecto(const char *proy);


   public:
           // Permite captura las ecuaciones del sistema e introducirlo al arreglo de trabajo
      int  Captura_ecuacion(const char *tit, const int Opcion_actual_menu);
           // Permite capturar la descripcion e introducirlo al arreglo de trabajo
      int  Captura_descripcion(const char *tit, const int tp, const int Opcion_actual_menu);
      void Lee_proyecto(const char *nombre);
      void Imprime_proyecto(void);
      void Imprime_sistema(const unsigned int Opcion_actual_menu);
      void Duplica_sistema(const unsigned int n_s, const char *nombre);
      void Borra_proyecto(void);
      void Borra_sistema(const unsigned int n_s);
      void Grabar(const int tp);
      void Graba_proyecto(void);
      void Generar(void);

      void Libera_memoria(void);
};

#endif
