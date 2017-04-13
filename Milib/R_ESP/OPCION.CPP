// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int Opcion(const char *txt1, const char *txt2, const char *txt3, const char *txt4, const char *txt5);
  // Presenta una ventana hasta con 5 lineas de texto aceptando la opcion aceptar y Cancelar, retornando
  // (1)   Aceptar
  // (2)   Cancelar
  // (3)   Cerrado de la ventana



#include "v_iconos.hpp"
#include "nucleo.hpp"
#include "teclado.hpp"


#ifdef _IDIOMA_ESPANOL_
   #define  TXT1 "INFORMACION"
#else
   #define  TXT1 "INFORMATION"
#endif

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;



int Opcion(const char *txt1, const char *txt2, const char *txt3, const char *txt4, const char *txt5)
{
   unsigned int x1,y1,x2,y2,ct = 0;
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Rojo1,Gris9,Blanco);
   Vt->Cambia_tipo_letra_titulo(8,0,-1,0,0,0);
   Vt->Ventana_centrada(TXT1,400,215,0);
   Vt->Dibuja();
   Vt->Limpia_recuadro(10,45,390,150,Blanco,Negro,Negro);
   Vt->Define_color(Blanco,Negro);
   Vt->Centra_texto(60,txt1);
   Vt->Centra_texto(80,txt2);
   Vt->Centra_texto(100,txt3);
   Vt->Centra_texto(120,txt4);
   Vt->Centra_texto(140,txt5);
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Icono *Ic1 = new Icono;
   Ic1->Define_icono("OK.ICO",x1 + 50,y2 - 50);
   Ic1->Dibuja();
   Icono *Ic2 = new Icono;
   Ic2->Define_icono("CANCEL.ICO",x2 -86,y2 - 50);
   Ic2->Dibuja();
   while(!ct) {
      NCO->Administrador();
      if(Tecla == ESC || !Programa_activo) ct = 3;
      // Revisa el estado de los iconos
      if(Vt->Itera()) ct = 3;
      if(Ic1->Oprimido()) ct = 1;
      if(Ic2->Oprimido()) ct = 2;
   }
   delete Ic1;
   delete Ic2;
   delete Vt;
   // Retorna el estado de terminacion
   return ct;
}

