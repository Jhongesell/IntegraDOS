// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __V_MARCO_HPP__
#define __V_MARCO_HPP__

#include "v_basica.hpp"


// Clase Ventana con marco
class Ventana_marco: public Ventana {

   private:

      unsigned int   Mouse_dentro_ventana:    1;      // Indica si el mouse esta adentro de la ventana
      char           C_rec_inf;                       // Color del recuadro superior
      char           C_rec_sup;                       // Color del recuadro inferior
      char           C_base_a;                        // Color de la base de la ventana activada
      char           C_base_n;                        // Color de la base de la ventana no activada

   public:
                     // Constructor de ventana con marco
                     Ventana_marco(void);
                     // Dibuja el marco de la ventana segun el TB
               void  Marco_ventana(const int tb) const;
                     // Define los colores de la ventana con marco
      virtual  void  Define_colores(const int c_v, const int c_r_i, const int c_r_s, const int c_b_a = Verde1, const int c_b_n = Negro) const;
                     // Revisa si el mouse esta dentro de la ventana con marco
               int   Mouse_dentro(void) const;
                     // Dibuja la ventana con marco
      virtual  void  Dibuja(void) const;
};

/*
Clase Ventana_marco:

   La clase VENTANA_MARCO permite definir una ventana de trabajo con
   marco el cual se activa si el mouse esta sobre la ventana, permite
   hacer todas las actividades de la clase VENTANA y cambial el color
   y forma del marco.

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

Ejemplo

   Ventana_marco *Vpresent = new Ventana_marco;
   Vpresent->Activa_graba_ventana(1);
   Vpresent->Ventana_centrada(480,300);
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
   while(Programa_activo && Tecla != ENTER && Tecla != ESC) {
      Administrador();
      Mouse_dentro();
   }
   delete Vpresent;


*/


#endif
