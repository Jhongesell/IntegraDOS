// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#include "ctrl_est.hpp"
#include "nucleo.hpp"
#include "r_var.hpp"
#include "cad_car.hpp"
#include "\libreria\man_arch.hpp"
#include "r_esp.hpp"
#include "edit_arc.hpp"
#include "\libreria\gen_rep.hpp"
#include "botontxt.hpp"
#include "vis_ayud.hpp"
#include "menu.hpp"
#include "barr_ind.hpp"
#include "vis_arch.hpp"
#include "\libreria\ana_exp.hpp"
#include "teclado.hpp"

// Definicion externa del objeto Nucleo
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;


extern "C" {
   #include <stdio.h>
   #include <dir.h>
   #include <io.h>
}



#ifdef _IDIOMA_ESPANOL_
   // Submenu de edicion
   static const char *M_edicion[] = {
      "Grabar Como ...",
      "Lee Archivo",
      "Imprime Contenido",
      "Copia al Portapapeles",
      "Copia del Portapapeles",
      "0"
   };

   static const char *C_M_edicion = "GLICP";


   #define TXT1      "Acepta"
   #define TXT2      "Cancela"
   #define OPC1      "AC"
   #define TXT3      "ERRORES"
   #define TXT4      "GRABA PROYECTO"
   #define TXT5      "Nombre del proyecto"
   #define TXT6      "Lee proyecto"
   #define TXT7      "DESCRIPCION DEL PROYECTO"
   #define TXT8      "DESCRIPCION DEL SISTEMA"
   #define TXT9      "ECUACIONES"
   #define TXT10     "PARAMETROS"
   #define TXT11     "FUNCIONES AUXILIARES"
   #define TXT12     "GENERA PROGRAMA EJECUTABLE"
   #define TXT13     "Generando programa C++"
   #define TXT14     "Generando archivo EXE"
   #define TXT15     "Compilando..."
   #define TXT15a    "(este proceso tarda varios segundos)"
   #define TXT16     "   + Termin¢ satisfactoriamente"
   #define TXT17     "Ligando ..."
   #define TXT18     "Archivo ejecutable generado con ‚xito,"
   #define TXT18a    "para usarlo salga de este programa,"
   #define TXT18b    "cambie de subdirectorio usando:"
   #define TXT18c    "   CD EXES"
   #define TXT18d    "y despu‚s teclee el nombre del archivo:"
   #define TXT19     "   + Hay errores"
   #define TXT20     "Existen muchos par metros"
   #define TXT22     "Existen muchas ecuaciones"
   #define TXT23     "El proyecto:"
   #define TXT24     "no tiene sistemas, cree al menos uno"
   #define TXT25     "El sistema:"
   #define TXT26     "no tiene ecuaciones"
   #define TXT27     "El nombre del sistema ya existe"
   #define TXT28     "El nombre del proyecto ya existe"
   #define TXT29     "Error, deben definirse por sistema"
   #define TXT30     "un m¡nimo dos ecuaciones diferenciales"
   #define TXT31     "Tiene m s ecuaciones de las permitidas"
   #define TXT32     "Para la versi¢n de prueba"
   #define TXT33     "%d es igual a la %d"
   #define TXT34     "El nombre de la expresi¢n"
   #define TXT40     "Si desea una versi¢n con m s"
   #define TXT41     "capacidad solicitela a:"
   #define TXT42     "integra@Dinamica2.fciencias.unam.mx"
   #define TXT50     "El contenido del directorio es:"
#else
   // Submenu de edicion
   static const char *M_edicion[] = {
      "Grabar Como ...",
      "Lee Archivo",
      "Imprime Contenido",
      "Copia al Portapapeles",
      "Copia del Portapapeles",
      "0"
   };

   static const char *C_M_edicion = "GLICP";


   #define TXT1      "Accept"
   #define TXT2      "Cancel"
   #define OPC1      "AC"
   #define TXT3      "ERRORS"
   #define TXT4      "SAVE PROJECT"
   #define TXT5      "Proyect name"
   #define TXT6      "Open proyect"
   #define TXT7      "PROJECT DESCRIPTION"
   #define TXT8      "SYSTEM DESCRIPTION"
   #define TXT9      "EQUATIONS"
   #define TXT10     "PARAMETRES"
   #define TXT11     "AUXILIARY FUNCTIONS"
   #define TXT12     "BUILT EXECUTABLE PROGRAM"
   #define TXT13     "Built program c++"
   #define TXT14     "Built EXE file"
   #define TXT15     "Compiling..."
   #define TXT15a    "(this process takes a few minutes)"
   #define TXT16     "   + Ends accordingly"
   #define TXT17     "Linking ..."
   #define TXT18     "Executable file built successfully,"
   #define TXT18a    "to use it exit this program,"
   #define TXT18b    "change directory using:"
   #define TXT18c    "   CD EXES"
   #define TXT18d    "after type file name:"
   #define TXT19     "   + There are errors"
   #define TXT20     "There are many parametres"
   #define TXT22     "There are many equations"
   #define TXT23     "The project:"
   #define TXT24     "No systems, build at least one"
   #define TXT25     "The system:"
   #define TXT26     "Has no equations"
   #define TXT27     "There is such system name"
   #define TXT28     "There is suche project name"
   #define TXT29     "Error, should be efined by system"
   #define TXT30     "Two differential equations minimum"
   #define TXT31     "More equations than those allowed"
   #define TXT32     "For the test version"
   #define TXT33     "%d equal to %d"
   #define TXT34     "expression name"
   #define TXT40     "Si desea una versi¢n con m s"
   #define TXT41     "capacity apply to:"
   #define TXT42     "integra@Dinamica2.fciencias.unam.mx"
   #define TXT50     "Directory content is:"
#endif




// Indica que borre el archivo CPP generado
#define BORRA_CPP
#define TAMANO_MAXIMO_CADENA 1000

// Configuracion de EXES de integra via interfaz
static char *Var_config[] = {
   "V1_DIMENSION",        // Dimension de la ventana (todos los ejes)
   "V1_DIMENSION_X",      // Dimension del eje X
   "V1_DIMENSION_Y",      // Dimension del eje Y
   "V1_DIMENSION_Z",      // Dimension del eje Z
   "V1_DIMENSION_X_I",    // Dimension del eje X minimo
   "V1_DIMENSION_X_F",    // Dimension del eje X maximo
   "V1_DIMENSION_Y_I",    // Dimension del eje Y minimo
   "V1_DIMENSION_Y_F",    // Dimension del eje Y maximo
   "V1_DIMENSION_Z_I",    // Dimension del eje Z minimo
   "V1_DIMENSION_Z_F",    // Dimension del eje Z maximo
   "V2_DIMENSION",        // Dimension de la ventana (todos los ejes)
   "V2_DIMENSION_X",      // Dimension del eje X
   "V2_DIMENSION_Y",      // Dimension del eje Y
   "V2_DIMENSION_Z",      // Dimension del eje Z
   "V2_DIMENSION_X_I",    // Dimension del eje X minimo
   "V2_DIMENSION_X_F",    // Dimension del eje X maximo
   "V2_DIMENSION_Y_I",    // Dimension del eje Y minimo
   "V2_DIMENSION_Y_F",    // Dimension del eje Y maximo
   "V2_DIMENSION_Z_I",    // Dimension del eje Z minimo
   "V2_DIMENSION_Z_F",    // Dimension del eje Z maximo
   "V1_ESCENARIO_X",      // Escenario X
   "V1_ESCENARIO_Y",      // Escenario Y
   "V1_ESCENARIO_Z",      // Escenario Z
   "V2_ESCENARIO_X",      // Escenario X
   "V2_ESCENARIO_Y",      // Escenario Y
   "V2_ESCENARIO_Z",      // Escenario Z
   "0"
};


// Dimensiones de la ventana 1
static char *Var_config0[] = {
   "V1_DIMENSION",        // Dimension de la ventana (todos los ejes)
   "V1_DIMENSION_X",      // Dimension del eje X
   "V1_DIMENSION_Y",      // Dimension del eje Y
   "V1_DIMENSION_Z",      // Dimension del eje Z
   "V1_DIMENSION_X_I",    // Dimension del eje X minimo
   "V1_DIMENSION_X_F",    // Dimension del eje X maximo
   "V1_DIMENSION_Y_I",    // Dimension del eje Y minimo
   "V1_DIMENSION_Y_F",    // Dimension del eje Y maximo
   "V1_DIMENSION_Z_I",    // Dimension del eje Z minimo
   "V1_DIMENSION_Z_F",    // Dimension del eje Z maximo
   "0"
};

// Dimensiones de la ventana 2
static char *Var_config1[] = {
   "V2_DIMENSION",        // Dimension de la ventana (todos los ejes)
   "V2_DIMENSION_X",      // Dimension del eje X
   "V2_DIMENSION_Y",      // Dimension del eje Y
   "V2_DIMENSION_Z",      // Dimension del eje Z
   "V2_DIMENSION_X_I",    // Dimension del eje X minimo
   "V2_DIMENSION_X_F",    // Dimension del eje X maximo
   "V2_DIMENSION_Y_I",    // Dimension del eje Y minimo
   "V2_DIMENSION_Y_F",    // Dimension del eje Y maximo
   "V2_DIMENSION_Z_I",    // Dimension del eje Z minimo
   "V2_DIMENSION_Z_F",    // Dimension del eje Z maximo
   "0"
};

// Escenarios de la ventana 1
static char *Var_config2[] = {
   "V1_ESCENARIO_X",      // Escenario X
   "V1_ESCENARIO_Y",      // Escenario Y
   "V1_ESCENARIO_Z",      // Escenario Z
   "0"
};

// Escenarios de la ventana 2
static char *Var_config3[] = {
   "V2_ESCENARIO_X",      // Escenario X
   "V2_ESCENARIO_Y",      // Escenario Y
   "V2_ESCENARIO_Z",      // Escenario Z
   "0"
};




// Permite captura las ecuaciones del sistema e introducirlo al arreglo de trabajo
int Ctrl_estructura::Captura_ecuacion(const char *tit, const int Opcion_actual_menu)
{
   unsigned int x1, y1, x2, y2, xi, e_a = 1;
   int sw,i,xtc,lg,tipo, ecu, par, fau, ct = 0, i_e, i_p, i_f, err = 0;
   char xcad[400], xcad2[400];
   const char *pt_texto;

   Ventana_iconos  *Vt  = new Ventana_iconos;
   Boton_texto     *Bt1 = new Boton_texto;
   Boton_texto     *Bt2 = new Boton_texto;
   Traza_indicador *Hor = new Traza_indicador;
   Traza_indicador *Ver = new Traza_indicador;
   Edita_archivo   *Ea  = new Edita_archivo;
   Controlador_movimiento *Cm = new Controlador_movimiento;

   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(tit,500,370,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Ver->Define_indicador(x2-61,y1+50,x2-61,y2-90,1);
   Ver->Dibuja();
   Hor->Define_indicador(x1+20,y2-91,x2-60,y2-91,0);
   Hor->Dibuja();
   Ea->Parametros(x1+21,y1+51,x2-61,y2-91,"");

   // Carga el texto a editar
   for(i_e = 0; i_e < Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Ecuaciones; i_e++) Ea->Adiciona_linea(1,Informacion->Inf_sistema[Opcion_actual_menu]->Ecuaciones[i_e],2);
   if(Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Ecuaciones) Ea->Adiciona_linea(1," ",2);
   for(i_p = 0; i_p < Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Parametros; i_p++) Ea->Adiciona_linea(1,Informacion->Inf_sistema[Opcion_actual_menu]->Parametros[i_p],2);
   if(Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Parametros) Ea->Adiciona_linea(1," ",2);
   for(i_f = 0; i_f < Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Func_aux; i_f++) Ea->Adiciona_linea(1,Informacion->Inf_sistema[Opcion_actual_menu]->Func_aux[i_f],2);

   Ea->Adiciona_linea(1," ",2);
   Ea->Activa_sw_grabar(0);
   Ea->Revisualiza_pantalla();
   Bt1->Define_boton(TXT1,x1+20,y2-40);
   Bt1->Dibuja();
   Bt2->Define_boton(TXT2,x2-104,y2-40);
   Bt2->Dibuja();
   // Objeto controlador de movimiento
   Cm->Parametros(2,2,OPC1,"");
   Cm->Fija_elemento_actual(e_a);
   while(!ct) {
     NCO->Administrador();
     if(!Programa_activo) break;
     xtc = 0;
     sw = Ver->Controlador(Ea->Linea_actual(),Ea->Total_lineas(),Ea->Lineas_desplegar());
     if(sw == 1) xtc = FLECHA_ARRIBA;
     if(sw == 2) xtc = FLECHA_ABAJO;
     // Barra controladora vertical
     sw = Hor->Controlador(Ea->Caracter_actual(),Ea->Caracteres_desplegar(),Ea->Caracteres_linea());
     if(sw == 1) xtc = FLECHA_IZQUIERDA;
     if(sw == 2) xtc = FLECHA_DERECHA;

      // Activa el menu de edicion
      if(NCO->Retorna_estado_mouse().button == BL) {
         vent.y = NCO->Retorna_estado_mouse().y;
         vent.x = NCO->Retorna_estado_mouse().x;
         // Menu de de edicion
         if(Ea->Ventana::Mouse_dentro()) {
            if(Opcion_menu(M_edicion,C_M_edicion,selec,vent)) {
               switch(selec) {
                  case 1: xtc = CTRL_F_W; break; // Grabar como ...
                  case 2: xtc = CTRL_K_R; break; // Lee archivo
                  case 3: xtc = CTRL_F_P; break; // Imprime el archivo
                  case 4: xtc = CTRL_F_C; break; // Copia el archivo al portapapeles
                  case 5: xtc = CTRL_V;   break; // Copia del portapapeles al archivo
               }
            }
         }
      }


     if(xtc) Ea->Controlador(Caracter,xtc);
     // Controla el movimiento de visualizacion del archivo
     if(Bt1->Mouse_dentro() || Bt2->Mouse_dentro()) {
        ct = Cm->Horizontal(Tecla,Caracter); // Control de movimiento de los botones
        e_a = Cm->Elemento_actual();
     } else Ea->Controlador(Caracter,Tecla);
     // Revisa el estado de los botones
     if(Vt->Itera()) ct = 3;
     if(Bt1->Oprimido() || ct == 1) e_a = ct = 1;
     if(Bt2->Oprimido() || ct == 2) e_a = ct = 2;
     // Actualiza el boton activo
     Bt1->Activar(e_a == 1);
     Bt2->Activar(e_a == 2);
     // Desactiva la salida por Esc
     if(ct == -1) ct = 0;
     if(ct == 1) {
        fau = ecu = par = err = 0;
        // Analizar la informacion capturada
        Analizador_sintactico *Aexp = new Analizador_sintactico;
        for(i = 0; i < Ea->Total_lineas(); i++) {
           strcpy(xcad,Ea->Retorna_contenido_linea(i+1));
           s_nocar(xcad,32);
           if(!xcad[0]) continue;
           // Analiza la estructura de la expresion
           Aexp->Crea_arbol(xcad);
           // Indica el tipo de expresion de la que se trate
           tipo = Aexp->Tipo_expresion(xcad);

           // Extrae el nombre del parametro
           xi = 0;
           while(xcad[xi] != '=') {
             xcad2[xi] = xcad[xi];
             xi ++;
           }
           xcad2[xi] = 0;
           xi = 0;
           // Revisa si es una expresion de configuracion del archivo EXE
           while(Var_config[xi][0] != '0') {
              if(Compara(xcad2,Var_config[xi])) {
                 tipo = 1;
                 break;
              }
              xi++;
           }

           switch(tipo) {
               case 1: fau ++; break; // Funciones auxiliares
               case 2: par ++; break; // Parametros
               case 4: ecu ++; break; // Ecuaciones diferenciales
           }
           // Revisa si existieron errores al analizar la expresion
           if(Aexp->Numero_errores()) err = 2;
        }

        // Revisa que exista almenos dos ecuaciones direfenciales
        if(ecu && ecu < 2) {
           NCO->Mensaje(TXT29,TXT30);
           err = 1;
        }
        // Revisa que no se exedan los maximos definidos para las ecuaciones
        if(ecu > NUMERO_ECUACIONES) {
           NCO->Mensaje(TXT22);
           NCO->Mensaje(TXT40,TXT41,TXT42);
           err = 1;
        }

        // Revisa que no se exedan los maximos definidos para los parametros
        if(par > NUMERO_MAX_PARAMETROS) {
           NCO->Mensaje(TXT20);
           NCO->Mensaje(TXT40,TXT41,TXT42);
           err = 1;
        }


        ///////////////////////////////////////////////////////////////////////
        // Valida por sistema que:                                          //
        //    Ecuaciones que sean unicas                                     //
        //    parametros que sean unicas                                     //
        //    funciones auxiliares que sean unicas                           //
        ///////////////////////////////////////////////////////////////////////
        if(!err) {
           unsigned int i1, i2, pos;
           char tcad1[100], tcad2[100], msg[200];
           // Toma linea por linea y extrae el nombre de la expresion
           for(i1 = 0; i1 < Ea->Total_lineas(); i1++) {
              strcpy(xcad,Ea->Retorna_contenido_linea(i1+1));
              s_nocar(xcad,32);
              if(!xcad[0]) continue;
              // Descarta las funciones auxiliares a graficar
              if(xcad[0] == 'F' && xcad[1] == '(' && xcad[3] == ')' && xcad[4] == '=') continue;
              // Extrae el nombre de la expresion
              if(Busca_caracter(xcad,'=',pos)) {
                 Substr(xcad,0,pos,tcad1);
                 // Lo compara con el resto de las expresiones
                 for(i2 = 0; i2 < Ea->Total_lineas(); i2++) {
                    if(i1 == i2) continue;
                    strcpy(xcad,Ea->Retorna_contenido_linea(i2+1));
                    s_nocar(xcad,32);
                    if(!xcad[0]) continue;
                    // Extrae el nombre de la expresion
                    if(Busca_caracter(xcad,'=',pos)) {
                       Substr(xcad,0,pos,tcad2);
                       // Compara si son iguales
                       if(Compara(tcad1,tcad2)) {
                          sprintf(msg,TXT33,i1+1,i2+1);
                          NCO->Mensaje(TXT34,msg);
                          err = 3;
                       }
                    }
                 }
              }
           }
        }

        // Visualiza los errores del analizador de expresiones
        if(err == 2) {
           Vis_archivo *Ve = new Vis_archivo;
           Ve->Parametros(" ",TXT3,500,400,VISUALIZA);
           for(i = 0; i < Ea->Total_lineas(); i++) {
               pt_texto = Ea->Retorna_contenido_linea(i+1);
               if(Lg_cadena_sin_espacios_final(pt_texto)) {
                  Aexp->Crea_arbol(pt_texto);
                  if(Aexp->Numero_errores()) {
                     Ve->Adiciona_linea(pt_texto);
                     for(int xxi = 0; xxi < Aexp->Numero_errores(); xxi++) {
                        Aexp->Retorna_error(xxi);
                        sprintf(xcad2,"   + %s",Aexp->Retorna_error(xxi));
                        Ve->Adiciona_linea(xcad2);
                     }
                     Ve->Adiciona_linea("");
                  }
               }
           }
           Ve->Loop();
           delete Ve;
        }
        // No hay errores 
        if(!err) {
           // Pasa la informacion capturada a las respectivas estructuras de datos
           if(i_e) {
              // Borra la informacion anterior del arreglo de trabajo
              for(i = 0; i < i_e; i++) delete []Informacion->Inf_sistema[Opcion_actual_menu]->Ecuaciones[i];
              delete []Informacion->Inf_sistema[Opcion_actual_menu]->Ecuaciones;
           }
           if(i_p) {
              // Borra la informacion anterior del arreglo de trabajo
              for(i = 0; i < i_p; i++) delete []Informacion->Inf_sistema[Opcion_actual_menu]->Parametros[i];
              delete []Informacion->Inf_sistema[Opcion_actual_menu]->Parametros;
           }
           if(i_f) {
              // Borra la informacion anterior del arreglo de trabajo
              for(i = 0; i < i_f; i++) delete []Informacion->Inf_sistema[Opcion_actual_menu]->Func_aux[i];
              delete []Informacion->Inf_sistema[Opcion_actual_menu]->Func_aux;
           }

           // Solicirta la memoria necesaria para contener el arreglo de trabajo
           Informacion->Inf_sistema[Opcion_actual_menu]->Ecuaciones = new char *[ecu];
           Informacion->Inf_sistema[Opcion_actual_menu]->Parametros = new char *[par];
           Informacion->Inf_sistema[Opcion_actual_menu]->Func_aux = new char *[fau];
           // Inserta la nueva informacion al arreglo de trabajo
           i_e = 0, i_p = 0, i_f = 0;
           for(i = 0; i < Ea->Total_lineas(); i++) {
              strcpy(xcad,Ea->Retorna_contenido_linea(i+1));
              s_nocar(xcad,32);
              if(!xcad[0]) continue;
              lg = strlen(xcad) + 1;
              tipo = Aexp->Tipo_expresion(xcad);

              // Extrae el nombre del parametro
              xi = 0;
              while(xcad[xi] != '=') {
                xcad2[xi] = xcad[xi];
                xi ++;
              }
              xcad2[xi] = 0;
              xi = 0;
              // Revisa si es una expresion de configuracion del archivo EXE
              while(Var_config[xi][0] != '0') {
                 if(Compara(xcad2,Var_config[xi])) {
                    tipo = 1;
                    break;
                 }
                 xi++;
              }

              switch(tipo) {
                case 1: // Funciones auxiliares
                  Informacion->Inf_sistema[Opcion_actual_menu]->Func_aux[i_f] = new char[lg];
                  strcpy(Informacion->Inf_sistema[Opcion_actual_menu]->Func_aux[i_f],xcad);
                  i_f ++;
                  break;
                case 2: // Parametros
                  Informacion->Inf_sistema[Opcion_actual_menu]->Parametros[i_p] = new char[lg];
                  strcpy(Informacion->Inf_sistema[Opcion_actual_menu]->Parametros[i_p],xcad);
                  i_p ++;
                  break;
                case 4: // Ecuaciones diferenciales
                  Informacion->Inf_sistema[Opcion_actual_menu]->Ecuaciones[i_e] = new char[lg];
                  strcpy(Informacion->Inf_sistema[Opcion_actual_menu]->Ecuaciones[i_e],xcad);
                  i_e ++;
                  break;
              }
           }
           // Actualiza el numero de lineas de la descripcion en el sistema
           Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Ecuaciones = i_e;
           Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Parametros = i_p;
           Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Func_aux   = i_f;
           Informacion->Modificada = 1;
        } else ct = 0;
        delete Aexp;
     }
   }

   delete Cm;
   delete Ea;
   delete Hor;
   delete Ver;
   delete Bt1;
   delete Bt2;
   delete Vt;
   return ct;
}

// Permite capturar la descripcion e introducirlo al arreglo de trabajo
int Ctrl_estructura::Captura_descripcion(const char *tit, const int tp, const int Opcion_actual_menu)
{
   unsigned int x1,y1,x2,y2;
   int sw,i,xtc,i1,xi,lg;
   unsigned int e_a = 1;
   int ct = 0;
   char xcad[400];

   Ventana_iconos  *Vt = new Ventana_iconos;
   Boton_texto     *Bt1 = new Boton_texto;
   Boton_texto     *Bt2 = new Boton_texto;
   Traza_indicador *Hor = new Traza_indicador;
   Traza_indicador *Ver = new Traza_indicador;
   Edita_archivo   *Ea  = new Edita_archivo;
   Controlador_movimiento *Cm = new Controlador_movimiento;


   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(tit,500,370,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Ver->Define_indicador(x2-61,y1+50,x2-61,y2-90,1);
   Ver->Dibuja();
   Hor->Define_indicador(x1+20,y2-91,x2-60,y2-91,0);
   Hor->Dibuja();
   Ea->Parametros(x1+21,y1+51,x2-61,y2-91,"");
   // Carga el texto a editar
   if(tp) i1 = Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Descripcion;
    else i1 = Informacion->N_e_Descripcion;
   if(i1) {
      for(i = 0; i < i1; i++) {
         if(tp) Ea->Adiciona_linea(1,Informacion->Inf_sistema[Opcion_actual_menu]->Descripcion[i],2);
          else Ea->Adiciona_linea(1,Informacion->Descripcion[i],2);
      }
   } else Ea->Adiciona_linea(1," ",2);
   Ea->Activa_sw_grabar(0);
   Ea->Revisualiza_pantalla();
   Bt1->Define_boton(TXT1,x1+20,y2-40);
   Bt1->Dibuja();
   Bt2->Define_boton(TXT2,x2-104,y2-40);
   Bt2->Dibuja();
   // Objeto controlador de movimiento
   Cm->Parametros(2,2,OPC1,"");
   Cm->Fija_elemento_actual(e_a);
   while(!ct) {
     NCO->Administrador();
     if(!Programa_activo) break;
     xtc = 0;
     sw = Ver->Controlador(Ea->Linea_actual(),Ea->Total_lineas(),Ea->Lineas_desplegar());
     if(sw == 1) xtc = FLECHA_ARRIBA;
     if(sw == 2) xtc = FLECHA_ABAJO;
     // Barra controladora vertical
     sw = Hor->Controlador(Ea->Caracter_actual(),Ea->Caracteres_desplegar(),Ea->Caracteres_linea());
     if(sw == 1) xtc = FLECHA_IZQUIERDA;
     if(sw == 2) xtc = FLECHA_DERECHA;
     if(xtc) Ea->Controlador(Caracter,xtc);

      // Activa el menu de edicion
      if(NCO->Retorna_estado_mouse().button == BL) {
         vent.y = NCO->Retorna_estado_mouse().y;
         vent.x = NCO->Retorna_estado_mouse().x;
         // Menu de de edicion
         if(Ea->Ventana::Mouse_dentro()) {
            if(Opcion_menu(M_edicion,C_M_edicion,selec,vent)) {
               switch(selec) {
                  case 1: xtc = CTRL_F_W; break; // Grabar como ...
                  case 2: xtc = CTRL_K_R; break; // Lee archivo
                  case 3: xtc = CTRL_F_P; break; // Imprime el archivo
                  case 4: xtc = CTRL_F_C; break; // Copia el archivo al portapapeles
                  case 5: xtc = CTRL_V; break;   // Copia del portapapeles al archivo
               }
            }
         }
      }


     // Controla el movimiento de visualizacion del archivo
     if(Bt1->Mouse_dentro() || Bt2->Mouse_dentro()) {
        ct = Cm->Horizontal(Tecla,Caracter); // Control de movimiento de los botones
        e_a = Cm->Elemento_actual();
     } else Ea->Controlador(Caracter,Tecla);
     // Revisa el estado de los botones
     if(Vt->Itera()) ct = 3;
     if(Bt1->Oprimido() || ct == 1) e_a = ct = 1;
     if(Bt2->Oprimido() || ct == 2) e_a = ct = 2;
     // Actualiza el boton activo
     Bt1->Activar(e_a == 1);
     Bt2->Activar(e_a == 2);
     // Desactiva la salida por Esc
     if(ct == -1) ct = 0;
     if(ct == 1) {
        if(ct) {
           if(i1) {
              // Borra la informacion anterior del arreglo de trabajo
              for(i = 0; i < i1; i++) {
                 if(tp) delete []Informacion->Inf_sistema[Opcion_actual_menu]->Descripcion[i];
                  else delete []Informacion->Descripcion[i];
              }
              if(tp) delete []Informacion->Inf_sistema[Opcion_actual_menu]->Descripcion;
               else delete []Informacion->Descripcion;
           }
           // Solicirta la memoria necesaria para contener el arreglo de trabajo
           lg = Ea->Total_lineas();
           if(tp) Informacion->Inf_sistema[Opcion_actual_menu]->Descripcion = new char *[lg];
            else Informacion->Descripcion = new char *[lg];
           // Inserta la nueva informacion al arreglo de trabajo
           xi = 0;
           for(i = 0; i < Ea->Total_lineas(); i++) {
              strcpy(xcad,Ea->Retorna_contenido_linea(i+1));
              if(tp > 1) {
                 s_nocar(xcad,32);
                 if(!xcad[0]) continue;
              }
              lg = strlen(xcad) + 1;
              if(tp) {
                 Informacion->Inf_sistema[Opcion_actual_menu]->Descripcion[i] = new char[lg];
                 strcpy(Informacion->Inf_sistema[Opcion_actual_menu]->Descripcion[i],xcad);
               } else {
                 Informacion->Descripcion[i] = new char[lg];
                 strcpy(Informacion->Descripcion[i],xcad);
              }
              xi ++;
           }
           // Actualiza el numero de lineas de la descripcion en el sistema
           if(tp) Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Descripcion = xi;
            else Informacion->N_e_Descripcion = xi;
           Informacion->Modificada = 1;
        }
     }
   }

   delete Cm;
   delete Ea;
   delete Hor;
   delete Ver;
   delete Bt1;
   delete Bt2;
   delete Vt;
   return ct;
}



// Borra el actual proyecto de memoria
void Ctrl_estructura::Borra_proyecto(void)
{
   if(!Informacion) return;
   unsigned int i;
   // Borra el nombre del proyecto
   if(Informacion->Nombre) delete []Informacion->Nombre;
   Informacion->Nombre = NULL;

   // Borra la descripcion de proyecto
   if(Informacion->N_e_Descripcion) {
      for(i = 0; i < Informacion->N_e_Descripcion; i++) delete []Informacion->Descripcion[i];
      delete []Informacion->Descripcion;
   }
   Informacion->Descripcion = NULL;
   Informacion->N_e_Descripcion = 0;

   // Borra los sistemas del proyecto
   for(i = 0; i < Informacion->Numero_sistemas; i++) Borra_sistema(i);
   Informacion->Inf_sistema = NULL;
   Informacion->Numero_sistemas = 0;
   delete Informacion;
}


// Borra el actual proyecto de memoria
void Ctrl_estructura::Borra_sistema(const unsigned int n_s)
{
   unsigned int xi;
   if(Informacion->Numero_sistemas < 0) return;
   // Borra el nombre del sistema
   delete []Informacion->Inf_sistema[n_s]->Nombre;

   if(Informacion->Inf_sistema[n_s]->N_e_Descripcion) {
      // Borra la descricion del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[n_s]->N_e_Descripcion; xi++) delete []Informacion->Inf_sistema[n_s]->Descripcion[xi];
      delete []Informacion->Inf_sistema[n_s]->Descripcion;
   }

   if(Informacion->Inf_sistema[n_s]->N_e_Ecuaciones) {
      // Borra las ecuaciones del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[n_s]->N_e_Ecuaciones; xi++) delete []Informacion->Inf_sistema[n_s]->Ecuaciones[xi];
      delete []Informacion->Inf_sistema[n_s]->Ecuaciones;
   }

   if(Informacion->Inf_sistema[n_s]->N_e_Parametros) {
      // Borra los parametros del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[n_s]->N_e_Parametros; xi++) delete []Informacion->Inf_sistema[n_s]->Parametros[xi];
      delete []Informacion->Inf_sistema[n_s]->Parametros;
   }

   if(Informacion->Inf_sistema[n_s]->N_e_Func_aux) {
      // Lee las funciones auxiliares del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[n_s]->N_e_Func_aux; xi++) delete []Informacion->Inf_sistema[n_s]->Func_aux[xi];
      delete []Informacion->Inf_sistema[n_s]->Func_aux;
   }
   delete Informacion->Inf_sistema[n_s];
   Informacion->Inf_sistema[n_s] = NULL;
   Informacion->Numero_sistemas --;
   Informacion->Modificada = 1;
}



// Duplica el actual sistema
void Ctrl_estructura::Duplica_sistema(const unsigned int n_s, const char *nombre)
{
   if(Informacion->Numero_sistemas >= NUMERO_SISTEMAS) return;
   unsigned int i, xi;
   char xcad[400];
   i = Informacion->Numero_sistemas++;
   // Genera la estructura para contener el sistema
   Informacion->Inf_sistema[i] = new Informacion_sistema;
   Informacion->Inf_sistema[i]->Nombre = new char[LG_MAX_NOM_SISTEMA+1];
   strcpy(Informacion->Inf_sistema[i]->Nombre,nombre);
   // Descripcion del sistema
   Informacion->Inf_sistema[i]->N_e_Descripcion = Informacion->Inf_sistema[n_s]->N_e_Descripcion;
   if(Informacion->Inf_sistema[i]->N_e_Descripcion) {
      Informacion->Inf_sistema[i]->Descripcion = new char *[Informacion->Inf_sistema[i]->N_e_Descripcion];
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Descripcion; xi++) {
          strcpy(xcad,Informacion->Inf_sistema[n_s]->Descripcion[xi]);
          Informacion->Inf_sistema[i]->Descripcion[xi] = new char[strlen(xcad)+1];
          strcpy(Informacion->Inf_sistema[i]->Descripcion[xi],xcad);
      }
   } else Informacion->Inf_sistema[i]->Descripcion = NULL;

   // Ecuaciones
   Informacion->Inf_sistema[i]->N_e_Ecuaciones = Informacion->Inf_sistema[n_s]->N_e_Ecuaciones;
   if(Informacion->Inf_sistema[i]->N_e_Ecuaciones) {
      Informacion->Inf_sistema[i]->Ecuaciones = new char *[Informacion->Inf_sistema[i]->N_e_Ecuaciones];
      // Lee las ecuaciones del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Ecuaciones; xi++) {
         strcpy(xcad,Informacion->Inf_sistema[n_s]->Ecuaciones[xi]);
         Informacion->Inf_sistema[i]->Ecuaciones[xi] = new char[strlen(xcad)+1];
         strcpy(Informacion->Inf_sistema[i]->Ecuaciones[xi],xcad);
      }
   } else Informacion->Inf_sistema[i]->Ecuaciones = NULL;

   // Parametros del sistema
   Informacion->Inf_sistema[i]->N_e_Parametros= Informacion->Inf_sistema[n_s]->N_e_Parametros;
   if(Informacion->Inf_sistema[i]->N_e_Parametros) {
      Informacion->Inf_sistema[i]->Parametros = new char *[Informacion->Inf_sistema[i]->N_e_Parametros];
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Parametros; xi++) {
          strcpy(xcad,Informacion->Inf_sistema[n_s]->Parametros[xi]);
          Informacion->Inf_sistema[i]->Parametros[xi] = new char[strlen(xcad)+1];
          strcpy(Informacion->Inf_sistema[i]->Parametros[xi],xcad);
      }
   } else Informacion->Inf_sistema[i]->Parametros = NULL;
   // Funciones auxiliares del sistema
   Informacion->Inf_sistema[i]->N_e_Func_aux = Informacion->Inf_sistema[n_s]->N_e_Func_aux;
   if(Informacion->Inf_sistema[i]->N_e_Func_aux) {
      Informacion->Inf_sistema[i]->Func_aux = new char *[Informacion->Inf_sistema[i]->N_e_Func_aux];
      // Lee las funciones auxiliares del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Func_aux; xi++) {
          strcpy(xcad,Informacion->Inf_sistema[n_s]->Func_aux[xi]);
          Informacion->Inf_sistema[i]->Func_aux[xi] = new char[strlen(xcad)+1];
          strcpy(Informacion->Inf_sistema[i]->Func_aux[xi],xcad);
      }
   } else Informacion->Inf_sistema[i]->Func_aux = NULL;
   Informacion->Modificada = 1;
}





// Revisa si se graba el proyecto, si TP es cero graba el proyecto, si no, solo graba si se modifico este
void Ctrl_estructura::Grabar(const int tp)
{
   if(!Informacion) return;
   if(tp && !Informacion->Modificada) return;
   char cad[MAXPATH];
   strcpy(cad,Informacion->Nombre);
   if(Captura_cadena(TXT4,TXT5,cad,20,1) == 1) {
      char xcad[MAXPATH];
      Manipulador_archivos ma;
      ma.Cambia_ext_path(cad,"PRY",xcad);
      strcpy(Informacion->Nombre,xcad);
      Graba_proyecto();
   }
   Informacion->Modificada = 0;
}



// Graba el proyecto con el nombre Informacion->NOMBRE
void Ctrl_estructura::Graba_proyecto(void)
{
   int i, xi;
   char xnombre[MAXPATH];
   sprintf(xnombre,"%s%s",PATH_PROYECTOS,Informacion->Nombre);
   Manipulador_archivos *Gp = new Manipulador_archivos;
   Gp->Parametros(xnombre,GRABA_ARCHIVO,200,TXT4);
   // Graba el nombre del proyecto
   Gp->Graba_linea(Informacion->Nombre);
   // Graba el numero de lineas de la descripcion del proyecto
   Gp->Graba_entero(Informacion->N_e_Descripcion);
   // Graba la descripcion de proyecto
   for(i = 0; i < Informacion->N_e_Descripcion; i++) Gp->Graba_linea(Informacion->Descripcion[i]);
   // Graba el numero de sistemas
   Gp->Graba_entero(Informacion->Numero_sistemas);
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      // Graba el nombre del sistema
      Gp->Graba_linea(Informacion->Inf_sistema[i]->Nombre);
      // Graba el numero de lineas de la descripcion del sistema
      Gp->Graba_entero(Informacion->Inf_sistema[i]->N_e_Descripcion);
      // Graba la descricion del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Descripcion; xi++) Gp->Graba_linea(Informacion->Inf_sistema[i]->Descripcion[xi]);
      // Graba el numero de lineas de las ecuasciones
      Gp->Graba_entero(Informacion->Inf_sistema[i]->N_e_Ecuaciones);
      // Graba las ecuaciones del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Ecuaciones; xi++) Gp->Graba_linea(Informacion->Inf_sistema[i]->Ecuaciones[xi]);
      // Graba el numero de parametros del sistema
      Gp->Graba_entero(Informacion->Inf_sistema[i]->N_e_Parametros);
      // Graba los parametros del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Parametros; xi++) Gp->Graba_linea(Informacion->Inf_sistema[i]->Parametros[xi]);
      // Graba el numero de funciones auxiliares del sistema
      Gp->Graba_entero(Informacion->Inf_sistema[i]->N_e_Func_aux);
      // Graba las funciones auxiliares del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Func_aux; xi++) Gp->Graba_linea(Informacion->Inf_sistema[i]->Func_aux[xi]);
   }
   delete Gp;
}


// Lee el proyecto de nombre NOMBRE 
// Nota: El nombre del proycto no posee extencion
void Ctrl_estructura::Lee_proyecto(const char *nombre)
{
   if(nombre[0] == 32) return;
   int i, xi;
   char xcad[210];
   // Borra el actual proyecto
   Borra_proyecto();
   // Solicita espacio para el proyecto
   Informacion = new Informacion_proyecto;
   // Inicializa la estructura que contendra a los sistemas
   Informacion->Inf_sistema = new Informacion_sistema *[NUMERO_SISTEMAS];
   for(i = 0; i < NUMERO_SISTEMAS; i++) Informacion->Inf_sistema[i] = NULL;
   Informacion->Numero_sistemas = 0;
   Informacion->Modificada = 0;
   Informacion->Nombre = new char [strlen(nombre)+1];
   s_trim(nombre,Informacion->Nombre);
   // Descarta subdirectorios
   if(Informacion->Nombre[0] == '[') {
      char temppath[MAXPATH];
      char tmpbusq[MAXPATH];
      char tmparch[MAXPATH];
      char temp[100];

      unsigned int i, i1;
      // Nombre del directorio
      strcpy(temp,Informacion->Nombre);
      temp[0] = 32;
      temp[12] = 32;
      s_trim(temp);
      // Nueva trayectoria
      sprintf(temppath,"%s%s\\",PATH_PROYECTOS,temp);
      // Trayectorias para proyectos y directorios
      sprintf(tmpbusq,"%s*.PRY",temppath);   
      sprintf(tmparch,"%s*.*",temppath);

      // Numero de directorios y proyectos
      Informacion->N_e_Descripcion = n_archivos_directorios(tmpbusq,FA_ARCH) + 2;
      Informacion->N_e_Descripcion += n_archivos_directorios(tmparch,FA_DIREC);

      // Introduce la descripcion del directorio
      Informacion->Descripcion = new char *[Informacion->N_e_Descripcion];
      strcpy(xcad,TXT50);
      Informacion->Descripcion[0] = new char[strlen(xcad)+1];
      strcpy(Informacion->Descripcion[0],xcad);
      Informacion->Descripcion[1] = new char[2];
      strcpy(Informacion->Descripcion[1]," ");
      i = 2;
      // Carga el nombre de los proyectos
      for(i1 = 0; i1 < n_archivos_directorios(tmpbusq,FA_ARCH); i1++) {
         // Obtiene el nombre del proyecto
         nombre_archivo_directorio(tmpbusq, FA_ARCH,!i1,xcad);
         // Copia el nombre de proyecto
         Informacion->Descripcion[i] = new char[strlen(xcad)+1];
         strcpy(Informacion->Descripcion[i],xcad);
         i++;

      }

      // Carga el nombre de los directorios
      for(i1 = 0; i1 < n_archivos_directorios(tmparch,FA_DIREC); i1++) {
         // Obtiene el nombre del archivo
         nombre_archivo_directorio(tmparch,FA_DIREC,!i1,xcad);
         // Copia el nombre del directorio
         sprintf(temp,"[%s]",xcad);
         Informacion->Descripcion[i] = new char[strlen(temp)+1];
         strcpy(Informacion->Descripcion[i],temp);
         i++;

      }
      return;
   }
   Manipulador_archivos *La = new Manipulador_archivos;
   char xnombre[MAXPATH];
   sprintf(xnombre,"%s%s.PRY",PATH_PROYECTOS,nombre);
   La->Parametros(xnombre,LEE_ARCHIVO,200,TXT6);
   // Lee el nombre del proyecto
   La->Lee_linea(xcad);
// No se toma en cuenta el nombre del proyecto grabado solo el del nombre de archivo
//   Informacion->Nombre = new char [strlen(nombre)+1]; // xcad
//   s_trim(nombre,Informacion->Nombre); // xcad
   // Lee el numero de lineas de la descripcion del proyecto
   La->Lee_entero(Informacion->N_e_Descripcion);
   // Lee la descripcion de proyecto
   if(Informacion->N_e_Descripcion) {
      Informacion->Descripcion = new char *[Informacion->N_e_Descripcion];
      for(i = 0; i < Informacion->N_e_Descripcion; i++) {
          La->Lee_linea(xcad);
          Informacion->Descripcion[i] = new char[strlen(xcad)+1];
          strcpy(Informacion->Descripcion[i],xcad);
      }
   } else Informacion->Descripcion = NULL;
   // Lee el numero de sistemas
   La->Lee_entero(Informacion->Numero_sistemas);
   if(Informacion->Numero_sistemas) {
      // Adiciona un nuevo sistema al proyecto
      for(i = 0; i < Informacion->Numero_sistemas; i++) {
        // Genera la estructura para contener el sistema
        Informacion->Inf_sistema[i] = new Informacion_sistema;
        // Lee el nombre del sistema
        La->Lee_linea(xcad);
        Informacion->Inf_sistema[i]->Nombre = new char[LG_MAX_NOM_SISTEMA+1];
        strcpy(Informacion->Inf_sistema[i]->Nombre,xcad);
        // Lee el numero de lineas de la descripcion del sistema
        La->Lee_entero(Informacion->Inf_sistema[i]->N_e_Descripcion);
        if(Informacion->Inf_sistema[i]->N_e_Descripcion) {
           // Lee la descricion del sistema
           Informacion->Inf_sistema[i]->Descripcion = new char *[Informacion->Inf_sistema[i]->N_e_Descripcion];
           for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Descripcion; xi++) {
               La->Lee_linea(xcad);
               Informacion->Inf_sistema[i]->Descripcion[xi] = new char[strlen(xcad)+1];
               strcpy(Informacion->Inf_sistema[i]->Descripcion[xi],xcad);
           }
        } else Informacion->Inf_sistema[i]->Descripcion = NULL;
        // Lee el numero de ecuaciones del sistema
        La->Lee_entero(Informacion->Inf_sistema[i]->N_e_Ecuaciones);
        if(Informacion->Inf_sistema[i]->N_e_Ecuaciones) {
           Informacion->Inf_sistema[i]->Ecuaciones = new char *[Informacion->Inf_sistema[i]->N_e_Ecuaciones];
           // Lee las ecuaciones del sistema
           for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Ecuaciones; xi++) {
               La->Lee_linea(xcad);
               Informacion->Inf_sistema[i]->Ecuaciones[xi] = new char[strlen(xcad)+1];
               strcpy(Informacion->Inf_sistema[i]->Ecuaciones[xi],xcad);
           }
        } else Informacion->Inf_sistema[i]->Ecuaciones = NULL;
        // Lee el numero de parametros del sistema
        La->Lee_entero(Informacion->Inf_sistema[i]->N_e_Parametros);
        if(Informacion->Inf_sistema[i]->N_e_Parametros) {
           Informacion->Inf_sistema[i]->Parametros = new char *[Informacion->Inf_sistema[i]->N_e_Parametros];
           // Lee los parametros del sistema
           for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Parametros; xi++) {
               La->Lee_linea(xcad);
               Informacion->Inf_sistema[i]->Parametros[xi] = new char[strlen(xcad)+1];
               strcpy(Informacion->Inf_sistema[i]->Parametros[xi],xcad);
           }
        } else Informacion->Inf_sistema[i]->Parametros = NULL;
        // Lee el numero de funciones auxiliares del sistema
        La->Lee_entero(Informacion->Inf_sistema[i]->N_e_Func_aux);
        if(Informacion->Inf_sistema[i]->N_e_Func_aux) {
           Informacion->Inf_sistema[i]->Func_aux = new char *[Informacion->Inf_sistema[i]->N_e_Func_aux];
           // Lee las funciones auxiliares del sistema
           for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Func_aux; xi++) {
               La->Lee_linea(xcad);
               Informacion->Inf_sistema[i]->Func_aux[xi] = new char[strlen(xcad)+1];
               strcpy(Informacion->Inf_sistema[i]->Func_aux[xi],xcad);
           }
        } else Informacion->Inf_sistema[i]->Func_aux = NULL;
      }
   }
   delete La;
}




// Imprime el proyecto
void Ctrl_estructura::Imprime_proyecto(void)
{
   unsigned int i, xi;
   Ctrl_reporte *Ip = new Ctrl_reporte;
   Ip->Parametros(0,60,120,CONFIGURACION,LPT1,"");
   // Imprime el nombre del proyecto
   Ip->Formato_impresion(1,0,Informacion->Nombre);
   // Imprime la descripcion de proyecto
   Ip->Formato_impresion(1,3,TXT7);
   for(i = 0; i < Informacion->N_e_Descripcion; i++) {
       Ip->Formato_impresion(5,1,Informacion->Descripcion[i]);
   }
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      // Imprime el nombre del sistema
      Ip->Formato_impresion(1,3,Informacion->Inf_sistema[i]->Nombre);
      if(Informacion->Inf_sistema[i]->N_e_Descripcion) Ip->Formato_impresion(1,3,TXT8);
      // Imprime la descricion del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Descripcion; xi++) {
          Ip->Formato_impresion(5,1,Informacion->Inf_sistema[i]->Descripcion[xi]);
      }
      Ip->Formato_impresion(1,2,TXT9);
      // Imprime las ecuaciones del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Ecuaciones; xi++) {
          Ip->Formato_impresion(5,1,Informacion->Inf_sistema[i]->Ecuaciones[xi]);
      }
      if(Informacion->Inf_sistema[i]->N_e_Parametros) Ip->Formato_impresion(1,2,TXT10);
      // Imprime los parametros del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Parametros; xi++) {
          Ip->Formato_impresion(5,1,Informacion->Inf_sistema[i]->Parametros[xi]);
      }
      if(Informacion->Inf_sistema[i]->N_e_Func_aux) Ip->Formato_impresion(1,2,TXT11);
      // Imprime las funciones auxiliares del sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Func_aux; xi++) {
          Ip->Formato_impresion(5,1,Informacion->Inf_sistema[i]->Func_aux[xi]);
      }
   }
   delete Ip;
}

// Imprime el sistema indicado
void Ctrl_estructura::Imprime_sistema(const unsigned int pos)
{
   unsigned int xi;
   Ctrl_reporte *Ip = new Ctrl_reporte;
   Ip->Parametros(0,60,120,CONFIGURACION,LPT1,"");
   // Imprime el nombre del sistema
   Ip->Formato_impresion(1,0,Informacion->Inf_sistema[pos]->Nombre);
   if(Informacion->Inf_sistema[pos]->N_e_Descripcion) Ip->Formato_impresion(1,3,TXT8);
   // Imprime la descricion del sistema
   for(xi = 0; xi < Informacion->Inf_sistema[pos]->N_e_Descripcion; xi++) {
       Ip->Formato_impresion(5,1,Informacion->Inf_sistema[pos]->Descripcion[xi]);
   }
   Ip->Formato_impresion(1,2,TXT9);
   // Imprime las ecuaciones del sistema
   for(xi = 0; xi < Informacion->Inf_sistema[pos]->N_e_Ecuaciones; xi++) {
       Ip->Formato_impresion(5,1,Informacion->Inf_sistema[pos]->Ecuaciones[xi]);
   }
   if(Informacion->Inf_sistema[pos]->N_e_Parametros) Ip->Formato_impresion(1,2,TXT10);
   // Imprime los parametros del sistema
   for(xi = 0; xi < Informacion->Inf_sistema[pos]->N_e_Parametros; xi++) {
       Ip->Formato_impresion(5,1,Informacion->Inf_sistema[pos]->Parametros[xi]);
   }
   if(Informacion->Inf_sistema[pos]->N_e_Func_aux) Ip->Formato_impresion(1,2,TXT11);
   // Imprime las funciones auxiliares del sistema
   for(xi = 0; xi < Informacion->Inf_sistema[pos]->N_e_Func_aux; xi++) {
       Ip->Formato_impresion(5,1,Informacion->Inf_sistema[pos]->Func_aux[xi]);
   }
   delete Ip;
}



// Genera archivo ejecutable
void Ctrl_estructura::Generar(void)
{
   // Valida que exista almenos un sistema en el proyecto
   if(!Informacion->Numero_sistemas) {
      NCO->Mensaje(TXT23,Informacion->Nombre,TXT24);
      return;
   } else {
      for(int i = 0; i < Informacion->Numero_sistemas; i++) {
          // Valida que los sistemas tengan ecuaciones
          if(!Informacion->Inf_sistema[i]->N_e_Ecuaciones) {
             NCO->Mensaje(TXT25,Informacion->Inf_sistema[i]->Nombre,TXT26);
             return;
          }
          // Valida que no existan mas ecuaciones que las permitidas por la version
          if(Informacion->Inf_sistema[i]->N_e_Ecuaciones >= NUMERO_ECUACIONES) {
             NCO->Mensaje(TXT25,Informacion->Inf_sistema[i]->Nombre,TXT31,TXT32);
             return;
          }
      }
   }



   // Visualiza los procesos intermedios para la generacion del archivo EXE
   Vis_archivo *Ve = new Vis_archivo;
   Ve->Parametros(" ",TXT12,400,300,VISUALIZA);
   Ve->Adiciona_linea(TXT13);
   Ve->Revisualiza();
   // Genera programa C++
   Genera_cpp();
   // Genera el archivo EXE
   Ve->Adiciona_linea(TXT14);
   Ve->Adiciona_linea(TXT15);
   Ve->Adiciona_linea(TXT15a);
   Ve->Revisualiza();
   // Genera exe
   char xcomando[300];
   char archivo[MAXPATH];
   Manipulador_archivos ma;
   ma.Cambia_ext_path(Informacion->Nombre,"CPP",archivo);
   if(n_archivos_directorios("C:\\INTEGRA\\SYS\\INTEGRA.486",FA_ARCH)) sprintf(xcomando,"BCC +C:\\INTEGRA\\SYS\\CON_COPR.CFG %s GRAPHICS.LIB LIBRERIA.LIB INTEGRA.LIB",archivo);
    else sprintf(xcomando,"BCC +C:\\INTEGRA\\SYS\\SIN_COPR.CFG %s GRAPHICS.LIB LIBRERIA.LIB INTEGRA.LIB EMU.LIB",archivo);
   // Redirecciona la salida stduot al archivo SALIDA.$$$
   freopen("SALIDA.$$$", "w", stdout);
   system(xcomando);
   // Redirecciona la salida stduot
   fclose(stdout);
   // Revisa si existe el archivo OBJ
   ma.Cambia_ext_path(Informacion->Nombre,"OBJ",archivo);
   if(n_archivos_directorios(archivo,FA_ARCH)) {
      // Se encontro el archivo OBJ
      Ve->Adiciona_linea(TXT16);
      Ve->Adiciona_linea(TXT17);
      Ve->Revisualiza();
      // Borrar el archivo obj
      remueve_archivos(archivo);
      // Revisa si existe el archivo EXE
      ma.Cambia_ext_path(Informacion->Nombre,"EXE",archivo);
      if(n_archivos_directorios(archivo,FA_ARCH)) {
         // Se encontro el archivo EXE
         Ve->Adiciona_linea(TXT16);
         Ve->Adiciona_linea(" ");
         Ve->Adiciona_linea(" ");
         Ve->Adiciona_linea(TXT18);
         Ve->Adiciona_linea(TXT18a);
         Ve->Adiciona_linea(TXT18b);
         Ve->Adiciona_linea(TXT18c);
         Ve->Adiciona_linea(TXT18d);
         char xnombre[MAXPATH];
         sprintf(xnombre,"   %s 0",archivo);
         Ve->Adiciona_linea(xnombre);
         sprintf(xnombre,"C:\\INTEGRA\\EXES\\%s",archivo);
         unlink(xnombre);
         rename(archivo,xnombre);
         // Borrar el archivo CPP
         ma.Cambia_ext_path(Informacion->Nombre,"CPP",archivo);
         sprintf(xnombre,"C:\\INTEGRA\\%s",archivo);
         // Borra el archivo .CPP
         #ifdef BORRA_CPP
            unlink(xnombre);
         #endif
         // Genera archivos de descripcion
         Genera_descripcion();
         Ve->Adiciona_linea(" ");
       } else {
         Ve->Adiciona_linea(TXT19);
         // Leer el archivo de errores
         Ve->Lee_archivo("SALIDA.$$$");
      }
    } else {
      // No se encontro el archivo OBJ
      Ve->Adiciona_linea(TXT19);
      // Leer el archivo de errores
      Ve->Lee_archivo("SALIDA.$$$");
   }
   Ve->Loop();
   // Cierra la ventana de mensajes
   delete Ve;
}


//Genera el archivo CPP para generar al ejecutable
void Ctrl_estructura::Genera_cpp(void)
{
   unsigned int i = 0, xi = 0, i1 = 0, i2 = 0 , i3 = 0, i4 = 0, pos;
   double val[6];
   char archivo[MAXPATH];

   char *xcad  = new char[TAMANO_MAXIMO_CADENA];
   char *xcad1 = new char[TAMANO_MAXIMO_CADENA];
   char *xcad2 = new char[TAMANO_MAXIMO_CADENA];
   char *xcad3 = new char[TAMANO_MAXIMO_CADENA];
   char *var   = new char[TAMANO_MAXIMO_CADENA];

   Manipulador_archivos ma;
   ma.Cambia_ext_path(Informacion->Nombre,"CPP",archivo);
   Ctrl_reporte *Ip = new Ctrl_reporte;
   Ip->Parametros(0,66,TAMANO_MAXIMO_CADENA,REPORTE_GRABADO,LPT1,archivo);
   Ip->Formato_impresion(1,1,"#include \"C:\\INTEGRA\\SYS\\INCLUDE\\DEF_USER.HPP\"");
   Ip->Formato_impresion(1,2,"extern \"C\" {");
   Ip->Formato_impresion(3,1,"#include <math.h>");
   Ip->Formato_impresion(1,1,"}");
   // Prototipo de la funcion inverso de X para evitar errores en ejecucion
   Ip->Formato_impresion(1,3,"extern double inv(const double x);");
   // Prototipo de la funcion signo de X 
   Ip->Formato_impresion(1,3,"extern double sig(const double x);");

   // Especifica el nombre de los sistemas
   Ip->Formato_impresion(1,3,"void Definicion_usuario::Inicializa(void)");
   Ip->Formato_impresion(1,1,"{");

   // Nombre del proyecto
   ma.Cambia_ext_path(Informacion->Nombre,"D",xcad);
   sprintf(xcad1,"Proyecto = \"%s\";",xcad);
   Ip->Formato_impresion(3,2,xcad1);
   // Informacion de los sistemas
   sprintf(xcad1,"Numero_sistemas = %d;",Informacion->Numero_sistemas);
   Ip->Formato_impresion(3,2,xcad1);
   Ip->Formato_impresion(3,2,"");
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
       sprintf(xcad,"Nombre_sistema[%d] = \"%s\";",i,Informacion->Inf_sistema[i]->Nombre);
       Ip->Formato_impresion(3,1,xcad);
   }
   Ip->Formato_impresion(1,1,"");

   // Especifica el nombre de las ecuaciones
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      // Informacion de las ecuaciones de cada sistema
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Ecuaciones ; xi++) {
          // Remplaza si existen las funciones auxiliares
          Remplaza_funciones_auxiliares(Informacion->Inf_sistema[i]->Ecuaciones[xi],xcad3,i);
          sprintf(xcad,"Nombre_ecuacion[%d][%d]=\"%s\";",i,xi,xcad3);
          Ip->Formato_impresion(3,1,xcad);
      }
   }

   // Especifica los parametros de cada sistema
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      // Graba el numero de ecuaciones por sistema
      sprintf(xcad,"Numero_ecuaciones[%d] = %d;",i,Informacion->Inf_sistema[i]->N_e_Ecuaciones);
      Ip->Formato_impresion(3,2,xcad);
      // Graba el numero de parametros
      sprintf(xcad,"Numero_parametros[%d] = %d;",i,Informacion->Inf_sistema[i]->N_e_Parametros);
      Ip->Formato_impresion(3,1,xcad);

      // Graba los Nombres de cada parametro
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Parametros ; xi++) {

         if(Busca_caracter(Informacion->Inf_sistema[i]->Parametros[xi],'=',pos)) {
            Substr(Informacion->Inf_sistema[i]->Parametros[xi],0,pos,xcad3);
            sprintf(xcad,"Nombre_parametro[%d][%d]=\"%s\";",i,xi,xcad3);
            Ip->Formato_impresion(3,1,xcad);
         }
      }
   }

   // Declaracion de las funciones de cada uno de los sistemas
   Ip->Formato_impresion(1,3,"");
   for(i = 0; i  < Informacion->Numero_sistemas; i++) {
       for(xi = 0; xi  < Informacion->Inf_sistema[i]->N_e_Ecuaciones; xi++) {
           sprintf(xcad,"funcion[%d][%d] = &Definicion_usuario::Fn%#02d%#02d;",i,xi,i,xi);
           Ip->Formato_impresion(3,1,xcad);
       }
   }

   // Incluye las funcione auxiliares a graficar
   Ip->Formato_impresion(1,2," ");
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      i1 = 0;
      // Funciones auxiliares a graficar
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Func_aux; xi++) {
         if(Informacion->Inf_sistema[i]->Func_aux[xi][0] == 'F' && (Informacion->Inf_sistema[i]->Func_aux[xi][2] == 'x' || Informacion->Inf_sistema[i]->Func_aux[xi][2] == 'y') && Informacion->Inf_sistema[i]->Func_aux[xi][4] == '=') {
            if(i1 < NUMERO_MAX_FUNCIONES_AUX) {
               sprintf(xcad,"Funciones_auxiliares[%d][%d] = \"%s\";",i,i1,Informacion->Inf_sistema[i]->Func_aux[xi]);
               Ip->Formato_impresion(3,1,xcad);
               i1++;
            } else NCO->Mensaje("Hay muchas funciones auxiliares a gr ficar","solo se tomaran 9 de ellas");
         }
      }
      sprintf(xcad,"Numero_funciones_auxiliares[%d] = %d;",i,i1);
      Ip->Formato_impresion(3,1,xcad);
   }

   // Rutina que asigna los valores a los parametros
   Ip->Formato_impresion(3,3,"Parametros(99,99);");
   Ip->Formato_impresion(1,1,"}");
   // Especifica el valor para cada parametro de todos los sistemas
   Ip->Formato_impresion(1,3,"void Definicion_usuario::Parametros(const unsigned int i, const unsigned int tp)");
   Ip->Formato_impresion(1,1,"{");
   for(i1 = 0; i1 < 2; i1++) {
      sprintf(xcad,"if(tp == %d || tp == 99) {",i1);
      Ip->Formato_impresion(3,1,xcad);
      for(i = 0; i < Informacion->Numero_sistemas; i++) {
         // Graba el valor de cada parametro
         for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Parametros ; xi++) {
            if(Busca_caracter(Informacion->Inf_sistema[i]->Parametros[xi],'=',pos)) {
               Substr(Informacion->Inf_sistema[i]->Parametros[xi],pos+1,100,xcad3);
                  sprintf(xcad,"if(i == %d || i == 99) _P[%d][%d][%d]=%s;",i,i1,i,xi,xcad3);
                  Ip->Formato_impresion(6,1,xcad);
            }
         }
      }
      Ip->Formato_impresion(3,1,"}");
   }
   Ip->Formato_impresion(1,1,"}");


   // Contiene los valores de configuracion del archivo EXE
   Ip->Formato_impresion(1,3,"void Definicion_usuario::Configuracion(void)");
   Ip->Formato_impresion(1,1,"{");
   // Cinfiguracion de la primera ventana
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      // valores por omision
      for(xi = 0; xi < 3; xi++) val[xi] = -10.0;
      for(xi = 0; xi < 3; xi++) val[3+xi] = 10.0;
      // Rastrea cual de los parametros es de configuracion
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Func_aux ; xi++) {
         if(Busca_caracter(Informacion->Inf_sistema[i]->Func_aux[xi],'=',pos)) {
            Substr(Informacion->Inf_sistema[i]->Func_aux[xi],0,pos,xcad2);
            Substr(Informacion->Inf_sistema[i]->Func_aux[xi],pos+1,100,xcad3);
            i2 = 0;
            while(Var_config0[i2][0] != '0') {
                // Es un parametro de configuracion
                if(Compara(xcad2,Var_config0[i2])) {
                   double xval = atof(xcad3);
                   // Remplaza el valor de omicion
                   if(i2 == 0 && xval > 0.0) {
                      for(xi = 0; xi < 3; xi++) val[xi]   = -xval;
                      for(xi = 0; xi < 3; xi++) val[3+xi] =  xval;
                   }
                   // Remplaza el valor del eje X
                   if(i2 == 1 && xval > 0.0) val[0] = -xval, val[3] =  xval;
                   // Remplaza el valor del eje Y
                   if(i2 == 2 && xval > 0.0) val[1] = -xval, val[4] =  xval;
                   // Remplaza el valor del eje Z
                   if(i2 == 3 && xval > 0.0) val[2] = -xval, val[5] =  xval;
                   // Remplaza el valor minimo del eje X
                   if(i2 == 4) val[0] = xval;
                   // Remplaza el valor maximo del eje X
                   if(i2 == 5) val[3] = xval;
                   // Remplaza el valor minimo del eje Y
                   if(i2 == 6) val[1] = xval;
                   // Remplaza el valor maximo del eje Y
                   if(i2 == 7) val[4] = xval;
                   // Remplaza el valor minimo del eje Z
                   if(i2 == 8) val[2] = xval;
                   // Remplaza el valor maximo del eje Z
                   if(i2 == 9) val[5] = xval;
                   break;
                }
                i2++;
            }
         }
      }
      for(xi = 0; xi < 6; xi++) {
          sprintf(xcad,"Config[0][%d][%d]=%f;",i,xi,val[xi]);
          Ip->Formato_impresion(3,1,xcad);
      }
   }
   // Cinfiguracion de la segunda ventana
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      // valores por omision
      for(xi = 0; xi < 3; xi++) val[xi] = -10.0;
      for(xi = 0; xi < 3; xi++) val[3+xi] = 10.0;
      // Valor solo para el curso temporal
      val[0] = -100.0;
      val[3] =  100.0;
      // Rastrea cual de los parametros es de configuracion
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Func_aux ; xi++) {
         if(Busca_caracter(Informacion->Inf_sistema[i]->Func_aux[xi],'=',pos)) {
            Substr(Informacion->Inf_sistema[i]->Func_aux[xi],0,pos,xcad2);
            Substr(Informacion->Inf_sistema[i]->Func_aux[xi],pos+1,100,xcad3);
            i2 = 0;
            while(Var_config1[i2][0] != '0') {
                // Es un parametro de configuracion
                if(Compara(xcad2,Var_config1[i2])) {
                   double xval = atof(xcad3);
                   // Remplaza el valor de omicion
                   if(i2 == 0 && xval > 0.0) {
                      for(xi = 0; xi < 3; xi++) val[xi]   = -xval;
                      for(xi = 0; xi < 3; xi++) val[3+xi] =  xval;
                   }
                   // Remplaza el valor del eje X
                   if(i2 == 1 && xval > 0.0) val[0] = -xval, val[3] =  xval;
                   // Remplaza el valor del eje Y
                   if(i2 == 2 && xval > 0.0) val[1] = -xval, val[4] =  xval; 
                   // Remplaza el valor del eje Z
                   if(i2 == 3 && xval > 0.0) val[2] = -xval, val[5] =  xval;
                   // Remplaza el valor minimo del eje X
                   if(i2 == 4) val[0] = xval;
                   // Remplaza el valor maximo del eje X
                   if(i2 == 5) val[3] = xval;
                   // Remplaza el valor minimo del eje Y
                   if(i2 == 6) val[1] = xval;
                   // Remplaza el valor maximo del eje Y
                   if(i2 == 7) val[4] = xval;
                   // Remplaza el valor minimo del eje Z
                   if(i2 == 8) val[2] = xval;
                   // Remplaza el valor maximo del eje Z
                   if(i2 == 9) val[5] = xval;
                   break;
                }
                i2++;
            }
         }
      }
      for(xi = 0; xi < 6; xi++) {
          sprintf(xcad,"Config[1][%d][%d]=%f;",i,xi,val[xi]);
          Ip->Formato_impresion(3,1,xcad);
      }
   }

//////////////////////////////////////////////////////////////////////////////////
// Agregar codigo para seleccionar escenarios para las ventanas
   // Definicion de los escenarios graficos
   Ip->Formato_impresion(3,1,"for(int i = 0; i < Numero_sistemas ; i++) {");
   // Primera ventana
   Ip->Formato_impresion(6,1,"Escenario[0][i].x = 1, Escenario[0][i].y = 2;");
   Ip->Formato_impresion(6,1,"if(Numero_ecuaciones[i] >= 3) Escenario[0][i].z = 3;");
   Ip->Formato_impresion(7,1,"else Escenario[0][i].z = 0;");
   // Segunda ventana
   //   Ip->Formato_impresion(6,1,"Escenario[1][i].x = 1, Escenario[1][i].y = 2;");
   //   Ip->Formato_impresion(6,1,"if(Numero_ecuaciones[i] >= 3) Escenario[1][i].z = 3;");
   //   Ip->Formato_impresion(7,1,"else Escenario[1][i].z = 0;");
   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Utilizo estoas para poner el curso temporal
   Ip->Formato_impresion(3,1,"Escenario[1][i].x = Numero_ecuaciones[i]+1, Escenario[1][i].y = 1;");
   Ip->Formato_impresion(3,1,"Escenario[1][i].z = 0;");
   ///////////////////////////////////////////////////////////////////////////////////////////////////
   Ip->Formato_impresion(3,1,"}");
///////////////////////////////////////////////////////////////////////////////////
   Ip->Formato_impresion(1,1,"}");


   // Genera las funciones de cada sistema
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
       for(i4 = 0; i4 < Informacion->Inf_sistema[i]->N_e_Ecuaciones; i4++) {
          sprintf(xcad,"double Definicion_usuario::Fn%#02d%#02d(void)",i,i4);
          Ip->Formato_impresion(1,2,xcad);
          Ip->Formato_impresion(1,1,"{");
          // Calcula la longitud de la expresion de la ecuacion
          i3 = strlen(Informacion->Inf_sistema[i]->Ecuaciones[i4]);
          // Busca el fin de la variable de asignacion
          Busca_caracter(Informacion->Inf_sistema[i]->Ecuaciones[i4],'=',i2);
          // Extrae la expresion de ecuacion
          Substr(Informacion->Inf_sistema[i]->Ecuaciones[i4],i2+1,i3,xcad2);

          // Remplaza las funciones auxiliares
          Remplaza_funciones_auxiliares(xcad2,xcad3,i);
          strcpy(xcad2,xcad3);
          // remplaza los parametros
          Remplaza_parametros(xcad2,xcad3,i);


          // Cambia el nombre de la variable por X[0], X[1], X[2]
          for(i1 = 0; i1 < Informacion->Inf_sistema[i]->N_e_Ecuaciones; i1++) {
             if(Busca_caracter(Informacion->Inf_sistema[i]->Ecuaciones[i1],'\'',i2)) {
                Substr(Informacion->Inf_sistema[i]->Ecuaciones[i1],0,i2,var);
                // Cambia las variables de las ecuaciones
                sprintf(xcad2,"_X[%d]",i1);
                Busca_remplaza_expresion(xcad3,var,xcad2);
             }
          }
          // Escribe la ecuacion diferencial
          sprintf(xcad,"return ( %s );",xcad3);
          Ip->Formato_impresion(3,1,xcad);
          Ip->Formato_impresion(1,1,"}");
       }
   }

   delete Ip;
   delete []xcad;
   delete []xcad1;
   delete []xcad2;
   delete []xcad3;
   delete []var;
}


void Ctrl_estructura::Remplaza_funciones_auxiliares(const char *exp, char *xexp, const unsigned int i)
{
   unsigned int st = 1, i2, i1, lg;

   char *var1 = new char[TAMANO_MAXIMO_CADENA];
   char *var2 = new char[TAMANO_MAXIMO_CADENA];

   strcpy(xexp,exp);
   // Revisa si existen funciones auxiliares
   if(Informacion->Inf_sistema[i]->N_e_Func_aux) {
      while(st) {
         st = 0;
         // Remplaza la funcion auxiliar dentro de la ecuacion
         for(i1 = 0; i1 < Informacion->Inf_sistema[i]->N_e_Func_aux ;i1++) {
            // Extrae el nombre de la funcion auxiliar
            lg = 0, i2 = 0;
            while(Informacion->Inf_sistema[i]->Func_aux[i1][lg] != '=') {
              var1[lg] = Informacion->Inf_sistema[i]->Func_aux[i1][lg];
              lg ++;
            }
            var1[lg] = 0;

            // Descarta las funciones a graficar
            if(var1[0] == 'F' && var1[1] == '(' && var1[3] == ')' && var1[4] == '=') continue;

            lg++;
            // Agrega parentesis de apertura
            var2[i2] = '(';
            i2++;
            // Extrae el funcion auxiliar
            while(Informacion->Inf_sistema[i]->Func_aux[i1][lg]) {
              var2[i2] = Informacion->Inf_sistema[i]->Func_aux[i1][lg];
              i2 ++, lg ++;
            }
            // Agrega el parentesis de cerradura
            var2[i2] = ')';
            var2[i2+1] = 0;

            // Cambia el nombre de la funcion auxiliar por la funcion auxiliar
            st = Busca_remplaza_expresion(xexp,var1,var2);
         }
      }
   }
   delete []var1;
   delete []var2;
}



void Ctrl_estructura::Remplaza_parametros(const char *exp, char *xexp, const unsigned int i)
{
   unsigned int i1, i2;
   char *var1 = new char[TAMANO_MAXIMO_CADENA];
   char *var2 = new char[TAMANO_MAXIMO_CADENA];

   strcpy(xexp,exp);
   // Revisa si existen parametros a remplazar
   if(Informacion->Inf_sistema[i]->N_e_Parametros) {
      // Cambia el nombre del parametro por la expresion adecuada
      for(i1 = 0; i1 < Informacion->Inf_sistema[i]->N_e_Parametros ;i1++) {
         // Extrae el nombre del parametro
         i2 = 0;
         while(Informacion->Inf_sistema[i]->Parametros[i1][i2] != '=') {
           var1[i2] = Informacion->Inf_sistema[i]->Parametros[i1][i2];
           i2 ++;
         }
         var1[i2] = 0;
         // Cambia el parametro
         sprintf(var2,"_P[P_a][%d][%d]",i,i1);
         Busca_remplaza_expresion(xexp,var1,var2);
      }
   }
   delete []var1;
   delete []var2;
}




// Libera la memoria del sistema
void Ctrl_estructura::Libera_memoria(void)
{
   // Libera la memoria de los objetos creados
   if(Iconos) delete []Iconos;
   if(Va)     delete Va;
   if(VDCID)  delete VDCID;
   if(VDC)    delete VDC;
   if(M1)     delete M1;
   if(VMAID)  delete VMAID;
   if(VMA)    delete VMA;
   Iconos = NULL;
   Va     = NULL;
   VDCID  = NULL;
   VDC    = NULL;
   M1     = NULL;
   VMAID  = NULL;
   VMA    = NULL;
}


// Genera los archivos de descripcion del proyecto
void Ctrl_estructura::Genera_descripcion(void)
{
   unsigned int i, xi;
   char arch[MAXPATH], xarch[MAXPATH];
   Manipulador_archivos ma;
   ma.Cambia_ext_path(Informacion->Nombre,"D",xarch);
   sprintf(arch,"C:\\INTEGRA\\DESCRIP\\%s%u",xarch,0);
   Manipulador_archivos *Gp;
   Gp = new Manipulador_archivos;
   Gp->Parametros(arch,GRABA_ARCHIVO,200,"");
   // Graba la descripcion del proyecto
   for(i = 0; i < Informacion->N_e_Descripcion; i++) Gp->Graba_linea(Informacion->Descripcion[i]);
   delete Gp;
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      sprintf(arch,"C:\\INTEGRA\\DESCRIP\\%s%u",xarch,i+1);
      Gp = new Manipulador_archivos;
      Gp->Parametros(arch,GRABA_ARCHIVO,200,"");
      // Graba la descripcion del proyecto
      for(xi = 0; xi < Informacion->Inf_sistema[i]->N_e_Descripcion; xi++) Gp->Graba_linea(Informacion->Inf_sistema[i]->Descripcion[xi]);
      delete Gp;
   }
}




// Indica si el nombre del sistema es unico o no
int Ctrl_estructura::Valida_nombre_sistema(const char *sist)
{
   int i, st = 1;
   // Revisa que no existe el nombre del sistema
   for(i = 0; i < Informacion->Numero_sistemas; i ++) {
     if(Compara(sist,Informacion->Inf_sistema[i]->Nombre)) {
        NCO->Mensaje(TXT27,sist); 
        st =0;
     }
   }
   return st;
}



// Indica si el nombre del proyecto es unico o no
int Ctrl_estructura::Valida_nombre_proyecto(const char *sist)
{
   int st = 1;
   char xnombre[MAXPATH];
   sprintf(xnombre,"%s%s",PATH_PROYECTOS,sist);
   if(n_archivos_directorios(xnombre,FA_ARCH)) {
        NCO->Mensaje(TXT28,sist); 
        st =0;
   }
   return st;
}


