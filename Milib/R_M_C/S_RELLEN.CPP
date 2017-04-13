// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A




void s_rellena(const char *cad, const char car, const unsigned int lg, char *xcad);
  // Rellena la cadena CAD con el caracter CAR hasta una longitud de L_G,
  // Si la cadena es mas grande no la modifica, retornando un puntero a ella




void s_rellena(const char *cad, const char car, const unsigned int lg, char *xcad)
{
   unsigned int i = 0;
   // Copia la cadena a una cadena temporal
   while(cad[i]) xcad[i] = cad[i], i++;
   if(i < lg) {
      // Reyena la cadena con el caracter indicado a la longitud solicitada
      for(; i < lg; i++) xcad[i] = car;
   }
   xcad[i] = 0;
}

