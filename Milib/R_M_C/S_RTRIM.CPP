// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

extern "C" {
   #include <string.h>
}

unsigned int s_rtrim(const char *cad, char *cadobj);
  // Quita los espacios que existan al final de la cadena CAD dejandola en la cadena CADOBJ
  // y retornando la longitud de ella

///////////////////////////REVISAR////////////////////////////////

unsigned int s_rtrim(const char *cad, char *cadobj)
{
   cadobj[0] = 0;
   unsigned int i = strlen(cad);
   if(!i) return 0;
   i--;
   // Copia la cadena original a una temporal
   strcpy(cadobj,cad);
   // Quita los espacios al final de la cadena
   for(; i > 0; i--) {
       if(cadobj[i] != 32) break;
   }
   cadobj[i+1] = 0;
   return (i+1);
}




