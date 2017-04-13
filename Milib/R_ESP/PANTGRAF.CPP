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


extern "C" {
   #include <stdio.h>
   #include <graphics.h>
   #include <stdlib.h>
   #include <dir.h>
}


#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "\n\n\nError en el modo gr fico: %s\n"
#else
   #define TXT1      "\n\n\nGraphyc mode error: %s\n"
#endif


extern int MODO_GRAFICO;
extern int MODO_GRAFICO_ACTIVO;
extern int X_MAX;
extern int Y_MAX;
extern int MAX_COLORES;


// Rutina que retorna el modo grafico seleccionado (no puede pertenecer a una clase)
int huge Selecc_modo_graf(void)
{
   return MODO_GRAFICO; // 1024 x 768 x 256   (3)
}                       // 800  x 600 x 256   (2)
                        // 640  x 480 x 256   (1)


// Constructor de la pantalla grafica
void Pantalla_Grafica::Inicializa(const char *path,const int graba, const int svga, const int ct, const int cf)
{
   // Inicializa las variables del modo grafico
   Manejador_Grafico = DETECT;
   MODO_GRAFICO = svga;
   // Graba la ventana de texto antes de inicializar el modo grafico
   Grabar = graba;
   if(Grabar) {
      // Asignaci¢n del nombre del archivo que contendra la imagen en modo texto
      ARCH_TEMP = new char[MAXPATH];
      Nombre_archivo_temporal(ARCH_TEMP);
      // Obtiene la pantalla de texto
      gettextinfo(&i_s);
      graba_pantalla_texto(1,1,i_s.screenwidth,i_s.screenheight,ARCH_TEMP);
   }
   // Installa el driver no estandar para manejo grafico
   if(svga) installuserdriver("SVGA256",Selecc_modo_graf);
   // Inicializa el modo grafico
   initgraph(&Manejador_Grafico,&Modo_Grafico,path);
   //  Revisa la inicializaci¢n del modo grafico
   if(graphresult()) {
      printf(TXT1,grapherrormsg(graphresult()));
      exit(1);
   }
   // Indica que el modo grafico esta activo
   MODO_GRAFICO_ACTIVO = 1;
   // Obtiene los valores maximos y minimos del sistema grafico
   X_MAX = getmaxx();
   Y_MAX = getmaxy();

   struct palettetype pal;
   getpalette(&pal);

   MAX_COLORES = getmaxcolor();
   // Definicion de los distintos colores usados en el sistema
   for(int i = 0; i < 16; i++) setrgbpalette(color[i],cn1[i],cn2[i],cn3[i]);
   //  Selecciona color de fondo y texto
   setbkcolor(cf);
   setcolor(ct);
   cleardevice();
}

// Destructor de la pantalla grafica
void Pantalla_Grafica::Destruye(void)
{
   closegraph();
   // Indica que el modo grafico esta desactivado
   MODO_GRAFICO_ACTIVO = 0;
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
      // Borra la variable temporal
      delete []ARCH_TEMP;
   }
}


/*
      setrgbpalette(Negro,0,0,0);
      setrgbpalette(Gris1,175,175,179);
      setrgbpalette(Gris2,160,160,160);
      setrgbpalette(Gris9,160,160,164);
      setrgbpalette(Azul1,0,192,164);
      setrgbpalette(Azul9,0,164,164);
      setrgbpalette(Verde1,0,60,0); //0,255,0);
      setrgbpalette(Verde9,0,90,0);
      setrgbpalette(Cian1,0,255,255); // 143,85,209);
      setrgbpalette(Cian9,0,164,164);
      setrgbpalette(Rojo1,255,0,0);
      setrgbpalette(Rojo9,164,0,0);
      setrgbpalette(Magenta1,255,0,255);
      setrgbpalette(Magenta9,0,0,128);
      setrgbpalette(Cafe,128,64,64);
      setrgbpalette(Amarillo,255,255,0);
      setrgbpalette(Blanco,255,255,255);
*/


