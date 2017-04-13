// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-C

#include "cad_car.hpp"
#include "nucleo.hpp"
#include "v_iconos.hpp"
#include "teclado.hpp"
#include "r_esp.hpp"

extern "C" {
   #include <stdlib.h>
   #include <stdio.h>
   #include <graphics.h>
   #include <conio.h>
   #include <string.h>
   #include <dos.h>
}


// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;


#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "INFORMACION"
#else
   #define TXT1      "INFORMATION"
#endif




// Visualiza el mensaje de error para el manejo de harware
int Mensaje_error_harware(char *msg);


#define IGNORE       0
#define RETRY        1
#define ABORT        2


extern int MODO_GRAFICO_ACTIVO;


#ifdef _IDIOMA_ESPANOL_
   // Errores Generales
   static char *a_errores[] = {
     "ERROR AL ASIGNAR MEMORIA, NO HAY LA SUFICIENTE",  //  0  OK
     "ERROR EN LOS PARAMETROS DE LLAMADA",              //  1  OK
     "ERROR NO SE PUEDE CREAR EL ARCHIVO:",             //  2
     "ERROR EN LA ESCRITURA DEL EL ARCHIVO:",           //  3
     "ERROR NO SE PUEDE ABRIR EL ARCHIVO:",             //  4  OK
     "ERROR EN LA LECTURA DEL EL ARCHIVO:",             //  5
     "ERROR NO EXISTE EL ARCHIVO:",                     //  6  OK
     "ERROR NO SE PUEDE BORRAR EL ARCHIVO",             //  7  OK
     "ERROR EN LOS INDICES, CREELOS NUEVAMENTE",        //  8
     "ERROR EN EL FORMATO DE LA FECHA",                 //  9
     "ERROR NO SE PUEDE ACCESAR ESE SUBDIRECTORIO",     // 10
     "ERROR NO SE PUEDE ACCESAR ESA UNIDAD",            // 11
     "ERROR NO HAY DISKETTE EN LA UNIDAD:",             // 12  OK
     "ERROR NO SE PUEDE ASIGNAR EL BUFER DE E/S",       // 13
     "ERROR EL NOMBRE DE ARCHIVO NO ES VALIDO",         // 14  OK
     "ERROR LA OPERACI¢N NO ES VALIDA"                  // 15  OK
   };

   // Errores de harware
   static char *err_harwere[] = {
       "PROTECCION CONTRA ESCRITURA",
       "UNIDAD DESCONOCIDA",
       "DISCO NO LISTO",
       "COMANDO DESCONOCIDO",
       "ERROR EN DATOS (CRC)",
       "MALA PETICION",
       "ERROR EN BUSQUEDA",
       "DESCONOCIDO EL TIPO DE UNIDAD",
       "SECTOR NO ENCONTRADO",
       "FALTA PAPEL",
       "FALLA AL GRABAR",
       "FALLA AL LEER",
       "FALLA GENERAL",
       "RESERVADO",
       "RESERVADO",
       "INVALIDO EL CAMBIO DE DISCO"
   };
#else
   // Errores Generales
   static char *a_errores[] = {
     "ERROR AL ASIGNAR MEMORIA, NO HAY LA SUFICIENTE",  //  0  OK
     "ERROR EN LOS PARAMETROS DE LLAMADA",              //  1  OK
     "ERROR NO SE PUEDE CREAR EL ARCHIVO:",             //  2
     "ERROR EN LA ESCRITURA DEL EL ARCHIVO:",           //  3
     "ERROR NO SE PUEDE ABRIR EL ARCHIVO:",             //  4  OK
     "ERROR EN LA LECTURA DEL EL ARCHIVO:",             //  5
     "ERROR NO EXISTE EL ARCHIVO:",                     //  6  OK
     "ERROR NO SE PUEDE BORRAR EL ARCHIVO",             //  7  OK
     "ERROR EN LOS INDICES, CREELOS NUEVAMENTE",        //  8
     "ERROR EN EL FORMATO DE LA FECHA",                 //  9
     "ERROR NO SE PUEDE ACCESAR ESE SUBDIRECTORIO",     // 10
     "ERROR NO SE PUEDE ACCESAR ESA UNIDAD",            // 11
     "ERROR NO HAY DISKETTE EN LA UNIDAD:",             // 12  OK
     "ERROR NO SE PUEDE ASIGNAR EL BUFER DE E/S",       // 13
     "ERROR EL NOMBRE DE ARCHIVO NO ES VALIDO",         // 14  OK
     "ERROR LA OPERACI¢N NO ES VALIDA"                  // 15  OK
   };

   // Errores de harware
   static char *err_harwere[] = {
       "WRITE PROTECT",
       "UNKNOWN UNIT",
       "DRIVE NOT READY",
       "UNKNOWN COMMAND",
       "DATA ERROR (CRC)",
       "BAD REQUEST",
       "SEEK ERROR",
       "UNKNOWN MEDIA TYPE",
       "SECTOR NOT FOUND",
       "PRINTER OUT OF PAPER",
       "WRITE FAULT",
       "READ FAULT",
       "GENERAL FAILURE",
       "RESERVED",
       "RESERVED",
       "INVALID DISK CHANGE"
   };
#endif



// Visualiza una ventana con el texto indicado para el error
void Control_Errores::Visualiza_error(const unsigned int  t_p, const char *text1, const char *text2, const char *text3, const char *text4, const char *text5)
{
  if(MODO_GRAFICO_ACTIVO) {
     Mensaje(text1,text2,text3,text4,text5);
     if(t_p) {
        closegraph();
        exit(1);
     }
  } else printf("\n\n %s \n %s \n %s \n %s \n %s",text1,text2,text3,text4,text5);
}


// Controla y visualiza el error ocurrido
void Control_Errores::Ctrl_error(const int xn_e, const char *xt1, const char *xt2)
{
   int t_e = 0, n_e = xn_e;
   char xc2[100];//, xc1[100];
   sprintf(xc2,"En la rutina: %s",xt2);
   if(n_e <= 0) {
      n_e *= -1;
      t_e = 1;
   }
//   if(strlen(xt2) > 1) strcat(xc1,xt2);
//    else xc1[0] = 0;
//   if(n_e == 2 || n_e == 3 || n_e == 4 || n_e == 5 || n_e == 6 || n_e == 7) {
//      strcpy(xc2,xc1);
//      strcpy(xc1,xt1);
//   }
   if(n_e == 6 || n_e == 8) xc2[0] = 0;
   if(n_e == 7) {
      strcpy(xc2,"Se crearan los indices");
      n_e --;
   }
//   Visualiza_error(t_e,a_errores[n_e],xc1,xc2);
   Visualiza_error(t_e,a_errores[n_e],xt1,xc2);
}



// Presenta una ventana hasta con 5 lineas de texto
void Control_Errores::Mensaje(const char *txt1, const char *txt2, const char *txt3, const char *txt4, const char *txt5)
{
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Rojo1,Gris9,Blanco);
   Vt->Ventana_centrada(TXT1,400,190,0);
   Vt->Dibuja();
   Vt->Limpia_recuadro(20,50,380,170,Blanco,Negro,Negro);
   Vt->Define_color(Blanco,Negro);
   Vt->Centra_texto(70,txt1);
   Vt->Centra_texto(90,txt2);
   Vt->Centra_texto(110,txt3);
   Vt->Centra_texto(130,txt4);
   Vt->Centra_texto(150,txt5);
   while(Programa_activo) {
      NCO->Administrador();
      if(Tecla == ESC || Vt->Itera()) break;
   }
   delete Vt;
}


// Presenta una ventana hasta con 5 lineas de texto aceptando la opcion aceptar y Cancelar, retornando
// (1)   Aceptar
// (2)   Cancelar
// (3)   Cerrado de la ventana
int Control_Errores::Opcion(const char *txt1, const char *txt2, const char *txt3, const char *txt4, const char *txt5)
{
   unsigned int x1,y1,x2,y2,ct = 0;
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Rojo1,Gris9,Blanco);
   Vt->Cambia_tipo_letra_titulo(8,0,-1,0,0,0);
   Vt->Ventana_centrada(TXT1,400,215,0);
   Vt->Dibuja();
   Vt->Limpia_recuadro(10,45,390,150,Blanco,Negro,Negro);
   Vt->Define_color(Blanco,Negro);
   Vt->Centra_texto(60,txt1);
   Vt->Centra_texto(80,txt2);
   Vt->Centra_texto(100,txt3);
   Vt->Centra_texto(120,txt4);
   Vt->Centra_texto(140,txt5);
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Icono *Ic1 = new Icono;
   Ic1->Define_icono("OK.ICO",x1 + 50,y2 - 50);
   Ic1->Dibuja();
   Icono *Ic2 = new Icono;
   Ic2->Define_icono("CANCEL.ICO",x2 -86,y2 - 50);
   Ic2->Dibuja();
   while(!ct) {
      NCO->Administrador();
      if(Tecla == ESC || !Programa_activo) ct = 3;
      // Revisa el estado de los iconos
      if(Vt->Itera()) ct = 3;
      if(Ic1->Oprimido()) ct = 1;
      if(Ic2->Oprimido()) ct = 2;
   }
   delete Ic1;
   delete Ic2;
   delete Vt;
   // Retorna el estado de terminacion
   return ct;
}








// Rutina controladora de errores de falta de memoria para el NEW
void Ctrl_error_memoria(void)
{
  NCO->Ctrl_error(0);
}

// Controla los errores en harware
int Ctrl_error_harware(int errval,int ax,int bp,int si)
{
   char msg[80];
   unsigned int di;
   int drive, errorno;

   di= _DI;
   // if this is not a disk error then it was another device having trouble
   if (ax < 0) {
      // report the error
      Mensaje_error_harware("ERROR EN DISPOSITIVO");
      // and return to the program directly requesting abort
      hardretn(ABORT);
   }
   // otherwise it was a disk error
   drive = ax & 0x00FF;
   errorno = di & 0x00FF;
   // report which error it was
   sprintf(msg, "ERROR: %s EN UNIDAD %c",err_harwere[errorno], 'A' + drive);
   // return to the program via dos interrupt 0x23 with abort, retry or ignore as input by the user
   hardresume(Mensaje_error_harware(msg));
   return ABORT;
}






// Visualiza el mensaje de error para el manejo de harware
int Mensaje_error_harware(char *msg)
{
   int retval, st = 0;
   retval = NCO->Opcion("",msg);
   switch(retval) {
     case 1: st = RETRY;  break;
     case 2: st = ABORT;  break;
     case 3: st = IGNORE; break;
   }
   return st;
}

#include <math.h>

// Indica el tipo de error en rutinas matematicas
extern int ERROR_MATEMATICO;
// Indica si se visualiza el error matematico
extern int VISUALIZA_ERROR_MATEMATICO;  



// Controla parte de los errores matematicos
int _matherr (struct exception *a)
{
  if(a->type == DOMAIN) {
     if(VISUALIZA_ERROR_MATEMATICO) NCO->Mensaje("Error en el dominio de la funci¢n",a->name);
     ERROR_MATEMATICO = 2;
  }
  if(a->type == SING) {
     if(VISUALIZA_ERROR_MATEMATICO) NCO->Mensaje("Error en argumento, resulta una singularidad",a->name);
     ERROR_MATEMATICO = 3;
  }
  if(a->type == OVERFLOW) {
     if(VISUALIZA_ERROR_MATEMATICO) NCO->Mensaje("Error el resultado es un numero muy grande",a->name);
     ERROR_MATEMATICO = 4;
  }
  if(a->type == UNDERFLOW) {
     if(VISUALIZA_ERROR_MATEMATICO) NCO->Mensaje("Error el resultado es un numero muy peque¤o",a->name);
     ERROR_MATEMATICO = 5;
  }
  if(a->type == TLOSS) {
     if(VISUALIZA_ERROR_MATEMATICO) NCO->Mensaje("Error hay perdida de cifras significativas",a->name);
     ERROR_MATEMATICO = 6;
  }

//    if (!strcmp(a->name,"sqrt")) {
//      a->retval = sqrt (-(a->arg1));
//      return 1;
//    }
  a->retval = 1;
  return 1;
}


// Calcula el inverso de X controlando el error de la division por cero 
double inv(const double x)
{
   if(x != 0.0) return (1.0/x); // retorna el inverso del numero X
   ERROR_MATEMATICO = 1;        // Division por cero
   if(VISUALIZA_ERROR_MATEMATICO) NCO->Mensaje("Se ha hecho una divisi¢n entre cero");
   return 1.0;
}


// Retorna (1)   Si X >= 1
//         (-1)  Si X <  0
double sig(const double x)
{
   if(x >= 0.0) return 1.0;
   return -1.0;
}

