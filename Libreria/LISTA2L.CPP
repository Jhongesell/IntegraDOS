// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#include "lista2l.hpp"
//extern "C" {
   #include <stdlib.h>
   #include <string.h>
//}



// Constructor de listas doblemente ligada
Lista2ligada::Lista2ligada(void)
{
   // Inicializa los datos
   Nodo_Actual = Nodo_Ultimo = Nodo_Primero = NULL;
   Total_nodos = N_nodo_actual = 0;
   C_nula = "";
}


Lista2ligada::~Lista2ligada()
{
   Borra_todos_nodos();
}


// Destructor de listas doblemente ligada
void Lista2ligada::Borra_todos_nodos(void)
{
   if (!Nodo_Primero) return;
   // Se posiciona al final de la lista
   Nodo_Actual = Nodo_Primero;
   // Borra cada uno de los nodos de la lista
   while (Nodo_Actual) {
      Nodo_temporal = Nodo_Actual;
      Nodo_Actual = Nodo_temporal -> Sig;
      Borra_nodo_temporal();
   }
   Nodo_Actual = Nodo_Ultimo = Nodo_Primero = NULL;
   Total_nodos = N_nodo_actual = 0;
}



// Adiciona un nodo en la posicion N_E a la lista
// Si t_p es
//    (1)  Se adiciona al inicio de la lista
//    (2)  Se adiciona al final de la lista
void Lista2ligada::Adicionar(const unsigned int n_e, const char *info, const int t_p)
{
   unsigned int x_n_e = n_e;
   if (t_p) x_n_e = 0;
   // La lista esta vacia
   if (Nodo_Primero == NULL) {
      Adiciona_nodo_temporal(info);
      Nodo_temporal -> Sig  = NULL;
      Nodo_temporal -> Ant  = NULL;
      Nodo_Actual = Nodo_Ultimo = Nodo_Primero = Nodo_temporal;
      N_nodo_actual = Total_nodos = 1;
      return;
   }
   // Inserta al inicio de la lista
   if (x_n_e == 1 || t_p == 1) {
      Adiciona_nodo_temporal(info);
      Nodo_temporal -> Sig  = Nodo_Primero;
      Nodo_temporal -> Ant  = NULL;
      Nodo_Primero  -> Ant  = Nodo_temporal;
      Nodo_Actual = Nodo_Primero = Nodo_temporal;
      N_nodo_actual = 1;
      return;
   }
   // Inserta al final de la lista
   if (x_n_e > Total_nodos || t_p == 2) {
      Adiciona_nodo_temporal(info);
      Nodo_temporal -> Sig  = NULL;
      Nodo_temporal -> Ant  = Nodo_Ultimo;
      Nodo_Ultimo   -> Sig  = Nodo_temporal;
      Nodo_Actual = Nodo_Ultimo = Nodo_temporal;
      N_nodo_actual = Total_nodos;
      return;
   }
   // Inserta en el nodo N_E de la lista
   if (x_n_e > 1 && n_e <= Total_nodos) {
      Busca(n_e);
      Adiciona_nodo_temporal(info);
      Nodo_temporal -> Sig = Nodo_Actual;
      Nodo_temporal -> Ant = Nodo_Actual -> Ant;
      Nodo_Actual -> Ant -> Sig = Nodo_temporal;
      Nodo_Actual -> Ant = Nodo_temporal;
      Nodo_Actual = Nodo_temporal;
   }
}


// Adiciona un nodo en el nodo temporal
void Lista2ligada::Adiciona_nodo_temporal(const char *info)
{
   Indice = Lg_cadena_sin_espacios_final(info);
   Nodo_temporal = new Nodol2l;
   if (Indice) {
      Nodo_temporal->Info = new char[Indice+1];
      Substr(info,0,Indice,Nodo_temporal->Info);
   } else Nodo_temporal->Info = C_nula;
   Total_nodos ++;
}


// Rutina que borra al nodo numero N_N de la lista
void Lista2ligada::Borra_nodo (const unsigned int n_n)
{
   // Revisa si la lista esta vacia
   if (!Nodo_Primero) return;
   // Revisa si la lista tiene un solo elemento
   if (Nodo_Primero == Nodo_Ultimo) {
      Nodo_temporal = Nodo_Primero;
      Nodo_Actual = Nodo_Primero = Nodo_Ultimo = NULL;
      N_nodo_actual = 0;
      Borra_nodo_temporal();
      return;
   }
   // Borra el primer nodo
   if (n_n <= 1) {
      Nodo_temporal = Nodo_Primero;
      Nodo_Primero  = Nodo_Primero -> Sig;
      Nodo_Primero -> Ant = NULL;
      Nodo_Actual   = Nodo_Primero;
      N_nodo_actual = 1;
      Borra_nodo_temporal();
      return;
   }
   // Borra el ultimo nodo
   if (n_n >= Total_nodos) {
      Nodo_temporal = Nodo_Ultimo;
      Nodo_Ultimo   = Nodo_Ultimo -> Ant;
      Nodo_Ultimo -> Sig = NULL;
      Nodo_Actual   = Nodo_Ultimo;
      Borra_nodo_temporal();
      N_nodo_actual = Total_nodos;
      return;
   }
   // Borra el nodo indicado en N_N
   if (n_n > 1 && n_n < Total_nodos) {
      Busca(n_n);
      Nodo_temporal = Nodo_Actual;
      Nodo_Actual -> Ant -> Sig = Nodo_Actual -> Sig;
      Nodo_Actual -> Sig -> Ant = Nodo_Actual -> Ant;
      Nodo_Actual = Nodo_Actual -> Sig;
      Borra_nodo_temporal();
      return;
   }
}

// Borra nodo temporal asi como la informacion que contiene
void Lista2ligada::Borra_nodo_temporal(void)
{
   if (Nodo_temporal) {
      if (Nodo_temporal->Info[0]) delete [](Nodo_temporal->Info);
      delete Nodo_temporal;
      Total_nodos --;
   }
}



// Rutina que Busca un nodo por el numero nodo
void Lista2ligada::Busca(const unsigned int n_n)
{
// Funciona el codigo, se quito para optimizar en tama¤o de codigo
//   if (n_n <= (Total_nodos / 2)) {
      Nodo_Actual = Nodo_Primero;
      N_nodo_actual = 1;
      while (Nodo_Actual->Sig) {
         if (N_nodo_actual == n_n) break;
         if (Nodo_Actual -> Sig) N_nodo_actual ++, Nodo_Actual = Nodo_Actual -> Sig;
          else break;
      }
//    } else {
//      Nodo_Actual = Nodo_Ultimo;
//      N_nodo_actual = Total_nodos;
//      while (Nodo_Actual->Ant) {
//         if (N_nodo_actual == n_n) break;
//         if (Nodo_Actual -> Ant) Nodo_Actual = Nodo_Actual -> Ant, N_nodo_actual --;
//          else break;
//      }
//   }
}





// Rutina que mueve el nodo numero N_A a la poscion N_O, retornando no cero en caso de existir un error
int Lista2ligada::Mover_nodo (const unsigned int n_a, const unsigned int n_o)
{
  // Valida la concordancia del movimiento
  if (n_a < 1 || n_a > Total_nodos || n_o < 1 || n_o > Total_nodos) return -1;
  // Si el nodo a mover y el destino son los mismos no hace nada
  if (n_a == n_o) return 0;
///////////////////////////
// Falta
///////////////////////////
  return 0;
}



// Rutina que Actualiza Los datos de la lista doblemente ligada
void Lista2ligada::Actualiza_datos(void)
{
   Total_nodos = 0;
   Nodo_Ultimo = Nodo_Primero;
   while (Nodo_Ultimo) {
      Total_nodos ++;
      if (!Nodo_Ultimo -> Sig) break;
      Nodo_Ultimo = Nodo_Ultimo -> Sig;
   }
}



// Retorna un puntero al contenido del nodo
const char *Lista2ligada::Contenido_nodo (const unsigned int n_n)
{
   pt_info = C_nula;
   if (n_n >=1 && n_n <= Total_nodos) {
      Busca(n_n);
      pt_info = Nodo_Actual->Info;
   }
   return pt_info;
}


// Retorna el numero de caracteres del nodo
unsigned int Lista2ligada::Retorna_longitud_nodo (const unsigned int n_n)
{
   if (n_n >=1 && n_n <= Total_nodos) {
      Busca(n_n);
      if (Nodo_Actual->Info[0]) return (strlen(Nodo_Actual->Info));
   }
   return 0;
}


// Optimizar procesos
// Cambia el contenido del nodo indicado
void Lista2ligada::Cambia_contenido_nodo (const char *info, const unsigned int n_n)
{
   if (n_n >=1 && n_n <= Total_nodos) {
      Busca(n_n);
      if (Nodo_Actual->Info[0]) delete [](Nodo_Actual->Info);
      Indice = Lg_cadena_sin_espacios_final(info);
      if (Indice) {
         Nodo_Actual->Info = new char[Indice+1];
         Substr(info,0,Indice,Nodo_Actual->Info);
      } else Nodo_Actual->Info = C_nula;
   }
}



