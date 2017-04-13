// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_intercambia_var(char *cad1, char *cad2);
  // Intercambia el valor de las cadenas CAD1 y CAD2

extern "C" {
   #include <string.h>
}

void s_intercambia_var(char *cad1, char *cad2)
{
   char *xcad = new char[strlen(cad1) +1];
   strcpy(xcad,cad1);
   strcpy(cad1,cad2);
   strcpy(cad2,xcad);
   delete xcad;
}
