// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#ifndef __V_ICONOS_HPP__
#define __V_ICONOS_HPP__

#include "v_titulo.hpp"
#include "icono.hpp"



// Clase Ventana con iconos para su control
class Ventana_iconos: public Ventana_titulo {
   private:
      Icono            *Ico1;                  // Puntero al primer icono
      Icono            *Ico2;                  // Puntero al segundo icono
      Icono            *Ico3;                  // Puntero al tercer icono
      char             *Nico1;                 // Contiene el nombre del primer icono
      char             *Nico2;                 // Contiene el nombre del segundo icono
      char             *Nico3;                 // Contiene el nombre del tercer icono
      unsigned int      IconoActual:      2;   // Indica el icono sobre el que se encuentra el mouse
      unsigned int      IconoPresionado:  2;   // Indica el icono presionado
      char              Tipo:             2;   // Indica la cantidad de iconos en la ventana

                        // Dibuja los iconos de la ventana
                  void  Dibuja_iconos(void) const;
                        // Carga los iconos
                  void  Carga_iconos(void) const;


   public:
                        // Constructor de ventana con iconos
                        Ventana_iconos(void);
                        // Destructor de ventana con iconos
                       ~Ventana_iconos();
                        // Define la ventana
      virtual     void  Define_ventana(const char *tit, const int x1, const int y1, const int x2, const int y2, const unsigned int tp) const;
                        // Define la ventana
      virtual     void  Ventana_centrada(const char *tit, const int lgx, const int lgy, const unsigned int tp, const int inc_x=0, const int inc_y=0) const;
                        // Dibuja la ventana
      virtual     void  Dibuja(void) const
                        {Ventana_titulo::Dibuja();Dibuja_iconos();}
                        // Revisa el estado de los iconos
      unsigned    int   Itera(void) const;
                        // Indica el icono actual
      unsigned    int   Icono_actual(void) const
                        {return IconoActual;}
                        // Inidca el icono presionado
      unsigned    int   Icono_presionado(void) const
                        {return IconoPresionado;}
                        // Define el nombre de los iconos
                  void  Nombre_iconos(const char *n1, const char *n2 = " ", const char *n3 = " ") const;
                        // Activa o desactiva la ventana (solo titulo)
      virtual     void  Ventana_activa(const int tp) const
                        {Ventana_titulo::Ventana_activa(tp); Dibuja_iconos();}
                        // Cambia el tamano de la ventana
      virtual     void  Cambiar_tamano(const int x1, const int y1, const int x2, const int y2) const;
};

/*

Clase Ventana_iconos:

   La clase VENTANA_ICONOS permite definir una ventana de trabajo
   con titulos e iconos teniendo todos los comportamientos de la
   ventan con titulos ademas de los comportamientos necesarios para
   manipular la ventana con iconos


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

      Para controlar la ventana use:
         Itera
            Indica si se preciono algun icono dentro de la ventana
         Icono_actual
            Indica el icono sobre el que el mouse esta actualmente
         Icono_presionado
            Indica el numero de icono presionado dentro de la ventana

      Despues puede utilizar todos los demas comportamientos segun sean
      las necesidades del programa. al destruirse el objeto restaura la
      ventana original (si se grabo esta) o redibuja con el color de
      refresco (si este fue activado)

      Nota: la ventana puede visualizar uno o dos iconos, por omicion
      estan puetos los iconos de cerrar y ayuda.
      Usa agregacion para los iconos.

Ejemplo:

   Ventana_iconos *Vpresent = new Ventana_iconos;
   Vpresent->Activa_graba_ventana(1);
   Vpresent->Ventana_centrada("ABOUT INTEGRA",480,300,0);
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
   while(!Vpresent->Itera() && Programa_activo && Tecla != ENTER && Tecla != ESC) Administrador();
   delete Vpresent;


*/


#endif



