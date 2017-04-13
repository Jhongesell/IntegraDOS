// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void Mensaje(const char *txt1, const char *txt2, const char *txt3, const char *txt4, const char *txt5);
// Presenta una ventana hasta con 5 lineas de texto




#include "v_iconos.hpp"
#include "teclado.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;


#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "INFORMACION"
#else
   #define TXT1      "INFORMATION"
#endif


void Mensaje(const char *txt1, const char *txt2, const char *txt3, const char *txt4, const char *txt5)
{
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Rojo1,Gris9,Blanco);
   Vt->Ventana_centrada(TXT1,400,190,0);
   Vt->Dibuja();
   Vt->Limpia_recuadro(20,50,380,170,Blanco,Negro,Negro);
   Vt->Define_color(Blanco,Negro);
   Vt->Centra_texto(70,txt1);
   Vt->Centra_texto(90,txt2);
   Vt->Centra_texto(110,txt3);
   Vt->Centra_texto(130,txt4);
   Vt->Centra_texto(150,txt5);
   while(Programa_activo) {
      NCO->Administrador();
      if(Tecla == ESC || Vt->Itera()) break;
   }
   delete Vt;
}


