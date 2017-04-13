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


#include "aplicac.hpp"
#include "v_grafic.hpp"
#include "bintegra.hpp"


// Clase de la Interfaz
class Integra: private Aplicacion, private Base_integra {

   private:

      ///////////////////////////////////////////////////
      //                       Iconos                  //   
      ///////////////////////////////////////////////////

      unsigned int t_ceroclina:        1;   // Indica que grafique isoclina
      unsigned int Inversion_ventanas: 1;   // Indica que se han invertido las ventanas
      int          Niconos;                 // Indica el numero de iconos activos
      int          Ipresionado;             // Indica el icono presionado
      int          Numero_funcion;          // Variable usada para saber que funcion auxiliar se graficara
      int          i;                       // Variable temporal
      char         cad[200];                // Variable temporal
      Icono       *Iconos;                  // Puntero a los iconos

              // Proceso inicial del programa
      void    Proceso_inicial(void);
              // Proceso final del programa   
      void    Proceso_final(void);
              // Controlador de la palicacion
      void    Controlador(void);
                 // Control de acciones dentro de integra
      void       Control_acciones(const unsigned int tp);
              // Pantalla de presentacion
      void    Pantalla_presentacion(void);
              // Pantalla de terminacion
      void    Pantalla_terminacion(void);
              // Pantalla de estado del sistema
      void    Pantalla_estado_sistema(void);
              // Redibuja pantalla
      void    Redibuja_pantalla(void);

              // Dibuja la ventana de la aplicacion
      void    Dibuja_aplicacion(void);
              // Visualiza las coordenadas del sistema
      void    Visualiza_coordenadas(void);

   public:

              // Ejecuta la aplicacion
              Aplicacion::Ejecuta;
};




