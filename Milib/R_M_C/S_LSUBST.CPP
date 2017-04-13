// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int s_lsubstr(const char *cad, const unsigned int n_c, char *cadobj);
  // Extrae de la cadena los primeros N_C caracteres, dejandola en CADOBJ


unsigned int s_lsubstr(const char *cad, const unsigned int n_c, char *cadobj)
{
  unsigned int i = 0;
  while(*cad) {
     if(i >= n_c) break;
     *cadobj = *cad;
     cadobj++, cad++, i++;
  }
  *cadobj = 0;
  return i;
}


