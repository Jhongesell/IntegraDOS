//////////////////////////////////////////////////////////////
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
//     Luis Nava Fernandez                                  //
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


#ifndef __USUARIO_HPP__
#define __USUARIO_HPP__

#include "definic.hpp"
#include "\libreria\gen_rep.hpp"
#include "C:\INTEGRA\SYS\INCLUDE\DEF_USER.HPP"
#include "C:\INTEGRA\SYS\INCLUDE\USER_DEF.HPP"
#include "\libreria\cadenas.hpp"


class M_Definicion_usuario: public Definicion_usuario, public Cadenas 
{

	protected:

		char           Cadena_graficar[200];                             // Almacena la cadena de la funcion a graficar
		unsigned int   Sistema_actual;                                   // Numero de sistema actual
		unsigned int   Guarda_posicion:                      1;          // Indica si se guarda la posicion anterior en los metodos numericos


		/////////////////////////////////////////////
		// Variables usadas por cada sistema       //
		/////////////////////////////////////////////

      // Graficacion de ejes coordenados
      C_3I           Color_ejes[NUMERO_SISTEMAS];                      // Color de ejes por cada sistema
      unsigned int   Color_grafica[NUMERO_SISTEMAS];                   // Color de la grafica
      unsigned int   Color_grafica_aux[NUMERO_SISTEMAS];               // Color de la grafica para las funciones auxiliares
      unsigned int   Grafica_puntos[NUMERO_SISTEMAS];                  // Grafica con puntos o lineas


      // Condicion inicial y final
      double         Ac_Condicion[2][NUMERO_SISTEMAS][NUMERO_ECUACIONES]; // Condicion inicial (posicion dentro de los ejes coordenados
      double         Ac_Tiempo[2][NUMERO_SISTEMAS];                       // Tiempo inicial
      double         At_Condicion[2][NUMERO_SISTEMAS][NUMERO_ECUACIONES]; // Condicion final (ultima integracion)
      double         At_Tiempo[2][NUMERO_SISTEMAS];                       // Tiempo final
      double         MAX_VALOR[2][NUMERO_SISTEMAS];                       // Valor maximo al realizar la integracion


      // Controla el movimiento dentro de los ejes coordenados
      C_3D           Paso_cursor[2][NUMERO_SISTEMAS];                  // Paso del cursor
      C_3D           Incremento_paso[2][NUMERO_SISTEMAS];              // Define el incremento del paso del cursor sobre el eje X, Y, Z


      // Ventanas de trabajo
      C_3D           Dim_maxima[2][NUMERO_SISTEMAS];                   // Dimencion maxima de la ventana de trabajo 
      C_3D           Dim_minima[2][NUMERO_SISTEMAS];                   // Dimencion minima de la ventana de trabajo 
      C_3D           Escala[2][NUMERO_SISTEMAS];                       // Escala en X, Y, Z
      C_2I           Centro[2][NUMERO_SISTEMAS];                       // Centro de la pantalla
      double         Angulos[2][NUMERO_SISTEMAS][2];                   // Angulos usados para la graficacion
      unsigned int   Graficar_dos_ventanas;                            // Indica si graficara en las dos ventanas simultaneamente
      unsigned int   Doble_ventana:        1;                          // Indica la ventana activa
                                                                       // (0) Una ventana visible a un mismo tiempo
                                                                       // (1) Dos ventanas visiblea a un mismo tiempo


      // Valores para las proyecciones
      char           C_Ecuaciones1[NUMERO_ECUACIONES+3][15];
      char           C_Ecuaciones2[NUMERO_ECUACIONES+3][15];
      unsigned int   ind_no_avil;

      // Parametros del campo vectorial
      double         Parametros_camp_vect[NUMERO_SISTEMAS][4];

      // Numero de puntos a graficar para funciones auxiliares
      double         NUMERO_PUNTOS;

      // Indica la ventana activa dentro del sistema
      unsigned int   Ventana_activa:       1;  


                     // Selecciona el sistema actual
      int            Selecciona_sistema(void);
                     // Parametros del sistema actual
      void           Cambia_parametros_sistema(void);
                     // Permite capturar y visualizar las funciones auxiliares indicadas por el usuario         
      void           Captura_funcion_auxiliar(void);
                     // Grafica la funcion pasada como parametro
      void           Grafica_funcion(const char *exp, const C_3D max, const C_3D min);
                     // Grafica ceroclinas e isoclinas
      void           Grafica_ceroclinas(const unsigned int tp, const unsigned int eje, const C_3D max, const C_3D min);
                     // Cambia las condiciones iniciales
      void           Cambia_Condicion_inicial(void);
                     // Genera la lista de los escenarios de cada sistema
      void           Genera_lista_escenarios(void);

                     // Dibuja un punto haciendo caso del escenario
      virtual void   Dibuja_punto(const double ini[], const double t, const int vtn, const char col = 255, const int tp = 0) = 0;
                     // Dibuja una linea haciendo caso del escenario
      virtual void   Dibuja_linea(const double ini[], const double fin[], const double t_ini, const double t_fin, const int vtn, const char col = 255, const int tp = 0) = 0;
                     // Dibuja un punto sin hacer caso del escenario
      virtual void   Dibuja_punto_ventana(const C_3D ini, const int vtn, const char col = 255, const int tp = 0) = 0;
                     // Dibuja una linea sin hacer caso del escenario
      virtual void   Dibuja_linea_ventana(const C_3D ini, const C_3D fin, const int vtn, const char col = 255, const int tp = 0) = 0;
                     // Control de acciones dentro de integra
      virtual void   Control_acciones(const unsigned int tp) = 0;
                     // Nombre de la ecuacion indicada en el sistema actual
      void           Nombre_ecuacion_sistema_actual(const unsigned int sis, const unsigned int i, char *cad);
                     // Cambia los valores de la ventana uno a la ventana dos
      void           Invierte_ventanas(void);
};


#endif





