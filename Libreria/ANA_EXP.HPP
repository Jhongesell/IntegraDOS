// Autor:     Antonio Carrillo Ledesma
// Direcci¢n: Amsterdam 312, colonia Hip¢dromo Condesa
// Tel‚fono:  574-43-53
// Propiedad intelectual, derechos reservados conforme a la ley (Registro en tr mite)
// 1993 -1994


#ifndef __ANA_EXP_HPP__
#define __ANA_EXP_HPP__

#include "cadenas.hpp"


#define PREORDEN       0
#define INORDEN        1
#define POSTORDEN      2


extern const char *Errores_a_s[];

// Estructura que contendra el contenido y tipo de nodo que se usara para crear el
// arbol de analisis sintactico.
typedef struct nodo_arbol
{
   nodo_arbol *NODO_IZQUIERDO;     // Apuntador al nodo hijo izquierdo
   nodo_arbol *NODO_DERECHO;       // Apuntador al nodo hijo derecho
   char       *NODO_CONTENIDO;     // Apuntador al contenido (El espacio se obtendra dinamicamente)
   int         NODO_TIPO;          // Indica el tipo de informacion que contendra el nodo
} NODO;




// Clase que maneja el arbol binario del analizador de expresiones
class Arbol_binario: public Cadenas
{
   private:


            // Borra los nodos internos apartir de PTR_NODO
      void  Borra_nodos(NODO *ptr_nodo);
            // Recorrido en preorden
      void  Recorrido_preorden(NODO *ptr_nodo);
            // Recorrido en inorden
      void  Recorrido_inorden(NODO *ptr_nodo);
            // Recorrido en postorden
      void  Recorrido_postorden(NODO *ptr_nodo);
            // Crea el nodo y copia a este la expresion pasada como parametro
      NODO *Crea_nodo(const char *exp, const int tipo);
            // Separa operador y operandos
      void  Separa_operandos(const char *exp, char *operador, char *nodo_izq, char *nodo_der);
            // Retorna el tipo de nodo del que se trate
            //   (1)      Variable
            //   (2)      Constante
            //   (3)      Operador
            //   (4)      Numero
            //   (100+x)  Funcion, donde x es el numero de parametros de esta
      int   Tipo_nodo(char *xoperd,const char *xn_der);


   protected:

      // Variables
      NODO *PTR_RAIZ;                           // Puntero al nodo raiz
      unsigned int Estado:1;                    // Indica el estado del sistema
      char *Xexp;                               // Variables de uso general
      char *xexp;
      char *Xoperd;
      char *Xn_der;
      char *Xn_izq;

      int  Num_max_errores;                     // Indica el numero maximo de errores
      int  Num_errores;                         // Indica el numero de errores en el analizador
      int *Errores;                             // Indica los errores dentro del analizador


            // Destruye el arbol
      void  Destruye_arbol(void);
            // Crea la estructura del arbol apartir de una expresion
      NODO *Crea_estructura_arbol(const char *exp);


   public:

            // Retorna la expresion en el siguiente formato
            // Si TP es PREORDEN regresa le expresion en preorden
            //          INORDEN regresa le expresion en inorden
            //          POSTORDEN regresa le expresion en postorden
      void  Retorna_expresion(char *xexp, const int tp);
};




// Clase Analizador sintactico
class Analizador_sintactico: private Arbol_binario  
{

   private:

            // Revisa que esten bien balanceados los agrupadores
     int    Revisa_balance(const char *exp, const int tp);
            // Prepara la expresion para el analisis completo
     void   Prepara_expresion_validacion(const char *exp, char *xexp);
            // Valida expresion tal como biene
     void   Valida_expresion(const char *exp);
            // Valida expresion inicial
     void   Valida_expresion_inicial(const char *exp);
            // Valida expresion final
     void   Valida_expresion_final(const char *exp);

          
   public:

                 // Retorna la expresion en el formato pedido
                 Arbol_binario::Retorna_expresion;

                 // Constructor y destructor
                 Analizador_sintactico(void);
                ~Analizador_sintactico();

                 // Retorna la cadena de error correspondiente al err_num
     const char *Retorna_error(const int err_num)
                 {return (Errores_a_s[Errores[err_num]]);}

                 // Retorna el numero de errores
     int         Numero_errores(void)
                 {return Num_errores;}


                 // Indica el tipo de expresion pasada como parametro
                 // (0)  No reconocida 
                 // (1)  Funcion
                 // (2)  Parametro
                 // (3)  Expresion aritmetica
                 // (4)  Ecuacion Diferencial
                 // (5)  Expresion de configuracion
     int         Tipo_expresion(const char *exp);

                 // Crea el arbol
     void        Crea_arbol(const char *exp);

};

#endif
