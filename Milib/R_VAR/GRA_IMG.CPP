// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#include "man_arch.hpp"

extern "C" {
   #include <graphics.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "GRABA PANTALLA"
#else
   #define TXT1 "SAVE WINDOW"
#endif



// Graba imagen grafica sin formato alguno, tomando pixel por pixel
void Graba_imagen_sin_formato(const char *arch, const int x1, const int y1, const int x2, const int y2)
{
   Manipulador_archivos *Grab_pant = new Manipulador_archivos;
   Grab_pant->Parametros(arch,GRABA_ARCHIVO,0,TXT1);
   unsigned int x = 0, y = 0, Ren = y2 - y1 + 1, Car = x2 - x1 + 1;
   unsigned int numero = 0;
   char color, xcol;
   Grab_pant->Graba_entero(Ren);
   Grab_pant->Graba_entero(Car);
   color = (char) getpixel(x1 + x,y1 + y);
   while(y <= Ren) {
      xcol = (char) getpixel(x1 + x,y1 + y);
      if(color == xcol) numero ++;
       else {
         Grab_pant->Graba_entero(numero);
         Grab_pant->Graba_caracter(color);
         color = xcol, numero = 1;
      }
      x++;
      if(x >= Car) y++, x = 0;
   }
   Grab_pant->Graba_entero(numero);
   Grab_pant->Graba_caracter(color);
   delete Grab_pant;
}


/*
void Graba_imagen_sin_formato(const char *arch, const int x1, const int y1, const int x2, const int y2)
{
   Manipulador_archivos *Grab_pant = new Manipulador_archivos;
   Grab_pant->Parametros(arch,GRABA_ARCHIVO,0,"GRABA PANTALLA");
   int x = 0, y = 0, Ren = y2 - y1 + 1, Car = x2 - x1 + 1;
   char *buff = new char[X_MAX+1];
   Grab_pant->Graba_entero(Ren);
   Grab_pant->Graba_entero(Car);
   for(y = 0; y < Ren; y++) {
      for(x = 0; x < Car; x++) buff[x] = (char) getpixel(x1 + x,y1 + y);
      Grab_pant->Graba_caracteres(buff,Car);
   }
   delete Grab_pant;
   delete []buff;
}

*/
