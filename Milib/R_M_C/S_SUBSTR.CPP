// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int s_substr(const char *cad, const unsigned  int p_i, const unsigned int n_c, char *cadobj);
// Extrae de la cadena CAD el numero de caracteres indicados en N_C a
// partir de la posicion P_I partiendo de cero, retornando un puntero a ella


unsigned int s_substr(const char *cad, const unsigned int p_i, const unsigned int n_c, char *cadobj)
{
   unsigned int i = 0, xi = 0;
   // Lee toda la cadena
   while(*cad) {
      // extrae la porci¢n indicada de esta
      if(i >= p_i) {
         if(xi >= n_c) break;
         *cadobj = *cad;
         cadobj++,xi++;
      }
      cad++,i++;
   }
   *cadobj = 0;
   return xi;
}


