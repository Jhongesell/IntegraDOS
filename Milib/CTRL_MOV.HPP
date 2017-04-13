// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __CTRL_MOV__
#define __CTRL_MOV__

#include "..\libreria\cadenas.hpp"


// Clase controladora de movimiento
class Controlador_movimiento: private Cadenas
{
   private:

      unsigned int  Movimiento_ciclico: 1;    // Indica si esta activa el movimiento ciclico
      unsigned int  Encontrado:         1;    // Indica si se encontro
      unsigned int  Tipo_paneo:         1;    // Indica el tipo de paneo usado
      unsigned int  Actual_elemento;          // Indica el elemeto actual
      unsigned int  Inicia_paneo;             // Indica el elemento inicial del paneo
      unsigned int  Actual_paneo;             // Inidca el actual elemento dentro del paneo
      unsigned int  Total_elementos;          // Indica el total de elementos
      unsigned int  Elementos_paneo;          // Indica el numero de elementos en el paneo
      char         *Cadena_busqueda;          // Cadena para buscar en ella el caracter de activacion
      char         *Cadena_descartar;         // Indica cuales opciones estan desavilitadas

   public:

                    // Constructo de la clase 
                    Controlador_movimiento(void);
                    // Destructor de la clase 
                   ~Controlador_movimiento();

                    // Paremetros de la clase controlador
               void Parametros(const unsigned int t_e, const unsigned int e_p, const char *c_b, const char *c_d);
                    // Controlador horizontal normal
      unsigned int  Horizontal(const int tecla, const char car);
                    // Controlador horizontal extendido (Soporta movimiento entre palabras)
               int  Horizontal(const int tecla, const char *text);
                    // Controlador vertical (acepta caracter  y enter)
      unsigned int  Vertical(const int tecla, const char car);
                    // Controlador vertical
               void Vertical(const int tecla);
                    // Busca el caracter dentro de la cadena de busqueda
               int  Busca_caracter(const char car, unsigned int &pos);
                    // Retorna el elemento actual
      unsigned int  Elemento_actual(void) const
                    {return Actual_elemento;}
                    // Retorna el elemento inicial del paneo
      unsigned int  Elemento_inicial_paneo(void) const
                    {return Inicia_paneo;}
                    // Retorna el actual elemento dentro del paneo
      unsigned int  Actual_elemento_paneo(void) const
                    {return Actual_paneo;}
                    // Retorna el numero de elementos del paneo
      unsigned int  Numero_elementos_paneo(void) const
                    {return Elementos_paneo;}
                    // Retorna el numero de elementos
      unsigned int  Numero_total_elementos(void) const
                    {return Total_elementos;}
                    // Fija el elemento actual
               void Fija_elemento_actual(const unsigned int actual);
                    // Fija el total de elementos
               void Fija_total_elementos(const unsigned int total) const
                    {((Controlador_movimiento*)this)->Total_elementos = total;}
                    // Fija los elementos de paneo
               void Fija_elementos_paneo(const unsigned int paneo) const
                    {((Controlador_movimiento*)this)->Elementos_paneo = paneo;}
                    // Fija si el movimiento es ciclico
               void Fija_movimiento_ciclico(const unsigned int ciclico) const
                    {((Controlador_movimiento*)this)->Movimiento_ciclico = ciclico;}
                    // Fija paneo para uso del menu
               void Fija_tipo_paneo(const unsigned int tipo) const
                    {((Controlador_movimiento*)this)->Tipo_paneo = tipo;}

};

#endif



