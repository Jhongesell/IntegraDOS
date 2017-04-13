// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



int s_es_car_ctrl(const char car);
  // Si el caracter CAR es del rango 0x01 a 0x1F raegresa (1) [Caracteres de control]
  // en caso contrario regresa (0)


int s_es_car_ctrl(const char car)
{
  if(car < 32) return 1;
  return 0;
}
