// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "icono.hpp"
#include "cad_car.hpp"
#include "man_arch.hpp"
extern "C" {
   #include <graphics.h>
   #include <stdlib.h>
   #include <dir.h>
}


extern char *PATH_SISTEMA;



/////////////////////////////////////////////////////////////////////////////////////
//                                                                                 //
//                      Comportamientos del Icono                                  //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////


// Define los elementos del boton
void  Icono::Define_icono(const char *archivo, const int x, const int y, const int lgx, const int lgy)
{
   if(Imagen) delete []Imagen;
   char xarch[MAXPATH], xinfo[5];
   long Longitud = 0;
   // Adiciona la trayectoria del archivo
   sprintf(xarch,"%s%s",PATH_SISTEMA,archivo);
   // Prepara el archivo del icono
   Manipulador_archivos *Lee_icono = new Manipulador_archivos;
   Lee_icono->Parametros(xarch,0,0,"Icono");
   // Lee las caracteristicas del icono
   Lee_icono->Lee_caracteres(xinfo,3);
   // Configura al boton que contendra al icono
   Largo_x = (int) xinfo[0];
   Largo_y = (int) xinfo[1];
   Colores = (int) xinfo[2];
   Longitud = Largo_x * Largo_y;
   // Solicita la memoria necesaria para contener al icono
   Imagen = new char [(unsigned int) Longitud+5];
   Lee_icono->Lee_caracteres(Imagen,Longitud);
   Boton::Abrir(x,y,lgx,lgy);
   delete Lee_icono;
}



// Dibuja la estructura del boton
void Icono::Dibuja(void) const
{
   int i = 0, x, y, pos;
   int lx = Largo_x > Largo_X()-4? Largo_X()-4:Largo_x;
   int ly = Largo_y > Largo_Y()-4? Largo_Y()-4:Largo_y;
   char color_p1, color_p2;
   // Visualiza el boton
   Ventana_marco::Dibuja();
   // Dibuja el icono
   for(y = 0; y < ly; y++) {
      for(x = 0; x < (Largo_x / 2); x++,i++) {
         pos = x * 2 + 2;
         if(pos < lx) {
            color_p1 = Imagen[i] >> 4;
            color_p2 = Imagen[i] & 0x0f;
            putpixel(X1+pos  ,Y1+y+3,color_p1);
            putpixel(X1+pos+1,Y1+y+3,color_p2);
         }
      }
   }
   // Visualiza el marco del boton
   Ventana_marco::Marco_ventana(1);
}



