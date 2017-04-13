// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_repetir(char *x_cad, const char *cad, const unsigned int num);
  // Repite la cadena CAD el numero de veces indicada en NUM dejandola en x_cad
  // Nombre del archivo: S_REPET.CPP

extern "C" {
   #include <string.h>
}


void s_repetir(char *x_cad, const char *cad, const unsigned int num)
{
  x_cad[0] = 0;
  unsigned int i;
  for(i = 0; i < num ; i++) strcat(x_cad,cad);
}

