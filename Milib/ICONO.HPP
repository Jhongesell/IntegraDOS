// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



// Definicion de la clase Icono


#ifndef __ICONO_HPP__
#define __ICONO_HPP__

extern "C" {
  #include <stdlib.h>
}

#include "boton.hpp"



// Clase Icono
class Icono: public Boton {
   private:
      char *Imagen;       // Contiene la imagen que se visualizara en el icono
      int Largo_x;        // Indica el largo en X del icono
      int Largo_y;        // Indica el largo en Y del icono
      int Colores;        // Indica los colores del icono

   public:

            // Costructor y Destructor de la clase Icono
            Icono(void)
            {Imagen = NULL;}
           ~Icono()
            {delete []Imagen;}
            // Define los elementos del boton
      void  Define_icono(const char *archivo, const int x, const int y, const int lgx = 36, const int lgy = 36);
            // Dibuja el icono
      void  Dibuja(void) const;
};

/*
    La clase de ICONO hereda todos los comportamientos de la
    clase BOTON, asi como los comportamientos pertinentes
    del manejo de iconos.

   Forma de uso:
      Primero defina el objeto:
         Ya sea dinamico o estatico

      Despues defina:
         Define_icono
            Define el nombre del archivo a visualizar como
            icono y la posicion de este dentro de la pantalla
      Despues
         Dibuja
           Dibuja el boton (solo el recuadro)

      Para controlar la ventana use
         Oprimido
            Revisa si fue presionado el boton
         Estado
            Devuelve el estado del boton

      Despues puede utilizar todos los demas comportamientos segun sean
      las necesidades del programa. al destruirse el objeto restaura la
      ventana original (si se grabo esta) o redibuja con el color de
      refresco (si este fue activado)

Ejemplo:

   Icono  Ic1;
   Ic1.Define_icono("OK.ICO",50,70);
   Ic1.Dibuja();
   while(1) {
      if(Ic1.Oprimido()) return;
   }


*/


#endif
