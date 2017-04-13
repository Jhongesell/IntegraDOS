// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifndef __CALNPOL_HPP__
#define __CALNPOL_HPP__


#include "cadenas.hpp"

#define NUM_MAX_ELEM_MEMORIA  10

class Calculadora_notacion_polaca: public Cadenas
{
   private:
     
                  // Indica el tipo de conversion activa el la calculadora
      int         Tipo_conversion;
                  // Indica si hay error matemático
      int         Error_matematico;
                  // Varibles definidas por el usuario
      long double MEM[NUM_MAX_ELEM_MEMORIA];

      
                  // Rutina que recibe un indicador de operacion asi como un numero determinado de operandos para realizar la operacion indicada
      long double Calcula(const unsigned int operacion, const long double op1, const long double op2);
   
   public:

                  Calculadora_notacion_polaca(void)
                  {
                      Inicializa_valor();
                  }
                  // Rutina que calcula el valor numerico de una expresion en notacion polaca con separador de operandos el pipe
      long double Evalua(const char *cadena);
                  // Asigna un valor a el elemento de la memoria indicado
      int         Asigna_valor(const unsigned int elem, const long double val);
                  // retorna el valor del elemento de la memoria indicado
      int         Retorna_valor(const unsigned int elem, long double &val);
                  // Inicializa la memoria 
      void        Inicializa_valor(void);
};

#endif
