// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_nocad(const char *cad, const char *cadb, char *cadobj);
  // Elimina de la cadena CAD los caracteres de la cadena X_CAD, retornandola en la cadena CADOBJ
  // Nombre del archivo: S_NOCAD.CPP




void s_nocad(const char *cad, const char *cadb, char *cadobj)
{
   const char *pcadb;
   unsigned int i = 0, st = 0, in;
   while(*cad) {
      if(*cad == *cadb) {
         pcadb = cadb;
         i = 0; st = 1;
         while(*pcadb) {
            if(*(cad+i) != *(pcadb+i)) st = 0;
            i++;
         }
         if(st) {
            for(in = 0; in < i; in++) cad++;
          } else {
            for(in = 0; in < i; in++) {
               *cadobj = *cad;
               cadobj++, cad++;
            }
         }
      } else cad++;
   }
   *cadobj = 0;
}
