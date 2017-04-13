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
   #define TXT1 "LEE PANTALLA"
#else
   #define TXT1 "READ WINDOW"
#endif



// Lee imagen grafica sin formato alguno, leyendo pixel por pixel
void Lee_imagen_sin_formato(const char *arch, const int x1, const int y1, const int x2, const int y2)
{
   Manipulador_archivos *Lee_pant  = new Manipulador_archivos;
   Lee_pant->Parametros(arch,LEE_ARCHIVO,0,TXT1);
   unsigned int x = 0, y = 0, ren_l, car_l;
   // Lee el numero de renglones y caracteres
   Lee_pant->Lee_entero(ren_l);
   Lee_pant->Lee_entero(car_l);
   unsigned int numero, i;
   char color;
   // Visualiza la imagen
   while(y <= ren_l) {
      Lee_pant->Lee_entero(numero);
      Lee_pant->Lee_caracter(color);
        for(i = 0; i < numero; i ++) {
            putpixel(x1 + x, y1 + y, (unsigned int) color);
            x ++;
            if(x >= car_l) x = 0, y++;
        }
   }
   delete Lee_pant;
}

/*
// Lee imagen grafica sin formato alguno, leyendo pixel por pixel
void Lee_imagen_sin_formato(const char *arch, const int x1, const int y1, const int x2, const int y2)
{
   Manipulador_archivos *Lee_pant  = new Manipulador_archivos;
   Lee_pant->Parametros(arch,LEE_ARCHIVO,0,"LEE PANTALLA");
   int x, y, ren_l, car_l, ren = y2 - y1 +1, car = x2 - x1 + 1;
   int Ren, Car, inc_x = 0, inc_y = 0;
   // Lee el numero de renglones y caracteres
   Lee_pant->Lee_entero(ren_l);
   Lee_pant->Lee_entero(car_l);
//   char *buff = new char[car_l + 1];
   // Ajusta a la longitud actual
   if(ren_l > ren) Ren = ren, inc_y = (ren_l - ren) / 2;
    else Ren = ren_l;
   if(car_l > car) Car = car, inc_x = (car_l - car) / 2;
    else Car = car_l;
   // Visualiza la imagen
   while(1) {
   for(y = 0; y < inc_y; y++) Lee_pant->Lee_caracteres(buff, car_l);
   for(y = 0; y < Ren; y++) {
       Lee_pant->Lee_caracteres(buff,car_l);
       for(x = 0; x < Car; x++) putpixel(x1 + x, y1 + y, (unsigned int) buff[inc_x + x]);
   }
   delete Lee_pant;
//   delete []buff;
}
*/
