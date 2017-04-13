// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A




   // Arreglo que contiene el nombre de las constantes usas y su valor numerico correspondiente
   char *Constantes[] = {
      "M_PI",         "3.14159265358979",  // Pi
      "M_PI_2",       "3.14159265358979",  // Pi/2
      "M_PI_4",       "3.14159265358979",  // Pi/4
      "M_1_PI",       "3.14159265358979",  // 1/Pi
      "M_2_PI",       "3.14159265358979",  // 2/Pi
      "M_1_SQRTPI",   "3.14159265358979",  // 1/sqrt(Pi)
      "M_2_SQRTPI",   "3.14159265358979",  // 2/sqrt(Pi)
      "M_E",          "2.71828182845904",  // e
      "M_LOG2E",      "2.71828182845904",  // log(e)
      "M_LOG10E",     "2.71828182845904",  // log10(e)
      "M_LN2",        "2.71828182845904",  // ln(2)
      "M_LN10",       "2.71828182845904",  // ln(10)
      "",""
   };


   // Arreglo que contiene el nombre de las funciones permitidas y su numero de parametros
   char *Funciones[] = {
      "sin",   "1",                    // seno
      "cos",   "1",                    // coseno
      "tan",   "1",                    // tangente
      "asin",  "1",                    // arco seno
      "acos",  "1",                    // arco coseno
      "atan",  "1",                    // arco tangente
      "sinh",  "1",                    // seno hiperbolico
      "cosh",  "1",                    // coseno hiperbolico
      "tanh",  "1",                    // tangente hiperbolico
      "fabs",  "1",                    // valor absoluto
      "floor", "1",                    // El mayor entero menor o igual a
      "fmod",  "2",                    // Calcula el modulo de X/Y
      "exp",   "1",                    // exponencia e a la x
      "ldexp", "2",                    // Calcula X  por 2 exponente a la Y
      "log",   "1",                    // logaritmo natural
      "log10", "1",                    // logaritmo base 10
      "sqrt",  "1",                    // raiz cuadrada
      "pow",   "2",                    // x a la y
      "pow10", "1",                    // 10 a la X
//////////////////////////////////////////////////////////////////////////////////
// Funciones agregadas para el evaluador de expresiones no son propias de       //
// Borland.                                                                     //
//                                                                              //
      "fact",  "1",                    // Factorial de x
      "inv",   "1",                    // Inverso de x
      "sig",   "1",                    // Signo de X (1 si X > 0, -1 si X < 0 y 0 si X es cero)
      "sqr",   "1",                    // Cuadrado de X
      "trc",   "1",                    // Parte entera de X
      "dec",   "1",                    // Parte decimal de X
      "max",   "2",                    // Maximo entre X e Y
      "min",   "2",                    // Minimo entre X e Y
//      "gra",   "0",                    // funciones trigonometricas en gradianes
//      "rad",   "0",                    // funciones trigonometricas en radianes
//      "deg",   "0",                    // funciones trigonometricas en grados
      "F",     "1",                    // Se usara para funciones auxiliares unicamente
//                                                                               //
/////////////////////////////////////////////////////////////////////////////////
      "",""
   };
