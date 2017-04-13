// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


// Clase que visualiza una ventana y el icono de flecla abajo
// en la ventana se muestra la opcion por omicion del arreglo pasado
// al presionar el icono se muestra el menu del cual al seleccionar
// un elemento de este, se copia en la ventana.

#ifndef __COMBO_B__
#define __COMBO_B__

#include "icono.hpp"
#include "definic.hpp"

class Combo_box {
   private:

       C_2I            ini;
       C_2I            lg;
       unsigned int    Actual;
       unsigned int    Selec;
       char           *cadena;
       const char     *Opc;
       const char     *Opc_des; 
       Icono          *Ic;
       Ventana_marco  *Vt;

   public:
                    // Destructor de la clase
                   ~Combo_box();
                    // Parametros de la clase
       void         Parametros(const char *Arr_menu[], const C_2I vent, const unsigned int act, const char *opc = "", const char *opc_des = "");
                    // Dibuja la clase
       void         Dibuja(const char *Arr_menu[]);
                    // Controla la seleccion dentro de la clase
       void         Controla(const int tecla, const char *Arr_menu[]);
                    // Retorna la opcion selecionada
       unsigned int Retorna_opcion(void) const
                    {return (Actual+1);}
       void         Fija_actual_opcion(unsigned int act)
                    {if(act) Actual = act-1;
                      else Actual = 0;}
};


/* 

Ejemplo de uso

static const char *M_archivos[] = {
   "Descripci¢n Proyecto",
   "Graba Pantalla",
   "Lee Pantalla",
   "Edita Archivo",
   "Visualiza Archivo",
   "Termina Programa",
   "0"
};


void mi_funcion(void)
{
   Combo_box ab;
   C_pt vent;
   int unsigned selec;
   vent.x = 100, vent.y = 100;
   ab.Parametros(M_archivos, vent,2);
   ab.Dibuja();
   while(Caracter != 'x') {
      NCO->Administrador();
      ab.Controla(Tecla, M_archivos);
   }
   select = ab.Retorna_opcion()
}


*/

#endif

