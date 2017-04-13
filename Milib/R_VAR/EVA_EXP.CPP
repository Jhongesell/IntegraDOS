// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#include "evaexpc.hpp"
#include "vis_ayud.hpp"
extern "C" {
   #include <string.h>
   #include <stdio.h>
}


#ifdef _IDIOMA_ESPANOL_
   #define TXT1 "AVISO";
#else
   #define TXT1 "MESSAGE";
#endif


// Evalua una expresion EXP regresando el valor numerico de esta y la cadena con dicho valor en RES
long double Evalua_expresion(const char *exp, char *res)
{
   long double nres;
   char *exp_fin = new char[strlen(exp)+10];
   // Acomoda en expresion una variable de asignacion
   sprintf(exp_fin,"x=%s",exp);
   
   Evalua_expresion_cadena eec;
   nres = eec.Evalua_expresion(exp_fin,res);
   if(eec.Error) {
      // visualiza los errores encontrados dentro de la expresion
      Vis_archivo *Ve = new Vis_archivo;
      Ve->Parametros(" ","TXT1",350,300,VISUALIZA);
      // Visualiza los errores en el analizador
      Ve->Adiciona_linea(exp);
      Ve->Adiciona_linea(" ");
      for(int i = 0; i < eec.Ev->Numero_errores(); i++) Ve->Adiciona_linea(eec.Ev->Retorna_error(i));
      Ve->Loop();
      delete Ve;
   }
   return nres;
}

