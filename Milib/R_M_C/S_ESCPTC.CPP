// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int s_es_car_punt(const char car);
  // Si el caracter CAR es ­"#$%&`()*+'-./:;<>?@=\[]^_{}|~¨  raegresa (1) [Caracteres de puntuaci¢n]
  // en caso contrario regresa (0)


int s_es_car_punt(const char car)
{
  if((car >= 33 && car <= 47) || (car >= 58 && car <= 64) || (car >= 91 && car <= 96) || (car >= 123 && car <= 126) || car == 168) return 1;
  return 0;
}
