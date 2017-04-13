// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __EDIT_ARC_HPP__
#define __EDIT_ARC_HPP__



#include "bvisedi.hpp"

class Edita_archivo: public Base_vis_edi
{
   public:
      void Controlador(const char car, const int tc);
      int  Edita_linea(const char car, const int tc);
      void Activa_sw_grabar(const int sw_grabar)
           {Sw_grabar = sw_grabar;}

};

#endif

