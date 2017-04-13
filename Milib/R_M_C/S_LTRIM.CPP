// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

unsigned int s_ltrim(const char *cad, char *cadobj);
  // Quita los espacios que existan al final de la cadena CAD dejandola en CADOBJ
  // y retornando la longitud de ella


unsigned int s_ltrim(const char *cad, char *cadobj)
{
  unsigned int i = 0;
  // Salta los espacios al inicio de la cadena
  while(*cad == ' ') cad++;
  // Copia el resto de la cadena
  while(*cad) {
     *cadobj = *cad;
     cadobj++, cad++, i++;
  }
  cadobj = 0;
  return i;
}


