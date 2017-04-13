// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



int s_es_espacios(const char *cad);
  // Rutina que retorna verdadero si solo contiene caracteres espacios en caso contrario retorna (0)


int s_es_espacios(const char *cad)
{
   while(*cad) {
      if(*cad != ' ') return 0;
      cad++;
   }
   return 1;
}