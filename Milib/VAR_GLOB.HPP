// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.21-D


// Variables globales que se usan en el sistema



#ifndef __VAR_GLOB_HPP__
#define __VAR_GLOB_HPP__


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                       Datos Generales del autor                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

// Definicion de datos generales del autor
const char *N_AUTOR = "Antonio Carrillo Ledesma";                   // Nombre del autor
const char *D_AUTOR = "Amsterdam 312, Col. Hipodromo Condesa";      // TelŠfono del autor
const char *T_AUTOR = "(915) 5-74-43-53 MŠxico D.F.";               // Direcci¢n del autor
const char *E_AUTOR = "antonio@Dinamica2.fciencias.unam.mx";        // e-mail del autor


int  Programa_activo;             // Indica si el programa esta o no activo
int  SW_salida;                   // Variable de control de salida
int  Tecla;                       // Almacena la tecla presionada activa
char Caracter;                    // Almacena el ultimo caracter activo
unsigned int N_texto_explicativo; // Indica cual texto explicativo a visualizar


char *PATH_SISTEMA;               // Trayectoria donde se encuentran los archivos del sistema
char *PATH_ACTUAL;                // Trayectoria actual del sistema
char *PATH_ORIGINAL;              // Trayectoria original del sistema

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Variables que definen el formato de la cadena de fecha, usadas en las    //
// rutinas de manipulaci¢n de fechas                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


int  TIPO_FECHA = 0;              // Define el formato de la cadena fecha
                                  // si TIPO_FECHA es (0)  el formato es DD/MM/[CC]YY
                                  //                  (1)  el formato es MM/DD/[CC]YY
                                  //                  (2)  el formato es [CC]YY/MM/DD
                                  // [CC] existira si la variable CENTURIA es (1)

char DELIMIT_FECHA = '/';         // Define el caracter delimitador de la fecha
                                  // Puede ser:  diagonal (/)
                                  //               punto    (.)
                                  //               guion    (-)

int  CENTURIA = 1;                // Indica se esta activa la centuria en la cadena fecha
                                  // si CENTURIA es  (0)  no esta activa la centuria
                                  //                 (1)  si esta activa la centuria

char ACT_CENTURIA[] = "19";       // Indica la actual centuria




//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                      Variables de uso general                            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


// Variables generales
//int TIPO_CURSOR = 2;           // Indica el tipo de cursor activo en el sistema
//int N_CAR_LIN = 80;            // Indica el numero maximo de caracteres por linea de la pantalla
//int N_LIN_PANT = 25;           // Indica el numero maximo de lineas en la pantalla
//int DECIMALES = 2;             // Indica el numero de decimales que usara el sistema
//int MARGEN = 0;                // Indica el margen izquierdo en la generaci¢n de reportes
//int PUERTO_IMP = 0;            // Indica el puerto en el cual se generara el reporte impreso (0 - LPT1, 1 - LPT2,...)
//int ERR_SYS1 = 0;              // Indica la variable de error dentro del sistema
//int ERR_SYS2 = 0;              // Indica la variable de error dentro del sistema
//int ERR_SYS3 = 0;              // Indica la variable de error dentro del sistema

int ERROR_MATEMATICO;            // Indica el tipo de error en rutinas matematicas
int VISUALIZA_ERROR_MATEMATICO;  // Indica si se visualiza el error matematico

// Variables aplicables a bufers en el sistema
//const int MAX_BUFER_TECLADO = max_len + 2; // Indica la maxima longitud del bufer del teclado
//int  BUFER_TECLADO[MAX_BUFER_TECLADO];     // Almacena el bufer del teclado
//char CAD_TMP[max_len];                     // Contiene una cadena temporal para usos variados en el sistema
//char GET_ULT[max_len];                     // Contiene la cadena capturada en el ultimo Get
//char EXP_ULT[max_len];                     // Contiene la cadena de la ultima expresi¢n capturada en la calculadora
//char RES_EXP[80];                          // Contiene la cadena con el ultimo resultado de la expresi¢n calculada por la calculadora

// Variables aplicables a las alarmas
//long ALARMA[5];                            // Almacena las horas en la cual sonara la alarma
//char M_ALARMA[4][81];                      // Almacena los mensajes que seran visualizados cuando se active la alarma





//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//       Variables definidas para usarse en ambiente grafico                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

int X_MAX = 0;                    // Indica el maximo numero de pixes horizontales hay en ambiente grafico
int Y_MAX = 0;                    // Indica el maximo numero de pixes verticales hay en ambiente grafico
int MAX_COLORES = 0;              // Indica el maximo numero de colores en el sistema
int MODO_GRAFICO;                 // Indica el modo grafico   640  x 480 x 16    (0)
                                  //                          640  x 480 x 256   (2)
                                  //                          800  x 600 x 256   (3)
                                  //                          1024 x 768 x 256   (4)
int MODO_GRAFICO_ACTIVO = 0;      // Indica si el modo grafico esta activo (0) Modo texto
                                  //                                       (1) Modo Grafico




#endif
