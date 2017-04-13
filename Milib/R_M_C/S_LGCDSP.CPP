// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int Longitud_cadena_sin_esp_fin(const char *cad);
  // Retorna la longitud de la cadena sin los espacios al final de esta




unsigned int Longitud_cadena_sin_esp_fin(const char *cad)
{
   unsigned int i = 0, pos = 0;
   const char *ptr = cad;
   while(*ptr) {
      if(*ptr != 32) pos = i;
      ptr++,i++;
   }
   if(!pos) {
      // Si la cadena es nula o solo cuenta con un caracter 
      if(!*cad || *cad == 32) return 0;
   }
   return (pos+1);
}
