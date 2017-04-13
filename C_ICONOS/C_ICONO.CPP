// Programa para convertir un icono de texto a uno leible dentro de programas
// Autor Antonio Carrillo Ledesma



#include <stdio.h>
#include <string.h>
//#include <colores.hpp>

extern unsigned int _stklen = 60000u;

void Convierte_colores(char &color)
{
   switch(color) {

      case 1:  color = 4;  break;
      case 3:  color = 6;  break;
      case 4:  color = 1;  break;
      case 6:  color = 3;  break;
      case 9:  color = 12; break;
      case 11: color = 14; break;
      case 12: color = 9;  break;
      case 14: color = 11; break;
/*
      case 0:  color = Negro; break;
      case 1:  color = Rojo1;  break;
      case 2:  color = Verde1;  break;
      case 3:  color = Cafe;  break;
      case 4:  color = Azul1;  break;
      case 5:  color = Magenta1;  break;
      case 6:  color = Cian1;  break;
      case 7:  color = Gris9; break;
      case 8:  color = Gris1;  break;
      case 9:  color = Rojo9; break;
      case 10: color = Verde9; break;
      case 11: color = Amarillo; break;
      case 12: color = Azul9;  break;
      case 13: color = Magenta9;  break;
      case 14: color = Cian9; break;
      case 15: color = Blanco; break;
*/
   }
}




void main(int argc,char *argv[])
{
   // Valida las opciones
   if(argc != 4) {
      printf("Convierte iconos con formato ASCII a formato binario que puede ser leido\n");
      printf("en cualquier programa.\n\n");
      printf("C_ICONO.EXE <Icono texto> <Icono binario> <Tipo>\n\n");
      printf("Donde tipo es: (1) 32 x 32 con 16 colores\n");
      printf("               (2) 64 x 64 con 16 colores\n");
      printf("               (3) 32 x 16 con 16 colores\n");
      printf("               (4) 16 x 16 con 16 colores\n");
      printf("               (5) 40 x 40 con 16 colores\n");
      return;
   }

   // Variables generales
   char car, car1, car2, Imagen[5120], temp[5120];
   char archivo_lectura[80], archivo_grabacion[80];
   char arr[66][66];
   int  y, x, i;
   FILE *ae, *as;


   // Define el tipo de icono default 32x32 de 16 colores
   char Largo_X = 32, Largo_Y = 32, Colores = 16;
   int  Longitud_icono = 512, Posicion_inicial = 252;

   // Icono de 64x64 de 16 colores
   if(argv[3][0] == '2') {
      Largo_X = 64, Largo_Y = 64, Colores = 16;
      Longitud_icono = 2048, Posicion_inicial = 252;
   }

   // Icono de 32x16 de 16 colores
   if(argv[3][0] == '3') {
      Largo_X = 32, Largo_Y = 16, Colores = 16;
      Longitud_icono = 512, Posicion_inicial = 252;
   }

   // Icono de 16x16 de 16 colores
   if(argv[3][0] == '4') {
      Largo_X = 16, Largo_Y = 16, Colores = 16;
      Longitud_icono = 256, Posicion_inicial = 236;
   }

   // Icono de 40x40 de 16 colores
   if(argv[3][0] == '5') {
      Largo_X = 40, Largo_Y = 40, Colores = 16;
      Longitud_icono = 800, Posicion_inicial = 200;
   }

   // A¤ade la extencion al nombre de los archivos
   strcpy(archivo_lectura,argv[1]);
   strcat(archivo_lectura,".ICT");
   strcpy(archivo_grabacion,argv[2]);
   strcat(archivo_grabacion,".ICO");

   // Lectura del archivo que contiene al icono en forma de texto ASCII
   ae = fopen(archivo_lectura,"rb");
   if(ae != NULL) {
      i = 0;
      while(!feof(ae)) {
         car = fgetc(ae);
         // Busca la primera apostrofe del archivo
         if(car != 39) continue;
         // Copia solo los caracteres que participan en el icono
         while(!feof(ae)) {
            if(car > 47 && car < 71) temp[i] = car, i++;
            car = fgetc(ae);
         }
      }
      fclose(ae);
   }


   // Invierte la imagen y excluye los caracteres de control
   i = 0;
   for(y = Largo_Y - 1; y >= 0; y--) {
      for(x = 0; x < Largo_X; x++) arr[y][x] = temp[i + Posicion_inicial], i ++;
   }


   // Convierte el icono texto a icono con formato binario
   i = 0;
   for(y = 0; y < Largo_Y; y++) {
      for(x = 0; x < Largo_X; x += 2) {
         // Separa los pixeles por pares
         car1 = arr[y][x];
         car2 = arr[y][x+1];
         // Los convierte el color a digito
         if(car1 < 58) car1 -= 48;
          else car1 -= 55;
         if(car2 < 58) car2 -= 48;
          else car2 -= 55;
         // Cambia los colores
         Convierte_colores(car1);
         Convierte_colores(car2);
         // Por ser de 16 colores junta dos pixeles en un caracter
         Imagen[i] = (car1 * 16) + car2;
         i++;
      }
   }

   // Graba el archivo con el icono convertido
   as = fopen(archivo_grabacion,"wb");
   if(as != NULL) {
      fputc(Largo_X,as);  // Graba la longitud del icono en X
      fputc(Largo_Y,as);  // Graba la longitud del icono en Y
      fputc(Colores,as);  // Graba el numero de colores del icono en X
      // Graba el icono
      for(i = 0; i < Longitud_icono; i++) fputc(Imagen[i],as);
      fclose(as);
   }

}



