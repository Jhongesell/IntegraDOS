// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int s_es_car_may(const char car);
  // Rutina que retorna verdadero si el caracter es letra mayoscula o ¥
  // en caso contrario retorna (0)
  // Soporta caracteres como  …‚Š¡¢•£—¤¥


int s_es_car_may(const char car)
{
  if((car > 64 && car < 91) || car == 165) return 1;
  return 0;
}

