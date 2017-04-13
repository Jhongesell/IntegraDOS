// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.0-A

/*
#include "man_arch.hpp"
#define LG_BUFF 1024

#ifdef _IDIOMA_ESPANOL_
   static const char *RUTINA = "COPIA ARCHIVO";
#else
   static const char *RUTINA = "COPY FILE";
#endif


void Copia_archivo(const char *archivo_fuente, const char *archivo_objeto)
{
   int lg = 0;
   char *buff = new char[LG_BUFF];
   Manipulador_archivos *Arc_lect = new Manipulador_archivos;
   Manipulador_archivos *Arc_grab = new Manipulador_archivos;
   Arc_lect->Parametros(archivo_fuente,LEE_ARCHIVO,LG_BUFF,RUTINA);
   Arc_grab->Parametros(archivo_objeto,GRABA_ARCHIVO,LG_BUFF,RUTINA);
   while(Arc_lect->Retorna_estado() != 2) {
      lg = (int) Arc_lect->Lee_caracteres(buff,LG_BUFF);
      Arc_grab->Graba_caracteres(buff, lg);
   }
   delete []buff;
   delete Arc_lect;
   delete Arc_grab;
}

*/
