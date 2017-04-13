// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.01-A


#include "porta_pa.hpp"
#include "r_esp.hpp"
#include "nucleo.hpp"
#include "cad_car.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;

extern "C" {
   #include <string.h>
}


#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "Hay m s elementos de los definidos"
   #define TXT2 "en el portapapeles"
   #define TXT3 "No hay elementos en el portapapeles"
   #define TXT4 "No esta definido ese elemento"
#else
   #define TXT1 "There are more elements than those"
   #define TXT2 "defined in the buffer"
   #define TXT3 "No elements in buffer"
   #define TXT4 "Not defined element"
#endif



////////////////////////////////////////////////////////////////
//  Clase que controla al porta papeles                       //
//  Permite inicializar y controlar el porta papeles          //
////////////////////////////////////////////////////////////////



// Inicializa el porta papeles
void Porta_papeles::Inicializa_porta_papeles(void)
{
   Informacion = NULL;
   Cad_temp = " ";
   Total_elementos = Actual_elemento = Activo = 0;
}


// Borra el contenido del porta papeles
void Porta_papeles::Borra_porta_papeles(void)
{
   if(Total_elementos) {
      for(i = 0; i < Total_elementos; i++) delete []Informacion[i];
      delete []Informacion;
   }
   Total_elementos = Actual_elemento = Activo = 0;
}

// Crea el porta papeles
void Porta_papeles::Crea_porta_papeles(unsigned int num_elem)
{
   Borra_porta_papeles();
   Informacion = new char *[num_elem];
   for(i = 0; i < num_elem; i++) Informacion[i] = NULL;
   Total_elementos = num_elem;
   Actual_elemento = 0;
   Activo = 1;
}

// Inserta una cadena en el porta papeles
void Porta_papeles::Inserta_porta_papeles(const char *cad)
{
   if(Activo) {
      if(Actual_elemento < Total_elementos) {
         Informacion[Actual_elemento] = new char[strlen(cad)+1];
         strcpy(Informacion[Actual_elemento],cad);
         Actual_elemento++;
      } else NCO->Mensaje(TXT1,TXT2);
   } else NCO->Mensaje(TXT3);
}


// Retorna la cadena del porta papeles
const char *Porta_papeles::Retorna_cadena_porta_papeles(unsigned int  num_elem)
{
   const char *tmp = Cad_temp;
   if(Activo) { 
      if(num_elem < Total_elementos) tmp = Informacion[num_elem];
       else NCO->Mensaje(TXT4,TXT2);
   } else NCO->Mensaje(TXT3);
   return tmp;
}



