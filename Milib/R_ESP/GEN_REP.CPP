// Autor:     Antonio Carrillo Ledesma
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.3-A




// Clase Ctrl_reporte para generar e imprimir un reporte de N_COL caracteres
// de ancho y N_LIN de largo de pagina, y con el caracter de control CAR_CTRL.


// Al acabar de imprimir una pagina mande el mensaje de la pagina
// actual usando Visualiza_texto(const char *texto); del nucleo


#include "gen_rep.hpp"
#include "nucleo.hpp"
#include "r_esp.hpp"
#include "r_var.hpp"
#include "vis_ayud.hpp"
#include "v_iconos.hpp"
#include "r_var.hpp"
#include "teclado.hpp"


extern "C" {
   #include <string.h>
   #include <stdlib.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "REPORTE GRABADO"
   #define TXT2      "Nombre"
   #define TXT4      "REPORTE VISUALIZADO"
   #define TXT5      "Imprimiendo pagina numero %d, presione ESC para cancelar"
   #define TXT6      "REPORTE IMPRESO"
   #define TXT7      "OPCIONES"
   #define TXT8      "Longitud de linea muy grande"
   #define TXT9      "Columna fuera de secuencia"
   #define TXT10     "¨Desea cancelar la impresi¢n?"
   #define TXT11     "Visualizando el reporte"
#else
   #define TXT1      "SAVE PRINT"
   #define TXT2      "Name"
   #define TXT4      "VIEW PRINT"
   #define TXT5      "Printing page number %d, press ESC to cancel"
   #define TXT6      "PRINT"
   #define TXT7      "OPTIONS"
   #define TXT8      "Line to long"
   #define TXT9      "Column out secuence"
   #define TXT10     "Do cancel report?"
   #define TXT11     "View print"
#endif


// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;


void Ctrl_reporte::Parametros(const unsigned int margen, const unsigned int renglo, const unsigned int colum, const unsigned int t_rep, const unsigned int puerto, const char *archivo)
{
   // Inicializa las variables
   int Nombre_solicitado = 0;
   Graba = 0;
   // Asignacion de variables
   MARGEN = margen, N_RENGLONES = renglo, N_COLUMNAS = colum, PUERTO = puerto;
   // Definicion de las cadenas de trabajo
   x_arr1 = new char [N_COLUMNAS+10];
   x_arr2 = new char [N_COLUMNAS+10];
   x_arr3 = new char [N_COLUMNAS+10];
   xcad   = new char [80];

   // Define el tipo de reporte a generar
   if(t_rep == CONFIGURACION) {
      T_REPORTE = Menu();
      if(T_REPORTE == CANCELADO) {
         ERROR = 1;
         return;
      }
      if(T_REPORTE == REPORTE_GRABADO) {
         char xarch[65];
         xarch[0] = 0;
         if(Captura_cadena(TXT1,TXT2,xarch,60,1) == 1) {
            N_archivo = new char [strlen(xarch) + 1];
            strcpy(N_archivo,xarch);
            Nombre_solicitado = 1;
          } else {
            ERROR = 1; // Terminar impresi¢n
            return;
         }
      }
   } else T_REPORTE = t_rep;

   switch(T_REPORTE) {
       case REPORTE_IMPRESO: // El reporte es impreso
       case REPORTE_VISUALIZADO: // El reporte sera visualizado
          N_archivo = new char [15];
          strcpy(N_archivo,"ARCH_PRN.$$$");
          Gr = new Manipulador_archivos;
          Gr->Parametros(N_archivo,GRABA_ARCHIVO,N_COLUMNAS,"GENERA REPORTE");
          break;
       case REPORTE_GRABADO: // El reporte sera grabado
          if(!Nombre_solicitado) {
             N_archivo = new char [strlen(archivo) + 1];
             strcpy(N_archivo,archivo);
          }
          Gr = new Manipulador_archivos;
          Gr->Parametros(N_archivo,GRABA_ARCHIVO,N_COLUMNAS,"GENERA REPORTE");
   }
   // Genera el mensaje de que pagina se esta imprimiendo
   sprintf(xcad,TXT5,Pagina);
   if(N_RENGLONES) NCO->Visualiza_texto(xcad);
}


Ctrl_reporte::~Ctrl_reporte()
{
   if(Graba) Graba_arreglo();
   if(Gr) delete Gr;
   delete []x_arr3;
   delete []x_arr1;
   delete []x_arr2;
   delete []xcad;
   if(!ERROR) {
      if(T_REPORTE == REPORTE_VISUALIZADO) {
         NCO->Visualiza_texto(TXT11);
         Vis_archivo *Ve = new Vis_archivo;
         Ve->Parametros(N_archivo,TXT4,500,400,VISUALIZA);
         Ve->Loop();
         delete Ve;
      }
      if(T_REPORTE == REPORTE_IMPRESO) {
         // Manda el archivo grabado a impresion (via administrador de impresion)
         NCO->Parametros_Administrador_impresion(N_archivo,PUERTO);
      }
   }
   delete []N_archivo;
}


// Formatea la salida para la impresi¢n
void Ctrl_reporte::Formato_impresion(const unsigned int x, const unsigned int y, const char *text) const
{
   if(ERROR) return;
   ((Ctrl_reporte*)this)->lg = strlen(text);

   if(y) {
      if(Graba) Graba_arreglo();
      for(((Ctrl_reporte*)this)->i = 1; ((Ctrl_reporte*)this)->i < y; ((Ctrl_reporte*)this)->i++) {
         Caracter_ctrl(13);
         Caracter_ctrl(10);
         ((Ctrl_reporte*)this)->Renglon ++;
      }
   }

   // Control de paginacion
   if(N_RENGLONES && Renglon > N_RENGLONES) Libera_pagina();


   if(!Graba) {
      for(((Ctrl_reporte*)this)->i = 0; ((Ctrl_reporte*)this)->i < N_COLUMNAS; ((Ctrl_reporte*)this)->i++) ((Ctrl_reporte*)this)->x_arr3[i] = 32;
      ((Ctrl_reporte*)this)->x_arr3[N_COLUMNAS] = 0, ((Ctrl_reporte*)this)->Columna = 1;
   }

   if(x + lg > N_COLUMNAS) {
      if(Graba) Graba_arreglo();
      NCO->Mensaje("",TXT8,text);
    } else {
      if(Columna > x) {
         if(Graba) Graba_arreglo();
         NCO->Mensaje("",TXT9,text);
       } else {
         for(((Ctrl_reporte*)this)->i = 0; ((Ctrl_reporte*)this)->i < lg; ((Ctrl_reporte*)this)->i++) ((Ctrl_reporte*)this)->x_arr3[MARGEN + x + i] = text[i];
         ((Ctrl_reporte*)this)->Graba = 1, ((Ctrl_reporte*)this)->Columna = x + i;
      }
   }
}


// Libera la actual pagina de impresion
void Ctrl_reporte::Libera_pagina(void) const
{
   // Graba la ultima linea generada
   if(Graba) Graba_arreglo();
   // Abanza el numero de lineas necesarias para hacer cambio de pagina
   for(((Ctrl_reporte*)this)->i = Renglon; ((Ctrl_reporte*)this)->i <= 66u; ((Ctrl_reporte*)this)->i++) {
      Caracter_ctrl(13);
      Caracter_ctrl(10);
   }
   // Asigna los valores corespondientes para enpezar en la nueva pagina
   ((Ctrl_reporte*)this)->Graba = 0,((Ctrl_reporte*)this)->Columna = ((Ctrl_reporte*)this)->Renglon = 1, ((Ctrl_reporte*)this)->Pagina++;
   // Genera el mensaje de que pagina se esta imprimiendo
   sprintf(xcad,TXT5,Pagina);
   NCO->Visualiza_texto(xcad);
}


// Graba o imprime la salida de un caracter
void Ctrl_reporte::Caracter_ctrl(const char car) const
{
   if(ERROR) return;
   Gr->Graba_caracter(car);
}


// Graba o imprime una linea de texto
void Ctrl_reporte::Graba_arreglo(void) const
{
   if(ERROR) return;
//////////////////////////////////////////////////////////////////
// Se elimina la suspencion del reporte por tecla ESC, esto lo debera
// hacer el propio programa
//   // ESC Solicitud de cancelacion del reporte
//   if(NCO->Busca_bufer_teclado(ESC)) {
//      if(NCO->Opcion("",TXT10) == 1) ((Ctrl_reporte*)this)->ERROR = 1;
//   }
//////////////////////////////////////////////////////////////////   
   ((Ctrl_reporte*)this)->i = Lg_cadena_sin_espacios_final(x_arr3);
   ((Ctrl_reporte*)this)->x_arr3[i] = 13, ((Ctrl_reporte*)this)->x_arr3[i+1] = 10, ((Ctrl_reporte*)this)->x_arr3[i+2] = 0;
   Gr->Graba_caracteres(x_arr3,i+2);
   // Asigna los valores corespondientes para enpezar en la nueva linea
   ((Ctrl_reporte*)this)->Graba = 0,((Ctrl_reporte*)this)->Columna = 1;
   ((Ctrl_reporte*)this)->Renglon ++;
}


// Menu de seleccion del tipo de reporte a generar (Impreso, Grabado o Visualizado)
unsigned int Ctrl_reporte::Menu(void)
{
   unsigned int x1, x2, y1, y2, ct = CANCELADO;
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(TXT7,300,210,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Define_color_texto(Negro);
   Vt->Visualiza_texto(100,60,TXT6);
   Vt->Visualiza_texto(100,110,TXT1);
   Vt->Visualiza_texto(100,160,TXT4);
   Icono *Ic1 = new Icono;
   Icono *Ic2 = new Icono;
   Icono *Ic3 = new Icono;
   // Definicion de los botones
   Ic1->Define_icono("PRINTER.ICO",x1 + 30,y1 +50);
   Ic1->Dibuja();
   Ic2->Define_icono("SAVE.ICO",x1 + 30,y1 +100);
   Ic2->Dibuja();
   Ic3->Define_icono("VISUALIZ.ICO",x1 + 30,y1 +150);
   Ic3->Dibuja();
   while(ct == CANCELADO) {
      NCO->Administrador();
      if(!Programa_activo) break;
      if(Vt->Itera() || Tecla == ESC) break;
      if(Ic1->Oprimido()) ct = REPORTE_IMPRESO;
      if(Ic2->Oprimido()) ct = REPORTE_GRABADO;
      if(Ic3->Oprimido()) ct = REPORTE_VISUALIZADO;
   }
   delete Ic1;
   delete Ic2;
   delete Ic3;
   delete Vt;
   return ct;
}

