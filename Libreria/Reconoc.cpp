// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#include "d:\libreria\Reconoc.hpp"
#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <stdio.h>
   #include <string.h>
#ifdef __PROG_DOS__
}
#endif


// Constructor de la clase
Reconocedor::Reconocedor(void)
{
   Cadena_reconocer = NULL;
   Cadena_separadores = NULL;
   Numero_elementos = 0;
   Arreglo = NULL;
   Estado = SIN_INICIALIZAR;
}


// Destructor de la clase
Reconocedor::~Reconocedor(void)
{
   Destruye();
   if (Cadena_reconocer) delete Cadena_reconocer;
   if (Cadena_separadores) delete Cadena_separadores;
}


// Construye el arreglo que contiene los componentes de la cadena
void Reconocedor::Construye(void)
{
   if (Cadena_separadores && Cadena_reconocer) {
      // Destruye el arreglo que contiene los componentes de la cadena
      Destruye();
      char *xcad = new char[strlen(Cadena_reconocer)+1];
      unsigned int ind = 0;
      unsigned int num_elem = Revisa_numero_elementos();
      if (num_elem) {
         Arreglo = new char*[num_elem];
         unsigned int i1 = 0, i2 = 0, st = 0;
         while (Cadena_reconocer[i1]) {
            i2 = 0, st = 0;
            while (Cadena_separadores[i2]) {
               if (Cadena_reconocer[i1] == Cadena_separadores[i2]) {
                  st = 1;
                  break;
               }
               i2 ++;
            }
            if (st) {
               if (ind) {
                  // Genera elemento 
                  Arreglo[Numero_elementos] = Asigna_puntero(xcad);
                  // Actualiza indice
                  Numero_elementos ++;
                  // Limpia la cadena temporal
                  ind = 0;
                  xcad[ind] = 0;
               }
             } else {
               // Almacena el caracter en la cadena temporal
               xcad[ind] = Cadena_reconocer[i1];
               ind ++;
               xcad[ind] = 0;
            }
            i1 ++;
         }
         // Genera elemento 
         if(ind) {
            Arreglo[Numero_elementos] = Asigna_puntero(xcad);
            // Actualiza indice
            Numero_elementos ++;
         }
         // Cambia el estado del objeto
         Estado = INICIALIZADA;
      }
      delete []xcad;
   }
}


// Destruye el arreglo que contiene los componentes de la cadena
void Reconocedor::Destruye(void)
{
   if (Estado == INICIALIZADA) {
      for (unsigned int i = 0; i < Numero_elementos; i ++) delete [] Arreglo[i];
      delete []Arreglo;
      Arreglo = NULL;
   }
   // Cambia el estado del objeto
   Estado = SIN_INICIALIZAR;
   // Actualiza indice
   Numero_elementos = 0;
}


// Revisa en cuantos elementos puede ser separada la cadena
unsigned int Reconocedor::Revisa_numero_elementos(void)
{
   unsigned int numero = 0, edo = 1, sw = 0;
   if (Cadena_separadores && Cadena_reconocer) {
      unsigned int i1 = 0, i2 = 0, st = 0;
      numero = 1;
      while (Cadena_reconocer[i1]) {
         i2 = 0, st = 0;
         while (Cadena_separadores[i2]) {
            if (Cadena_reconocer[i1] == Cadena_separadores[i2]) {
               st = 1;
               break;
            }
            i2 ++;
         }
         if (!st) {
            if (edo && sw) numero ++;
            edo = 0;
            sw = 1;
         } else edo = 1;
         i1 ++;
      }
   }
   return numero;
}


// Asigna una cadena para su separacion
void Reconocedor::Asigna_cadena(const char *cad)
{
   if (cad == NULL) return;
   if (Cadena_reconocer) delete Cadena_reconocer;
   Cadena_reconocer = Asigna_puntero(cad);
}

// Asigna los separadores que seran usados en la cadena para su separacion
void Reconocedor::Asigna_separadores(const char *cad)
{
   if (cad == NULL) return;
   if (Cadena_separadores) delete Cadena_separadores;
   Cadena_separadores = Asigna_puntero(cad);
}




// Retorna en la variable CAD al elemento numero NUM
void Reconocedor::Elemento(const unsigned int num, char *cad)
{
   cad[0] = 0;
   if (Estado != SIN_INICIALIZAR) {
      if(num < Numero_elementos) strcpy(cad,Arreglo[num]);
   }
}

// Retorna un puntero al elemento numero NUM
char *Reconocedor::Elemento(const unsigned int num)
{
   char *ptr = NULL;
   if (Estado != SIN_INICIALIZAR) {
      if(num < Numero_elementos) ptr = Arreglo[num];
   }
   return ptr;
}
