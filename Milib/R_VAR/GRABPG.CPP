// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#include "man_arch.hpp"
//#include "compact.hpp"

extern "C" {
   #include <graphics.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "GRABA PANTALLA"
#else
   #define TXT1 "SAVE WINDOW"
#endif



void graba_pantalla_grafica(const int x1, const int y1, const int x2, const int y2, const char *archivo);
  // Graba el contenido de un recuadro de la pantalla en el archivo ARCHIVO


void graba_pantalla_grafica(const int x1, const int y1, const int x2, const int y2, const char *archivo)
{
   unsigned int long_vent = 0, Memoria = 54000u;
   unsigned int  xy1 = y1, xy2 = y2, i = 0;
   unsigned int Tramos = 1, inc = 0;
   char *buff;

   long_vent = imagesize(x1,xy1,x2,xy2);
   // Revisa si la pantalla se puede grabar como una sola imagen
   while(!(long_vent < Memoria)) {
      Tramos++;
      inc = (y2 - y1 + 1) / Tramos;
      long_vent = imagesize(x1,xy1,x2,xy1+inc);
   }
   // Inicializa el objeto manipulador de archivos
   Manipulador_archivos *Grab_pant = new Manipulador_archivos;
   Grab_pant->Parametros(archivo,GRABA_ARCHIVO,0,TXT1);
   // graba el numero de tramos en el cual se partira la imagen
   Grab_pant->Graba_entero(Tramos);
   // Graba la cantidad de renglones de la imagen
   Grab_pant->Graba_entero(inc);
   for(i = 0; i < Tramos; i++) {
     // calcula la porcion del tramo a  grabar
     xy2 = y2 - (inc * (Tramos - (i + 1)));
     // Optiene el tama¤o de la imagen
     long_vent = imagesize(x1,xy1,x2,xy2);
     buff = new char[long_vent];
     // Obtiene la imagen
     getimage(x1,xy1,x2,xy2,buff);
     // Graba la Longitud de la imagen
     Grab_pant->Graba_entero(long_vent);
     // Graba la imagen
     Grab_pant->Graba_caracteres(buff,long_vent);
     // Actualiza el siguiente tramo a grabar
     xy1 += inc;
     delete []buff;
   }
   delete Grab_pant;
}


/*
void graba_pantalla_grafica(const int x1, const int y1, const int x2, const int y2, const char *archivo)
{
   unsigned int long_vent = 0, Memoria = 54000u;
   unsigned int  xy1 = y1, xy2 = y2, i = 0;
   unsigned int lg_comp, Tramos = 1, inc = 0;
   char *buff, *xbuff ;

   long_vent = imagesize(x1,xy1,x2,xy2);
   // Revisa si la pantalla se puede grabar como una sola imagen
   while(!(long_vent < Memoria)) {
      Tramos++;
      inc = (y2 - y1 + 1) / Tramos;
      long_vent = imagesize(x1,xy1,x2,xy1+inc);
   }
   Compacta *Cp = new Compacta;
   // Inicializa el objeto manipulador de archivos
   Manipulador_archivos *Grab_pant = new Manipulador_archivos;
   Grab_pant->Parametros(archivo,GRABA_ARCHIVO,0,"GRABA PANTALLA");
   // graba el numero de tramos en el cual se partira la imagen
   Grab_pant->Graba_entero(Tramos);
   // Graba la cantidad de renglones de la imagen
   Grab_pant->Graba_entero(inc);
   for(i = 0; i < Tramos; i++) {
     // calcula la porcion del tramo a  grabar
     xy2 = y2 - (inc * (Tramos - (i + 1)));
     // Optiene el tama¤o de la imagen
     long_vent = imagesize(x1,xy1,x2,xy2);
     buff = new char[long_vent];
     // Obtiene la imagen
     getimage(x1,xy1,x2,xy2,buff);
     // Solicita la memoria necesaria para compactar
     xbuff = new char[Cp->Longitud_memoria_compactacion(buff,long_vent)];
     // Compacta la imagen
     Cp->Compacta_cadena(buff,long_vent,xbuff,lg_comp);
     // Graba la Longitud de la imagen
     Grab_pant->Graba_entero(lg_comp);
     // Graba la imagen
     Grab_pant->Graba_caracteres(xbuff,lg_comp);
     // Actualiza el siguiente tramo a grabar
     xy1 += inc;
     delete []xbuff;
     delete []buff;
   }
   delete Grab_pant;
   delete Cp;
}
*/



