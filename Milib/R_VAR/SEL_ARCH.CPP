// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "s_arch.hpp"
#include "..\libreria\man_arch.hpp"
extern "C" {
  #include <string.h>
  #include <dir.h>
}

// Retorna un nombre de archivo con la extencion dada, este archivo el
// usuario lo selecciona de la ventana de archivos mostrada
// Si TP es (0) No es necesario que el archivo exista
//          (1) Es necesario que el archivo exista
// Retorna  (0) Si no se realizo la seleccion
//          (1) Si se realizo la seleccion
int Retorna_archivo_seleccionado(char *arch, const unsigned int tp, const char *tit, const char *ext, const char *path)
{
   int sw = 0;
   char cad1[MAXPATH];
   Manipulador_archivos ma;
   Selecciona_muestra_archivos *Sa = new Selecciona_muestra_archivos;
   Sa->Parametros(path,tit);
   sw = Sa->Ciclo(tp);
   if(sw) {
      Sa->Retorna_archivo(cad1);
      if(ext[0]) ma.Cambia_ext_path(cad1,ext,arch);
       else strcpy(arch,cad1);
   }
   delete Sa;
   return sw;
}


