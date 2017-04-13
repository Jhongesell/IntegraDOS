// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



// Definicion de la clase boton de texto


#ifndef __BOTON_HPP__
#define __BOTON_HPP__


#include "v_marco.hpp"
#include "mouse.hpp"


// Clase Boton
class Boton: public Ventana_marco {

   protected:
                     
      unsigned int   Boton_activo:   1;  // Indica si el boton esta activo o no

   public:
                     // Indica la posicion donde se visualizara el boton
               void  Abrir(const unsigned int x, const unsigned y, const int lgx = 36, const int lgy = 36, const int cf = Gris9) const
                     {Ventana_marco::Define_ventana(x,y,x+lgx,y+lgy,cf);}
                     // Revisa si el boton es oprimido por el mouse dentro de la ventana
               int   Oprimido(void) const;
                     // Revisa si el boton del mouse es oprimido dentro de la ventana
     unsigned  int   Presionado(const unsigned int boton) const;
                     // Dibuja el boton
     virtual   void  Dibuja(void) const = 0;
                     // Activa el boton
     virtual   void  Activar(const unsigned int tp) const
                     {Boton_activo = tp;}
                     // Retorna el estado del boton
     virtual   int   Estado(void) const
                     {return Boton_activo;}

};


/*
    La clase de BOTON hereda todos los comportamientos de la
    clase Ventana_marco, asi como los comportamientos pertinentes
    del manejo de botones.

   Forma de uso:
      Primero defina el objeto:
         Ya sea dinamico o estatico

      Despues defina:
         Abrir
           Indica la posicion del icono dentro de la pantalla
           y la longitud de este
         Activar
           Indica si el boton estara o no activo
      Despues:
         Dibuja
           Dibuja el boton (solo el recuadro)

      Para controlar la ventana use:
         Oprimido
            Revisa si fue presionado el boton
         Estado
            Devuelve el estado del boton

      Despues puede utilizar todos los demas comportamientos segun sean
      las necesidades del programa. al destruirse el objeto restaura la
      ventana original (si se grabo esta) o redibuja con el color de
      refresco (si este fue activado)


*/


#endif

