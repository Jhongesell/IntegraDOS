// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "boton.hpp"
#include "nucleo.hpp"
extern "C" {
   #include <graphics.h>
}

// Objeto global al nuclo del sistema
extern Nucleo *NCO;



// Revisa si se oprime el primer boton boton del mouse y revisa su permanencia dento del boton
int Boton::Oprimido(void) const
{
   int st = 0;
   // Cambia el tipo de cursor del mouse
   if(Ventana_marco::Mouse_dentro()) NCO->Selecciona_tipo_mouse(2);
   // Revisa si fue presionado algun boton o se tecleo Enter
   if(Presionado(BR)) {
      // Mover el contenido para simular el presionado del boton
      Ventana_marco::Marco_ventana(0);
      unsigned int Lg = imagesize(X1+2,Y1+2,X2-2,Y2-2);
      char *buff = new char[Lg+10];
      getimage(X1+2,Y1+2,X2-3,Y2-3,buff);
      setcolor(Actual_color_ventana());
      line(X1+2,Y1+2,X2-1,Y1+2);
      line(X1+2,Y1+2,X1+2,Y2-1);
      putimage(X1+3,Y1+3,buff,0);
      delete []buff;
      // Controla el mouse si esta presionado
      while(Presionado(BR)) NCO->Administrador();
      NCO->Muestra_mouse(NO);
      setcolor(Actual_color_ventana());
      line(X2-2,Y1+1,X2-2,Y2-1);
      line(X1+1,Y2-2,X2-1,Y2-2);
      line(X1+1,Y2-3,X2-1,Y2-3);
      Dibuja();
      // Revisa si al terminar de presiona el mouse sigue dentro
      st = Ventana_marco::Mouse_dentro();
   }
   return st;
}


unsigned int Boton::Presionado(const unsigned int boton) const
{
   // Cambia el tipo de cursor del mouse
   if(Ventana_marco::Mouse_dentro()) NCO->Selecciona_tipo_mouse(2);
   // Revisa si el boton del mouse fue presionado dentro de la ventana
   return Ventana_marco::Presionado(boton);
}

