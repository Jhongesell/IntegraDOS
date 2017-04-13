// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int s_es_car_min(const char car);
  // Rutina que retorna verdadero si el caracter es letra minuscula o  …‚Š¡¢•£—¤
  // en caso contrario retorna (0)
  // Soporta caracteres como  …‚Š¡¢•£—¤¥


int s_es_car_min(const char car)
{
  if((car > 96 && car < 123) || car == 160 || car == 133 || car == 130 || car == 138 || car == 141 || car == 161 || car == 162 || car == 149 || car == 163 || car == 151 || car == 129 || car == 164) return 1;
  return 0;
}

