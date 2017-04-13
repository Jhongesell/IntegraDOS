// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A



#ifndef __RECONOC_HPP__
#define __RECONOC_HPP__
#include <stdio.h>
#include "cadenas.hpp"

#define SIN_INICIALIZAR               0
#define INICIALIZADA                  1


// Clase que dada una cadena y un indicador de separacion, parta la cadena en 
// los elementos que la constituyen construyendo con estos un arreglo el cual
// Puede ser accedido por el usuario

//////////////////////////////////////////////////////////////////////
// Guardar la posicion donde inicia la cadena separada de la cadena original
// Activer las funciones Resto y anterior usando Substr y la posicion de la cadena
//////////////////////////////////////////////////////////////////////

class Reconocedor: virtual public Cadenas 
{
    private:

       char         *Cadena_reconocer;
       char         *Cadena_separadores;
       unsigned int  Numero_elementos;
       char        **Arreglo;
       unsigned int  Estado;
       
    public:
                     // Constructores
                     Reconocedor(void);
                     Reconocedor(const char *cad, const char *sep)
                     {
                        Cadena_reconocer = NULL;
                        Cadena_separadores = NULL;
                        Numero_elementos = 0;
                        Arreglo = NULL;
                        Estado = SIN_INICIALIZAR;
                        Asigna_cadena__separador(cad, sep);
                     }
                     // Destructor
                    ~Reconocedor(void);
                    
                     // Construye el arreglo que contiene los componentes de la cadena
       void          Construye(void);
                     // Destruye el arreglo que contiene los componentes de la cadena
       void          Destruye(void);
                     // Asigna la cadena y los separadores que seran usados en la cadena para su separacion
       void          Asigna_cadena__separador(const char *cad, const char *sep)
                     {
                        Asigna_cadena(cad);
                        Asigna_separadores(sep);
                        Construye();
                     }
                     // Asigna una cadena para su separacion
       void          Asigna_cadena(const char *cad);
                     // Asigna los separadores que seran usados en la cadena para su separacion
       void          Asigna_separadores(const char *cad);
                     // Revisa en cuantos elementos puede ser separada la cadena
       unsigned int  Revisa_numero_elementos(void);
                     // Retorna el numero de elementos existentes en el arreglo
       int           Retorna_numero_elementos(void)
                     {return Numero_elementos;}
                     // Retorna en la variable CAD al elemento numero NUM
       void          Elemento(const unsigned int num, char *cad);
                     // Retorna un puntero al elemento numero NUM
       char         *Elemento(const unsigned int num);
       
};


#endif 

//       unsigned int *Posicion_cadena;
//       unsigned int  Separa_num_bytes;
//       unsigned int  Activa_separa_num_bytes;

//       char         *Primera(void)
//       char         *Ultima(void);
//       char         *Siguiente(void);
//       char         *Anterior(void);
//       unsigned int  Indice_actual(void);
//       char         *Cadena_restante(void);
//       char         *Cadena_anterior(void);

