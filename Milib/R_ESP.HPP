//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//       Rutinas que permiten la captura de caracteres formateados          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////



// Autor:     Antonio Carrillo Ledesma
// Direcci¢n: Amsterdam 312, colonia Hipodromo Condesa
// Telefono:  574-43-53
// Propiedad intelectual, derechos reservados conforme a la ley (Registro en tramite)
// 1993 -1994



#ifndef __R_ESP_HPP__
#define __R_ESP_HPP__

#include "definic.hpp"


void Estado_sistema(void);
  // Visualiza el estatus del sistema
  // Nombre del archivo: ESD_SYS.CPP

void Calculadora(void);
  // Visualiza una ventana para escribir la expresion a ser evaluada
  // Soporta funciones trigonometricas
  // Nombre del archivo: CALC.CPP

void Calendario(const int tp, const char *xfecha);
  // Visualiza una ventana en la cual muestra el calendario del mes indicado por la cadena
  // de fecha pasada como parametro si tp es cero, si tp es distinto de cero asume la fecha 
  // actual del sistema
  // Nombre del archivo: CALEND.CPP

void Tabla_ascii(void);
  // Visualiza una ventana en la cual se visualiza la tabla del codigo ASCII
  // Nombre del archivo: TASCII.CPP
  
int Retorna_archivo_seleccionado(char *arch, const unsigned int tp, const char *tit, const char *ext = "", const char *path="*.*");
  // Retorna un nombre de archivo con la extencion dada, este archivo el
  // usuario lo selecciona de la ventana de archivos mostrada
  // Si TP es (0) No es necesario que el archivo exista
  //          (1) Es necesario que el archivo exista
  // Retorna  (0) Si no se realizo la seleccion
  //          (1) Si se realizo la seleccion
  // Nombre del archivo: SEL_ARCH.CPP

void V_archivo(const char *arch_hlp);
  // Rutina que visualiza el contenido del archivo ARCH_HLP
  // Nombre del archivo: VIS_AYU.CPP

void V_E_archivo(const char *arch, const char *tit, const int tp_edic, const int selecc);
  // Edita o visualiza un archivo de nombre ARCH, con titulo en la ventana se TIT
  // Si TP_EDIC es (EDITA) edita el archivo
  //               (VISUZALIZA) visualiza el archivo
  // SI Selecc es  (1) Presenta una ventana para seleecionar el archivo
  //               (0) Usa el archivo ARCH
  // Nombre del archivo: VE_ARCH.CPP

int Opcion_menu(const char *Arr_menu[], const char *opc, unsigned int &selec, const C_2I vent, const char *opc_des = "");
  // Retorna la opcion SELEC del menu pasado en ARR_MENU con numero de elementos NUM en la posicion VENT
  // Nombre del archivo: SEL_MEN.CPP


//////////////////////////////////////////////
// Rutinas de control de errores
/////////////////////////////////////////////

void Ctrl_error_memoria(void);
  // Rutina controladora de errores de falta de memoria para el NEW

int Ctrl_error_harware(int errval,int ax,int bp,int si);
  // Rutina que controla los mensajes de error en harware


int Captura_cadena(const char *titulo, const char *exp, char *cad, const int lg, const int tp);
   // Captura una cadena

int Captura_cadena(const char *titulo, const char *exp, long int &num);
   // Captura una cadena

int Captura_cadena(const char *titulo, const char *exp, double &num, const int tp);
   // Captura una cadena

int Captura_cadenas(const char *titulo, const char *exp1, char *cad1, const char *exp2, char * cad2, const int lg, const int tp);
   // Captura dos cadenas

int Captura_cadenas(const char *titulo, const char *exp1, double &num1, const char *exp2, double &num2, const int tp);
   // Captura dos cadenas

int Captura_cadenas(const char *titulo, const char *exp1, long int &num1, const char *exp2, long int &num2, const char *exp3, long int &num3);
   // Captura tres numeros long

int Captura_cadenas(const char *titulo, const char *exp1, double &num1, const char *exp2, double &num2, const char *exp3, double &num3, const int tp);
   // Captura tres numeros double

int Captura_cadenas(const char *titulo, const char *exp1, double &num1, const char *exp2, double &num2, const char *exp3, double &num3,const char *exp4, double &num4, const int tp);
   // Captura 4 numeros dobles


#endif
