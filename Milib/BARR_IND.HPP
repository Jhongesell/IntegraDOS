// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#ifndef __BARR_IND_HPP__
#define __BARR_IND_HPP__

#include "Icono.hpp"

class Traza_indicador
{
  private:

      unsigned int   Tipo:      1; // Indica el tipo de barra indicadora
      unsigned int   Sw:        1; // Control de visualizacion
      unsigned int   Porcent;      // Porcentaje del total
      char           C_fdo;        // Color de fondo
      char           C_ind;        // Color de indicador
      unsigned int   X1,X2,Y1,Y2;  // Posicion de la barra indicadora
      unsigned int   ant_por;      // Variables temporales
      unsigned int   inc;
      unsigned int   tmp;
      unsigned int   por;
      Icono         *Icon1;        // Icono uno
      Icono         *Icon2;        // Icono dos


   public:
           ~Traza_indicador();
      void  Define_indicador(const int x1, const int y1, const int x2, const int y2,const int tipo = 1);
      int   Controlador(const long ea, const long et, const long ep);
      void  Dibuja(void);
};


#endif


