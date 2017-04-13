// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



// Definicion de la clase seleccionador


#ifndef __SELECC_HPP__
#define __SELECC_HPP__

#include "boton.hpp"


// Clase Boton de texto
class Seleccionador: public Boton {
   public:
            // Costructor de la clase
            Seleccionador(void)
            {Boton_activo = 0;}
            // Define los elementos del boton
      void  Define_boton(const int x, const int y, const int tp = 0, const int lg = 10, const int cl = Blanco) const;
            // Dibuja el boton
      void  Dibuja(void) const;
};


#endif
