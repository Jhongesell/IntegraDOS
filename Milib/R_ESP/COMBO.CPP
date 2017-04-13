// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#include "combo_b.hpp"
#include "nucleo.hpp"
#include "teclado.hpp"
#include "cad_car.hpp"
#include "r_esp.hpp"

extern "C" {
  #include <graphics.h>
  #include <dos.h>
}


extern int X_MAX;
extern int Y_MAX;
extern Nucleo *NCO;




// Destructor de la clase
Combo_box::~Combo_box()
{
   delete Ic;
   delete Vt;
   delete []cadena;
}


// Retorna la opcion SELEC del menu pasado en ARR_MENU con numero de elementos NUM en la posicion VENT
void Combo_box::Parametros(const char *Arr_menu[], const C_2I vent, const unsigned int act, const char *opc, const char *opc_des)
{
   unsigned int i = 0, x;
   lg.x = lg.y = 0;
   while(Arr_menu[i][0] != '0') {     
      x = textwidth(Arr_menu[i]);
      lg.x = lg.x > x ? lg.x : x;
      i++;
   }
   lg.x += 12, ini.x = vent.x, ini.y = vent.y;
   Actual = 0;
   if(act > 0 && act <= i) Actual = act -1;
   cadena = new char [200];
   Vt = new Ventana_marco;
   Vt->Define_ventana(ini.x,ini.y,ini.x + lg.x,ini.y +19,Blanco);
   Ic = new Icono;
   Ic->Define_icono("FABA.ICO",ini.x + lg.x+3,ini.y,20,19);
   Opc = opc, Opc_des = opc_des;
}

// dibuja la ventana de la clase
void Combo_box::Dibuja(const char *Arr_menu[])
{
   // Dibuja el icono
   Ic->Dibuja();
   // Dibuja la ventana
   Vt->Dibuja();
   // Copia el elemento seleccionado
   s_nocar(Arr_menu[Actual],127,cadena);
   // Visualiza la opcion por omoci¢n
   Vt->Visualiza_texto(10,7,cadena);
}

// Controla la selecion 
void Combo_box::Controla(const int tecla, const char *Arr_menu[])
{
   Selec = 0;
   // Si se presiono ESC retorna
   if(tecla == ESC) return;
   // Si se presiono el icono pasa a muestrar el menu
   if(Ic->Oprimido()) {
      delay(300);
      NCO->Administrador(); 
      lg = ini, lg.y += 20;
      // Controla la muestra del menu
      if(Opcion_menu(Arr_menu,Opc,Selec,lg,Opc_des)) {
         // Se selecciono algun elemento del menu
         Actual = Selec -1;
         // Visualiza la ventana con el nuevo elemento seleccionado
         Dibuja(Arr_menu);
      }
   }
}


