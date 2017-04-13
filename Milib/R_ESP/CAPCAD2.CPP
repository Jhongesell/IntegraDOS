// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#include "v_iconos.hpp"
#include "teclado.hpp"
#include "icono.hpp"
#include "get.hpp"
#include "cad_car.hpp"
#include "r_var.hpp"
#include "r_esp.hpp" 
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;



// Captura dos cadenas
int Captura_cadenas(const char *titulo, const char *exp1, char *cad1, const char *exp2, char * cad2, const int lg, const int tp)
{
   unsigned int x1,y1,x2,y2,ct = 0;

   Ventana_iconos *Vt  = new Ventana_iconos;
   Icono          *Ic1 = new Icono;
   Icono          *Ic2 = new Icono;
   Get            *G1  = new Get;
   Get            *G2  = new Get;

   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(titulo,300,250,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Limpia_recuadro(20,60,280,180,Blanco,Negro,Negro);
   Vt->Define_color(Blanco,Negro);
   Vt->Visualiza_texto(30,75,exp1);
   Vt->Visualiza_texto(30,125,exp2);
   s_r_t(cad1,lg,32);
   G1->Activa_get(1);
   G1->Parametros(cad1,x1+30,y1+85,20);
   G1->Dibuja();
   s_r_t(cad2,lg,32);
   G2->Parametros(cad2,x1+30,y1+135,20);
   G2->Dibuja();

   Ic1->Define_icono("OK.ICO",x1+20,y2-50);
   Ic1->Dibuja();
   Ic2->Define_icono("CANCEL.ICO",x2-56,y2-50);
   Ic2->Dibuja();


   while(!ct) {
      NCO->Administrador();
      if(!Programa_activo) break;
      // Edicion
      G1->Edita(Caracter,Tecla);
      G2->Edita(Caracter,Tecla);

      // Revisa el estado de los iconos
      if(Vt->Itera()) ct = 3;
      if(Ic1->Oprimido()) ct = 1;
      if(Ic2->Oprimido() || Tecla == ESC) ct = 2;
      if(G1->Presionado(BR) || Tecla == FLECHA_ARRIBA) {
         G1->Activa_get(1);
         G2->Activa_get(0);
      }
      if(G2->Presionado(BR) || Tecla == FLECHA_ABAJO || Tecla == ENTER) {
         G2->Activa_get(1);
         G1->Activa_get(0);
      }
      if(ct == 1) {
         G1->Cadena_editada(cad1);
         G2->Cadena_editada(cad2);
         s_trim(cad1);
         s_trim(cad2);
         if(tp) {
            if(Valida_nombre_archivo(cad1)) {
               NCO->Ctrl_error(14,cad1);
               ct = 0;
            }
            if(Valida_nombre_archivo(cad2)) {
               NCO->Ctrl_error(14,cad2);
               ct = 0;
            }
         }
      }
   }
   delete G1;
   delete G2;
   delete Ic1;
   delete Ic2;
   delete Vt;
   return ct;
}

