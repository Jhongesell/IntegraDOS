// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


// Rutinas pertenecientes a la clase Pantalla_grafica

#include "pantgraf.hpp"
#include "r_var.hpp"
#include "r_esp.hpp"
#include "colores.hpp"
#include "def_col.hpp"
#include "prtgraph.h"

void  InitPBGI(void);


extern "C" {
   #include <stdio.h>
   #include <graphics.h>
   #include <stdlib.h>
   #include <dir.h>
}


extern int MODO_GRAFICO;
extern int MODO_GRAFICO_ACTIVO;
extern int X_MAX;
extern int Y_MAX;
extern int MAX_COLORES;


#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "Error en el modo grafico"
#else
   #define TXT1 "Error in graphics mode"
#endif




// Rutina que retorna el modo grafico seleccionado (no puede pertenecer a una clase)
int huge Selecc_modo_graf(void)
{
   return MODO_GRAFICO; // 1024x768x256  4
}                       // 800x600x256   3
                        // 640x480x256   2


// Destructor de la pantalla grafica
void Pantalla_Grafica::Destruye(void)
{
   closegraph();
   // Indica que el modo grafico esta desactivado
   MODO_GRAFICO_ACTIVO = 0;
/////////////////
   BGI_closegraph();
   PRT_End();
/////////////////
   // Restaura la anterior ventana de texto  antes de inicializar el modo grafico
   if(Grabar) {
      // Selecciona el modo de texto
      textmode(i_s.currmode);
      // Restablece la pantalla de texto que existia anteriormente
      lee_pantalla_texto(1,1,i_s.screenwidth,i_s.screenheight,ARCH_TEMP);
      // Restablece el color de texto y fondo
      textattr(i_s.attribute);
      // Se posiciona en la siguiente linea
      gotoxy(i_s.curx,i_s.cury - 1);
   }
   delete []ARCH_TEMP;
}


// Constructor de la pantalla grafica
void Pantalla_Grafica::Inicializa(const char *path,const int graba, const int svga, const int ct, const int cf)
{
   // Graba la ventana de texto antes de inicializar el modo grafico
   Grabar = graba;
   if(Grabar) {
      // Asignaci¢n del nombre del archivo que contendra la imagen en modo texto
      ARCH_TEMP = new char[100];
      Nombre_archivo_temporal(ARCH_TEMP);
      gettextinfo(&i_s);
      graba_pantalla_texto(1,1,i_s.screenwidth,i_s.screenheight,ARCH_TEMP);
   }
////////////////////
   InitPBGI();
////////////////////
   // Inicializa las variables del modo grafico
   Manejador_Grafico = DETECT;
   MODO_GRAFICO = svga;
   // Installa el driver no estandar para manejo grafico
   if(svga) installuserdriver("SVGA256",Selecc_modo_graf);
   // Inicializa el modo grafico
////////////////////////////
   BGI_initgraph( &Manejador_Grafico,&Modo_Grafico,path, Scrn_BGIgroup );
////////////////////////////
//   initgraph(&Manejador_Grafico,&Modo_Grafico,path);
   //  Revisa la inicializaci¢n del modo grafico
   if(graphresult()) {
      printf("\n\n\n %s: %s\n",TXT1,grapherrormsg(graphresult()));
      exit(1);
   }
   // Indica que el modo grafico esta actovo
   MODO_GRAFICO_ACTIVO = 1;
   // Obtiene los valores maximos y minimos del sistema grafico
   X_MAX = getmaxx();
   Y_MAX = getmaxy();
   MAX_COLORES = getmaxcolor();
   // Definicion de los distintos colores usados en el sistema
   for(int i = 0; i < 16; i++) setrgbpalette(color[i],cn1[i],cn2[i],cn3[i]);
   //  Selecciona color de fondo y texto
   setbkcolor(cf);
   setcolor(ct);
   cleardevice();
}

