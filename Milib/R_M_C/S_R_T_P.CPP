// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_r_t(char *cad, const unsigned int lg, const char car);
  // Reyena o trunca una cadena CAD a una determinada longitud LG reyenando
  // con el caracter CAR por la derecha, retornando un puntero a ella



void s_r_t(char *cad, const unsigned int lg, const char car)
{
   unsigned int i = 0, xi = 0;
   while(*cad) {
      if(i >= lg) break;
      i++, cad ++;
   }
   for(xi = i; xi < lg; xi++) *cad = car, cad++;
   *cad = 0;
}
