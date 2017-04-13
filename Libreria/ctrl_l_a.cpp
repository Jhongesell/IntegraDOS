// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#include "d:\Libreria\ctrl_l_a.hpp"
#pragma hdrstop


void Ctrl_lista_archivos::Graba_lista_archivos(void)
{
   Manipulador_archivos ma;
   ma.Parametros(Nombre_lista, GRABA_ARCHIVO, MAXPATH, "");
   if (!ma.Retorna_estado()) {
      for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) {
         if(Arreglo_nombres[i][0]) ma.Graba_linea(Arreglo_nombres[i]);
      }
   }
}
              
void Ctrl_lista_archivos::Lee_lista_archivos(void)
{
   Inicializa_arreglo();
   Manipulador_archivos ma;
   ma.Parametros(Nombre_lista, LEE_ARCHIVO, MAXPATH, "");
   if (!ma.Retorna_estado()) {
     int i = 0;
     char xarch[MAXPATH];
     while (ma.Retorna_estado() != 2) {
        ma.Lee_linea(xarch);
        strcpy(Arreglo_nombres[i],xarch);
        i++;
        if (i >= NUM_MAX_ARCHIVOS) break;
     }
   } 
}
      
// Inserta un nombre al inicio de la lista
void Ctrl_lista_archivos::Inserta_nombre(const char *arch)
{
   for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) {
       if (Compara_ignorando_minusculas(arch, Arreglo_nombres[i]))  Arreglo_nombres[i][0] = 0;
   }
   // Despalza los nombres al siguiente indice
   for (int i = (NUM_MAX_ARCHIVOS - 1); i > 0; i--) {
    strcpy(Arreglo_nombres[i], Arreglo_nombres[i-1]);
   }
   // Copia el nuevo indice
   strcpy(Arreglo_nombres[0], arch);
   // Graba la lista de archivos
   Graba_lista_archivos();
   // Lee la nueva lista omitiendo los nulos
   Lee_lista_archivos();
}


// Retorna el nombre indicado de la lista
void Ctrl_lista_archivos::Retorna_nombre(const int i, char *arch, bool &st)
{ 
   if ( i < NUM_MAX_ARCHIVOS) strcpy(arch,Arreglo_nombres[i]);
   if ( arch[0]) st = true;
    else st = false;
}

              // Indica si existe almenos un elemento en la lista
bool Ctrl_lista_archivos::Existe_almenos_uno(void)
{
   bool st = false;
   for (int i = 0; i < NUM_MAX_ARCHIVOS; i++) {
      if(Arreglo_nombres[i][0]) st = true;
   }
   return st;
}        

