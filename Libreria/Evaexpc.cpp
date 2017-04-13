// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <string.h>          
   #include <stdio.h>
#ifdef __PROG_DOS__
}
#endif
#include "evaexpc.hpp"
#include "calnpol.hpp"
#pragma hdrstop





// Evalua una expresion EXP regresando el valor numerico de esta y la cadena con dicho valor en RES
long double Evalua_expresion_cadena::Evalua_expresion(const char *exp, char *res)
{
   exp_not_pol = new char[strlen(exp)+100];
   long double nres = 0;
   Error = 0;
   res[0] = 0;

   // Realiza el analisis sintactico de la expresion
   Ev = new Analizador_sintactico;
   Ev->Crea_arbol(exp);
   if (!Ev->Numero_errores()) {
      // Retorna la cadena en notacion polaca
      Ev->Retorna_expresion(exp_not_pol,POSTORDEN);
      // Obtiene el resultado de la expresion polaca
      Calculadora_notacion_polaca cnp;
      nres = cnp.Evalua(exp_not_pol);
      sprintf(res,"%1.16Lg",nres);
   } else Error = 1;
   delete []exp_not_pol;
   delete Ev;
   return nres;
}
