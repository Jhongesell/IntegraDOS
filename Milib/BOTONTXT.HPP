// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



// Definicion de la clase boton de texto


#ifndef __BOTONTXT_HPP__
#define __BOTONTXT_HPP__

extern "C" {
   #include <string.h>
}

#include "boton.hpp"


// Clase Boton de texto
class Boton_texto: public Boton {

   private:
      char     *Nombre;        // Contiene el nombre del boton

   public:
            Boton_texto(void)
            {Boton_activo = 0, Nombre = NULL;}
           ~Boton_texto()
            {delete []Nombre;}

            // Define los elementos del boton
      void  Define_boton(const char *nombre, const int x, const int y, const int lgx = 84, const int lgy = 24, const int clf = Gris9, const int clt = Negro) const;
            // Dibuja el boton
      void  Dibuja(void) const;
            // Dibuja el borde indicador de boton activo
      void  Dibuja_indicador(void) const;
            // Define el nombre del boton
      void  Define_nombre(const char *nombre) const;
            // Activa o desactiva el boton redibujando el indicador
      void  Activar(const int tp) const;
};

/*
    La clase de BOTON_TEXTO hereda todos los comportamientos de la
    clase BOTON, asi como los comportamientos pertinentes
    del manejo de botones de texto.

   Forma de uso:
      Primero defina el objeto:
         Ya sea dinamico o estatico

      Despues defina:
         Define_boton
            Define el nombre del boton a visualizar como
            la posicion de este dentro de la pantalla
      Despues
         Dibuja
           Dibuja el boton (solo el recuadro)
         Dibuja_indicador
           Dibuja un indicador si esta activo o no este

      Para controlar la ventana use
         Oprimido
            Revisa si fue presionado el boton
         Estado
            Devuelve el estado del boton
         Activar
            Activa o desactiva el boton

      Despues puede utilizar todos los demas comportamientos segun sean
      las necesidades del programa. al destruirse el objeto restaura la
      ventana original (si se grabo esta) o redibuja con el color de
      refresco (si este fue activado)

Ejemplo:

   Boton_texto     Bt1;
   Bt1.Define_boton(TXT1,x1+20,y2-50);
   Bt1.Dibuja();
   if(Bt1.Mouse_dentro()) // Actividad si esta el mose sobre de el
   if(Bt1.Oprimido())     // Actividad si se presiona este por el mose

*/


#endif

