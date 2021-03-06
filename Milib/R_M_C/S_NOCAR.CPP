// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci�n: Amsterdam 312 col. Hipodromo Condesa
// Tel�fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr�mite
// Revisi�n  1.1-A



void s_nocar(char *cad, const char car, const unsigned int t_p);
  // Quita de la cadena CAD, la(s) ocurrencia(s) del caracter CAR
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s�lo la primera ocurrencia del caracter
  //           (3) extrae s�lo la �ltima ocurrencia del caracter
  // Nombre del archivo: S_SOLCTR.CPP


void s_nocar(char *cad, const char car, const unsigned int t_p)
{
  unsigned int i = 0, xi = 0, sw = 1;
  switch(t_p) {
    case 1:
       while(cad[i]) {
          if(cad[i] != car) cad[xi++] = cad[i];
          i++;
       }
       cad[xi] = 0;
       break;
    case 2:
       while(cad[i]) {
          if(sw) {
             if(cad[i] != car) cad[xi++] = cad[i];
              else sw = 0;
          } else cad[xi++] = cad[i];
          i++;
       }
       cad[xi] = 0;
       break;
  }
}


/*
    case 3:
       // error en algoritmo
       strrev(cad);
       while(*cad) {
          if(sw) {
             if(*cad != car) x_cad[i++]  = *cad;
              else {
                cad++;
                sw = 0;
             }
           } else {
             x_cad[i++] = *cad;
             cad++;
          }
       }
       x_cad[i] = 0;
       strrev(x_cad);
*/

