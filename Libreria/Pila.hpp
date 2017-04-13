// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A

#ifndef __PILA_HPP__
#define __PILA_HPP__

#include "cadenas.hpp"

#define ALFABETICA  0
#define NUMERICA    1
#define ENTEROS     2

// Clase PILA permite manipular una pila de longitud variable creada dinamicamente
class PILA: virtual public Cadenas
{
   private:
      unsigned int    NUM_ELEMENTOS_PILA;    // Indica el numero maximo de elementos de la pila
      long double    *NBufer;                // Puntero a los elementos de la pila
      char          **CBufer;                // Puntero a los elementos de la pila
      char           *Cxcad;                 // Puntero a una cadena temporal
      int            *EBufer;
      unsigned int    Elemento_actual;       // Indica el elemento actual
      unsigned int    Desbordamiento:  1;    // Indica si existe desbordamineto
      unsigned int    Vacia:           1;    // Indica si la pila esta vacia
      unsigned int    Tipo_pila;             // Indica si es pila numerica (1) o alfabetica

   public:
          // Constructor y destructor de la pila
          PILA(const unsigned int num_elementos_pila, const unsigned int tipo);
         ~PILA();
         
      int Meter(const long double cadena);
      int Meter(const int cadena);
      int Meter(const char *cadena);
      int Sacar(long double &cad);
      int Sacar(int &cad);
      int Sacar(char *cad);
      int Pila_desbordamiento(void)
          {return Desbordamiento;}
      int Pila_vacia(void)
          {return Vacia;}
};

#endif

/*
// Clase PILA permite manipular una pila de longitud variable creada dinamicamente
class PILA: virtual public Cadenas
{
   private:
      unsigned int    NUM_ELEMENTOS_PILA;    // Indica el numero maximo de elementos de la pila
      char          **Bufer;                 // Puntero a los elementos de la pila
      char           *xcad;                  // Puntero a una cadena temporal
      int             Elemento_actual;       // Indica el elemento actual
      unsigned int    Desbordamiento:  1;    // Indica si existe desbordamineto
      unsigned int    Vacia:           1;    // Indica si la pila esta vacia

   public:
          // Constructor y destructor de la pila
          PILA(unsigned int num_elementos_pila);
         ~PILA();
         
      int Meter(const char *cadena);
      int Sacar(char *cad);
      int Pila_desbordamiento(void)
          {return Desbordamiento;}
      int Pila_vacia(void)
          {return Vacia;}
};

*/
