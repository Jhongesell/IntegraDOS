// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __V_TITULO_HPP__
#define __V_TITULO_HPP__

#include "v_marco.hpp"
#include "colores.hpp"
#include "mouse.hpp"


// Clase ventana con titulo
class Ventana_titulo: public Ventana_marco {

   private:
      char            pt_x1, pt_y1, pt_x2, pt_y2;  // Almacena la posicion del area de titulo
      char            C_fdo;                       // Almacena el color de fondo, titulo y texto
      char            C_tit_act;
      char            C_tit_no_act;
      char            C_txt;
      char           *Titulo;
      char            Lt_xtipo;                    // Almacena el tipo de Letra actual dentro del la ventana
      int             Lt_xmulx;                    // Almacena la escala en X
      char            Lt_xdivx;
      char            Lt_xmuly;                    // Almacena la escala en Y
      char            Lt_xdivy;
      unsigned int    Lt_xhori:     1;             // Almacena la horientacion de la letra
      unsigned int    Tit_centrado: 1;             // Indica si el titulo sera centrado o no


                      // Dibuja el titulo de la ventana
               void   Dibuja_titulo(void) const;


   public:
                     // Constructor y destructor de la clase
                     Ventana_titulo(void);
                    ~Ventana_titulo();

                     // Definicion de la llamada de la ventana con titulo
      virtual  void  Ventana_centrada(const char *tit, const int lgx, const int lgy, const int inc_x=0, const int inc_y=0) const;
      virtual  void  Define_ventana(const char *tit, const int x1, const int y1, const int x2, const int y2) const;
                     // Define el nombre del titulo
               void  Define_titulo(const char *tit) const;
                     // Define la posicion de la ventana dentro del titulo
               void  Define_posicion_titulo(const int x1, const int y1, const int x2, const int y2) const
                     {((Ventana_titulo*)this)->pt_x1 = x1,((Ventana_titulo*)this)->pt_y1 = y1,((Ventana_titulo*)this)->pt_x2 = x2,((Ventana_titulo*)this)->pt_y2 = y2;}
                     // Cambia el tipo de letra, horientacion y tama¤o del titulo
               void  Cambia_tipo_letra_titulo(const int tipo, const int horient, const int multx, const int divx, const int multy, const int divy, const int centr = 1) const
                     {((Ventana_titulo*)this)->Lt_xtipo = tipo, ((Ventana_titulo*)this)->Lt_xhori = horient, ((Ventana_titulo*)this)->Lt_xmulx = multx, ((Ventana_titulo*)this)->Lt_xdivx = divx, ((Ventana_titulo*)this)->Lt_xmuly = multy, ((Ventana_titulo*)this)->Lt_xdivy = divy, ((Ventana_titulo*)this)->Tit_centrado = centr;}
                     // Define los colores que existen el la ventana
      virtual  void  Define_colores(const int cfdo, const int ctita, const int ctitna, const int ctxt) const
                     {((Ventana_titulo*)this)->C_fdo = cfdo, ((Ventana_titulo*)this)->C_tit_act = ctita, ((Ventana_titulo*)this)->C_tit_no_act = ctitna, ((Ventana_titulo*)this)->C_txt = ctxt;}
                     // Dibuja la ventana con el titulo
      virtual  void  Dibuja(void) const
                     {Ventana_marco::Dibuja();Dibuja_titulo();}
                     // Activa o desactiva la ventana (Cambia el color del titulo)
      virtual  void  Ventana_activa(const int tp) const
                     {Ventana_marco::Ventana_activa(tp);Dibuja_titulo();}
                     // Cambia el tamano de la ventana
      virtual  void  Cambiar_tamano(const int x1, const int y1, const int x2, const int y2) const
                     {Ventana_marco::Cambiar_tamano(x1,y1,x2,y2); Ventana_titulo::Dibuja();}
};


/*
Clase Ventana_titulo:

   La clase VENTANA_TITULO permite definir una ventana de trabajo
   con un titulo, asi como todas las actividedes que realiza la
   clase VENTANA_MARCO asi tambien manipular el color y titulo de
   esta.

   Forma de uso:
      Primero defina el objeto:
         Ya sea dinamico o estatico
      Despues defina:
         Activa_graba_ventana
            Por omicion esta apagado, si decea grabarla activarlo con 1.
            (recomendable)
         Activa_color_refresco
            Por omicion no esta activada, activela con el color de el
            fondo antes de activar la ventana (Si no hay texto entes
            es mejor de la ventana usar esta)

      Despues defina:
         Define_ventana
            Define la ventana en una determinada area de la
            ventana dada por X1,Y1,X1,Y2
         Centra_texto
            Define una ventana centrada indicando la longitud
            de esta pormedio de longitud en X y Y

      Despues
         Dibuja
            Dibuja la venatana

      Despues puede utilizar todos los demas comportamientos segun sean
      las necesidades del programa. al destruirse el objeto restaura la
      ventana original (si se grabo esta) o redibuja con el color de
      refresco (si este fue activado)


Ejemplo:

   Ventana_titulo *Vpresent = new Ventana_titulo;
   Vpresent->Activa_graba_ventana(1);
   Vpresent->Ventana_centrada("ABOUT INTEGRA",480,300);
   Vpresent->Dibuja();
   Vpresent->Define_color_texto(Negro);
   Vpresent->Define_tipo_letra(4,0,2,2,2,2);
   Vpresent->Centra_texto(60,NOMBRE_SISTEMA);
   Vpresent->Define_color_texto(Azul9);
   Vpresent->Define_tipo_letra(5,0,1,3,1,3);
   sprintf(xcad,"Version: %s  @",VERSION_SISTEMA);
   Vpresent->Centra_texto(110,xcad);
   Vpresent->Define_color_texto(Azul1);
   Vpresent->Define_tipo_letra(7,0,1,3,1,3);
   Vpresent->Centra_texto(140,DESCRIPCION1);
   Vpresent->Centra_texto(155,DESCRIPCION2);
   Vpresent->Centra_texto(170,DESCRIPCION3);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   Vpresent->Define_color_texto(Negro);
   Vpresent->Visualiza_texto(20,220,"Authors: Humberto Carrillo");
   Vpresent->Visualiza_texto(20,230,"         Luis Nava");
   Vpresent->Visualiza_texto(20,240,"         Antonio Carrillo");
   Vpresent->Define_tipo_letra(2,0,-2,0,0,0);
   Vpresent->Define_color_texto(Rojo1);
   sprintf(xcad,"Fecha de compilaci¢n: %s Hora: %s",__DATE__,__TIME__);
   Vpresent->Visualiza_texto(20,280,xcad);
   sprintf(xcad,"(@) Todos los derechos reservados, registro en t rmite %s ",__DATE__);
   Vpresent->Visualiza_texto(20,290,xcad);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   while(Programa_activo && Tecla != ENTER && Tecla != ESC) Administrador();
   delete Vpresent;


*/


#endif
