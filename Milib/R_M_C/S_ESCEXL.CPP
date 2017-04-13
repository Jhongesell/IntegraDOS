// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int s_es_car_extleng(const char car);
  // Si el caracter CAR es del rango 0x80 a 0xA7 raegresa (1)  [Caracteres de extenci¢n del lenguaje]
  // en caso contrario regresa (0)
  // Nombre del archivo: S_ESCEXL.CPP


int s_es_car_extleng(const char car)
{
  if(car > 127 && car < 169) return 1;
  return 0;
}
