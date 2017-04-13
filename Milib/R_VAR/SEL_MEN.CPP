// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "menu.hpp"
#include "nucleo.hpp"
#include "teclado.hpp"
#include "definic.hpp"
extern "C" {
   #include <graphics.h>
}

// Definicion del objeto Nucleo
extern Nucleo *NCO;
extern int     Tecla;
extern char    Caracter;
extern int     Programa_activo;
extern int     X_MAX;
extern int     Y_MAX;


// Retorna la opcion SELEC del menu pasado en ARR_MENU con numero de elementos NUM en la posicion VENT
int Opcion_menu(const char *Arr_menu[], const char *opc, unsigned int &selec,  const C_2I vent, const char *opc_des)
{
   unsigned int i = 0, x;
   C_2I ini, lg;
   lg.x = lg.y = 0;
   while(Arr_menu[i][0] != '0') {
      x = textwidth(Arr_menu[i]);
      lg.x = lg.x > x ? lg.x : x;
      i++;
   }
   lg.x += 12;
   lg.y = 13 * i + 20;
   ini.x = vent.x + lg.x >= X_MAX ? X_MAX - (lg.x + 10) : vent.x;
   ini.y = vent.y;
   lg.y = vent.y + lg.y >= Y_MAX ? lg.y - ((vent.y + lg.y + 10) - Y_MAX) : lg.y;
   selec = 0;
   // Parametros del menu
   Menu *M1 = new Menu;
   M1->Activa_graba_ventana(1);
   M1->Define_ventana_menu(ini.x,ini.y,ini.x + lg.x,ini.y + lg.y,Gris1);
   // Asigna los parametros al menu
   M1->Parametros_menu(Arr_menu,opc,opc_des,i);
   // Dibuja el manu
   M1->Dibuja();
   while(!selec) {
      NCO->Administrador();
      if(!Programa_activo) break;
      if(Tecla == ESC) break;
      // Controla el movimiento del menu
      if(M1->Controlador_menu(Caracter,Tecla,selec)) break;
   }
   delete M1;
   return selec;
}

