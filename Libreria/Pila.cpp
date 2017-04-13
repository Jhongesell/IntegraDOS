// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <string.h>
#ifdef __PROG_DOS__
}
#endif
#include "pila.hpp"
#pragma hdrstop



// Constructor de la pila
PILA::PILA(const unsigned int num_elementos_pila, const unsigned int tipo)
{
   // Inicializa el indicador de la pila
   Desbordamiento = 0;
   Vacia = 1;
   Elemento_actual = 0;
   Tipo_pila = tipo;
   // Define la longitud de la pila y el numero de elementos de esta
   NUM_ELEMENTOS_PILA = num_elementos_pila;
   if (Tipo_pila == NUMERICA) {
      // Solicita la memoria para la pila
      NBufer = new long double[NUM_ELEMENTOS_PILA+1];
      // Solicita la memoria para la cadena temporal de trabajo
      for (unsigned int i = 0; i < NUM_ELEMENTOS_PILA; i++) NBufer[i] = 0.0;
   } 
   if (Tipo_pila == ENTEROS) {
      // Solicita la memoria para la pila
      EBufer = new int[NUM_ELEMENTOS_PILA+1];
      // Solicita la memoria para la cadena temporal de trabajo
      for (unsigned int i = 0; i < NUM_ELEMENTOS_PILA; i++) EBufer[i] = 0;
   } 
   if (Tipo_pila == ALFABETICA) {
      // Solicita la memoria para la pila
      CBufer = new char *[NUM_ELEMENTOS_PILA+1];
      // Solicita la memoria para la cadena temporal de trabajo
      for (unsigned int i = 0; i < NUM_ELEMENTOS_PILA; i++) CBufer[i] = NULL;
   }
}


// Destructor de la pila
PILA::~PILA()
{
   // Libera la memoria solicitada para la pila
   if (Tipo_pila == NUMERICA) delete[] NBufer;
   if (Tipo_pila == ENTEROS)  delete[] EBufer;
   if (Tipo_pila == ALFABETICA) {
      for (unsigned int i = 0; i < NUM_ELEMENTOS_PILA; i++) delete CBufer[i];
      delete[] CBufer;
   }
}


// Mete un numero en la pila
int PILA::Meter(const long double cadena)
{
   if (Elemento_actual < NUM_ELEMENTOS_PILA) {
      NBufer[Elemento_actual] = cadena;
      Elemento_actual ++;
      Desbordamiento = 0;
   } else Desbordamiento = 1;
   Vacia = 0;
   return Desbordamiento;
}


// Mete un numero en la pila
int PILA::Meter(const int cadena)
{
   if (Elemento_actual < NUM_ELEMENTOS_PILA) {
      EBufer[Elemento_actual] = cadena;
      Elemento_actual ++;
      Desbordamiento = 0;
   } else Desbordamiento = 1;
   Vacia = 0;
   return Desbordamiento;
}

// Mete la cadena en la pila
int PILA::Meter(const char *cadena)
{
   if (Elemento_actual < NUM_ELEMENTOS_PILA) {
      Cxcad = Asigna_puntero(cadena);
      strcpy(Cxcad,cadena);
      CBufer[Elemento_actual] = Cxcad;
      Elemento_actual ++;
      Desbordamiento = 0;
   } else Desbordamiento = 1;
   Vacia = 0;
   return Desbordamiento;
}

// Saca de la pila un numero
int PILA::Sacar(long double &cad)
{
   cad = 0;
   if (Elemento_actual > 0) {
      Elemento_actual --;
      cad = NBufer[Elemento_actual];
      NBufer[Elemento_actual] = 0.0;
   } 
   if (Elemento_actual) Vacia = 0;
    else Vacia = 1;
   return Vacia;
}

// Saca de la pila un numero
int PILA::Sacar(int &cad)
{
   cad = 0;
   if (Elemento_actual > 0) {
      Elemento_actual --;
      cad = EBufer[Elemento_actual];
      EBufer[Elemento_actual] = 0;
   } 
   if (Elemento_actual) Vacia = 0;
    else Vacia = 1;
   return Vacia;
}


// Saca de la pila una cadena
int PILA::Sacar(char *cad)
{
   cad[0] = 0;
   if (Elemento_actual > 0) {
      Elemento_actual --;
      strcpy(cad,CBufer[Elemento_actual]);
      delete CBufer[Elemento_actual];
      CBufer[Elemento_actual] = NULL;
   } 
   if (Elemento_actual) Vacia = 0;
    else Vacia = 1;
   return Vacia;
}


