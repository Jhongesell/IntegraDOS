// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

/*

#include "compact.hpp"




unsigned int Compacta::Longitud_memoria_compactacion(const char *cad, const unsigned int lg)
{
   const char *xcar = cad;
   unsigned int i = 0, lg_comp = 0, lg_memoria = lg; 
   long lg_maxima = 0, lg_numero = 0;

   T_compactacion = 0;

   // Analiza la cadena a compactar 
   while(i < lg) {
     if(*xcar == *cad) lg_numero ++;
      else {
         if(lg_numero > lg_maxima) lg_maxima = lg_numero;
         lg_comp ++, lg_numero = 1;
         xcar = cad;
     }
     cad ++;
     i++;
   }

   // Fija el tipo de compresion segun factor de compresion
   if(lg_maxima <  256l) {
      if((lg_comp * 2) < lg) T_compactacion = 1, lg_memoria = lg_comp * 2;
   }
   if(lg_maxima >= 256l && lg_maxima <  65536l) {
      if((lg_comp * 3) < lg) T_compactacion = 2, lg_memoria = lg_comp * 3;
   }
   // Retorna el total de memoria necesaria
   return (lg_memoria+1);
}




// Compacta la cadena CAD de longitud LG dejandola en la cadena CAD_COMP de longitud LG_COMP
void Compacta::Compacta_cadena(const char *cad, const unsigned int lg, char *cad_comp, unsigned int &lg_comp)
{
   const char *xcar = cad, *pt1 = cad;
   unsigned int i = 0, numero = 0;

   // Indica el tipo de compactacion
   *cad_comp = (char) T_compactacion;
   cad_comp++;
   lg_comp = 1;

   // Hace la compactacion 
   switch(T_compactacion) {
      case 0: // Sin compactacion 
         while(i < lg) {
            *cad_comp++ = *pt1++;
            i++;
         }
         lg_comp = i + 1;
         break;
      case 1: // Compactacion a un byte
         while(i < lg) {
           if(*xcar == *pt1) numero ++;
            else {
              *cad_comp = (char) numero;
              *(cad_comp + 1) = *xcar;
              cad_comp += 2;
              lg_comp += 2;
              xcar = pt1, numero = 1;
           }
           pt1++;
           i++;
         }
         *cad_comp = (char) numero;
         *(cad_comp + 1) = *xcar;
         lg_comp += 2;
         break;
      case 2: // Compactacion a dos bytes
         while(i < lg) {
           if(*xcar == *pt1) numero ++;
            else {
              *cad_comp = (char) ((numero << 8) >> 8);
              *(cad_comp + 1) = (char) (numero >> 8);
              *(cad_comp + 2) = *xcar;
              cad_comp += 3;
              lg_comp += 3;
              xcar = pt1, numero = 1;
           }
           pt1++;
           i++;
         }
         *cad_comp = (char) ((numero << 8) >> 8);
         *(cad_comp + 1) = (char) (numero >> 8);
         *(cad_comp + 2) = *xcar;
         lg_comp += 3;
         break;
   }
}


// Solicita la memoria necesaria para la descompactacion
unsigned int Compacta::Longitud_memoria_descompactacion(const char *cad_comp, const unsigned int lg_comp)
{
   unsigned int numero = 0, xi = 0, lg = 0;
   T_compactacion  = (unsigned int) *cad_comp;

   if(!T_compactacion) lg = lg_comp - 1;
   cad_comp++;
   if(T_compactacion == 1) {
      while(xi < lg_comp-1) {
          numero  = (unsigned int) *cad_comp; 
          cad_comp += 2;
          lg += numero;
          xi += 2;
      }
   }

   if(T_compactacion == 2) {
      while(xi < lg_comp-1) {
          numero  = (unsigned int) *cad_comp; 
          cad_comp += 3;
          lg += numero;
          xi += 3;
      }
   }
   return lg;
}


// Descompacta la cadena CAD_COMP de longitud LG_COMP en la cadena CAD de longitud LG
void Compacta::Descompacta_cadena(const char *cad_comp, const unsigned int lg_comp, char *cad, unsigned int &lg)
{
   unsigned numero = 0, i = 0, xi = 0;
   lg = 0;

   T_compactacion  = (unsigned int) *cad_comp;
   cad_comp ++;
   switch(T_compactacion) {
      case 0:
         while(xi < lg_comp-1) {
             *cad++ = *cad_comp++;
             xi++;
         }
         lg = xi;
         break;
      case 1:
         while(xi < lg_comp-1) {
             numero  = (unsigned int) *cad_comp; 
             for(i = 0; i < numero; i++) *cad++ = *(cad_comp + 1);
             cad_comp += 2;
             lg += numero;
             xi += 2;
         }
         break;
      case 2:
         while(xi < lg_comp-1) {
             numero  = (unsigned int) *cad_comp + (unsigned int) (*(cad_comp + 1) << 8);
             for(i = 0; i < numero; i++) *cad++ = *(cad_comp + 2);
             cad_comp += 3;
             lg += numero;
             xi += 3;
         }
         break;
   }
}

*/

