// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void s_formato(const char *xcad, const unsigned int n_e, const unsigned int n_d, const unsigned int t_p, char *cadobj);
  // Formatea una cadena numerica CAD con comas con N_E enteros y N_D decimales,
  // dejandola en la variable CADOBJ, la cadena resultante puede ser de mayor longitud
  // Si T_P es (0) no se combierten las comas en apostrofes en los miles
  //           (1) si se combierten las comas en apostrofes en los miles


#include "cad_car.hpp"
#include "..\libreria\cadenas.hpp"
extern "C" {
  #include <string.h>
}



void s_formato(const char *xcad, const unsigned int n_e, const unsigned int n_d, const unsigned int t_p, char *cadobj)
{
  int i = 0, ind = 0, x_ind = 0, neg = 0;
  unsigned int p_c = 0, lg;
  char s_for[81], cad[81];
  Cadenas s;
  cadobj[0] = '0', cadobj[1]=0;
  lg = s_trim(xcad,cad);
  if(!s_es_digito(cad) || !lg) return;

  // Quita el signo de menos de la cadena
  if(s.Busca_caracter(cad,'-',p_c)) cad[p_c] = ' ', neg = 1;

  // Quita los espacios de la cadena
  s_trim(cad);
  // Invierte la cadena
  strrev(cad);
  // Busca el punto decimal
  if(s.Busca_caracter(cad,'.',p_c)) while(i <= p_c) s_for[i] = cad[i], i++;
  ind = i;
  while(i < strlen(cad)) {
     if(x_ind > 2) x_ind = 0, s_for[ind] = '\'', ind ++; 
     s_for[ind] = cad[i];
     ind++, i++,x_ind++;
  }
  // Si la cantidad es negativa agrega el signo menos a la cadena
  if(neg) s_for[ind++] = '-';
  // Si no contiene enteros agrega un cero antes del punto decimal
  if(!ind) s_for[ind] = '0', ind ++;
  s_for[ind] = 0;

  // Si T_P es distinto de cero, cambia la coma, por apostrofe en los millones
  if(t_p) {
     i = 0;
     for(ind = 0; ind < strlen(s_for); ind++) {
	      if(s_for[ind] == '\'' && !i) s_for[ind] = ',', i = 1;
     }
  }

  // Se ajusta la longitud de la cadena
  ind = n_e + n_d + 1;
  if(!n_d) ind --;
  x_ind = strlen(s_for);
  if(x_ind < ind) {
     for( i = x_ind; i < ind; i++) s_for[i] = ' ';
     s_for[ind] = 0;
  }

  // Invierte la cadena
  strrev(s_for);
  strcpy(cadobj,s_for);
}
