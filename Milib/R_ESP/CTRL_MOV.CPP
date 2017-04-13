// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A.1



#include "ctrl_mov.hpp"
#include "teclado.hpp"

extern "C" {
   #include <ctype.h>
   #include <string.h>
}


// Costructor del control de moviemiento
Controlador_movimiento::Controlador_movimiento(void)
{
  Cadena_busqueda = NULL;
  Cadena_descartar = NULL;
  Inicia_paneo = Actual_paneo = Actual_elemento = 1;
  Movimiento_ciclico = 0;
  Tipo_paneo = 1;
}


// Destructor del control de moviemiento
Controlador_movimiento::~Controlador_movimiento()
{
  delete []Cadena_busqueda;
  delete []Cadena_descartar;
}



// Paremetros de la clase controlador
void Controlador_movimiento::Parametros(const unsigned int t_e, const unsigned int e_p, const char *c_b, const char *c_d)
{
   Total_elementos = t_e, Elementos_paneo = e_p;
   Cadena_busqueda   = new char[strlen(c_b)+1];
   Cadena_descartar = new char[strlen(c_d)+1];
   strcpy(Cadena_busqueda,c_b);
   strcpy(Cadena_descartar,c_d);
}



// Busca el caracter en la cadena de busqueda y descarte
int Controlador_movimiento::Busca_caracter(const char car, unsigned int &pos)
{
   pos = 0;
   // Busca por el caracter tecleado
   if(car) {
      unsigned int  i = 0;
      while(Cadena_descartar[i]) {
         if(toupper(car) == Cadena_descartar[i]) return 0; // Caracter descartado
         i++;
      }
      i = 0;
      while(Cadena_busqueda[i]) {
         if(toupper(car) == Cadena_busqueda[i]) {
            pos = i + 1;   // Posicion donde se encontro
            return 1;      // Caracter encontrado
         }
         i++;
      }
   }
   return 0;  // Caracter no encontrado
}



// Controlador horizontal
// Retorna el elemento seleccionado por enter o la tecla
unsigned int Controlador_movimiento::Horizontal(const int tecla, const char car)
{
   unsigned int Estado = 0;
   if(Cadena_busqueda[0]) Busca_caracter(car,Estado);
   // Revisa la accion indicada desde el teclado
   switch(tecla) {
     case ENTER: // Elemento seleccionado
       Estado = Actual_elemento;
       break;
     case HOME:// Se posiciona al inicio
       Actual_elemento = Actual_paneo = Inicia_paneo = 1;
       break;
     case END: // Se posiciona al final
       // Se posiciona al final
       Fija_elemento_actual(Total_elementos);
       break;
     case FLECHA_DERECHA:  // Siguiente elemento
       if(Actual_elemento < Total_elementos) {
          // Se posiciona en el siguiente elemento
          Actual_elemento++;
          // Actualiza el paneo
          if(Actual_paneo < Elementos_paneo) Actual_paneo ++;
           else {
             // Revisa el paneo
             if(Inicia_paneo < Total_elementos-Elementos_paneo) Inicia_paneo++;
          }
        } else {
          // Revisa el tipo de movimiento
          if(Movimiento_ciclico) Actual_elemento = Actual_paneo = Inicia_paneo = 1; // Se posiciona al inicio
       }
       break;
     case FLECHA_IZQUIERDA: // Anterior elemento
       if(Actual_elemento > 1) {
          // Se posiciona en el siguiente elemento
          Actual_elemento--;
          // Actualiza el paneo
          if(Actual_paneo > 1) Actual_paneo --;
           else {
            // Revisa el paneo
            if(Inicia_paneo > 1) Inicia_paneo--;
          }
        } else {
          // Revisa el tipo de movimiento
          if(Movimiento_ciclico) Fija_elemento_actual(Total_elementos);  // Se posiciona al final
       }
   }
   return Estado;
}



// Controlador vertical
// Retorna el elemento seleccionado por enter o la tecla
unsigned int Controlador_movimiento::Vertical(const int tecla, const char car)
{
   int xtecla = tecla;
   unsigned int Estado = 0;
   if(Cadena_busqueda[0]) Busca_caracter(car,Estado);
   // Revisa la accion indicada desde el teclado
   switch(xtecla) {
     case ENTER: // Elemento seleccionado
       Estado = Actual_elemento;
       break;
     case CTRL_PGUP: // Inicio
       Actual_elemento = Actual_paneo = Inicia_paneo =1;
       break;
     case CTRL_PGDN: // Final
       Fija_elemento_actual(Total_elementos);
       break;
     case CTRL_HOME: // Posicionarse al inicio del paneo
       Actual_paneo = 1, Actual_elemento = Inicia_paneo;
       break;
     case CTRL_END: // Posicionarse al final del paneo
       if(Inicia_paneo + Elementos_paneo < Total_elementos) {
          Actual_elemento = Inicia_paneo + (Elementos_paneo-1);
          Actual_paneo = Elementos_paneo;
        } else {
          Actual_elemento = Inicia_paneo + (Total_elementos - Inicia_paneo);
          Actual_paneo = Total_elementos - Inicia_paneo + 1;
       }
       break;
     case FLECHA_ABAJO:  // Anterior elemento
       if(Actual_elemento < Total_elementos) {
          // Se posiciona en el siguiente elemento
          Actual_elemento++;
          // Actualiza el paneo
          if(Actual_paneo < Elementos_paneo) Actual_paneo ++;
           else {
             // Revisa el paneo
             if(Inicia_paneo+Elementos_paneo <= Total_elementos) Inicia_paneo++;
          }
        } else {
          // Revisa el tipo de movimiento
          if(Movimiento_ciclico) Actual_elemento = Actual_paneo = Inicia_paneo =1; // Se posiciona al inicio
       }
       break;
     case FLECHA_ARRIBA:  // Siguiente elemeto
       if(Actual_elemento > 1) {
          // Se posiciona en el siguiente elemento
          Actual_elemento--;
          // Actualiza el paneo
          if(Actual_paneo > 1) Actual_paneo --;
           else {
            // Revisa el paneo
            if(Inicia_paneo > 1) Inicia_paneo--;
          }
        } else {
          // Revisa el tipo de movimiento
          if(Movimiento_ciclico) Fija_elemento_actual(Total_elementos);  // Se posiciona al final
       }
       break;
     case PGDN: // Pagina arriba
       if(Inicia_paneo > Elementos_paneo) {
          // Retrocede una pagina
          Actual_elemento -= Elementos_paneo;
          Inicia_paneo -= Elementos_paneo;
        } else {
          // Revisa el tipo de movimiento
          if(Movimiento_ciclico) Fija_elemento_actual(Total_elementos);  // Se posiciona al final
           else Actual_elemento = Actual_paneo = Inicia_paneo =1;
       }
       break;
     case PGUP: // Pagina abajo
       // Revisa si es posible el avanze de pagina
       if(Actual_elemento+Elementos_paneo >= Total_elementos-Elementos_paneo) {
          // Revisa el tipo de movimiento
          if(Movimiento_ciclico) Actual_elemento = Actual_paneo = Inicia_paneo =1;  // Se posiciona al inicio
           else Fija_elemento_actual(Total_elementos);     // Se posiciona al final
        } else {
          // Avanza una pagina
          Actual_elemento += Elementos_paneo;
          Inicia_paneo += Elementos_paneo;
       }
   }
   return Estado;
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


// Controlador horizontal
int Controlador_movimiento::Horizontal(const int tecla, const char *text)
{
   int Estado = 0;
   // Revisa la accion indicada desde el teclado
   switch(tecla) {
     case HOME:// Se posiciona al inicio
       Actual_elemento = Actual_paneo = Inicia_paneo =1;
       break;
     case END: // Se posiciona al final  de la cadenal (ultimo caracter distinto de espacio)
       Actual_elemento = Lg_cadena_sin_espacios_final(text);
       Fija_elemento_actual(Actual_elemento+1);
       break;
     case FLECHA_DERECHA:  // Siguiente elemento
       if(Actual_elemento < Total_elementos) {
          // Se posiciona en el siguiente elemento
          Actual_elemento++;
          // Actualiza el paneo
          if(Actual_paneo < Elementos_paneo) Actual_paneo ++;
           else {
             // Revisa el paneo
             if(Inicia_paneo < Total_elementos-Elementos_paneo) Inicia_paneo++;
          }
       } else {
          Actual_elemento = Total_elementos;
          Fija_elemento_actual(Actual_elemento+1);
       }
       break;
     case CTRL_FLECHA_DERECHA:  // Siguiente palabra
       Actual_elemento = Posicion_palabra_siguiente(text,Actual_elemento-1,0) + 1;
       // Revisa si es necesario pasar a la siguiente Linea
       if(!Actual_elemento) {
          Actual_elemento = 1;
          Estado = FLECHA_ABAJO__HOME;
        } else {
          // Actualiza el paneo
          if(Actual_elemento < Elementos_paneo) Actual_paneo = Actual_elemento, Inicia_paneo = 1;
           else {
             Inicia_paneo = Actual_elemento - (Elementos_paneo - 5);
             Actual_paneo = (Actual_elemento - Inicia_paneo) +1;
          }
       }
       break;
     case FLECHA_IZQUIERDA:    // Anterior elemento
       if(Actual_elemento > 1) {
          // Se posiciona en el siguiente elemento
          Actual_elemento--;
          // Actualiza el paneo
          if(Actual_paneo > 1) Actual_paneo --;
           else {
            // Revisa el paneo
            if(Inicia_paneo > 1) Inicia_paneo--;
          }
       }
       break;
     case CTRL_FLECHA_IZQUIERDA:  // Anterior palabra
       Actual_elemento = Posicion_palabra_anterior(text,Actual_elemento-1,0) + 1;
       if(!Actual_elemento) Actual_elemento = 1, Estado = FLECHA_ARRIBA__END; // se posiciona al final de la anterior linea
        else {
          // Actualiza el paneo
          if(Actual_elemento < Elementos_paneo) Actual_paneo = Actual_elemento, Inicia_paneo = 1;
           else {
             Inicia_paneo = Actual_elemento - (Elementos_paneo - 5);
             Actual_paneo = (Actual_elemento - Inicia_paneo) +1;
          }
       }
       break;
     case TAB:
       if(Actual_elemento + TABULADOR < Total_elementos) {
          // Se posiciona en el siguiente elemento
          Actual_elemento += TABULADOR;
          // Actualiza el paneo
          if(Actual_paneo + TABULADOR < Elementos_paneo) Actual_paneo += TABULADOR;
           else {
             // Revisa el paneo
             if(Inicia_paneo + TABULADOR < Total_elementos-Elementos_paneo) Inicia_paneo += TABULADOR;
          }
       } else {
          Actual_elemento = Lg_cadena_sin_espacios_final(text);
          Fija_elemento_actual(Actual_elemento+1);
       }
       break;
     case SHIFT_TAB:
       if(Actual_elemento > TABULADOR) {
          // Se posiciona en el siguiente elemento
          Actual_elemento-= TABULADOR;
          // Actualiza el paneo
          if(Actual_paneo > TABULADOR) Actual_paneo -= TABULADOR;
           else {
            // Revisa el paneo
            if(Inicia_paneo > TABULADOR) Inicia_paneo-= TABULADOR;
             else Actual_elemento = Actual_paneo = Inicia_paneo = 1;
          }
       } else Actual_elemento = Actual_paneo = Inicia_paneo = 1;
       break;
   }
   return Estado;
}


// Controlador vertical
void Controlador_movimiento::Vertical(const int tecla)
{
   int xtecla = tecla;
   // Revisa la accion indicada desde el teclado
   switch(xtecla) {
     case CTRL_PGUP: // Inicio
       Actual_elemento = Actual_paneo = Inicia_paneo = 1;
       break;
     case CTRL_PGDN: // Final
       Fija_elemento_actual(Total_elementos);
       break;
     case CTRL_HOME: // Posicionarse al inicio del paneo
       Actual_paneo = 1, Actual_elemento = Inicia_paneo;
       break;
     case CTRL_END: // Posicionarse al final del paneo
       if(Inicia_paneo + Elementos_paneo < Total_elementos) {
          Actual_elemento = Inicia_paneo + (Elementos_paneo-1);
          Actual_paneo = Elementos_paneo;
        } else {
          Actual_elemento = Inicia_paneo + (Total_elementos - Inicia_paneo);
          Actual_paneo = Total_elementos - Inicia_paneo + 1;
       }
       break;
     case FLECHA_ABAJO:  // Anterior elemento
       if(Actual_elemento < Total_elementos) {
          // Se posiciona en el siguiente elemento
          Actual_elemento++;
          // Actualiza el paneo
          if(Actual_paneo < Elementos_paneo) Actual_paneo ++;
           else {
             // Revisa el paneo
             if(Inicia_paneo+Elementos_paneo <= Total_elementos) Inicia_paneo++;
          }
       }
       break;
     case FLECHA_ARRIBA:  // Siguiente elemeto
       if(Actual_elemento > 1) {
          // Se posiciona en el siguiente elemento
          Actual_elemento--;
          // Actualiza el paneo
          if(Actual_paneo > 1) Actual_paneo --;
           else {
            // Revisa el paneo
            if(Inicia_paneo > 1) Inicia_paneo--;
          }
       }
       break;
     case PGDN: // Pagina arriba
       if(Inicia_paneo > Elementos_paneo) {
          // Retrocede una pagina
          Actual_elemento -= Elementos_paneo;
          Inicia_paneo -= Elementos_paneo;
       } else Actual_elemento = Actual_paneo = Inicia_paneo =1;
       break;
     case PGUP: // Pagina abajo
       // Revisa si es posible el avanse de pagina
       if(Actual_elemento+Elementos_paneo >= Total_elementos-Elementos_paneo) {
          // Se posiciona al final
          Fija_elemento_actual(Total_elementos);
        } else {
          // Avanza una pagina
          Actual_elemento += Elementos_paneo;
          Inicia_paneo += Elementos_paneo;
       }
       break;
   }
}



// Fija el elemento actual
void Controlador_movimiento::Fija_elemento_actual(const unsigned int actual)
{
   if(actual <= 1) Actual_elemento = Actual_paneo = Inicia_paneo = 1;
   if(actual >= Total_elementos) {
      Actual_elemento = Total_elementos;
      if(Total_elementos > Elementos_paneo) Actual_paneo = Elementos_paneo, Inicia_paneo = Total_elementos - Elementos_paneo + 1;
       else Actual_paneo = Total_elementos, Inicia_paneo = 1;
   }
   if(actual > 1 && actual < Total_elementos) {
      Actual_elemento = actual;
      if(Actual_elemento > Elementos_paneo) {
         if(Tipo_paneo) {
            // Paneo para todas las funciones exepto el menu
            Inicia_paneo = Actual_elemento - Elementos_paneo + 1;
            Actual_paneo = Elementos_paneo;
         } else Actual_paneo = Actual_elemento - Inicia_paneo; // Paneo para el menu
         Actual_paneo = Actual_elemento - Inicia_paneo; 
      } else Actual_paneo = Actual_elemento, Inicia_paneo = 1;
   }
}

