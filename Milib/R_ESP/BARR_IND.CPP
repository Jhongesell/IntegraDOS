// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "barr_ind.hpp"
#include "nucleo.hpp"
#include "icono.hpp"

extern "C" {
   #include <graphics.h>
}

// Objeto global al nuclo del sistema
extern Nucleo *NCO;

// Indica el retardo al presionar botones
#define RETARDO 100


void Traza_indicador::Define_indicador(const int x1, const int y1, const int x2, const int y2, const int tipo)
{
   C_fdo = Gris9, C_ind = Blanco, Sw = 1;
   Tipo = tipo, X1 = x1, X2 = x2, Y1 = y1, Y2 = y2;
   Icon1 = new Icono;
   Icon2 = new Icono;
   if(Tipo) {
      Icon1->Define_icono("FARR.ICO",x1+2,y1+2,20,19);
      Icon2->Define_icono("FABA.ICO",x1+2,y2-18,20,19);
    } else {
      Icon1->Define_icono("FIZQ.ICO",x1+2,y1+2,20,19);
      Icon2->Define_icono("FDER.ICO",x2-20,y1+2,20,19);
   }
   Porcent = 0;
}


Traza_indicador::~Traza_indicador()
{
   delete Icon1;
   delete Icon2;
}


int Traza_indicador::Controlador(const long ea, const long et, const long ep)
{
   if(et > ep) {
      ant_por = 0, inc = 0, tmp = 0, por = 0;
      if(et > 0) {
         por = int (ea * 100 / et);
         if(ea <= 1) por = 0;
         if(ea >= et) por = 100;
      }
      if(por != Porcent) ant_por = Porcent, Porcent = por, Sw = 1;
      if(Sw) {
         // Apaga el mouse
         NCO->Muestra_mouse(NO);
         setfillstyle(1,C_fdo);
         if(Tipo) {
            tmp = (Y2 - Y1) -58;
            inc = (tmp * ant_por) / 100 + (Y1 + 23);
            bar(X1+2,inc,X1+21,inc+15);
            setfillstyle(1,Gris1);
            setcolor(Negro);
            inc = (tmp * Porcent) / 100 + (Y1 + 23);
            bar(X1+2,inc,X1+21,inc+15);
            rectangle(X1+2,inc,X1+21,inc+15);
            setcolor(Blanco);
            line(X1+3,inc+1,X1+20,inc+1);
            line(X1+3,inc+1,X1+3,inc+14);
         } else {
            tmp = (X2 - X1) -59;
            inc = (tmp * ant_por) / 100 + (X1 + 23);
            bar(inc,Y1+2,inc+15,Y1+21);
            setfillstyle(1,Gris1);
            setcolor(Negro);
            inc = (tmp * Porcent) / 100 + (X1 + 23);
            bar(inc,Y1+2,inc+15,Y1+21);
            rectangle(inc,Y1+2,inc+15,Y1+21);
            setcolor(Blanco);
            line(inc+1,Y1+3,inc+14,Y1+3);
            line(inc+1,Y1+3,inc+1,Y1+20);
         }
         Sw = 0;
      }
   }
   if(Icon1->Presionado(BR) || Icon2->Presionado(BR)) delay(RETARDO);
   // Revisa el estado de los iconos
   if(Icon1->Presionado(BR)) return 1;
   if(Icon2->Presionado(BR)) return 2;
   return 0;
}


void Traza_indicador::Dibuja(void)
{
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   // dibuja el borde de la barra indicadora
   setfillstyle(1,C_fdo);
   if(Tipo) bar(X1,Y1,X1+24,Y2);
    else bar(X1,Y1,X2,Y1+22);
   // Dibuja los iconos
   Icon1->Dibuja();
   Icon2->Dibuja();
   Sw = 1;
   Porcent = 0;
}

