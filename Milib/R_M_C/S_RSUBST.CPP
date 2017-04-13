// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

extern "C" {
   #include <string.h>
}

unsigned int s_rsubstr(const char *cad, const unsigned int n_c, char *cadobj);
  // Extrae de la cadena los £ltimos N_C de la cadena, dejandola el CADOBJ



unsigned int s_rsubstr(const char *cad, const unsigned int n_c, char *cadobj)
{
   unsigned int i = strlen(cad), xi = 0, xxi;
   if(i >= n_c) {
      xxi = i - n_c;
      while(cad[xxi]) cadobj[xi] = cad[xxi], xi++ , xxi++;
      cadobj[xi] = 0;
   } else strcpy(cadobj,cad);
   return strlen(cadobj);
}





