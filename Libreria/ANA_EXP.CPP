// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Dirección: Amsterdam 312 col. Hipodromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


/*
+  Rutina que valide que antes de un espacio hay (1) y despues de un espacio hay (2)
   letra    esp   simbolo ()*-/+,
   numero   esp   simbolo )*+/-,
   simbolo  esp   letra/numero()
+ Aplicar la rutina anterior para analizar la expresion despues de eliminar espacios inecesarios

*/


#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <string.h>
   #include <stdlib.h>
   #include <stdio.h>

#ifdef __PROG_DOS__
}
#endif
#include "ana_exp.hpp"
#include "a_r_mat.hpp"
#pragma hdrstop



#define LETRA          1
#define NUMERO         2
#define OPERADOR       3
#define AGRUPADOR      4
#define IGUAL          5
#define SEPARADOR      6 

// Indica que se sustituyan las constantes
//#define SUSTITUYE_CONSTANTES


////////////////////////////////////////////////////////////
//
// No permitir 9e*3 o 9e/2 ni 9e  en notacion exponencial
// 
//
//
////////////////////////////////////////////////////////////


// Arreglo que contiene la descripcion de los errores
#ifdef _IDIOMA_ESPANOL_
   const char *Errores_a_s[] = {
      "EXISTEN CARACTERES DESCONOCIDOS",                            // 0
      "NO EXISTE NINGUN SIGNO DE IGUALDAD",                         // 1
      "EXISTE MAS DE UN SIGNO DE IGUALDAD",                         // 2
      "EXISTEN OPERANDOS DENTRO DE LA VARIABLE DE ASISGNACION",     // 3
      "EXISTE UN OPERANDO SIN OPERADOR",                            // 4
      "EXISTE UN OPERADOR SIN OPERANDO",                            // 5
      "ERROR DE SINTAXIS",                                          // 6
      "EL PUNTO DECIMAL SOLO ES APLICABLE A EXPRESIONES NUMERICAS", // 7
      "LA COMA SE USA SOLO PARA SEPARAR PARAMETROS EN LA FUNCION",  // 8
      "",                                                           // 9
      "NO HAY EXPRESION DESPUES DEL SIGNO IGUAL",                   // 10
      "EL NOMBRE DE LA VARIABLE DEBE EMPEZAR CON UNA LETRA",        // 11
      "LA EXPRESION NUMERICA DEBE CONTENER UN SOLO PUNTO",          // 12
      "EXISTE MAS DE UN INDICADOR DE NOTACION EXPONENCIAL",         // 13
      "ERROR EN LA NOTACION EXPONENCIAL",                           // 14
      "PARENTESIS MAL BALANCEADOS",                                 // 15
      "LLAVES MAL BALANCEADOS",                                     // 16
      "PARENTESIS CUADRADOS MAL BALANCEADOS",                       // 17
      "ERROR EN EL NUMERO DE PARAMETROS DE LA FUNCION",             // 18
      "LA FUNCION NO ES VALIDA",                                    // 19
      "EXPRESION NO RECONOCIDA",                                    // 20
      "EXISTEN DEMASIADOS ERRORES EN EL ANALIZADOR",                // 21
      "FALTA PARAMETRO EN LA FUNCION"                               // 22
   };
#else 
   const char *Errores_a_s[] = {
      "THERE ARE UNKNOWN CHARACTERS",                               // 0
      "THERE IS NONE EQUAL SYMBOL",                                 // 1
      "THERE IS MORE THAN ONE EQUAL SYMBOL",                        // 2
      "THERE ARE OPERATIVES WITHIN THE DESIGNED VARIABLE",          // 3
      "THERE IS AN OPERATIVE WITHOUT OPERATOR",                     // 4
      "THERE IS AN OPERATOR WITHOUT OPERATIVE",                     // 5
      "SYNTAX  ERROR",                                              // 6
      "DECIMAL DOT IS ONLY APPLIED TO NUMERICAL EXPRESSIONS",       // 7
      "A COMMA IS ONLY USED TO SEPARATE PARAMETERS IN A FUNCTION",  // 8
      "",                                                           // 9
      "THERE IS NO EXPRESSION AFTER AN EQUAL SYMBOL",               // 10
      "A VARIABLE NAME SHOULD START WITH A LETTER",                 // 11
      "A NUMERICAL EXPRESSION SHOULD HAVE ONLY ONE DOT",            // 12
      "THERE IS MORE THAN ONE EXPONENTIAL NOTATION INDICATOR",      // 13
      "ERROR ON THE EXPONENTIAL NOTATION",                          // 14
      "UNBALANCED PARENTHESIS",                                     // 15
      "UNBALANCED KEYS",                                            // 16
      "UNBALANCED BRACKETS",                                        // 17
      "FUNTION PARAMETERS NUMBER ERROR",                            // 18
      "INVALID FUNCTION",                                           // 19
      "UNKNOWN EXPRESSION",                                         // 20
      "TOO MANY ERRORS IN THE ANALIZER",                            // 21
      "FUNCTION PARAMETER MISSING"                                  // 22
   };
#endif




// Constructor del arbol sintactico
Analizador_sintactico::Analizador_sintactico(void)
{
   // Inicializa el puntero al nodo raiz
   PTR_RAIZ = NULL;
   //Inicializa la variable de estado
   Estado = Num_errores = 0, Num_max_errores = 50;
   //Solicita memoria para los errores
   Errores = new int[Num_max_errores];
   Xexp = Asigna_puntero(400);
}


// Destructor del arbol sintactico
Analizador_sintactico::~Analizador_sintactico()
{
   // Borra todo el arbol
   Destruye_arbol();
   // Borra la estructura que contiene los errores
   delete []Errores;
   delete []Xexp;
}


// Crea el arbol
void Analizador_sintactico::Crea_arbol(const char *exp)
{
   if (!exp[0]) return;
   int i, lg_exp, st;
   Estado = 1;

   // inicializa las variables de control de errores
   for (i = 0; i < Num_max_errores; i++) Errores[i] = 0;
   Num_errores = 0;

   
   //  Valida la expresion tal como biene
   Valida_expresion(exp);
   if (Num_errores)  return;  


   // Revisa apareamiento de agrupadores
   st = Revisa_balance(exp,0); 
   if (st) {
      Errores[Num_errores++] = st+14; 
      return; 
   }


   // Prepara la expresion para el resto de las validaciones
   lg_exp = strlen(exp) + 10; 
   char *expresion = Asigna_puntero(lg_exp); 
   Prepara_expresion_validacion(exp,expresion); 


   // Hace la primera revision
   Valida_expresion_inicial(expresion); 
   if (Num_errores) {
      delete []expresion;
      return;
   }

   // Hace la segunda revision
   Valida_expresion_final(expresion);
   if (Num_errores) {
      delete []expresion;
      return;
   }

   // Solicita la memoria necesaria para el proceso
   Xn_der = Asigna_puntero(lg_exp);
   Xn_izq = Asigna_puntero(lg_exp);
   xexp = Asigna_puntero(lg_exp);


   // Destruye el arbol binario (si existe este)
   Destruye_arbol();
 
   // Analiza la espresion formando un arbol binario
   PTR_RAIZ = Crea_estructura_arbol(expresion);

   // Borra variables temporales
   delete []xexp;
   delete []expresion;
   delete []Xn_der;
   delete []Xn_izq;
}



// Valida la expresion tal como biene
void Analizador_sintactico::Valida_expresion(const char *exp)
{
   // Valida expresio
   const char *xexp = exp;
   while (*xexp) {
      if (*xexp < 32 || *xexp > 126) Errores[Num_errores++] = 0;
      xexp++;
   }
/*
/////////////////////////////////////////////////////////////
// Revisar como hacer correctamente la validacion
////////////////////////////////////////////////////////////
   // Valida los tokens
   char *token, *buff;
   char separ[] = " =";
   buff = strdup(exp);
   token = strtok(buff,separ);
   while (token != NULL) {
      // Analiza token
      ///   token
      ///
      // Obtiene el siguiente token
      token = strtok(NULL,separ);
   }
   free(buff);
////////////////////////////////////////////////////////////
*/
}



// Revisa que esten bien balanceados los parentesis, llaves o corchetes 
// Si TP es (0) Revisa todos 
//          (1) Revisa parentesis 
//          (2) Revisa llaves
//          (3) Revisa corchetes 
// Regresa  (0) Sin errores 
//          (1) Parentesis mal balanceados 
//          (2) Llaves mal balanceados 
//          (3) Corchetes mal balanceados
int Analizador_sintactico::Revisa_balance(const char *exp, const int tp)  
{
   int n_corchetes = 0, n_llaves = 0, n_parentesis = 0, st = 0; 
   int stp = 0, stl = 0, stc = 0;
   while (*exp) {
     switch (*exp) {
        case '[': // Revisa que los corchetes esten apareados
          n_corchetes ++;
          break;
        case ']':
          n_corchetes --;
          break;
        case '{': // Revisa que las llaves esten apareados
          n_llaves ++;
          break;
        case '}':
          n_llaves --;
          break;
        case '(': // Revisa que los parentesis esten apareados
          n_parentesis ++;
          break;
        case ')': // Revisa que los parentesis esten apareados
          n_parentesis --;
          break;
     }
     // Revisa que no exista parentesis, llaves o corchetes de cerradura sin apertura
     if (n_parentesis < 0) stp = 1;
     if (n_llaves < 0)     stl = 2;
     if (n_corchetes < 0)  stc = 3;
     exp++;
   }
   // Revisa que no exista parentesis no parareados
   if (n_parentesis) stp = 1;
   if (n_llaves)     stl = 2;
   if (n_corchetes)  stc = 3;
   
   if (!tp) {
      if (stp) st = stp;
      if (stl) st = stl;
      if (stc) st = stc;
   }
   if (tp == 1 && stp) st = stp;
   if (tp == 2 && stl) st = stl;
   if (tp == 3 && stc) st = stc;

   return st;
}


// Prepara la expresion para el analisis completo
void Analizador_sintactico::Prepara_expresion_validacion(const char *exp, char *xexp)
{
   unsigned int i = 0, xi = 0;
   while (exp[i]) {
      switch (exp[i]) {
         case '[': // Los cambia por apertura de parentesis
         case '{':
           xexp[xi++] = '(';
           break;
         case ']': // Los cambia por cerradura de parentesis
         case '}':
           xexp[xi++] = ')';
           break;
         case ' ': // Quita espacios en blanco
           break;
         default:
           xexp[xi++] = exp[i];
      }
      i++;
   }
   xexp[xi] = 0;
}

// Valida expresion primera pasada
void Analizador_sintactico::Valida_expresion_inicial(const char *exp)
{
   #define NUM_ELEM_ARRAY 7
   int xi, i = 0;
   // Si el actual caracter es igual a ...
   const char *Arr1[] = { 
      "*-+/,",
      ".",
      "=",
      "'",
      "([{",
      "&",
      "|" 
   };
   // El siguiente caracter no debe de ser ...
   const char *Arr2[] = { 
      "*-+/,.=)]}><?:|&",
      "([{eE)]}=*-+/><?:|&",
      "*,./)]}?:&|",              // Se excluye el signo '=' para permitir ==, >=, <=
      "*+-/([{}])><?:|&,.'0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
      ")]}*=/.><?:|&",
      "?:|*-/+-)]}.,'><=",
      "?:&*-/+-)]}.,'><="
   };

   unsigned int pos;
   // Revisa toda la expresion
   while (exp[i]) {
      if (exp[i+1]) {
         // Revisa todo el contenido del arreglo
         for (xi = 0; xi < NUM_ELEM_ARRAY; xi++) {
             // Busca si el caracter actual esta el en primer arreglo
             if (Busca_caracter(Arr1[xi],exp[i],pos)) {
                // busca si el siguiente caracter esta en el segundo arreglo
                if (Busca_caracter(Arr2[xi],exp[i+1],pos)) {
                   // Indica el error encontrado
                   Errores[Num_errores++] = 6;
                }
             }
         }
      }
      i++;
   }
}



// Valida expresion segunda pasada
void Analizador_sintactico::Valida_expresion_final(const char *exp)
{
  int n_s_igual = 0, n_parentesis = 0, Exp_numerica = 0, i = 0;
  char car, car2, car3;
  unsigned int pos, lg = strlen(exp);
  // Revisa la exp
  car = exp[i];
  car2 = exp[i+1];
  if (i) car3 = exp[i-1];
   else car3 = 0;
  while (car) {
     switch (car) {
        case '?':
        case '<':
        case '>':
        case ':':
        case '&':
        case '|':
        case ' ': Exp_numerica = 0; 
          break;
        case ',':
          if (!n_parentesis) Errores[Num_errores++] = 8;
          if (car3 == '(') Errores[Num_errores++] = 22;
        case '+':
        case '-':
        case '*':
        case '/':
          // Valida que no exista otro operador sin operando *+/-.,)
          if (Busca_caracter("*+/,)",car2,pos)) Errores[Num_errores++] = 5;
          // Existen operandos antes del signo de igualdad
          if (!n_s_igual) Errores[Num_errores++] = 3;
          if (Es_caracter_digito(car2)) Exp_numerica = 1;
           else Exp_numerica = 0;
          break;
        case '(': // Revisa que los parentesis esten apareados
          n_parentesis ++;
          // No puede existir (, (* (/  
          if (Busca_caracter("*,/",car2,pos)) Errores[Num_errores++] = 5;
          if (Es_caracter_digito(car2)) Exp_numerica = 1;
           else Exp_numerica = 0;
          break;
        case ')': // Revisa que los parentesis esten apareados
          n_parentesis --,Exp_numerica = 0;;
          // No puede existir )[0-9]
          if (Es_caracter_digito(car2)) Errores[Num_errores++] = 4;
          // No puede existir )(
          if (car2 == '(') Errores[Num_errores++] = 5;
          break;
        case '=':
          n_s_igual ++;
          if (!car2) Errores[Num_errores++] = 10;
          if (Busca_caracter("*,/",car2,pos)) Errores[Num_errores++] = 5;
          if (Es_caracter_digito(car2)) Exp_numerica = 1;
           else Exp_numerica = 0;
          break; // Cuenta el numero de signos igual el la expresion
        case '.': // Valida el uso del punto decimal
          // Verifica que el punto decimal sea precedido y antecedido por un numero
          if (!(Es_caracter_digito(car3) || Busca_caracter("*+=/-,(",car3,pos))) Errores[Num_errores++] = 7;
          if (!Es_caracter_digito(car2)) Errores[Num_errores++] = 7;
          // Error mas de un punto decimal en una misma expresion numerica
          // el punto decimal solo en expresiones numericas
          break;
        case 'e':
        case 'E':
          break;
        default:
          if (Es_caracter_alfabetico(car)) {
             if (Exp_numerica) Errores[Num_errores++] = 11;
             Exp_numerica = 0;
          }
          if (car < 38) Errores[Num_errores++] = 0;
          switch (car) {
             case 59:
             case 64:
             case 96:
               Errores[Num_errores++] = 0;
          }

     }
     // Controla la cantidad maxima de errores en el analizador
     if (Num_errores >= Num_max_errores-5) {
        Errores[Num_max_errores-5] = 21;
        break;
     }
     i++;
     car = exp[i];
     car2 = exp[i+1];
     car3 = exp[i-1];
  }
  // Error caracteres desconocidos al final de la expresion
  if (Busca_caracter("*-/+",exp[lg - 1],pos)) Errores[Num_errores++] = 5;

  // Revisa la existencia unica del signo igual
  if (!n_s_igual) Errores[Num_errores++] = 1;
  if (!Busca_caracter(exp,'?',pos)) {
     if (n_s_igual > 1) Errores[Num_errores++] = 2;
  }
}




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Indica el tipo de expresion pasada como parametro 
// (0)  No reconocida
// (1)  Funcion
// (2)  Parametro
// (3)  Expresion aritmetica
// (4)  Ecuacion Diferencial
// (5)  Expresion de configuracion
int Analizador_sintactico::Tipo_expresion(const char *exp)
{
   char xcad[200];
   unsigned int pos, lg = strlen(exp);
   // Ecuacion diferencial
   if (Busca_caracter(exp,'\'',pos)) return 4;
   // Parametros
   if (!Busca_caracteres(exp,"*+/,)(",pos)) {
      if (!(exp[0] == 'F' && exp[1] == '(' && (exp[2] == 'x' || exp[2] == 'y') && exp[3] == ')' && exp[4] == '=')) {
         if (Busca_caracter(exp,'=',pos)) {
            Substr(exp,pos+1,lg,xcad);
            if (Es_cadena_numero(xcad)) return 2;
         }
      }
   }
   // Funciones auxiliares
   return 1;
}





///////////////////////////////////////////////////////////////////
//                                                               //
//                     Clase Arbol binario                       //
//                                                               //
///////////////////////////////////////////////////////////////////






// Crea la estructura del arbol apartir de una expresion
NODO *Arbol_binario::Crea_estructura_arbol(const char *exp)
{
   NODO *ptr_nodo = NULL;
   // Si cadena es nula retorna un puntero nulo
   if (!exp[0]) return ptr_nodo;

   // No valida si encuentra que es una expresion con comparaciones interfas
   unsigned int pos;
   if (Busca_caracter(exp,'?',pos)) return ptr_nodo;

   // Solicita memoria para contener las cadenas temporales
   unsigned int lg = strlen(exp)+1;
   char *xoperd = Asigna_puntero(lg+20);
   char *xn_izq = Asigna_puntero(lg);
   char *xn_der = Asigna_puntero(lg);

   // Se optiene el contenido del operador y sus operandos
   Separa_operandos(exp,xoperd,xn_izq,xn_der);


   // obtiene el tipo de nodo
   int tipo_nodo = Tipo_nodo (xoperd,xn_der);

   // Crea el nodo con la informacion del operador y el tipo de nodo que sera este
   ptr_nodo = Crea_nodo (xoperd,tipo_nodo);
   // Si el puntero es nulo retorna un puntero nulo
   if (ptr_nodo) {
      // Crea de manera recursiva el nodo izquierdo
      ptr_nodo->NODO_IZQUIERDO = Crea_estructura_arbol(xn_izq);
      // Crea de manera recursiva el nodo derecho
      ptr_nodo->NODO_DERECHO = Crea_estructura_arbol(xn_der);
   }
   // Libera la memoria ocupada
   delete []xoperd;
   delete []xn_izq;
   delete []xn_der;
   // retorna un puntero al nodo creado
   return ptr_nodo;
}

// Retorna el tipo de nodo del que se trate
int Arbol_binario::Tipo_nodo (char *xoperd,const char *xn_der)
{
   int i = 0, st = 1, tipo_nodo = 0;
   unsigned int pos;
   // Revisa el tipo de nodo

   // El contenido es un numero
   if ((xoperd[0] > 47 && xoperd[0] < 58) || xoperd[0] == '.') tipo_nodo = 4;

   // El contenido es un operando
   if (Busca_caracter("*+/-,=",xoperd[0],pos)) tipo_nodo = 3;

   // Revisa si es una funcion, y en caso de ser si esta es valida
   if (Es_caracter_alfabetico(xoperd[0]) && xn_der[0] == ',') {
      i = 0, st = 1, tipo_nodo = 99;
      // Revisa si esta en la tabla de Funciones
      while (Funciones[i][0]) {
         // Si es una Funcion pone en el tipo de nodo 100 + el numero de parametros de esta
         if (Compara(Funciones[i],xoperd)) {
            tipo_nodo = atoi(Funciones[i+1])+ 100;
            st = 0;
            break;
         }
         i += 2;
      }
      if (st) Errores[Num_errores++] = 19;

      // Revisa si su parametro es nulo
      if (tipo_nodo > 99) {
         // Asume que no hay parametros
         if (xn_der[1] == 0) tipo_nodo = 100;
      }

      // Funcion no valida
      if (tipo_nodo == 99) Errores[Num_errores++] = 19;
       else {
         // Revisa el numero de parametros de la funcion
         int i = 0, par = 0, n_v = 0;
         while (xn_der[i]) {
            if (xn_der[i] == '(') par ++;
            // Busca el siguiente operador segun precedencia definidas
            if (!par) {
               // Revisa si existe el operador de esa precedencia
               if (',' == xn_der[i]) n_v++;
            }
            if (xn_der[i] == ')') par --;
            i++;
         }
         // Numero de parametros erroneos
         if ((n_v+100) != tipo_nodo) Errores[Num_errores++] = 18;
      }
   }

   // Revisa si es constante
   if (Es_caracter_alfabetico(xoperd[0]) && !tipo_nodo) {
      i = 0;
      // Revisa si esta en la tabla de Constantes
      while (Constantes[i][0]) {
         // Si es una constante cambia esta por el valor indicado en la tabla Constantes
         if (Compara(Constantes[i],xoperd)) {
            #ifdef SUSTITUYE_CONSTANTES
               strcpy(xoperd,Constantes[i+1]);  // Sustituye la constante
            #else
               strcpy(xoperd,Constantes[i]);    // No sustituye la constante
            #endif
            tipo_nodo = 2;
            break;
         } else i += 2;
      }
   }
   return tipo_nodo;
}




// Crea el nodo y copia a este la expresion pasada como parametro
NODO *Arbol_binario::Crea_nodo (const char *exp, const int tipo)
{
   // Puntero al nodo a ser creado
   NODO *ptr_nodo = NULL;
   // Si la cadena es nula retorna un puntero nulo
   if (!exp[0]) return ptr_nodo;
   // Se solicita la memoria para crear el nodo
   ptr_nodo = new NODO;
   // Los punteros a nodos hijos son puestos a nulos
   ptr_nodo->NODO_DERECHO = ptr_nodo-> NODO_IZQUIERDO = NULL;
   // Solicitud de la memoria necesaria para el contenido del nodo
   ptr_nodo->NODO_CONTENIDO = Asigna_puntero(exp);
   // Copia el tipo de informacion del nodo
   ptr_nodo->NODO_TIPO = tipo;
   // Retorna un puntero al nodo recien creado
   return ptr_nodo;
}


// Separa operador y operandos
void Arbol_binario::Separa_operandos(const char *exp, char *operador, char *nodo_izq, char *nodo_der)
{

   //  Inicializa las variables a nulos
   operador[0] = nodo_izq[0] = nodo_der[0] = 0;
   const char *Prioridad_operador[] = {"+-","*/",","};
   int st, par, xi, n_e;
   unsigned int pos, i, lg;

   // Si la cadena es nula retorna
   if (!exp[0]) return;
   strcpy(xexp,exp);

   // Variables para trabajo
   i = 0, lg = 0;

   // Buscar al signo igual y separar la variable de resultado de la expresion
   if (Busca_caracter(exp,'=',pos)) {
      lg = strlen(exp);
      // La variable de asignacion es el nodo izquierdo
      Substr(exp,0,pos,nodo_izq);
      // La expresion es el nodo derecho
      Substr(exp,pos+1,lg,nodo_der);
      // El operador es '='
      operador[0] = '=', operador[1] = 0;
      return;
   }

   // Separa la expresion encerrada dentro de parentesis
   while (xexp[0] == '(') {
      par = 0, i = 0;
      while (xexp[i]) {
         if (xexp[i] == ')') par--;
         if (xexp[i] == '(') par++;
         // Encuentra el final de la expresion encerrada entre parentesis
         if (!par) {
            if (xexp[i+1]) {
               lg = strlen(xexp);
               // Existen otras expresiones
               Substr(xexp,0,i+1,nodo_izq);
               Substr(xexp,i+2,lg,nodo_der);
               operador[0] = xexp[i+1], operador[1] = 0;
               return;
             } else {
               // Expresion unica
               // Retira parentesis externos
               for (xi = 0; xi < i; xi++) xexp[xi] = xexp[xi+1];
               xexp[i-1] = 0;
            }
            break;
         }
         i++;
      }
   }


   // Si el primer caracter es una coma, se tomara como parametros de una funcion
   if (xexp[0] == ',') {
      i = 0;
      // Elimina la coma de la cadena de trabajo
      while (xexp[i]) {
         xexp[i] = xexp[i+1];
         i++;
      }
//////////////////////////////////////////////////////
// Quitar
//      // Busca la posicion del siguiente parametro
//      i = Busca_caracter(xexp,',');
//      if (i > -1) {
//         // Existen mas parametros
//         s_substr(xexp,0,i,nodo_izq);
//         s_substr(xexp,i+1,lg,nodo_der);
//         operador[0] = xexp[i], operador[1] = 0;
//         return;
//      }
   }


   // Si el primer caracter es menos unario (-)
   if (xexp[0] == '-') {
      i = 1;
      while (xexp[i]) {
         nodo_der[i-1]= xexp[i];
         i++;
      }
      nodo_der[i-1]= 0;
      // El segundo caracter es un digito
      if (Es_caracter_digito(xexp[1])) {
         // El resto de la expresion es un numero, se toma como menos unario de una expresion
         if (Es_cadena_numero(nodo_der)) {
            operador[0] = '-', operador[1] = 0;
            return;
         }
       } else { // El segundo caracter es alfabetico
         st = 1;
         // Busca si tiene operadores
         if (Busca_caracteres(nodo_der,"(*+/-),",pos)) {
            i = 0, par = 0;
            while (nodo_der[i]) {
               if (nodo_der[i] == '(') par ++;
               if (!par) {
                  if (Busca_caracter("+*-/,",nodo_der[i],pos)) st = 0;
               }
               if (nodo_der[i] == ')') par --;
               i++;
            }
         }
         if (st) {
            // Si no tiene es una variable o una funcion sola
            operador[0] = '-', operador[1] = 0;
            // El nodo izquierdo tiene un cero
            nodo_izq[0] = '0', nodo_izq[1] = 0;
            return;
         }
      }
      nodo_der[0] = 0;
   }

   // Si el primer caracter es (+)
   if (xexp[0] == '+') {
      i = 0;
      // Elimina el signo mas de la cadena de trabajo
      while (xexp[i]) xexp[i] = xexp[i+1], i++;
   }

   // Si empieza con un caracter numerico
   if (Es_caracter_numerico(xexp[0])) {
      // descarta que sea un menos unario
      if (xexp[0] != '-') {
         // Es un numero ordinario o en notacion exponencial
         if (Es_cadena_numero(xexp)) {
            strcpy(operador,xexp);
            return;
         }
      }
      // Separa la expresion revisando la precedencia de los operadores
      lg = strlen(xexp);
      for (xi = 0; xi < 3; xi++) {
         i = 0, par = 0;
         while (xexp[i]) {
            n_e = 0;
            if (xexp[i] == '(') par ++;
            // Descarta que sea un numero en notacion exponencial
            if (i > 2 && (xexp[i] == '+' || xexp[i] == '-')) {
               if (Es_caracter_digito(xexp[i-2]) && (xexp[i-1] == 'e' || xexp[i-1] == 'E') && Es_caracter_digito(xexp[i+1])) n_e = 1;
            }
            // Busca el siguiente operador segun precedencia definidas
            if (!n_e && !par && i > 0) {
               // Revisa si existe el operador de esa precedencia
               if (Busca_caracter(Prioridad_operador[xi],xexp[i],pos)) {
                  Substr(xexp,0,i,nodo_izq);
                  Substr(xexp,i+1,lg,nodo_der);
                  operador[0] = xexp[i], operador[1] = 0;
                  return;
               }
            }
            if (xexp[i] == ')') par --;
            i++;
         }
      }
   }


   // Si el primer caracter es alfabetico, o esta antecedido por un menos unario
   if (Es_caracter_alfabetico(xexp[0]) ||(xexp[0] == '-' && Es_caracter_alfabetico(xexp[1]))) {
      // Revisa si hay operadores
      if (!Busca_caracteres(xexp,"(*+/-),",pos)) {
         // Variable o constante sola
         strcpy(operador,xexp);
         return;
       } else {
         lg = strlen(xexp);
         i = 0, st = 1, par = 0;
         while (xexp[i]) {
            if (xexp[i] == '(') par ++;
            if (!par) {
               if (Busca_caracter("*-/+,",xexp[i],pos)) st = 0;
            }
            if (xexp[i] == ')') par --;
            i++;
         }
         if (st) {
            // Es una funcion sola
            if (Busca_caracter(xexp,'(',i)) {
               Substr(xexp,0,i,operador);
               Substr(xexp,i+1,lg,Xexp);
               // Para reconocerlos como parametro lo inicia con una coma
               Xexp[strlen(Xexp)-1] = 0;
               sprintf(nodo_der,",%s",Xexp);
               return;
            }
         }

         // Separa la expresion revisando la precedencia de los operadores
         for (xi = 0; xi < 3; xi++) {
            i = 0, par = 0;
            while (xexp[i]) {
               n_e = 0;
               if (xexp[i] == '(') par ++;
               // Descarta que sea un numero en notacion exponencial
               if (i > 2 && (xexp[i] == '+' || xexp[i] == '-')) {
                  if (Es_caracter_digito(xexp[i-2]) && (xexp[i-1] == 'e' || xexp[i-1] == 'E') && (xexp[i+1])) n_e = 1;
               }
               // Busca el siguiente operador segun precedencia definidas
               if (!n_e && !par && i > 0) {
                  // Revisa si existe el operador de esa precedencia
                  if (Busca_caracter(Prioridad_operador[xi],xexp[i],pos)) {
                     Substr(xexp,0,i,nodo_izq);
                     Substr(xexp,i+1,lg,nodo_der);
                     operador[0] = xexp[i], operador[1] = 0;
                     return;
                  }
               }
               if (xexp[i] == ')') par --;
               i++;
            }
         }
      }
      return;
   }

   // Expresion no reconocida
   Errores[Num_errores++] = 20;
}


// Borra todo el arbol
void Arbol_binario::Destruye_arbol(void)
{
   // Borra todo los nodos apartir de la raiz
   Borra_nodos(PTR_RAIZ);
   PTR_RAIZ = NULL;
}

// Borra todos los nodos internos apartir del puntero al nodo pasado como parametro
void Arbol_binario::Borra_nodos(NODO *ptr_nodo)
{
   if (ptr_nodo) {
      // Libera la memoria ocupada por el contenido del nodo
      delete[] (ptr_nodo->NODO_CONTENIDO);
      // Borra en forma recursiva los nodos
      Borra_nodos(ptr_nodo->NODO_DERECHO);
      Borra_nodos(ptr_nodo->NODO_IZQUIERDO);
      // Libera la memoria ocupada por el nodo
      delete ptr_nodo;
   }
}

// Recorrido en preorden
void Arbol_binario::Recorrido_preorden(NODO *ptr_nodo)
{
   if (ptr_nodo) {
      strcat(Xexp,ptr_nodo->NODO_CONTENIDO);
      strcat(Xexp,"|");
      Recorrido_preorden(ptr_nodo->NODO_IZQUIERDO);
      Recorrido_preorden(ptr_nodo->NODO_DERECHO);
   }
}

// Recorrido en inorden
void Arbol_binario::Recorrido_inorden(NODO *ptr_nodo)
{
   if (ptr_nodo) {
      Recorrido_inorden(ptr_nodo->NODO_IZQUIERDO);
      strcat(Xexp,ptr_nodo->NODO_CONTENIDO);
      strcat(Xexp,"|");
      Recorrido_inorden(ptr_nodo->NODO_DERECHO);
   }
}

// Recorrido en postorden
void Arbol_binario::Recorrido_postorden(NODO *ptr_nodo)
{
   if (ptr_nodo) {
      Recorrido_postorden(ptr_nodo->NODO_IZQUIERDO);
      Recorrido_postorden(ptr_nodo->NODO_DERECHO);
      strcat(Xexp,ptr_nodo->NODO_CONTENIDO);
      strcat(Xexp,"|");
   }
}


// Retorna la expresion en el siguiente formato
// Si TP es PREORDEN regresa le expresion en preorden
//          INORDEN regresa le expresion en inorden
//          POSTORDEN regresa le expresion en postorden
void Arbol_binario::Retorna_expresion(char *xexp, const int tp)
{
   xexp[0] = 0;
   if (Estado) {
      Xexp[0] = 0;
      if (!Num_errores) {
         if (tp == PREORDEN)  Recorrido_preorden(PTR_RAIZ);
         if (tp == INORDEN)   Recorrido_inorden(PTR_RAIZ);
         if (tp == POSTORDEN) Recorrido_postorden(PTR_RAIZ);
      }
      strcpy(xexp,Xexp);
   }
}






























