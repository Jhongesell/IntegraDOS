/////////////////////////////////////////////////////////////
//                                                          //
//              I N T E G R A    V E R. 2.01                //
//       (Analisador de sistemas din micos interativos)     //
//                                                          //
// Desarrollado para ambiente DOS en modo de video          //
// VGA y SVGA a 256 colores.                                //
//                                                          //
// Autores:                                                 //
//     Humberto Carrillo Calvet                             //
//     Antonio Carrillo Ledesma                             //
//     Luis A. Nava Fernandez                               //
//                                                          //
// Desarrollado en:                                         //
//     Laboratorio de Dinamica no Lineal                    //
//     Facultad de Ciencias, UNAM, M‚xico D.F.              //
//     Tel‚fono 6 22-48-70                                  //
//     E-mail: dinamica@athena.fciencias.unam.mx            //
//                                                          //
// Propiedad intelectual, todos los derechos reservados     //
// conforme a la ley, registro en tr mite.                  //
// Revisi¢n  1.1-A                                          //
//                                                          //
//////////////////////////////////////////////////////////////



#ifndef __M_NUMER__
#define __M_NUMER__


#include "m_d_usr.HPP"


class Metodos_numericos: virtual public M_Definicion_usuario 
{

   private:

                    
      unsigned int  Integracion_positiva:        1;  // Inidica si la integracion es en tiempo positivo o negativo
      double        c_ini[NUMERO_ECUACIONES];        // Condicion inicial en la integracion numerica
      double        Tiempo_actual;                   // Tiempo actual en la integracion numerica
      double        Tiempo_integracion;              // Tiempo de integracion
      double        Paso_integracion;                // Actual paso de integracion
      double        Paso;                            // Variable que contiene el paso de integracion para metodos de paso variable
      double        xNorma, Norma;                   // variables para calcular la norma  
      double        aux1[NUMERO_ECUACIONES];
      double        aux2[NUMERO_ECUACIONES];
      double        aux3[NUMERO_ECUACIONES];
      double        aux4[NUMERO_ECUACIONES];
      double        aux5[NUMERO_ECUACIONES];
      double        aux6[NUMERO_ECUACIONES];
      double        aux7[NUMERO_ECUACIONES];
      unsigned int  i;                               // Variable auxiliar

                    // Puntero a un arreglo de metodos numericos
      void          (Metodos_numericos::*Metodos[NUMERO_METODOS_NUMERICOS])(void);
                    // Metodo Runge_Kutta de orden 4
      void          Runge_Kutta4(void);
                    // Metodo Hune
      void          Heun(void);
                    // Metodo Punto medio
      void          Punto_medio(void);
                    // Metodo Euler Modificado
      void          Euler_modificado(void);
                    // Metodo de Euler
      void          Euler(void);
                    // Metodo de Euler paso variable
      void          Euler_p_v(void);
                    // Metodo de Euler Modificado paso variable
      void          Euler_modificado_p_v(void);
                    // Metodo de Runge_Kutta de orden 4 paso variable
      void          Runge_Kutta4_p_v(void);
                    // Metodo de Runge_Kutta_Fehlberg
      void          Runge_Kutta_fehlberg(void);


                    // Controla el error en calculos
      void          Error_calculo(void);
                    // Controla error por desbordamiento numerico
      int           Desbordamiento_numerico(void);
                    // Revisa si se desea suspender la integracion numerica
      int           Suspende_integracion(void);


   protected:                                           

      //////////////////////////////////////////////////
      // Parametros de los metodos numericos          //
      //////////////////////////////////////////////////

                    // Indica el metodo actual de integracion
      unsigned int  Metodo_N_actual[NUMERO_SISTEMAS];
                    // Parametros de los metodos numericos
      double        Parametros_metodo[NUMERO_SISTEMAS][NUMERO_METODOS_NUMERICOS][4];
                    // Numero de iteracion actual en la integracion numerica
      unsigned long Numero_iteraciones;
                    // Paso maximo de integracion en metodos de P.V.
      double        Paso_maximo;
                    // Paso minimo de integracion en metodos de P.V.
      double        Paso_minimo;

		Ctrl_reporte *Rep_num;                                        // Apuntador al manejador de reporte de la integracion numerica
      char          C_imp[300];                                     // Contienen la cadena con la que se generara el reporte
		unsigned int  Reporte_activo:                       1;        // Indica si esta activo la generacion del reporte de la integracion numerica



////////////////////////////////////////
// cambiar el numero de parametros segun
// el maximo numero de parametros por metodo
////////////////////////////////////////

   public:

                    // Inicializa la clase
      virtual  void Inicializa(void);
                    // Cambia el metodo de integracion actual
      void          Selecciona_metodo_integracion(void);
                    // Integra numericamente
      void          Integra_numericamente(const int ti);
};

#endif


