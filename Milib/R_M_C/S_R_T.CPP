// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_r_t(const char *cad, const unsigned int lg, const char car, char *cadobj);
  // Reyena o trunca la cadena CAD a una determinada longitud LG reyenando
  // con el caracter CAR por la derecha


void s_r_t(const char *cad, const unsigned int lg, const char car, char *cadobj)
{
   unsigned int i = 0, xi = 0;
   while(*cad) {
      if(i >= lg) break;
      *cadobj = *cad, i++, cad++, cadobj++;
   }
   for(xi = i; xi < lg; xi++) *cadobj = car, cadobj++;
   *cadobj = 0;
}





