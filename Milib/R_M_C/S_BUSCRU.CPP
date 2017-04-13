// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int s_bus_ult_car(const char *cad, const char car, unsigned int &pos);
  // Indica la posici¢n en que fue encontrado el ultimo caracter CAR en la cadena CAD,
  // inicia en pos, retorna -1 si no lo encuentra


int s_bus_ult_car(const char *cad, const char car, unsigned int &pos)
{
   unsigned int i = 0, sw = 0;
   while(*cad) {
      if(*cad == car) pos = i, sw = 1;
      cad++, i++;
   }
   return sw;
}

