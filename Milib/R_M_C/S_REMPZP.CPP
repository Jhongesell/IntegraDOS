// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

void s_remplaza(const char *cad, const char car, const char xcar, char *xcad);
  // Remplaza de la cadena CAD el caracter CAR por XCAR, dejandola en XCAD


void s_remplaza(char *cad, const char car, const char xcar, char *xcad)
{
   while(*cad) {
     *xcad = *cad;
     if(*cad == car) *xcad = xcar;
     cad++, xcad++;
   }
}

