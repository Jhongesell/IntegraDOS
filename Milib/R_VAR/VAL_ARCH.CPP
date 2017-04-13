// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int Valida_nombre_archivo(const char *nom_arch);
   // Valida que el nombre de archivo sea valido en caso contrario indica el
   // tipo de error en el nombre
   // (0) Sin errores
   // (1) Caracteres no validos

#include "cad_car.hpp"
extern "C" {
   #include <dir.h>
}


int Valida_nombre_archivo(const char *nom_arch)
{
   char xarch[MAXPATH], *pc;
   pc = xarch;
   s_trim(nom_arch,xarch);
   while(*pc) {
     if(*pc < 36 || (*pc > 36 && *pc < 45) || *pc == 47 || (*pc > 58 && *pc < 65) || *pc == 91 || (*pc > 92 && *pc < 95) || *pc == 96 || *pc > 122) return 1;
     pc++;
   }
   return 0;
}


