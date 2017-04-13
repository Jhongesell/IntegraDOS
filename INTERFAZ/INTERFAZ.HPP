// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#include "aplicac.hpp"
#include "m_proy.hpp"
#include "m_sistem.hpp"


class Interfaz: private Aplicacion, private Selecciona_sistema, private Selecciona_proyecto
{
   private:

      unsigned int     Accion;               // Indica la accion a realizar en el sistema

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
              // Controla a los proyectos
         void Controla_proyecto(void);
              // Controla a los sistemas
         void Control_sistemas(void);
           // Dibuja la pantalla de la aplicacion
      void Dibuja_aplicacion(void);


   public:

                 // Ejecuta la aplicacion
           Aplicacion::Ejecuta;

};



