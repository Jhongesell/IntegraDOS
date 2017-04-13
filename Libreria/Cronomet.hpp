// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#include "hor_seg.hpp"

// Clase Cronometro
class Cronometro 
{
   private:
      unsigned int Activo:1;
      long         Segundos_inicio;
      long         Segundos_final;
      long         Segundos_almacen_1;

   public:

           Cronometro(void)
           {Activo = 0, Segundos_inicio = Segundos_final = 0;}
      void Inicia(void)
           {activo = 1; Segundos_inicio = n_seg();}
      void Finaliza(void)
           {Activo = 0; Segundos_final = n_seg();}
      void Reinicializa(void)
           {Activo = 1; Segundos_inicio = n_seg() - tiempo();}
      long Tiempo(void)
           {return (Activo ? (n_seg() - Segundos_inicio):(Segundos_final - Segundos_inicio) );}
};

