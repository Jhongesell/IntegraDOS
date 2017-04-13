// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

/*

////////////////////////////////////////////////////////////////////////
// Usar la ventana de visualizacion para visualizar el estado del 
// sistema, revisar que tipo de valores hay que agregar y cuales
// quitar ....
////////////////////////////////////////////////////////////////////////

#include "v_iconos.hpp"
#include "nucleo.hpp"
#include "teclado.hpp"
#include "cad_car.hpp"
#include "r_var.hpp"

extern "C" {
  #include <stdlib.h>
}

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;


extern const char *N_AUTOR;     // Nombre del autor
extern const char *D_AUTOR;     // Direccion del autor
extern const char *T_AUTOR;     // Telefono del autor


// Visualiza el estatus del sistema
void Estado_sistema(void)
{
   char cad[80], xcad[80];
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Cian9,Gris9,Blanco);
   Vt->Ventana_centrada("Estado del sistema",450,180,0);
   Vt->Dibuja();
   Vt->Define_color_texto(Negro);
   Vt->Visualiza_texto(30,60,"Autor:");
   Vt->Visualiza_texto(30,70,"Direcci¢n:");
   Vt->Visualiza_texto(30,80,"Tel‚fono:");
   Vt->Visualiza_texto(30,120 ,"Espacio total del disco:");
   Vt->Visualiza_texto(30,130,"Espacio usado del disco:");
   Vt->Visualiza_texto(30,140,"Espacio libre del disco:");
   Vt->Visualiza_texto(30,150,"Total de memoria disponible:");
   for(int i = 0; i < 4; i++) Vt->Visualiza_texto(388,120+(i*10),"Bytes");
   Vt->Define_color_texto(Cian9);
   Vt->Visualiza_texto(130,60,N_AUTOR);
   Vt->Visualiza_texto(130,70,D_AUTOR);
   Vt->Visualiza_texto(130,80,T_AUTOR);
   Vt->Define_color_texto(Rojo1);
   ltoa(esp_disco(0,2),cad,10);
   s_formato(cad,8,0,1,xcad);
   Vt->Visualiza_texto(280,120,xcad);
   ltoa(esp_disco(0,3),cad,10);
   s_formato(cad,8,0,1,xcad);
   Vt->Visualiza_texto(280,130,xcad);
   ltoa(esp_disco(0,1),cad,10);
   s_formato(cad,9,0,1,xcad);
   Vt->Visualiza_texto(280,140,xcad);
   ultoa(esp_memoria(1),cad,10);
   s_formato(cad,9,0,1,xcad);
   Vt->Visualiza_texto(280,150,xcad);
   while(1) {
      NCO->Administrador();
      if(Tecla == ESC || !Programa_activo || Vt->Itera()) break;
   }
   delete Vt;
}

*/
