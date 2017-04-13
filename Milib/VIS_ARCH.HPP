// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __VIS_ARCH_HPP__
#define __VIS_ARCH_HPP__



#include "bvisedi.hpp"

class Visualiza_archivo: public Base_vis_edi
{
   private:
      long  i;                  // Indice para varios usos
   public:
      void Controlador(const int tc);
};

#endif


