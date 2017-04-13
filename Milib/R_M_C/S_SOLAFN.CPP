// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// TelÇfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr†mite
// Revisi¢n  1.1-A


void s_solo_car_alfnum(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres alfanumericos de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean alfanumericos
  //           (5) Extrae s¢lo los primeros caracteres que no sean alfanumericos
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean alfanumericos
  // Acepta los siguientes caracteres como alfanumericos [ABCDEFGHIJKLMNOPQRSTUVWXYZZabcdefghijklmnopqrstuvwxyz†ÖÇäç°¢¢£óÅ§• -_0123456789.+]
  // Nombre del archivo: S_SOLAFN.CPP


#include "cad_car.hpp"
extern "C" {
   #include <string.h>
}

/*
void s_solo_car_alfnum(const char *cad, const unsigned int t_p, char *xcad)
{
   int i = 0, xi = 0, CTE_TRB = 1;
   if(t_p > 3) CTE_TRB = 0;
   // Extrae todos los caracteres alfabeticos de la cadena
   if(t_p == 1 || t_p == 4) {
      while(cad[i]) {
	 if(s_es_car_alfnum(cad[i]) == CTE_TRB) {
	    xcad[xi] = cad[i];
	    xi ++;
	 }
	 i++;
      }
      xcad[xi] = 0;
   }

   // Extrae solo los carcateres alfabeticos al principio de la cadena
   if(t_p == 2 || t_p == 5) {
      while(cad[i] && s_es_car_alfnum(cad[i]) == CTE_TRB) {
	 xcad[xi] = cad[i];
	 xi ++;
	 i++;
      }
      xcad[xi] = 0;
   }

   // Retorna solo los carcateres alfabeticos al final de la cadena
   if(t_p == 3 || t_p == 6) {
      i = strlen(cad) - 1;
      while(i >= 0 && s_es_car_alfnum(cad[i]) == CTE_TRB) {
	 xcad[xi] = cad[i];
	 xi ++;
	 i--;
      }
      xcad[xi] = 0;
      strrev(xcad);
   }

   xcad[xi] = 0;
}
*/
