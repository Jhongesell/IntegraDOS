// Autor:     Antonio Carrillo Ledesma
// Direcci¢n: Amsterdam 312, colonia Hipodromo Condesa
// Telefono:  574-43-53
// Propiedad intelectual, derechos reservados conforme a la ley (Registro en tramite)
// 1993 -1994



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
           {Activo = Segundos_inicio = Segundos_final = 0;}
      void Inicia(void)
           {activo = 1; Segundos_inicio = n_seg();}
      void Finaliza(void)
           {Activo = 0; Segundos_final = n_seg();}
      void Reinicializa(void)
           {Activo = 1; Segundos_inicio = n_seg() - tiempo();}
      long Tiempo(void)
           {return (Activo ? (n_seg() - Segundos_inicio):(Segundos_final - Segundos_inicio) );}
};

