// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#include "aplicac.hpp"


extern "C" {
   #include <graphics.h>
   #include <stdlib.h>
   #include <dos.h>
   #include <stdio.h>
}



// Clase de la Interfaz
class Ctrl_grafico: private Aplicacion
{
   private:

      Icono        *Iconos;                      // Puntero a los iconos
      int           Niconos;                     // Indica el numero de iconos activos
      int           Ipresionado;                 // Indica el icono presionado
      unsigned int  selec;
      C_2I          vent;
      unsigned int  Imagen_cargada:1;            // Indica si se cargo una imagen dentro del area de trabajo
      char          *Archivo;                    // Indica el nombre del archivo que actualmente se visualiza
      char          *archivo;                    // Variable temporal

           // Pantalla de presentacion
      void Pantalla_presentacion(void);
           // Pantalla de terminacion
      void Pantalla_terminacion(void);
           // Inicializa la clase
      void Proceso_inicial(void);
           // Destruye la clese
      void Proceso_final(void);
           // Pantalla de estado del sistema
      void Pantalla_estado_sistema(void);
           // Redibuja pantalla
      void Redibuja_pantalla(void);

           // Controlador de la palicacion
      void Controlador(void);

           // Dibuja la ventana de la aplicacion
      void Dibuja_aplicacion(void);
           // Duplica la actual imagen
      void Duplica(void);
           // Renombra la actual imagen
      void Renombra(void);

   public:

           // Ejecuta la aplicacion
           Aplicacion::Ejecuta;

};




