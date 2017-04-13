// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int s_trim(const char *cad, char *cadobj);
  // Quita los espacios que existan al inicio y fin de la cadena CAD, retornando la longitud de ella


unsigned int s_trim(const char *cad, char *cadobj)
{
   unsigned int ind = 0, i = 0, pos = 0;
   // Salta los espacios al inicio de la cadena
   while(cad[i] == 32) i++;
   if(!cad[i]) {
      cadobj[0] = 0;
      return 0;
   }
   // Copia el resto de la cadena
   while(cad[i]) {
      cadobj[ind] = cad[i];
      if(cadobj[ind] != 32) pos = ind;
      i++, ind++;
   }
   pos++;
   cadobj[pos] = 0;
   return pos;
}



