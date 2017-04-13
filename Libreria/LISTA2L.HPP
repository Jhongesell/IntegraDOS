// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


//////////////////////////////////////////////////////////////////
// Manejador de lista doblemente ligada para cadenas de texto   //
// no mayores a 64Kb.                                           //
//////////////////////////////////////////////////////////////////


// Falta comportamientos para mover, copiar nodos

#ifndef __LISTA2L_HPP__
#define __LISTA2L_HPP__

#include "cadenas.hpp"

// Estructura de el nodo de la lista doblemente ligada
typedef struct nodo_lista {
  nodo_lista *Sig; // Nodo siguiente
  nodo_lista *Ant; // Nodo anterior
  char *Info;      // Puntero a la informacion
} Nodol2l;


// Definicion de la clase del manejador de la lista doblemente ligada
class Lista2ligada: public Cadenas
{

  protected:
     Nodol2l      *Nodo_Primero;
     Nodol2l      *Nodo_Actual;
     Nodol2l      *Nodo_Ultimo;
     Nodol2l      *Nodo_temporal;
     unsigned int  Total_nodos;
     unsigned int  N_nodo_actual;
     unsigned int  Indice;
     const char   *pt_info;
     char         *C_nula;


                     // Borra nodo temporal asi como la informacion que contiene
     void            Borra_nodo_temporal(void);
                     // Adiciona un nodo en el nodo temporal
     void            Adiciona_nodo_temporal(const char *info);


  public:
                     Lista2ligada(void);
                    ~Lista2ligada();
                     // Adiciona un nodo y su contenido
     void            Adicionar(const unsigned int n_e, const char *info, const int t_p);
                     // Borra el nodo indicado
     void            Borra_nodo(const unsigned int n_e);
                     // Borra todos los nodos de la lista
     void            Borra_todos_nodos(void);
                     // Mueve el nodo a la posicion indicada
     int             Mover_nodo(const unsigned int n_a, const unsigned int n_n);
                     // Busca el numero de nodo indicado
     void            Busca(const unsigned int n_n);
                     // Actualiza datos de la lista doblemente ligada
     void            Actualiza_datos(void);
                     // Retorna un puntero al contenido del nodo
     const char     *Contenido_nodo(const unsigned int n_n);
                     // Cambia el contenido del nodo
     void            Cambia_contenido_nodo(const char *info, const unsigned int n_n);
                     // Retorna el nodo actual
     unsigned int    Retorna_Actual_nodo(void) const
                     {return N_nodo_actual;}
                     // Retorna el total de nodos
     unsigned int    Retorna_total_nodos(void) const
                     {return Total_nodos;}
                     // Retorna el numero de caracteres del nodo
     unsigned int    Retorna_longitud_nodo(const unsigned int n_n);
};


#endif


/*
template <class T>
class Lista2ligada {
  protected:
     // Estructura a la lista doblemente ligada
     struct nodo {
        nodo *Sig;
        nodo *Ant;
        T    *Info;
     };
     typedef  nodo Nodol2l;

     Nodol2l *Nodo_Primero;
     Nodol2l *Nodo_Actual;
     Nodol2l *Nodo_Ultimo;
     Nodol2l *Nodo_temporal;
     long     Total_nodos;
     long     N_nodo_actual;



  public:
              Lista2ligada(void);
             ~Lista2ligada()
              {Borra_todos_nodos();}
     T       *Adicionar(const long n_e, T *xinfo);
     void     Eliminar(const long n_e);
     void     Borra_todos_nodos(void);
     void     Posicionarse_en(const long n_n);
     int      Mover_nodo(const long n_a, const long n_n);
//     long     Busca(Nodol2l *nodo);
     void     Busca(const long n_n);
     void     Actualiza_datos(void);
     T       *Contenido_nodo(const long n_n);
     long     Retorna_total_nodos(void)
              {return Total_nodos;}
};
*/
