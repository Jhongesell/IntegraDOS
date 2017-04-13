// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_ini_num(char *cad, const unsigned int l_g);
  // Inicializa una cadena CAD con un cero a la derecha de longitud N_C
  // Nombre del archivo: S_ININUM.CPP



void s_ini_num(char *cad, const unsigned int l_g)
{
  for(int i = 0; i < l_g; i++) cad[i] = 32;
  cad[l_g - 1] = '0'; 
  cad[l_g] = 0;
}

