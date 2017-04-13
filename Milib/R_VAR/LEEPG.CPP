// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


//#include "cad_car.hpp"
#include "man_arch.hpp"
//#include "compact.hpp"

extern "C" {
   #include <graphics.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "LEE PANTALLA"
#else
   #define TXT1 "READ WINDOW"
#endif


void lee_pantalla_grafica(const int x1, const int y1, const char *archivo);
// Rutina que lee en contenido de un recuadro apartir de un archivo insertandolo en la pantalla en la posicion X1,Y1


void lee_pantalla_grafica(const int x1, const int y1, const char *archivo)
{
   unsigned int Longitud;
   unsigned int  Tramos, Renglones, xy1 = y1, i;
   // Solicita la memoria para el bufer
   char *buff;

   // Inicializa el objeto que leera la imagen grafica
   Manipulador_archivos *Lee_pant = new Manipulador_archivos;
   Lee_pant->Parametros(archivo,LEE_ARCHIVO,0,TXT1);
   // Lee el numero de tramos a leer
   Lee_pant->Lee_entero(Tramos);
   // Lee el numero de renglones
   Lee_pant->Lee_entero(Renglones);
   for(i = 0; i < Tramos; i++) {
      // Lee la longitud del bloque a leer
      Lee_pant->Lee_entero(Longitud);
      buff = new char[Longitud];
      // Lee el bloque que contiene la imagen grafica
      Lee_pant->Lee_caracteres(buff,Longitud);
      // Solicita la memoria necesaria para la descompactacion
      putimage(x1,xy1,buff,0);
      // Incrementa la posicion de visualizacion de la imagen siguiente
      xy1 += Renglones;
      delete []buff;
   }
   delete Lee_pant;
}


/*
void lee_pantalla_grafica(const int x1, const int y1, const char *archivo)
{
   unsigned int Longitud, lg;
   unsigned int  Tramos, Renglones, xy1 = y1, i;
   // Solicita la memoria para el bufer
   char *buff, *xbuff;

   Compacta *Cp = new Compacta;
   // Inicializa el objeto que leera la imagen grafica
   Manipulador_archivos *Lee_pant = new Manipulador_archivos;
   Lee_pant->Parametros(archivo,LEE_ARCHIVO,0,"LEE PANTALLA");
   // Lee el numero de tramos a leer
   Lee_pant->Lee_entero(Tramos);
   // Lee el numero de renglones
   Lee_pant->Lee_entero(Renglones);
   for(i = 0; i < Tramos; i++) {
      // Lee la longitud del bloque a leer
      Lee_pant->Lee_entero(Longitud);
      buff = new char[Longitud];
      // Lee el bloque que contiene la imagen grafica
      Lee_pant->Lee_caracteres(buff,Longitud);
      // Solicita la memoria necesaria para la descompactacion
      xbuff = new char[Cp->Longitud_memoria_descompactacion(buff,Longitud)];
      // Descompacta la imagen
      Cp->Descompacta_cadena(buff,Longitud,xbuff,lg);
      // Visualiza la imagen grafica
      putimage(x1,xy1,xbuff,0);
      // Incrementa la posicion de visualizacion de la imagen siguiente
      xy1 += Renglones;
      delete []xbuff;
      delete []buff;
   }
   delete Lee_pant;
   delete Cp;
}
*/


