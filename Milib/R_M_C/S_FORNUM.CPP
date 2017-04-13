// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_for_num(const char *xcad, const unsigned int n_e, const unsigned int n_d, char *s_for);
  // Rutina que ajusta la cadena numerica a una determinada longitud N_E + N_D + 1
  // con numero de decimales N_D y maximo numero de enteros N_E
  // Nota: La cadena queda cargada a la izquierda

#include "cad_car.hpp"
#include "..\libreria\cadenas.hpp"
extern "C" {
   #include <string.h>
}


void s_for_num(const char *xcad, const unsigned int n_e, const unsigned int n_d, char *s_for)
{
   char cad[81], d_cad[21], e_cad[41];
   int i = 0, ind = 0, neg = 0, lg = 0;
   unsigned int p_c = 0;
   Cadenas s;
   
   cad[0] = d_cad[0] = e_cad[0] = 0;
   s_for[0] = '0',s_for[1] = 0;
   if(xcad[0] == 0) return;
   lg = s_trim(xcad,cad);
   if(!s_es_digito(cad) || n_e > 40 || n_d > 20) return;

   // se quita el signo (+) de la cadena
   if(s.Busca_caracter(cad,'+',p_c)) cad[p_c] = ' ';
   // Quita de la cadena el caracter de (-), para ser agregado despues del formateo
   if(s.Busca_caracter(cad,'-',p_c)) cad[p_c] = ' ', neg = 1;

   // Quita de la cadena ceros no significativos
   for(i = 0; i < lg; i++) {
       if(cad[i] == '0') cad[i] = ' ';
        else break;
   }
   lg = s_trim(cad);

   // Separaci¢n de decimales
   s.Busca_caracter(cad,'.',p_c);
   if(n_d) {
      if(p_c) {
         for(i = p_c; i < lg; i++, ind ++) d_cad[ind] = cad[i];
      } else d_cad[ind] = '.', ind ++;
      d_cad[ind] = 0;
      // Formateo de decimales
      for(i = ind; i < n_d + 1; i++) d_cad[i] = '0';
      d_cad[n_d + 1] = 0;
   }

   // Separaci¢n de enteros
   if(!p_c) p_c = lg;
   for(i = 0; i < p_c; i++) e_cad[i] = cad[i];
   if(!i) e_cad[1] = '0', i++;
   e_cad[i] = 0, ind = 0;
   // Inserta el signo negativo si existe en la cadena
   if(neg) s_for[0] = '-', ind++;
   // Agregar los enteros a la cadena de fomateo
   for(i = 0; i < strlen(e_cad); i++, ind++) s_for[ind] = e_cad[i];
   // Inserci¢n de los decimales en la cadena de formateo
   for(i = 0; i < strlen(d_cad); i++, ind++) s_for[ind] = d_cad[i];
   s_for[ind] = 0;
   // Se ajusta la longitud de la cadena
   ind = n_e + n_d + 1;
   if(!n_d) ind --;
   for( i = strlen(s_for); i < ind; i++) s_for[i] = ' ';
   s_for[i] = 0;
}
