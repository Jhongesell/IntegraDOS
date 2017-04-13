// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <string.h>
#ifdef __PROG_DOS__
}
#endif
#include "d:\libreria\ClavSeg.hpp"
#pragma hdrstop


// Constructor de la clase
Clave_seguridad::Clave_seguridad(void)
{
   Encriptada[0] = 0;
   Desencriptada[0] = 0;
   Clave[0] = 0;
}


// Encripta la clave
const char *Clave_seguridad::Encripta(const char *cv)
{
   strcpy(Clave,cv);
   Ctrl_proceso(true);
   return (Encriptada);
}

// Desencripta la clave
const char *Clave_seguridad::Desencripta(const char *cv)
{
   strcpy(Clave,cv);
   Ctrl_proceso(false);
   return (Desencriptada);
}

// Controla el proceso de encriptacion y desencriptacion
void Clave_seguridad::Ctrl_proceso(const bool st)
{
   char i = 0;
   if (st) {
      // Encripta
      strcpy(Encriptada,Clave);
      strrev(Encriptada);
      while (Encriptada[i]) {
         Encriptada[i] = Convierte_may_min(Encriptada[i],1) + i + 2;
         i++;
      }
    } else {
      // Desencripta
      strcpy(Desencriptada,Clave);
      while (Desencriptada[i]) {
         Desencriptada[i] = Convierte_may_min(Desencriptada[i] - i - 2,1);
         i++;
      }
      strrev(Desencriptada);
   }
}



/*
// Para probar clave de acceso
   Clave_seguridad cs;
   MessageBox(Handle,cs.Encripta("cectdt"),TXT01,MB_OK);
   MessageBox(Handle,cs.Desencripta(cs.Encripta("cectdt")),TXT01,MB_OK);

*/
