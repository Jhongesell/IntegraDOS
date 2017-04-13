// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#include "v_iconos.hpp"
#include "teclado.hpp"
#include "ctrl_tec.hpp"
#include "icono.hpp"
#include "get.hpp"
#include "cad_car.hpp"
#include "r_var.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;



// Captura una cadena
int Captura_cadena(const char *titulo, const char *exp, double &num, const int tp)
{
   unsigned int x1,y1,x2,y2,ct = 0;

   Ventana_iconos *Vt  = new Ventana_iconos;
   Icono          *Ic1 = new Icono;
   Icono          *Ic2 = new Icono;
   Get            *G1  = new Get;

   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(titulo,260,200,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Limpia_recuadro(20,60,240,130,Blanco,Negro,Negro);
   Vt->Define_color(Blanco,Negro);
   Vt->Visualiza_texto(30,75,exp);
   G1->Activa_get(1);
   G1->Parametros(num,x1+30,y1+85,20,tp);
   G1->Dibuja();


   Ic1->Define_icono("OK.ICO",x1+20,y2-50);
   Ic1->Dibuja();
   Ic2->Define_icono("CANCEL.ICO",x2-56,y2-50);
   Ic2->Dibuja();


   while(!ct) {
      NCO->Administrador();
      if(!Programa_activo) break;
      // Edicion
      G1->Edita(Caracter,Tecla);
      // Revisa el estado de los iconos
      if(Vt->Itera()) ct = 3;
      if(Ic1->Oprimido() || Tecla == ENTER) ct = 1;
      if(Ic2->Oprimido() || Tecla == ESC) ct = 2;
      G1->Presionado(BR);
      if(ct == 1) {
         G1->Cadena_editada(num);
      }
   }
   delete G1;
   delete Ic1;
   delete Ic2;
   delete Vt;
   return ct;
}
