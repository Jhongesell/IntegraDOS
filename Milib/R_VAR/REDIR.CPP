// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int Redireccina(const int opc, const char *arch);
  // Redirecciona la entrada, salida, error, auxiliar e impresion estandar al archivo ARCH retornando -1 si hay algun error
  // (1) Entrada   standar al archivo
  // (2) Salida    standar al archivo
  // (3) Error     standar al archivo
  // (4) Auxiliar  standar al archivo
  // (5) Impresora standar al archivo
  // (6) Archivo a la entrada standar

  extern "C" {
   #include <stdio.h>
}

int Redireccina(const int opc, const char *arch)
{
   if(opc == 1) {
      if(!freopen(arch, "a", stdin )) return -1;
   }
   if(opc == 2) {
      if(!freopen(arch, "a", stdout)) return -1;
   }
   if(opc == 3) {
      if(!freopen(arch, "a", stderr )) return -1;
   }
   if(opc == 4) {
      if(!freopen(arch, "a", stdaux )) return -1;
   }
   if(opc == 5) {
      if(!freopen(arch, "a", stdprn )) return -1;
   }
   if(opc == 6) {
      if(!freopen(arch, "r", stdin )) return -1;
   }
   return -1;
}


