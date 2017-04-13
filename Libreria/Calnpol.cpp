// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A




///////////////////////////////////////////////////////////////
// Optimizar para no manejar cadenas
// Usar tipo de conversion para funciones trigonometricas
// Aceptar el operador ^ para potencias
// Poner memorias de 1 a la 10
///////////////////////////////////////////////////////////////


#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <math.h>
   #include <stdio.h>
   #include <string.h>
#ifdef __PROG_DOS__
}
#endif
#include "calnpol.hpp"
#include "pila.hpp"
#pragma hdrstop





// Rutina que recibe un indicador de operacion asi como un numero determinado de operandos para realizar la operacion indicada
long double Calculadora_notacion_polaca::Calcula(const unsigned int operacion, const long double op1, const long double op2)
{
   long double res, xres1;
   // operaciones basicas
   switch (operacion) {
     case 1:
        res = op1 * op2;
        break;
     case 2:
        if (op2 != 0.0) res = op1 / op2;
         else res = 0.0, Error_matematico = 1;
        break;
     case 3:
        res = op1 + op2;
        break;
     case 4:
        res = op1 - op2;
        break;
     // Funciones trigonometricas
     case 30:   // Seno
        res = sinl(op1);
        break;
     case 31:   // Coseno
        res = cosl(op1);
        break;
     case 32:   // Tangente
        res = tanl(op1);
        break;
     case 33:   // Arco seno
        res = asinl(op1);
        break;
     case 34:   // Arco coseno
        res = acosl(op1);
        break;
     case 35:   // Arco tangente
        res = atanl(op1);
        break;
     case 36:   // Seno hiperbolico
        res = sinhl(op1);
        break;
     case 37:   // Coseno hiperbolico
        res = coshl(op1);
        break;
     case 38:   // Tangente hiperbolico
        res = tanhl(op1);
        break;
     // Otras funciones
     case 39:   // Valor absoluto
        res = fabsl(op1);
        break;
     case 40:   // Exponencia e a la x
        res = expl(op1);
        break;
     case 41:   // Logaritmo natural
        res = logl(op1);
        break;
     case 42:   // Logaritmo base 10
        res = log10l(op1);
        break;
     case 43:   // Raiz cuadrada de X
        res = sqrtl(op1);
        break;
     case 44:  // Cuadrado de X
        res = op1 * op1;
        break;
     case 45:   // Calcula X  por 2 exponente a la Y
        res = ldexpl(op1,op2);
        break;
     case 46:   // X ala Y
        res = powl(op1,op2);
        break;
     case 47:  // 10 a la X
        res = pow10l( (int) op1 );
        break;
     case 50:   // Calcula el modulo de X/Y
        res = fmodl(op1,op2);
        break;
     case 51:   // Factorial de x
        res = 1;
        {
          for(int i = 2; i <= op1; i++) res *= i;
        }
        break;
     case 52:  // Inverso de x
        if (op2 != 0.0) res = op1 / op2;
         else res = 0.0, Error_matematico = 1;
        break;
     case 60:  // Signo de X (1 si X > 0, -1 si X < 0 y 0 si X es cero)
        res =  op1 >= 0.0 ? (op1 == 0.0 ? 0.0 : 1.0) : -1.0;
        break;
     case 61:  // El mayor entero menor o igual a X
        res = floorl(op1);
        break;
     case 62:  // Parte entera de X
        xres1 = modfl(op1,&res);
        break;
     case 63:  // Parte decimal de X
        res = modfl(op1,&xres1);
        break;
     case 70:  // Maximo entre X e Y
        res = op1 >= op2 ? op1 : op2;
        break;
     case 71:  // Minimo entre X e Y
        res = op1 <= op2 ? op1 : op2;
        break;
   }
   return res;
}


// Rutina que calcula el valor numerico de una expresion en notacion polaca con separador de operandos el pipe
long double Calculadora_notacion_polaca::Evalua(const char *cadena)
{
   Error_matematico = 0;
   if (!cadena[0]) return 0;
   // Indica el numero maximo de elementos en la pila
   int i = 0, xi = 0, tp = 0;
   long double res, op1, op2;
   char xcad[100];
   xcad[0] = 0;

   PILA pila(20,NUMERICA);
   while (cadena[i+1]) {
      // Almacena los caracteres dentro de la cadena temporal
      if (cadena[i] == ',') i += 2;
      if (cadena[i] != '|') xcad[xi] = cadena[i], xi++, xcad[xi] = 0;
       else {
         xi = tp = 0, op1 = 0.0, op2 = 0.0;

//////////////////////////////////////////////////////////////////////////////
//         // Asume que es una variable
//         if (Es_caracter_alfabetico(xcad[0]) && strlen(xcad) < 3) goto Etiqueta1;
//////////////////////////////////////////////////////////////////////////////
         
         // Descarto la coma como separador de parametros
         if (Compara(xcad,","))  {
             xcad[0] = 0;
             goto Etiqueta2;
         }

         // Revisa si es un numero
         if(Es_caracter_digito(xcad[0])) goto Etiqueta1;

         // Revisa si es una constante 
         if(xcad[0] == 'M') {
           // Sustitucion de valores en memoria y constantes
           if (Compara(xcad,"M01"))              tp = 101, res = MEM[0];
            else if (Compara(xcad,"M02"))        tp = 101, res = MEM[1];
            else if (Compara(xcad,"M03"))        tp = 101, res = MEM[2];
            else if (Compara(xcad,"M04"))        tp = 101, res = MEM[3];
            else if (Compara(xcad,"M05"))        tp = 101, res = MEM[4];
            else if (Compara(xcad,"M06"))        tp = 101, res = MEM[5];
            else if (Compara(xcad,"M07"))        tp = 101, res = MEM[6];
            else if (Compara(xcad,"M08"))        tp = 101, res = MEM[7];
            else if (Compara(xcad,"M09"))        tp = 101, res = MEM[8];
            else if (Compara(xcad,"M10"))        tp = 101, res = MEM[9];
            else if (Compara(xcad,"M_PI"))       tp = 100, res = M_PI;
            else if (Compara(xcad,"M_PI_2"))     tp = 100, res = M_PI_2;
            else if (Compara(xcad,"M_PI_4"))     tp = 100, res = M_PI_4;
            else if (Compara(xcad,"M_1_PI"))     tp = 100, res = M_1_PI;
            else if (Compara(xcad,"M_2_PI"))     tp = 100, res = M_2_PI;
            else if (Compara(xcad,"M_1_SQRTPI")) tp = 100, res = M_1_SQRTPI;
            else if (Compara(xcad,"M_2_SQRTPI")) tp = 100, res = M_2_SQRTPI;
            else if (Compara(xcad,"M_E"))        tp = 100, res = M_E;
            else if (Compara(xcad,"M_LOG2E"))    tp = 100, res = M_LOG2E;
            else if (Compara(xcad,"M_LOG10E"))   tp = 100, res = M_LOG10E;
            else if (Compara(xcad,"M_LN2"))      tp = 100, res = M_LN2;
            else if (Compara(xcad,"M_LN10"))     tp = 100, res = M_LN10;
           if (tp) {
              pila.Meter(res);
              xcad[0] = 0;
              tp = 0;
              goto Etiqueta2;
           }
         }


         // Funciones de operaciones basicas
         if ((xcad[0] == '+' || xcad[0] == '-' || xcad[0] == '*' || xcad[0] == '/') && xcad[1] == 0) {
            pila.Sacar(op2);
            pila.Sacar(op1);
            switch (xcad[0]) {
              case '*': tp = 1; break; // Producto
              case '/': tp = 2; break; // Division
              case '+': tp = 3; break; // Suma
              case '-': tp = 4; break; // Resta
            }
            res = Calcula(tp,op1,op2);
            pila.Meter(res);
            xcad[0] = 0;
            tp = 0;
            goto Etiqueta2;
         }
         

         // Funciones que requieren un solo parametro
         if (Compara(xcad,"sin"))   tp = 30;  // Seno
          else if (Compara(xcad,"cos"))   tp = 31;  // Coseno
          else if (Compara(xcad,"tan"))   tp = 32;  // Tangente
          else if (Compara(xcad,"asin"))  tp = 33;  // Arco seno
          else if (Compara(xcad,"acos"))  tp = 34;  // Arco coseno
          else if (Compara(xcad,"atan"))  tp = 35;  // Arco tangente
          else if (Compara(xcad,"sinh"))  tp = 36;  // Seno hiperbolico
          else if (Compara(xcad,"cosh"))  tp = 37;  // Coseno hiperbolico
          else if (Compara(xcad,"tanh"))  tp = 38;  // Tangente hiperbolico
          else if (Compara(xcad,"abs"))   tp = 39;  // Valor absoluto
          else if (Compara(xcad,"exp"))   tp = 40;  // Exponencia e a la X
          else if (Compara(xcad,"log"))   tp = 41;  // Logaritmo natural
          else if (Compara(xcad,"log10")) tp = 42;  // Logaritmo base 10
          else if (Compara(xcad,"sqrt"))  tp = 43;  // Raiz cuadrada
          else if (Compara(xcad,"sqr"))   tp = 44;  // Cuadrado de X
          else if (Compara(xcad,"fact"))  tp = 51;  // Factorial de X
          else if (Compara(xcad,"inv"))   tp = 52;  // Inverso de X
          else if (Compara(xcad,"sig"))   tp = 60;  // Signo de X (1 si X > 0, -1 si X < 0 y 0 si X es cero)
          else if (Compara(xcad,"floor")) tp = 61;  // El mayor entero menor o igual a X
          else if (Compara(xcad,"trc"))   tp = 62;  // Parte entera de X
          else if (Compara(xcad,"dec"))   tp = 63;  // Parde decimal de X
          else if (Compara(xcad,"pow10")) tp = 47;  // X por 10 a la Y
         if (tp) {
            pila.Sacar(op1);
            res = Calcula(tp,op1,op2);
            pila.Meter(res);
            xcad[0] = 0;
            tp = 0;
            goto Etiqueta2;
         }

         // Funciones que requieren dos parametros
         if (Compara(xcad,"fmod"))  tp = 50;  // Calcula el modulo de X/Y
          else if (Compara(xcad,"ldexp")) tp = 45;  // Calcula X  por 2 exponente a la Y
          else if (Compara(xcad,"pow"))   tp = 46;  // X a la Y
          else if (Compara(xcad,"max"))   tp = 70;  // Maximo entre X e Y
          else if (Compara(xcad,"min"))   tp = 71;  // Minimo entre X e Y
         if (tp) {
            pila.Sacar(op2);
            pila.Sacar(op1);
            res = Calcula(tp,op1,op2);
            pila.Meter(res);
            xcad[0] = 0;
            tp = 0;
            goto Etiqueta2;
         }

//////////////////////////////////////////////////////////////////////////////
//         // Funciones que no requieren parametros
//         if (Compara(xcad,"gra"))  Tipo_conversion = 1;  // Funciones trigonometricas en gradianes
//         if (Compara(xcad,"rad"))  Tipo_conversion = 2;  // Funciones trigonometricas en radianes
//         if (Compara(xcad,"deg"))  Tipo_conversion = 3;  // Funciones trigonometricas en grados
//////////////////////////////////////////////////////////////////////////////


      Etiqueta1:

         // Introduce el valor en la pila
         if (xcad[0]) {
            res = _atold(xcad);
            pila.Meter(res);
            xcad[0] = 0;
         }

         // Signo igual termina el proceso
         if (cadena[i+1] == '=') break;

      Etiqueta2:
      }
      i++;
      // Aborta el calculo por error numerico
      if (Error_matematico) return 0.0;
   }
   pila.Sacar(res);
   return res;
}

// Asigna un valor a el elemento de la memoria indicado
int Calculadora_notacion_polaca::Asigna_valor(const unsigned int elem, const long double val)
{
   int st = 0;
   if (elem <= NUM_MAX_ELEM_MEMORIA) MEM[elem] = val;
    else st = 1;
   return st;
}

// retorna el valor del elemento de la memoria indicado
int Calculadora_notacion_polaca::Retorna_valor(const unsigned int elem, long double &val)
{
   int st = 0;
   val = 0.0;
   if (elem <= NUM_MAX_ELEM_MEMORIA) val = MEM[elem];
    else st = 1;
   return st;
}

// Inicializa la memoria 
void Calculadora_notacion_polaca::Inicializa_valor(void)
{
   for (unsigned int i = 0; i < NUM_MAX_ELEM_MEMORIA; i++) MEM[i] = 0.0;
}


