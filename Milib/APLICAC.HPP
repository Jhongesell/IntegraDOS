// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#ifndef __APLICAC_HPP__
#define __APLICAC_HPP__

#include "pantgraf.hpp"
#include "v_iconos.hpp"
#include "teclado.hpp"
#include "vis_ayud.hpp"
#include "s_arch.hpp"
#include "r_esp.hpp"


// Clase prototipo de aplicaciones
class Aplicacion
{
   private:

      unsigned int      M_Anterior;        // Numero de mensaje anterior
      unsigned int      M_Activo;          // Numero de mensaje actual
      unsigned int      st:            3;  // Variables temporales
      Pantalla_Grafica *Pg;                // Declaracion del objeto Pantalla Grafica

                   // Inicializador de la clase
              void Inicializa(const char *tit, const int t_v);
                   // Destructor de la clase
              void Destruye(void);

   protected:

      Ventana_iconos   *Vt;                // Declaracion del objeto ventana de trabajo
      char            **Texto_explicativo; // Texto explicativo

   public:

                   // Ejecuta la aplicacion 
              void Ejecuta(const char *tit, const int t_v);
                   // el titulo de la ventana principal y el tipo
                   // de video grafico:
                   // (4) SVGA 1024x768 256 colores
                   // (3) SVGA  800x600 256 colores
                   // (2) SVGA  640x480 256 colores
                   // (0) VGA   640x480  16 colores

                   // Dibuja la ventana base
              void Dibuja(void);
                   // Define el titulo de la ventana principal
              void Define_titulo(const char *tit)
                   {Vt->Define_titulo(tit);}
                   // Visualizar la cadena en el area de mensajes
              void Visualiza_texto(const char *texto);
                   // Proceso inicial de la aplicaci¢n
      virtual void Proceso_inicial(void) = 0;
                   // Proceso final de la aplicaci¢n
      virtual void Proceso_final(void) = 0;
                   // Controlador de la palicacion
      virtual void Controlador(void) = 0;
                   // Pantalla de presentacion
      virtual void Pantalla_presentacion(void) = 0;
                   // Pantalla de terminacion
      virtual void Pantalla_terminacion(void) = 0;
                   // Pantalla de estado del sistema
      virtual void Pantalla_estado_sistema(void) = 0;
                   // Redibuja la pantalla
      virtual void Redibuja_pantalla(void) = 0;
};

#endif


/*
////////////////////////////////////////////////
// Modo de uso de la clase aplicacion         //
////////////////////////////////////////////////




#include "aplicac.hpp"


// Clase de la Instala
class Instala: public Aplicacion {

   private:


      Icono         *Iconos;                     // Puntero a los iconos
      int            Niconos;                    // Indica el numero de iconos activos
      int            Ipresionado;                // Indica el icono presionado



   public:

           // Proceso inicial del programa
      void Proceso_inicial(void);
           // Proceso final del programa   
      void Proceso_final(void);
           // Controlador de la palicacion
      void Controlador(void);
           // Pantalla de presentacion
      void Pantalla_presentacion(void);
           // Pantalla de terminacion
      void Pantalla_terminacion(void);
           // Dibuja la ventna de la aplicacion
      void Dibuja(void);
};


#include "nucleo.hpp"



// Definicion externa del objeto Nucleo
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;
// Dimenciones de la pantalla
extern int X_MAX;
extern int Y_MAX;
extern char *ARCHIVO_AYUDA;       // Indica el archivo que se mostrara de ayuda
// Indica cual texto explicativo a visualizar
extern unsigned int N_texto_explicativo;




void Instala::Proceso_inicial(void)
{
   // Indica el archivo de ayuda
   ARCHIVO_AYUDA = "C:\\INTEGRA\\SYS\\HELP\\INTEGRA.HLP";
   Texto_explicativo = new char *[3];
   Texto_explicativo[0] = "";
   Texto_explicativo[1] = "Terminar el programa y retornar al DOS";
   Texto_explicativo[2] = "Visualiza la ayuda sobre el sistema";
   // Dibuja la pantalla de trabajo
   Dibuja();
}


void Instala::Proceso_final(void)
{

}


void Instala::Controlador()
{

}


// Dibuja la pantalla de trabajo
void Instala::Dibuja(void)
{
   // Dibuja la ventana con iconos
   Aplicacion::Dibuja();
}



// Presentacion inicial
void Instala::Pantalla_presentacion(void)
{
}


// Pantalla de terminacion
void Instala::Pantalla_terminacion(void)
{

}




// Programa principal de integra
void main(int argc, char *argv[])
{
   int svga = 0, st = 0;
   // Revisa a que modo grafico iniciara la pantalla Grafica
   if(argc == 2) {
      svga = argv[1][0] - '0';
      if(svga < 0 || svga > 4 || svga == 1) st = 1;
   } else st = 1;
   if(st) printf("\n\n%s\n\n%s\n\n%s N\n\n%s\n%s\n%s\n%s\n%s\n",TXT1,TXT2,argv[0],TXT3,TXT4,TXT5,TXT6,TXT7);
    else {
      Instala Is;
      Is.Ejecuta(TXT1,svga);
   }
}



*/


