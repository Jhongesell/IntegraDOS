// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

////////////////////////
// No se a revisado
////////////////////////

int s_bus_todas_cad(const char *cad, const char *x_cad, const unsigned int t_p);
  // Busca en la cadena CAD los caracteres de la cadena X_CAD, retornando la
  // posici¢n donde inicia esta, retornando -1 si no la encuentra
  // Si T_P es (0) Encuentra la primera ocurrencia
  //           (1) Encuentra la siguente ocurrencia (Utiliza variable estatica
  //               para guardar la posici¢n de la ultima ocurrencia)

extern "C" {
   #include <string.h>
}


int s_bus_todas_cad(const char *cad, const char *x_cad, const unsigned int t_p)
{
   static unsigned int i = 0;
   int n_c = 0, in = 0;
   if(!t_p) i = 0;
    else i++;
   n_c = strlen(x_cad);
   while(cad[i]) {
      if(cad[i] == x_cad[0]) {
         for(in = 0; in < n_c; in++) {
             if(cad[i+in] != x_cad[in]) {
	             in = 0;
                break;
             }
	      }
	      if(in) return i;
      }
      i++;
   }
   i = 0;
   return -1;
}
