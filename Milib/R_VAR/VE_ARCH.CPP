// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "vis_ayud.hpp"
#include "r_esp.hpp"

extern "C" {
   #include <string.h>
   #include <dir.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT2     "VISUALIZA ARCHIVO"
   #define TXT3     "EDITA ARCHIVO"
#else
   #define TXT2     "VIEW FILE"
   #define TXT3     "EDIT FILE"
#endif


// Edita o visualiza un archivo de nombre ARCH, con titulo en la ventana se TIT
// Si TP_EDIC es (EDITA) edita el archivo
//               (VISUZALIZA) visualiza el archivo
// SI Selecc es  (1) Presenta una ventana para seleecionar el archivo
//               (0) Usa el archivo ARCH
void V_E_archivo(const char *arch, const char *tit,const int tp_edic, const int selecc)
{
   int sw = 1;
   char archivo[MAXPATH];
   char *TXT;
   strcpy(archivo,arch);
   if(selecc) {
      if(tp_edic == EDITA) TXT = TXT3;
       else TXT = TXT2;
      sw = Retorna_archivo_seleccionado(archivo,0,TXT,"TXT",arch); // No hace cambio de extencion
   }
   if(sw) {
      Vis_archivo *Ve = new Vis_archivo;
      Ve->Parametros(archivo,tit,500,400,tp_edic);
      Ve->Loop();
      delete Ve;
   }
}
