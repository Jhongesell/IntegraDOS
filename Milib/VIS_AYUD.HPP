// Autor:     Antonio Carrillo Ledesma
// Direcci¢n: Amsterdam 312, colonia Hip¢dromo Condesa
// Tel‚fono:  574-43-53
// Propiedad intelectual, derechos reservados conforme a la ley (Registro en tr mite)


#ifndef __VIS_AYUD_HPP__
#define __VIS_AYUD_HPP__

#define VISUALIZA  1
#define EDITA      0


#include "v_iconos.hpp"
#include "barr_ind.hpp"
#include "vis_arch.hpp"
#include "edit_arc.hpp"


class Vis_archivo {
   private:
      Ventana_iconos    *Vt;
      Traza_indicador   *Hor;
      Traza_indicador   *Ver;
      Visualiza_archivo *Va;
      Edita_archivo     *Ed;
      int                TP;

   public:
           // Parametros del visualizador
      void Parametros(const char *arch, const char *tex_exp, const int lgx, const int lgy, const int tp);
           // Loop de control
      void Loop(void);
           // Lee elarchivo y lo incerta al final
      void Lee_archivo(const char *arch);
           // Adiciona una linea de texto al final
      void Adiciona_linea(const char *cad);
           // Revisualiza
      void Revisualiza(void);
};

#endif
