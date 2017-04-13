// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

/*
#include "cad_car.hpp"
extern "C" {
   #include <string.h>
   #include <dir.h>
}   


// Retorna un nombre de archivo con extenci¢n EXT
void cambia_ext_path(const char *path, const char *ext, char *pathmod)
{
   char xpath[MAXPATH];
   int lg = s_trim(path,xpath) -1;
   while(lg) {
     if(xpath[lg] != 46) {
        if(xpath[lg] == 92 || xpath[lg] == 58) lg = 0;
         else lg --;
     } else xpath[lg] = 0, lg = 0;
   }
   strcat(xpath,ext);
   strcpy(pathmod,xpath);
}
*/

