// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A



#ifndef __CTRL_L_A_H__
#define __CTRL_L_A_H__


#include <dir.h>
#include <string.h>
#include "man_arch.hpp"
#include "cadenas.hpp"


#define NUM_MAX_ARCHIVOS 10

class Ctrl_lista_archivos: public Cadenas
{
   private:
      char    Nombre_lista[MAXPATH];
      char    Arreglo_nombres[NUM_MAX_ARCHIVOS][MAXPATH];
       
              // Inicializa el arreglo
      void    Inicializa_arreglo(void)
              { for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) strcpy(Arreglo_nombres[i],""); }
              // Graba la lista de archivos
      void    Graba_lista_archivos(void);
              // Lee la lista de archivos
      void    Lee_lista_archivos(void);
      
   public:

              // Constructor
              Ctrl_lista_archivos(void)
              { Inicializa_arreglo(); }

              // inicializa lista de archivos 
      void    Inicializa_lista_archivos(const char *path)
              { 
                strcpy(Nombre_lista,path);
                Lee_lista_archivos();
              }  
              // Inserta un nombre al inicio de la lista
      void    Inserta_nombre(const char *arch);
              // Retorna el nombre indicado de la lista
      void    Retorna_nombre(const int i, char *arch, bool &st);
              // Indica si existe almenos un elemento en la lista
      bool    Existe_almenos_uno(void);
};

#endif
