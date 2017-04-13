// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



////////////////////////
// No se a revisado
////////////////////////
void s_acom_esp_cad(const char *cad, char *xcad, const unsigned int lg);
  // Acomoda la cadena CAD en la cadena XCAD en rangos de LG longitud, si una palabra cae en
  // dicho rango es recorrida al sigiente reyenando esa porcion con espacios

extern "C" {
   #include <string.h>
}



void s_acom_esp_cad(const char *cad, char *xcad, const unsigned int lg)
{
  int xlg = strlen(cad);
  if(xlg <= lg) strcpy(xcad,cad);
   else {
     int xpos = 0, i = 0, xi = 0, pos = 0, sw = 0;
     while(xlg >= pos) {
       if(xlg >= pos + lg) {
          i = lg;
          while(cad[i] != ' ' && i > 0) i--;
       } else i = xlg - pos, sw = 1;
       if(!i) {
          for(xi = 0; xi < lg; xi++) xcad[xpos + xi] = cad[xi + pos];
          pos += lg;
        } else {
          for(xi = 0; xi < i; xi++)  xcad[xpos + xi] = cad[xi + pos];
          for(xi = i; xi < lg; xi++) xcad[xpos + xi] = ' ';
          pos += i;
       }
       xpos += lg;
       if(sw) break;
     }
     xcad[xpos] = 0;
  }
}
