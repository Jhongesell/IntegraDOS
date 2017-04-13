// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int Valida_graba_sobreescribe(const char *arch);
   // Revisa si existe el archivo preguntando si se sobreescribe este
   // Retorna 1 si se puede grabar y 0 en caso contrario


#include "r_var.hpp"
#include "nucleo.hpp"

extern "C" {
   #include <dos.h>
}

// Definicion externa del objeto Nucleo
extern Nucleo *NCO;


#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "El archivo ya existe, desea sobreescribirlo"
#else
   #define TXT1 "File already exist. Save anyway?"
#endif



int Valida_graba_sobreescribe(const char *arch)
{
   if(n_archivos_directorios(arch,FA_ARCH)) {
      if(NCO->Opcion(TXT1,arch) != 1) return 0;
   }
   return 1;
}