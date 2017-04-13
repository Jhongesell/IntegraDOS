// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __NUCLEO_HPP__
#define __NUCLEO_HPP__

#include "..\libreria\tiempo.hpp"
#include "mouse.hpp"
#include "v_marco.hpp"
#include "porta_pa.hpp"
#include "ctrl_tec.hpp"
#include "admin_ip.hpp"
#include "teclado.hpp"
#include "c_error.hpp"

// Indica si el programa esta o no activo
extern int Programa_activo;
// Variable de control
extern int  SW_salida;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;


///////////////////////////////////////////////////////////////
// Clase que funge como nucleo del sistema, administra los   //
// siguientes recursos:                                      //
//                                                           //
//         Teclado                                           //
//         Mouse                                             //
//         Porta papeles                                     //
//         Ventana de mensajes                               //
//         Impresi¢n                                         // 
//         Control de errores                                //
//                                                           //
// De una manera organizada y completa para poder ser        //
// compartidos por toda clase que lo requiera                //
///////////////////////////////////////////////////////////////





class Nucleo: private Porta_papeles, private Ctrl_teclado, private GMouse, private Administrador_impresion, private Control_Errores, private Tiempo
{

   private:

      unsigned int      Sw_impresora: 1;  // Controla que no entre en loop el control de errores en impresion
      long              N_s_e;            // Numero de segundos para que se active el protector de pantalla
      long              N_s_t;            // Numero de segundos transcurridos despues del ultimo proceso
      long              sw;               // Variable temporal para el control de la visualizacion de la hora
      Ventana_marco    *Vm;               // Definicion del objeto ventana de mensajes
      char             *cad;              // Cadena temporal

	public:


           ///////////////////////////////////////////////////
           // Rutinas de control del nucleo                 //
           ///////////////////////////////////////////////////

           // Constructor de la clase
           Nucleo(void);

           // Inicializa el nucleo
      void Inicializa(void);
           // destruye el nucleo
      void Destruye(void);
           // Actualiza el teclado, mouse y las actividades asociadas con estos
      void Administrador(void);
           // Lee el estado de teclado y el mouse
      void Actualiza_teclado_mouse(void) const
           {Retorna_bufer_teclado(Caracter,Tecla); Mpos(); if(Tecla == ALT_Q) Programa_activo = 0, SW_salida = 1;}
           // Dibuja la ventana de mensajes
      void Dibuja(void) const;

           ///////////////////////////////////////////////////
           // Rutinas de control de la ventana de mensajes  //
           ///////////////////////////////////////////////////

           // Rutina que visualiza el texto en la ventana de mensajes
      void Visualiza_texto(const char *texto) const;
           // Rutina que visualiza la hora del sistema
      void Visualiza_hora(void) const;


           ///////////////////////////////////////////////////
           // Protector de pantalla                         //
           ///////////////////////////////////////////////////

           // Controlador del protector de pantalla
      void Protector_pantalla(void);

           //////////////////////////////////////////////////
           //  Rutinas de MOUSE                            //
           //////////////////////////////////////////////////

           GMouse::MInicializa;
           GMouse::Retorna_estado_mouse;
           GMouse::Muestra_mouse;
           GMouse::Establece_limites;
           GMouse::Restaura_limites;
           GMouse::Ismoving;
           GMouse::Selecciona_tipo_mouse;
           GMouse::Ctrl_mouse;


           //////////////////////////////////////////////////
           //  Rutinas de CTRL_TECLADO                     //
           //////////////////////////////////////////////////

           Ctrl_teclado::Almacena_bufer_teclado;
           Ctrl_teclado::Limpia_bufer_teclado;
           Ctrl_teclado::Busca_bufer_teclado;
           Ctrl_teclado::Estado_Insert;

           //////////////////////////////////////////////////
           //  Rutinas de ADMINISTRADOR DE IMPRESION       //
           //////////////////////////////////////////////////

           Administrador_impresion::Parametros_Administrador_impresion;

           //////////////////////////////////////////////////
           //  Rutinas de PORTA PAPELES                    //
           //////////////////////////////////////////////////

           Porta_papeles::Crea_porta_papeles;
           Porta_papeles::Inserta_porta_papeles;
           Porta_papeles::Retorna_total_elementos;
           Porta_papeles::Retorna_cadena_porta_papeles;



           Control_Errores::Mensaje;
           Control_Errores::Opcion;
           Control_Errores::Ctrl_error;
           Control_Errores::Visualiza_error;
};


#endif

//////////////////////////////////////////////////////////////////////
/*

+ El control de esta clase lo tiene la clase APLICACION el usuario no la 
  construye, destruye ni la controla, solo puede hacer uso de sus recursos

+ Para Imprimir un archivo pormedio del administrador de impresion
  requiere estar grabado en dico, entonces especifique:
      NCO->Parametros_Administrador_impresion(const char *arch);
  indicando el nombre del archivo a imprimir el nucleo se encargara 
  de imprimirlo.

+ Para hacer uso del porta papeles ahaga lo siguiente:
     a) Indique la cantidad de lineas a mandar al porta papeles 
           NCO->Crea_porta_papeles(unsigned int num_elem);
     b) Inserte cada una de las cadenas al portapapeles
           NCO->Inserta_porta_papeles(const char *cad);
     c) Para saber si esta activo el porta papeles
           NCO->Retorna_Estado_porta_papeles(void);
     d) Para saber el numero de elementos en el porta papeles
           NCO->Retorna_total_elementos(void);
     e) Para recuperar una cadena especifica del porta papeles
           const char    NCO->Retorna_cadena_porta_papeles(unsigned int  num_elem);

+ Para visualizar un mensaje en la ventana de visualizacion de mensajes
  escriba este en el siguiente comportamiento
      NCO->Visualiza_texto(const char *texto);



*/
//////////////////////////////////////////////////////////////////////




