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




// Captura tres numeros
int Captura_cadenas(const char *titulo, const char *exp1, double &num1, const char *exp2, double &num2, const char *exp3, double &num3,const char *exp4, double &num4, const int tp)
{
   unsigned int x1,y1,x2,y2,ct = 0;
   int get_act = 0, get_sw = 0, i = 0;

   Ventana_iconos *Vt  = new Ventana_iconos;
   Icono          *Ic1 = new Icono;
   Icono          *Ic2 = new Icono;
   Get            *Gts = new Get[4];

   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(titulo,260,350,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Limpia_recuadro(20,60,240,270,Blanco,Negro,Negro);
   Vt->Define_color(Blanco,Negro);
   Vt->Visualiza_texto(30,75 ,exp1);
   Vt->Visualiza_texto(30,125,exp2);
   Vt->Visualiza_texto(30,175,exp3);
   Vt->Visualiza_texto(30,225,exp4);
   Gts[0].Activa_get(1);
   Gts[0].Parametros(num1,x1+30,y1+85,20, tp);
   Gts[0].Dibuja();
   Gts[1].Parametros(num2,x1+30,y1+135,20, tp);
   Gts[1].Dibuja();
   Gts[2].Parametros(num3,x1+30,y1+185,20, tp);
   Gts[2].Dibuja();
   Gts[3].Parametros(num4,x1+30,y1+235,20, tp);
   Gts[3].Dibuja();

   Ic1->Define_icono("OK.ICO",x1+20,y2-50);
   Ic1->Dibuja();
   Ic2->Define_icono("CANCEL.ICO",x2-56,y2-50);
   Ic2->Dibuja();


   while(!ct) {
      NCO->Administrador();
      if(!Programa_activo) break;
      // Revisa el estado de los iconos
      if(Vt->Itera()) ct = 3;
      if(Ic1->Oprimido()) ct = 1;
      if(Ic2->Oprimido() || Tecla == ESC) ct = 2;

      for(i = 0; i < 4; i++) {
         if(Gts[i].Presionado(BR)) get_act = i, get_sw = 1;
         Gts[i].Edita(Caracter,Tecla);
      }
      if(Tecla == FLECHA_ARRIBA && get_act) get_act--, get_sw = 1;
      if((Tecla == FLECHA_ABAJO || Tecla == ENTER) && get_act < 3) get_act++, get_sw = 1;
      if(get_sw) {
         for(i = 0; i < 4; i++) Gts[i].Activa_get(i == get_act);
         get_sw = 0;
      }
      if(ct == 1) {
         Gts[0].Cadena_editada(num1);
         Gts[1].Cadena_editada(num2);
         Gts[2].Cadena_editada(num3);
         Gts[3].Cadena_editada(num4);
      }
   }
   delete []Gts;
   delete Ic1;
   delete Ic2;
   delete Vt;
   return ct;
}
