// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  2.5-B



#include "v_iconos.hpp"
#include "teclado.hpp"
#include "cad_car.hpp"
#include "r_var.hpp"
#include "get.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;


extern "C" {
  #include <stdlib.h>
}


#ifdef _IDIOMA_ESPANOL_
   #define TXT1     "CALCULADORA"
#else
   #define TXT1     "CALCULATOR"
#endif



void Calculadora(void)
{
   char xcad[100], cad[100];
   unsigned int x1, y1, x2, y2;
   Ventana_iconos *Vt = new Ventana_iconos;
   Get *G1 = new Get;

   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Cian9,Gris9,Blanco);
   Vt->Ventana_centrada(TXT1,190,80,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);

   xcad[0] = 0;
   s_r_t(xcad,50,' ');
   G1->Parametros(xcad,x1+10,y1+40,20,"TK");
   G1->Activa_get(1);
   G1->Dibuja();

   while(1) {
      NCO->Administrador();
      // Revisa el estado del icono
      if(Vt->Itera() || !Programa_activo || Tecla == ESC) break;
      // Edicion
      G1->Edita(Caracter,Tecla);
      G1->Presionado(BR);
      if(Tecla == ENTER) {
         G1->Cadena_editada(cad);
         // Evaluador de expresiones
         Evalua_expresion(cad,xcad);
         s_r_t(xcad,50,' ');
         G1->Cambia_cadena(xcad);
         G1->Dibuja();
      }
   }
   delete G1;
   delete Vt;
}




