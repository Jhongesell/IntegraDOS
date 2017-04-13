// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  2.2-A


//////////////////////////////////////////////////////////////////
// Hacer menu horizontal
// Simplificar la visualizacion dentro del menu si no hay paneo
//////////////////////////////////////////////////////////////////


#include "menu.hpp"
#include "cad_car.hpp"
#include "r_esp.hpp"
#include "r_var.hpp"
#include "teclado.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;

extern "C" {
   #include <string.h>
   #include <conio.h>
   #include <graphics.h>
   #include <stdlib.h>
}


Menu::Menu(void)
{
   Posx = Posy = 10;
   Lgop = L_CAR + 5;
   Cfda = Gris9;
   Ctxn = Negro;
   Ctxa = Blanco;
   Ctxd = Gris9;
   Tipo_menu = 1;
   xNea = 0;
   Texto_menu = NULL;
   Ctrl_mov = NULL;

}


// Destructor de la clase Menu
Menu::~Menu()
{
   // Libera la memoria solicitada para contener los parametros del menu
   delete []Texto_menu;
   delete  Ctrl_mov;
}


// Define la ventana del menu
void Menu::Define_ventana_menu(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const char c_f) const
{
   // Define ventana del menu
   Define_ventana(x1,y1,x2,y2,c_f);
   ((Menu*)this)->Cfdn = c_f, ((Menu*)this)->Cfdd = c_f;
}


// Carga los parametros del menu y visualiza este
void Menu::Parametros_menu(const char **text, const char *opc, const char *opcd, const unsigned int t_e, const unsigned int t_m) const
{
   unsigned int i = 0, e_p, xt_e = t_e;
   // Solicita la memoria necesaria para almacenar los elementos del menu
   if(xt_e >= 1) {
      ((Menu*)this)->Texto_menu = new const char *[xt_e];
      // Carga los elementos a visualizar del menu
      for(i = 0; i < xt_e; i++) Texto_menu[i] = text[i];
    } else {
      xt_e = 1;
      ((Menu*)this)->Texto_menu = new const char *[2];
      Texto_menu[0] = " ";
   }
   ((Menu*)this)->Tipo_menu = t_m;
   e_p = ((Largo_Y()-(Posy+5)) / Lgop);
   if(e_p > xt_e)  e_p = xt_e;
   ((Menu*)this)->Ctrl_mov = new Controlador_movimiento;
   Ctrl_mov->Parametros(xt_e,e_p,opc,opcd);
   // fija el paneo para ser usado en el menu
   Ctrl_mov->Fija_tipo_paneo(0);
   Ctrl_mov->Fija_elemento_actual(1);
}


// Visualiza al menu dentro de la ventana
void Menu::Visualiza_menu(void) const
{
   char ctx;                                     //  Color de texto
   unsigned int i;                               //  Indice
   unsigned int y;                               //  Indica la posicion sobre la cual se visualizara el texto
   unsigned int pos, tmp;                        //  Variables temporales
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   // Inicia el ciclo para visualizar los elementos del actual paneo
   for(i = 0; i < Ctrl_mov->Numero_elementos_paneo(); i++) {
       y = Posy + (i * Lgop);
       tmp = 0;
       while(Texto_menu[(i+Ctrl_mov->Elemento_inicial_paneo()) -1][tmp]) {
          if(Texto_menu[(i+Ctrl_mov->Elemento_inicial_paneo()) -1][tmp] == 127) {
             if(!Ctrl_mov->Busca_caracter(Texto_menu[(i+Ctrl_mov->Elemento_inicial_paneo()) -1][tmp+1],pos)) {
                tmp = 0;
                break;
             }
          }
          tmp ++;
       }
       if(i+Ctrl_mov->Elemento_inicial_paneo() == Ctrl_mov->Elemento_actual()) {
          ctx = tmp ? Ctxa : Gris1;
          setfillstyle(1,Cfda);
          bar(X1+2,y+Y1,X1+Largo_X()-3,Y1+Posy+((i+1)*Lgop));
          // Sombra para el elemento activo
          Define_color_texto(Negro);
          Visualiza_texto(Posx,y + 3 +((Lgop - L_CAR) /2) ,Texto_menu[(i+Ctrl_mov->Elemento_inicial_paneo()) -1]);
       } else {
          setfillstyle(1,Actual_color_fondo());
          bar(X1+2,y+Y1,X1+Largo_X()-3,Y1+Posy+((i+1)*Lgop));
          ctx = tmp ? Ctxn : Ctxd;
          if(!tmp) {
             // Sombra para el elemento desactivado
             Define_color_texto(Blanco);
             Visualiza_texto(Posx+1,y +2+ ((Lgop - L_CAR) /2) ,Texto_menu[(i+Ctrl_mov->Elemento_inicial_paneo()) -1]);
          }
       }
       Define_color_texto(ctx);
       Visualiza_texto(Posx,y + 2 +((Lgop - L_CAR) /2) ,Texto_menu[(i+Ctrl_mov->Elemento_inicial_paneo()) -1]);
   }
}


// Controla la selecion dentro del menu
// En la variable POS
// (0)           Sin seleccion
// ( 1,2,3,... ) Opcion seleccionada
// Retorna
// (0)          Opcion seleccionada
// (1)          Se presiono el mouse fuera de la ventana
unsigned int Menu::Controlador_menu(const char car, const int t_c, unsigned int &opc) const
{
   int xt_c = t_c;
   unsigned int st = 0, xopc = 0, N_ea = Ctrl_mov->Elemento_actual();
   opc = 0;
   // Revisa si se selecciono por medio del mouse
   if(xt_c == -1) {
      // Revisa la seleci¢n del mouse
      st = Seleccion_mouse(xopc);
      // Revisa si fue seleccionado algun elemento del menu
      if(xopc) xt_c = ENTER, opc = xopc;
   }
   // Revisa si se presiono el mouse dentro de la ventana
   if(!st) {
      // Controla el moviemiento dentrol del menu por teclas o flechas
      opc = Tipo_menu ? Ctrl_mov->Vertical(xt_c,car) : Ctrl_mov->Horizontal(xt_c,car);
      // Revisa la visualizaci¢n del menu
      if(N_ea != Ctrl_mov->Elemento_actual()) Visualiza_menu();
   }
   return st;
}



// Indica si fue seleccionada alguna opcion del menu con el mouse
// La opcion seleccionada se indica en la variable POS
// Retorna   (0) si el mouse fue presionado dentro
//           (1) si el mouse se presiono fuera de la ventana
unsigned int Menu::Seleccion_mouse(unsigned int &pos) const
{
   unsigned int md = Ventana::Mouse_dentro();
   unsigned int st = 0;
   pos = 0;
   // Se presiona el boton del mouse
   if(NCO->Retorna_estado_mouse().button == BR) {
      // Revisa si fue presionado dentro de la ventana o fuera de esta
      if(md) {
         // Si el mouse permanece presionado dentro de la ventana
         while(NCO->Retorna_estado_mouse().button == BR && md) {
            // Optiene la posicion del mouse dentro del menu
            Posicion_mouse(pos);
            if(pos) {
               if(Ctrl_mov->Elemento_actual() != pos) {
                  // Actualiza la posicion dentro de la ventana
                  Ctrl_mov->Fija_elemento_actual(pos);
                  // Visualiza el menu
                  Visualiza_menu();
                  delay(300);
               }
            }
            // Obtiene la posicion del mouse dentro de la ventana
            NCO->Administrador();
         }
         // Indica el elemento seleccionado en el menu
      } else st = 1;  // El mouse se presiono fuera de la ventana
   }
   return st;
}



// Indica en que opci¢n del menu se encuentra el mouse
void Menu::Posicion_mouse(unsigned int &pos) const
{
   pos = 0;
   if(Ventana::Mouse_dentro()) {
      unsigned int i, e_p;
      int x, y;
      e_p = Ctrl_mov->Numero_elementos_paneo() > Ctrl_mov->Numero_total_elementos() ? Ctrl_mov->Numero_total_elementos() : Ctrl_mov->Numero_elementos_paneo();
      Ventana::Posicion_mouse(x,y);
      for(i = 0; i < e_p; i++) {
         if(x >= 2 && y >= (Posy+(i*Lgop)) && x <= (Largo_X()-2) && y <= (Posy+((i+1)*Lgop))) {            
            pos = Ctrl_mov->Elemento_inicial_paneo() + i;
            break;
         }
      }
   }
}

