// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __M_SISTEM_HPP__
#define __M_SISTEM_HPP__


#include "ctrl_est.hpp"

// Clase de sistemas
class Selecciona_sistema: virtual protected Ctrl_estructura {

   private:
      char *Sistemas;            // Puntero a el nombre de los proyectos
      const char **Arr_menu;     // Puntero a los elementos del menu
      long  x_elem_menu;         // Indica la actual descripcion a visualizar
      int   X1,X2,Y1,Y2;         // Dimencion de la ventana de trabajo

   public:

      void  Parametros(const int x1,const int y1,const int x2,const int y2);
      void  Carga_sistemas(void);
            //Carga la descripcion del sistema
      void  Carga_descripcion_sistema(void);
      void  Controlador(char caracter, int tecla);
      void  Nuevo(void);
      void  Visualiza(void);
};


#endif