// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "interfaz.hpp"
#include "nucleo.hpp"
#include "teclado.hpp"
#include "vis_ayud.hpp"
#include "version.hpp"
#include "dir.h"

extern "C" {
   #include <stdio.h>
   #include <string.h>
}

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


// Archivos de ayuda
const char *M_AYUDA_ARCHIVO[] = {
   "C:\\INTEGRA\\SYS\\HELP\\INTERFAZ.HLP",
   "C:\\INTEGRA\\SYS\\HELP\\FINTERFA.HLP",
   "C:\\INTEGRA\\SYS\\HELP\\AYUDAG.HLP",
   "C:\\INTEGRA\\SYS\\HELP\\FUNCIONE.HLP",
   "C:\\INTEGRA\\SYS\\HELP\\ACERCADE.HLP",
};



#ifdef _IDIOMA_ESPANOL_
   // Menu de ayuda
   const char *M_AYUDA_MENU[] = {
      "Que es INTERFAZ",
      "Manejo de INTERFAZ",
      "Funciones Gen‚ricas",
      "Funciones Validas",
      "Acerca de...",
      "0"
   };
   const char *M_AYUDA_MENU_TEC = "QMFUA";

   #define TXT2     "Use INTERFAZ N"
   #define TXT3     "Donde N es (3) SVGA 1024x768 256 colores (*)"
   #define TXT4     "           (2) SVGA  800x600 256 colores (*)"
   #define TXT5     "           (1) SVGA  640x480 256 colores (*)"
   #define TXT6     "           (0) VGA   640x480  16 colores"
   #define TXT7     " (*) Corra primero el programa HGXMOUSE.EXE"
   #define TXT8     "Acerca de ..."
   #define TXT18    "DESCRIPCION PROYECTO"
	#define TXT19    "¨Desea borrar el sistema se¤alado?"
	#define TXT20    "DUPLICA EL SISTEMA"
	#define TXT21    "Nombre actual:"
	#define TXT22    "Nombre nuevo:"
	#define TXT23    "RENOMBRA SISTEMA"
	#define TXT24    "ECUACIONES DEL SISTEMA"
	#define TXT27    "DESCRIPCION DEL SISTEMA"
	#define TXT28    "Escriba el nombre del proyecto"
	#define TXT29    "Borrar el proyecto se¤alado"
	#define TXT30    "Duplica el proyecto se¤alado"
	#define TXT31    "Renombra el proyecto se¤alado"
	#define TXT32    "Imprimiendo el proyecto se¤alado"
	#define TXT33    "Edici¢n de la descripci¢n del proyecto se¤alado"
	#define TXT34    "Generando archivo ejecutable del proyecto se¤alado"
	#define TXT35    "Nombre del nuevo sistema"
	#define TXT36    "Borrar el sistema se¤alado"
	#define TXT37    "Duplica el sistema se¤alado"
	#define TXT38    "Renombra el sistema se¤alado"
	#define TXT39    "Edita ecuaciones del sistema se¤alado"
	#define TXT42    "Edita la descripci¢n del sistema se¤alado"
	#define TXT43    "Imprimiendo el sistema se¤alado"
	#define TXT50    "CAMBIA DIRECTORIO"
	#define TXT51    "Nombre del directorio"
	#define TXT52    "CREA DIRECTORIO"
	#define TXT53    "BORRA DIRECTORIO"

   #define TXT60    "Esta opci¢n solo se aplica a proyectos"
   #define TXT61    "no a directorios."

	#define TXT100   "Acerca de ..."
	#define TXT101   "SISTEMA INTEGRA"
	#define TXT102   "ANALIZADOR DE SISTEMAS DINAMICOS"
	#define TXT103   "Autores:"
	#define TXT104   "Humberto Carrillo, Antonio Carrillo y Luis Nava"
	#define TXT105   "Laboratorio de Din mica no Lineal"
	#define TXT106   "Facultad de Ciencias"
	#define TXT107   "Universidad Nacional Aut¢noma de M‚xico"
	#define TXT109   "Esta es una versi¢n de prueba"


	#define Ms00  ""
	#define Ms01  "Terminar el programa y retornar al DOS"
	#define Ms02  "Visualiza la ayuda del sistema"
	#define Ms03  "Seleccione el nombre del proyecto o directorio"
	#define Ms04  "Descripci¢n del proyecto o contenido del directorio"
	#define Ms05  "Genera un nuevo proyecto"
	#define Ms06  "Abre el proyecto o cambia al directorio se¤alado"
	#define Ms07  "Borra el proyecto o directorio se¤alado"
	#define Ms08  "Duplica el proyecto se¤alado"
	#define Ms09  "Renombra el proyecto se¤alado"
	#define Ms10  "Imprime o visualiza el proyecto se¤alado"
	#define Ms11  "Edita la descripci¢n del proyecto se¤alado"
	#define Ms12  "Genera el archivo ejecutable"
	#define Ms13  "Seleccione el nombre del sistema"
	#define Ms14  "Descripci¢n del sistema"
	#define Ms15  "Crea un nuevo sistema"
	#define Ms16  "Borra el sistema se¤alado"
	#define Ms17  "Duplica el sistema se¤alado"
	#define Ms18  "Renombra el sistema se¤alado"
	#define Ms19  "Edita las ecuaciones del sistema se¤alado"
	#define Ms22  "Edita la descripci¢n del sistema se¤alado"
	#define Ms23  "Genera el archivo ejecutable del proyecto actual"
	#define Ms24  "Imprime o visualiza el sistema se¤alado"
	#define Ms25  "Graba el proyecto actual"
	#define Ms26  "Retorna al manejador de proyectos"
#else
   // Menu de ayuda
   const char *M_AYUDA_MENU[] = {
      "Que es INTERFAZ",
      "Manejo de INTERFAZ",
      "Funciones Gen‚ricas",
      "Funciones Validas",
      "Acerca de...",
      "0"
   };
   const char *M_AYUDA_MENU_TEC = "QMFUA";

   #define TXT2     "Use INTERFAZ N"
   #define TXT3     "Donde N es (3) SVGA 1024x768 256 colores (*)"
   #define TXT4     "           (2) SVGA  800x600 256 colores (*)"
   #define TXT5     "           (1) SVGA  640x480 256 colores (*)"
   #define TXT6     "           (0) VGA   640x480  16 colores"
   #define TXT7     " (*) Corra primero el programa HGXMOUSE.EXE"
   #define TXT8     "About de ..."
   #define TXT18    "PROJECT DESCRIPTION"
	#define TXT19    "Do you want to delete current system?"
	#define TXT20    "DUPLICATE SYSTEM"
	#define TXT21    "Current name:"
	#define TXT22    "New Name:"
	#define TXT23    "RENAME SYSTEM"
	#define TXT24    "SYSTEM EQUATIONS"
	#define TXT27    "SYSTEM DESCRIPTION"
	#define TXT28    "Write project name"
	#define TXT29    "Delete current project"
	#define TXT30    "Duplicate current project"
	#define TXT31    "Rename current project"
	#define TXT32    "Printing current project"
	#define TXT33    "Current project description editting"
	#define TXT34    "Make current project executing file"
	#define TXT35    "New System name"
	#define TXT36    "Delete current system"
	#define TXT37    "Duplicate current system"
	#define TXT38    "Rename current system"
	#define TXT39    "Edit system equations"
	#define TXT42    "Edit system description"
	#define TXT43    "Printing current system"
	#define TXT50    "CHANGE DIRECTORY"
	#define TXT51    "Directory name"
   #define TXT52    "MAKE DIRECTORY"
	#define TXT53    "DELETE DIRECTORY"

   #define TXT60    "Esta opci¢n solo se aplica a proyectos"
   #define TXT61    "no a directorios."

	#define TXT100   "About ..."
	#define TXT101   "INTEGRA SYSTEM"
	#define TXT102   "DYNAMICAL SYSTEMS ANALYZER"
	#define TXT103   "Authors:"
	#define TXT104   "Humberto Carrillo, Antonio Carrillo y Luis Nava"
	#define TXT105   "Non Linear Dynamics Laboratory"
	#define TXT106   "School of Sciences"
	#define TXT107   "Universidad Nacional Aut¢noma de M‚xico"
	#define TXT109   "This is a testing version"


	#define Ms00  ""
	#define Ms01  "End program and return to DOS"
	#define Ms02  "Visualize system help"
	#define Ms03  "Select project name or directory"
	#define Ms04  "Project description or directory content"
	#define Ms05  "Produce a new project"
	#define Ms06  "Open current project or change current directory"
	#define Ms07  "Delete project or current directory"
	#define Ms08  "Duplicate current project"
	#define Ms09  "Rename current project"
	#define Ms10  "Print or visualize current project"
	#define Ms11  "Current project description editting"
	#define Ms12  "Make current project executing file"
	#define Ms13  "Choose system name"
	#define Ms14  "System description"
	#define Ms15  "New System"
	#define Ms16  "Delete current system"
	#define Ms17  "Duplicate current system"
	#define Ms18  "Rename current system"
	#define Ms19  "Edit current system equations"
	#define Ms22  "Edit system description"
	#define Ms23  "Make executing file"
	#define Ms24  "Print or visualize current system"
	#define Ms25  "Save project"
	#define Ms26  "Return to projects handler"
#endif




// Proceso inicial de la aplicacion
void Interfaz::Proceso_inicial(void)
{
	// Inicializa variables
	Accion = 30;
	Iconos = NULL;
	Va     = NULL;
   VDCID  = NULL;
   VDC    = NULL;
   M1     = NULL;
   VMAID  = NULL;
   VMA    = NULL;
   Texto_explicativo = new char *[25];
   Texto_explicativo[0] = Ms00;
   Texto_explicativo[1] = Ms01;
   Texto_explicativo[2] = Ms02;
   Texto_explicativo[3] = Ms03;
   Texto_explicativo[4] = Ms04;
   Texto_explicativo[5] = Ms05;
   Texto_explicativo[6] = Ms06;
   Texto_explicativo[7] = Ms07;
   Texto_explicativo[8] = Ms08;
   Texto_explicativo[9] = Ms09;
   Texto_explicativo[10] = Ms10;
   Texto_explicativo[11] = Ms11;
   Texto_explicativo[12] = Ms12;
   Texto_explicativo[13] = Ms13;
   Texto_explicativo[14] = Ms14;
   Texto_explicativo[15] = Ms15;
   Texto_explicativo[16] = Ms16;
   Texto_explicativo[17] = Ms17;
   Texto_explicativo[18] = Ms18;
   Texto_explicativo[19] = Ms19;
   Texto_explicativo[20] = Ms22;
   Texto_explicativo[21] = Ms23;
   Texto_explicativo[22] = Ms24;
   Texto_explicativo[23] = Ms25;
   Texto_explicativo[24] = Ms26;

   // Indica que la ventana principal no esta activa
   Vt->Ventana_activa(0);

   // Inicializacion del arreglo de trabajo
   Informacion = NULL;

   // Ruta de proyectos
   strcpy(PATH_PROYECTOS,"C:\\INTEGRA\\PROYECT\\");


   Controlador();
}

// Poceso final de la aplicacion
void Interfaz::Proceso_final(void)
{
   // Se agrago para que el programa cierre los procesos abiertos
   Programa_activo = 1;
   // Graba el proyecto
   Ctrl_estructura::Grabar(1);
   // Borra la estructura de informacion
   Ctrl_estructura::Borra_proyecto();
   // Libera la memoria usada por las pantallas
   Ctrl_estructura::Libera_memoria();
}


// Controla a la aplicacion
void Interfaz::Controlador(void)
{
   switch(Accion)
   {
       case 30:
         // Define el titulo de la ventana
         Define_titulo(VER_INTERFAZ);
         // Redibuja la ventana
         Dibuja_aplicacion();
         // Definicion del objeto
         Selecciona_proyecto::Parametros(0,0,X_MAX,Y_MAX);
         Accion = 35;
       case 35:
         // Control de proyectos
         Controla_proyecto();
         break;
       case 40:
         // Redibuja la ventana
         Dibuja_aplicacion();
         // Parametros de seleccion de sistemas
         Selecciona_sistema::Parametros(0,0,X_MAX,Y_MAX);
         Accion = 45;
       case 45:
         // Control de sistemas
         Control_sistemas();
         break;
   }
}



// Controla la ejecucion del proyecto
void Interfaz::Controla_proyecto(void)
{
   int sw = 1;
   Selecciona_proyecto::Controlador(Caracter,Tecla);
   switch(Ipresionado) {
      case 1: // Nuevo proyecto
        Grabar(1);
        Visualiza_texto(TXT28);
        sw = Selecciona_proyecto::Nuevo();
        if(!sw) {
           char cad3[100];
           sprintf(cad3,"%s { %s }",VER_INTERFAZ,Informacion->Nombre);
           Define_titulo(cad3);
           Captura_descripcion(TXT18,0,0);
           Selecciona_proyecto::Carga_descripcion_proyectos();
           Accion = 40;
           Informacion->Modificada = 1;
        }
        break;
      case 2: // Carga el actual proyecto o cambia de directorio
        if(Informacion->Nombre[0] != '[') { // Carga un proyecto
           char cad3[100];
           sprintf(cad3,"%s { %s }",VER_INTERFAZ,Informacion->Nombre);
           Define_titulo(cad3);
           sw = 0;
           Accion = 40;
         } else {                           // Cambia el directorio
           if(Informacion->Nombre[1] != '.') {
              char temp[20], temp1[200];
              strcpy(temp,Informacion->Nombre);
              temp[0] = 32;
              temp[12] = 32;
              s_trim(temp);
              strcpy(temp1,PATH_PROYECTOS);
              sprintf(PATH_PROYECTOS,"%s\%s\\",temp1,temp);
            } else {
              unsigned int i = 1, lg = strlen(PATH_PROYECTOS);
              if(lg > 3) {
                 PATH_PROYECTOS[lg-1] = 0;
                 while(PATH_PROYECTOS[lg-i] != '\\') i++;
                 PATH_PROYECTOS[lg-i+1] = 0;
              }
           }
           Carga_proyectos();
        }
        break;
      case 11:
      case 3: // Borra el actual proyecto
        Grabar(1);
        Visualiza_texto(TXT29);
        Selecciona_proyecto::Borrar();
        break;
      case 4: // Duplica proyecto actual
        Grabar(1);
        Visualiza_texto(TXT30);
        if(Informacion->Nombre[0] != '[') Selecciona_proyecto::Duplicar();
         else NCO->Mensaje(TXT60,TXT61);
        break;
      case 5: // Renombra el proyecta actual
        Grabar(1);
        Visualiza_texto(TXT31);
        if(Informacion->Nombre[0] != '[') Selecciona_proyecto::Renombrar();
         else NCO->Mensaje(TXT60,TXT61);
        break;
      case 6: // Impreme el proyecto actual
        Grabar(1);
        Visualiza_texto(TXT32);
        // Descarta subdirectorios
        if(Informacion->Nombre[0] != '[') Imprime_proyecto();
         else NCO->Mensaje(TXT60,TXT61);
        break;
      case 7: // Captura la descripcion del proyecto actual
        Visualiza_texto(TXT33);
        // Descarta subdirectorios
        if(Informacion->Nombre[0] != '[') Captura_descripcion(TXT18,0,0);
         else NCO->Mensaje(TXT60,TXT61);
        Selecciona_proyecto::Carga_descripcion_proyectos();
        break;
      case 8: // Genera archivo ejecutable del proyecto actual
        Grabar(1);
        Visualiza_texto(TXT34);
        // Descarta subdirectorios
        if(Informacion->Nombre[0] != '[') Generar();
         else NCO->Mensaje(TXT60,TXT61);
        break;
     case 9:  // Cambia de directorio
       {
          char cad[80];
          strcpy(cad,"NONAME");
          if(Captura_cadena(TXT50,TXT51,cad,20,1) == 1) {
              if(cad[0] != '.') {
                 char temp[200];
                 s_trim(cad);
                 strcpy(temp,PATH_PROYECTOS);
                 sprintf(PATH_PROYECTOS,"%s\%s\\",temp,cad);
               } else {
                 unsigned int i = 1, lg = strlen(PATH_PROYECTOS);
                 if(lg > 3) {
                    PATH_PROYECTOS[lg-1] = 0;
                    while(PATH_PROYECTOS[lg-i] != '\\') i++;
                    PATH_PROYECTOS[lg-i+1] = 0;
                 }
              }
              Carga_proyectos();
          } 
       }
       break;
     case 10: // Nuevo directorio 
       {
          char cad[80];
          strcpy(cad,"NONAME");
          if(Captura_cadena(TXT52,TXT51,cad,20,1) == 1) {
             char temp[200];
             sprintf(temp,"%s\%s",PATH_PROYECTOS,cad);
             mkdir(temp);
             Carga_proyectos();
          }
       }
       break;
//     case 11: // Borra directorio
//       {
//          char cad[80];
//          strcpy(cad,"NONAME");
//          if(Captura_cadena(TXT53,TXT51,cad,20,1) == 1) {
//             char temp[200];
//             sprintf(temp,"%s\%s",PATH_PROYECTOS,cad);
//             rmdir(temp);
//             Carga_proyectos();
//          }
//       }
//       break;
   }
   if(sw && Ipresionado) Selecciona_proyecto::Visualiza();
}




// Controla a los sistemas
void Interfaz::Control_sistemas(void)
{
   char cad1[80], cad2[80];
   int sw = 1;
   Selecciona_sistema::Controlador(Caracter,Tecla);
   switch(Ipresionado) {
     case 1: // Nuevo sistema
       Visualiza_texto(TXT35);
       Selecciona_sistema::Nuevo();
       break;
     case 2: // Borra el sistema actual
       Visualiza_texto(TXT36);
       if(NCO->Opcion(TXT19,Informacion->Inf_sistema[Opcion_actual_menu]->Nombre) == 1) {
          Borra_sistema(Opcion_actual_menu);
          for(int xi = Opcion_actual_menu; xi < NUMERO_SISTEMAS+1; xi++) Informacion->Inf_sistema[xi] = Informacion->Inf_sistema[xi+1];
       }
       Selecciona_sistema::Carga_sistemas();
       break;
     case 3: // Duplica sistema actual
       Visualiza_texto(TXT37);
       strcpy(cad1,Informacion->Inf_sistema[Opcion_actual_menu]->Nombre);
       strcpy(cad2,Informacion->Inf_sistema[Opcion_actual_menu]->Nombre);
		 if(Captura_cadenas(TXT20,TXT21,cad1,TXT22,cad2,LG_MAX_NOM_SISTEMA,0) == 1) {
          if(Valida_nombre_sistema(cad2)) {
             Duplica_sistema(Opcion_actual_menu,cad2);
             Selecciona_sistema::Carga_sistemas();
          }
       }
       break;
     case 4: // Renombra sistema alctual
       Visualiza_texto(TXT38);
       strcpy(cad1,Informacion->Inf_sistema[Opcion_actual_menu]->Nombre);
       strcpy(cad2,Informacion->Inf_sistema[Opcion_actual_menu]->Nombre);
       if(Captura_cadenas(TXT23,TXT21,cad1,TXT22,cad2,LG_MAX_NOM_SISTEMA,0) == 1) {
          if(Valida_nombre_sistema(cad2)) {
             strcpy(Informacion->Inf_sistema[Opcion_actual_menu]->Nombre,cad2);
             Selecciona_sistema::Carga_sistemas();
          }
       }
       break;
     case 5: // Captura ecuaciones, parametros y funciones auxiliares
		 Visualiza_texto(TXT39);
       Captura_ecuacion(TXT24,Opcion_actual_menu);
       break;
     case 6: // Captura la descripcion del sistema
		 Visualiza_texto(TXT42);
       Captura_descripcion(TXT27,1,Opcion_actual_menu);
       Selecciona_sistema::Carga_descripcion_sistema();
       break;
     case 7: // Genera el archivo ejecutable
       Grabar(1);
       Visualiza_texto(TXT34);
       Generar();
       break;
     case 8: // Imprime el sistema actual
       Grabar(1);
       Visualiza_texto(TXT43);
       Imprime_sistema(Opcion_actual_menu);
       break;
     case 9: // Graba el actual proyecto
       Grabar(0);
       char cad3[100];
       sprintf(cad3,"%s { %s }",VER_INTERFAZ,Informacion->Nombre);
       Define_titulo(cad3);
       break;
     case 10: // Retorna al menajador de proyectos
       Grabar(1);
       sw = 0;
       Accion = 30;
   }
   if(sw && Ipresionado) Selecciona_sistema::Visualiza();
}


// Ventana de presentacion inicial
void Interfaz::Pantalla_presentacion(void)
{
   Visualiza_texto(TXT100);
   Ventana_iconos *Vpresent = new Ventana_iconos;
   Vpresent->Activa_graba_ventana(1);
   Vpresent->Ventana_centrada(VER_INTERFAZ,480,300,0);
   Vpresent->Dibuja();
   Vpresent->Define_color_texto(Negro);
   Vpresent->Limpia_recuadro(20,50,460,130,Gris1,Blanco,Gris9);
   Vpresent->Define_tipo_letra(1,0,4,4,4,4);
   Vpresent->Centra_texto(60,TXT101);
   Vpresent->Define_color_texto(Azul9);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   Vpresent->Centra_texto(110,TXT102);
   Vpresent->Define_color_texto(Negro);
   Vpresent->Centra_texto(140,VERSION);

   Vpresent->Define_tipo_letra(2,0,-2,0,0,0);
   Vpresent->Define_color_texto(Rojo1);
   Vpresent->Centra_texto(155,FECHA_VERSION);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   Vpresent->Define_color_texto(Negro);
   Vpresent->Centra_texto(190,TXT103);
   Vpresent->Centra_texto(206,TXT104);
   Vpresent->Centra_texto(240,TXT105);
   Vpresent->Centra_texto(250,TXT106);
   Vpresent->Centra_texto(260,TXT107);

   Vpresent->Define_color_texto(Rojo1);
   Vpresent->Centra_texto(280,REGISTRO);
   while(!Vpresent->Itera() && Programa_activo && Tecla != ENTER && Tecla != ESC) NCO->Administrador();
   delete Vpresent;
   // Mensaje del sistema
   NCO->Mensaje(TXT102,VERSION,"",TXT109);
}


// Pantalla de terminacion
void Interfaz::Pantalla_terminacion(void)
{
}


// Dibuja la ventana de la aplicacion
void Interfaz::Dibuja_aplicacion(void)
{
   // Dibuja la ventana con iconos
   Aplicacion::Dibuja();
   // Dibuja la area de iconos
   Vt->Limpia_recuadro(1,26,X_MAX-1,67,Gris1,Negro,Negro);
}


// Pantalla de estado del sistema
void Interfaz::Pantalla_estado_sistema(void)
{

}


// Redibuja pantalla
void Interfaz::Redibuja_pantalla(void)
{
//   Dibuja_aplicacion();
}



// Programa de aplicacion
void main(int argc, char *argv[])
{
   int svga = 0, st = 0;
   // Revisa a que modo grafico iniciara la pantalla Grafica
   if(argc == 2) {
      svga = argv[1][0] - '0';
      if(svga < 0 || svga > 3) st = 1;
   } else st = 1;
   if(st) printf("\n\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",VER_INTERFAZ,TXT2,TXT3,TXT4,TXT5,TXT6,TXT7);
    else {
      // Definicion del objeto interfaz
      Interfaz *It;
      It = new Interfaz;
      // Ejecuta la aplicacion interfaz
      It->Ejecuta("INTERFAZ",svga);
      delete It;
   }
}






