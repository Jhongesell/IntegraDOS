// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_ini_cad(char *cad, const char car, const unsigned int lg);
  // Inicializa la cadena CAD con el caracter CAR de L_G longitud
  // Nombre del archivo: S_INICAD.CPP


void s_ini_cad(char *cad, const char car, const unsigned int lg)
{
  for(unsigned int i = 0; i < lg; i++) *cad = car, cad++;
  *cad = 0;
}

