// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-B


// Definicion de la clase NUCLEO

#include "nucleo.hpp"
#include "tiempo.hpp"
#include "teclado.hpp"
#include "r_var.hpp"


extern "C" {
   #include <graphics.h>
   #include <dir.h>
}

extern int X_MAX;
extern int Y_MAX;


#define TIEMPO_ACTIVACION  180  // Indica el tiempo de activacion del protector de pantalla
#define BUFER_TECLADO       40  // Indica la longitud del bufer del teclado

//#define SALVA_PANTALLA        // indica si esta activo el protector de pantalla




// Constructor de la clase Nucleo
Nucleo::Nucleo(void)
{
   // Iniciacion de variables
   N_s_e = TIEMPO_ACTIVACION;
   N_s_t = Numero_segundos();
   sw = 1;
   Sw_impresora = 0;
}

// Inicializa los objetos de la clase
void Nucleo::Inicializa(void)
{
   // Activa el control de teclado
   Inicializa_bufer_teclado(BUFER_TECLADO);

   // Inicializa la cadena temporal
   cad = new char[30];
   // Define el area de mensajes
   Vm = new Ventana_marco;
   Vm->Define_ventana(3,Y_MAX-30,X_MAX-3,Y_MAX-2,Gris1);


   // Inicializa el porta papeles
   Inicializa_porta_papeles();
   // Inicializa el administrador de impresion
   Inicializa_Administrador_impresion();
}



//Destruye los objetos de la clase
void Nucleo::Destruye(void)
{
   // Borra el controlador de teclado
   Borra_bufer_teclado();
   // Borra el contenido del porta papeles
   Borra_porta_papeles();
   // Borra el administrador de impresion
   Borra_Administrador_impresion();
   // Destruye la ventana de mensajes
   delete Vm;
   // Destruye cadena temporal
   delete []cad;
}


// Dibuja la ventana de la aplicacion
void Nucleo::Dibuja(void) const
{
   // Dibuja ventana de mensajes
   Vm->Dibuja();
   // Visualiza el texto explicativo
   Visualiza_texto("");
   Visualiza_hora();
   ((Nucleo*)this)->sw = 1;
}

// Visualizar la cadena en el area de mensajes
void Nucleo::Visualiza_texto(const char *texto) const
{
   // Limpia y pone el recuadro en el area de mensajes
   Vm->Limpia_recuadro(5,4,Vm->Largo_X()-90,Vm->Largo_Y()-4,Gris1,Blanco,Gris9);
   Vm->Define_color_texto(Negro);
   Vm->Define_tipo_letra(2,0,2,2,2,2);
   Vm->Visualiza_texto(11,8,texto,0,Vm->Largo_X()-105);
   Vm->Define_tipo_letra(0,0,-1,0,0,0);
}


// Rutina que visualiza la hora del sistema
void Nucleo::Visualiza_hora(void) const
{
   Hora_actual(cad,3,1);
   Vm->Limpia_recuadro(Vm->Largo_X()-80,4,Vm->Largo_X()-5,Vm->Largo_Y()-4,Gris1,Blanco,Gris9);
   Vm->Marco(Vm->Largo_X()-80,4,Vm->Largo_X()-5,Vm->Largo_Y() -4,Blanco,Gris9);
   Vm->Define_tipo_letra(2,0,2,2,2,2);
   Vm->Define_color_texto(Blanco);
   Vm->Visualiza_texto(Vm->Largo_X()-69,8,cad);
   Vm->Define_color_texto(Rojo1);
   Vm->Visualiza_texto(Vm->Largo_X()-70,7,cad);
   Vm->Define_tipo_letra(0,0,-1,0,0,0);
}


// Administrador
void Nucleo::Administrador(void)
{
   // Actualiza el estado del teclado y el mouse
   Actualiza_teclado_mouse();
   #ifdef SALVA_PANTALLA
   // Actualiza el tiempo de activacion del salvado de pantalla
   if(Ismoving() || Tecla != -1) N_s_t = n_seg();
    else {
   #endif
      // Controla al administrador de impresion
      if(!Sw_impresora) {
         Sw_impresora = 1;
         if(Retona_estado_Administrador_impresion()) {
            Controlador_Administrador_impresion();
            //////////////////////////////////////////
            // Falta visualizar la pagina actual de impresion
            // Falta controlar la impresion cuando algo falla
            //   (Repetir pagina, continuar apartier de la pagina..)
            //////////////////////////////////////////
         }
         Sw_impresora = 0;
      }
      #ifdef SALVA_PANTALLA
      // Revisa activacion por desuso
      if(n_seg() > N_s_t + N_s_e) {
         Protector_pantalla();
         // Actualiza el tiempo
         N_s_t = n_seg();
      }
      #endif
      // Visualiza la hora
      if(sw == 1) Visualiza_hora();
   #ifdef SALVA_PANTALLA
   }
   #endif
   // Controla la visualizacion de la hora
   if(!(Numero_segundos() % 5)) sw ++;
    else sw = 0;

   // Hace visible el mouse
   Muestra_mouse(SI);
   // Cambia el tipo de cursor del mouse
   Selecciona_tipo_mouse(1);
}
/////////////////////////////////////////////
//  if(SISTEMA_OCUPADO) {
//     Proceso_ocupado
//   } else {
//     Proceso_no_ocupado
//     Visualiza_hora
//     Control_impresora
//     Protector_pantalla
//  }
/////////////////////////////////////////////



// Protector de pantalla
void Nucleo::Protector_pantalla(void)
{
   // Apaga el mouse
   Muestra_mouse(NO);
   // Asigna un nombre de archivo temporal
   char nom_tmp[MAXPATH];
   Nombre_archivo_temporal(nom_tmp);
   // Graba la pantalla
   graba_pantalla_grafica(0,0,X_MAX,Y_MAX,nom_tmp);
   // Limpia la pantalla
   cleardevice();
   // Hace visible el mouse
   Muestra_mouse(SI);
   // Ciclo del protector de pantalla
   while(Programa_activo) {
      // Actualiza el estado del teclado y el mouse
      Actualiza_teclado_mouse();
///////////////////////////////////
// Activar aqui la ventana de proteccion
///////////////////////////////////
      // Revisa si se a movido el mouse o tecleado algun caracter
      if(Ismoving() || Tecla != -1) break;
   }
   // Hace no visible el mouse
   Muestra_mouse(NO);
   // Lee la pantalla
   lee_pantalla_grafica(0,0,nom_tmp);

   #ifdef SALVA_PANTALLA

//   // Limpia la pantalla
//   clearviewport();
   // Inicializacion de variables
   randomize();
   unsigned int  j, count, color;
   long          k;
   double        auxiliar, xaux, yaux, angulo, ang, teta;
   complex       a, b, c, d, teta1, alfa, w, w1, valor, punto, aux, tempo;
   while(1) {
      count = random(32000);
      w1 = valor = punto = aux = complex(0.0, 0.0);
      alfa  = complex(0.9999 * cos(1.0), 0.9999 * sin(1.0));
      w     = conj(w1);
      auxiliar = 1 / (random(10) + 0.001);
      angulo = (double) (count);
      while(auxiliar > 0) {
         ang   = (double)(angulo + auxiliar);
         teta  = (double)(ang * M_PI / 180.0);
         teta1 = complex(0.0,teta);
         a = alfa * (exp(teta1) - 2.0 * w);
         b = alfa * (4.0 * w - 2.0 * exp(teta1) - w1 * exp(teta1) + 2.0);
         c = w; c *= -1.0;
         d = 1.0 + 2.0 * w;
         for(j = 0; j < 25 ; j++) {
            xaux = 3.0 * cos(2.0 * j * M_PI / 20.0);
            yaux = 3.0 * sin(2.0 * j * M_PI / 20.0);
            valor = complex( xaux, yaux);
            color = random(16);
            for(k = 0; k < 4000l; k++) {
               aux = valor - 2.0;
               if(norm( aux ) >= 1.0) punto = alfa * valor;
                else punto = (a * valor + b) / (c * valor + d);
               valor = punto;
               putpixel((X_MAX/2) + (int)(real(valor) * 110),(Y_MAX/2) - (int)(imag(valor) * 110),color);
            }
            // Lee el codigo del teclado
            Retorna_bufer_teclado(Caracter,Tecla);
            // Lee el mouse
            Mpos();
            // Revisa si se a movido el mouse o tecleado algun caracter
            if(Ismoving() || Tecla != -1) return;
         }
         auxiliar -= (1 / (random(100) + 0.0001));
      }
   }

   #endif
}


// Falta controlar procesos de maxima prioridad
