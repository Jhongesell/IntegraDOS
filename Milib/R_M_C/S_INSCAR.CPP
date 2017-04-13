// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_ins_car(char *cad, const unsigned int pos, const char car);
  // Inserta en la cadena CAD en la posicion POS el caracter CAR sin alterar longitud


void s_ins_car(char *cad, const unsigned int pos, const char car)
{
  unsigned int i = 0;
  char xcar = car ,ycar;
  while(*cad) {
     if(i >= pos) ycar = *cad, *cad = xcar, xcar = ycar;
     cad++, i++;
  }
}

