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


#include "m_numer.hpp"
#include "campo_v.hpp"
#include "v_grafic.hpp"


// Clase de la Interfaz
class Base_integra: virtual public Metodos_numericos, virtual public Campo_vectorial {

   protected:

      ///////////////////////////////////////////////////
      //              Ventana de Trabajo               //
      ///////////////////////////////////////////////////
      Ctrl_ventana_grafica *VT1;                              // Ventana de trabajo
      unsigned int   Coord_rectangulares:  1;                 // Indica si se visualizaran las graficas en coordenadas rectangulares o polares

      ///////////////////////////////////////////////////
      //                       Iconos                  //
      ///////////////////////////////////////////////////
      unsigned int   Estado_ventana:       3;                 // Indica el estado de la ventana de trabajo
                                                              // (2) Ventana de media pantalla
                                                              // (3) Ventana de pantalla completa


      /////////////////////////////////////////////////////////////
      // Valores para controlar la visualizacion de coordenadas  //
      /////////////////////////////////////////////////////////////
                     
      unsigned int   v_coord: 1;                              // Inidica si se revisualizaran las coordenadas
      unsigned int   Visualiza_coordenadas_sistema: 1;        // Indica si se visualizan las coordenadas
      char          *Coordenadas;                             // Cadena para soportar las coordenadas a visualizar

      /////////////////////////////////////////////////
      //                Menus                        //
      /////////////////////////////////////////////////
      unsigned int   selec;                                   // Opcion seleccionada del menu
      C_2I           vent;                                    // Posicion del menu dentro de la pantalla

      //////////////////////////////////////////////////
      //               Control de Proyeccion          //
      //////////////////////////////////////////////////
      C_3D           aux1, aux2;                              // Variable temporal     
      double         aux[NUMERO_ECUACIONES+2];                // Variable temporal



           // Inicializa las variables del sistema
      void Inicializa_variables(void);
      void Redimensiona(unsigned int i);

           // Graba el estado del proyecto
      void Graba_estado(const int tp, const char *xarch);
           // Lee el estado del proyecto
      int  Lee_estado(const char *archivo, const int tp);
           // Imprime el estado de los sistemas
      void Imprime_estado(const int tr, const int dp);

           // Selecciona el escenario
      int  Escenarios(const int tp);
           // Cambia los parametros de la pantalla
      int  Cambia_dimenciones_pantalla(const int tp);
           // Control del cursor (movimiento)
      void Controla_cursor(const int tecla);
           // Selecciona los ejes coordenados a visualizar
      void Selecciona_colores(void);

           // Dibuja la ventana de trabajo
      void Dibuja_ventana_trabajo(void);
           // Dibuja un punto haciendo caso del escenario
      void Dibuja_punto(const double ini[], const double t, const int vtn, const char col = 255, const int tp = 0);
           // Dibuja una linea haciendo caso del escenario
      void Dibuja_linea(const double ini[], const double fin[], const double t_ini, const double t_fin, const int vtn, const char col = 255, const int tp = 0);
           // Dibuja un punto sin hacer caso del escenario
      void Dibuja_punto_ventana(const C_3D ini, const int vtn, const char col = 255, const int tp = 0);
           // Dibuja una linea sin hacer caso del escenario
      void Dibuja_linea_ventana(const C_3D ini, const C_3D fin, const int vtn, const char col = 255, const int tp = 0);

           // Actualiza las coordenadas del cursor dentro de la ventana de trabajo
      void Actualiza_coordenadas(void);
};

