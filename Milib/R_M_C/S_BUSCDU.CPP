// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

////////////////////////
// No se a revisado
////////////////////////
extern "C" {
   #include <string.h>
}

int s_bus_ult_cad(const char *cad, const char *x_cad, const unsigned int pos);
  // Busca en la cadena CAD los caracteres de la cadena X_CAD, retornando la
  // posici¢n donde inicia la ultima ocurrencia, retornando -1 si no la encuentra



int s_bus_ult_cad(const char *cad, const char *x_cad, const unsigned int pos)
{
   int in = 0, i = strlen(cad) - 1, n_c = strlen(x_cad) - 1;
   if(i > pos) i = pos;
   while(i >= 0) {
      if(cad[i] == x_cad[n_c]) {
         for(in = 0; in < n_c + 1; in++) {
            if(cad[i - in] != x_cad[n_c - in]) {
               in = 0;
               break;
            }
         }
         if(in) return i - n_c;
      }
      i--;
   }
   return -1;
}

