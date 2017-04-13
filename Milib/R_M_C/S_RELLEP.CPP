// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_rellena(char *cad, const char car, const unsigned int lg);
  // Rellena la cadena CAD con el caracter CAR hasta una longitud de L_G
  // Si esta es mas grande no la modifica

extern "C" {
   #include <string.h>
}

void s_rellena(char *cad, const char car, const unsigned int lg)
{
  unsigned int n_c = strlen(cad);
  if(n_c < lg) {
     for(; n_c < lg ; n_c ++) cad[n_c] = car;
     cad[lg] = 0;
  }
}

