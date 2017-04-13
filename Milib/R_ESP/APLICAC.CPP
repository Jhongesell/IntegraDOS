// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "aplicac.hpp"
#include "var_glob.hpp"
#include "nucleo.hpp"
#include <new.h>
#include "r_esp.hpp"
#include "r_var.hpp"


extern "C" {
   #include <dir.h>
   #include <string.h>
   #include <graphics.h>
}


// Define el valor del STAK
extern unsigned int _stklen = 65000u;
// Indica cual texto explicativo a visualizar
extern unsigned int N_texto_explicativo;


// Definicion del objeto Nucleo
Nucleo *NCO;
extern const char *M_AYUDA_MENU[];
extern const char *M_AYUDA_MENU_TEC;
extern const char *M_AYUDA_ARCHIVO[];
// Indica el tipo de error en rutinas matematicas
extern int ERROR_MATEMATICO;
// Indica si se visualiza el error matematico
extern int VISUALIZA_ERROR_MATEMATICO;  


#ifdef _IDIOMA_ESPANOL_
   #define TXT1     "¨Desea terminar el programa?"
   #define TXT2     "VISUALIZA ARCHIVO"
   #define TXT3     "EDITA ARCHIVO"
   #define TXT4     "AYUDA"
   #define TXT5     "Terminar programa y retornar a l sistema operativo"
   #define TXT6     "Visualiza archivo de texto"
   #define TXT7     "Edita archivo de texto"
   #define TXT8     "Ayuda del sistema"
   #define TXT9     "Calculadora cient¡fica"
   #define TXT10    "Calendario"
   #define TXT11    "Editando el archivo de notas"
   #define TXT12    "ARCHIVO DE NOTAS"
   #define TXT13    "Visualizando el estado de sistema"
   #define TXT14    "Visualizando la tabla ASCII"

#else
   #define TXT1     "Do you want to end the program?"
   #define TXT2     "VIEW FILE"
   #define TXT3     "EDIT FILE"
   #define TXT4     "HELP"
   #define TXT5     "End program and return to DOS"
   #define TXT6     "View text file"
   #define TXT7     "Edit text file"
   #define TXT8     "System help"
   #define TXT9     "Scientific calculator"
   #define TXT10    "Calendar"
   #define TXT11    "Editing notes file"
   #define TXT12    "NOTES FILE"
   #define TXT13    "View system state"
   #define TXT14    "View ASCII table"
#endif                           

const char *PANT = "PANT_COM.$$$";

// Inicializa las variables y procesos de la aplicaci¢n
void Aplicacion::Inicializa(const char *tit, const int t_v)
{
   // Control de errores
   set_new_handler(Ctrl_error_memoria);
   harderr(Ctrl_error_harware);
   // Inicializa las variables que controlan los errores matematicos
   ERROR_MATEMATICO = 0; // Sin errores 
   VISUALIZA_ERROR_MATEMATICO = 1; // Que visualize errores 

   // Inicializa variables de mensajes
   N_texto_explicativo = M_Anterior = M_Activo = 0;
   Programa_activo = 1, SW_salida = 0;
   // Trayectoria del sistema
   PATH_SISTEMA  = new char[MAXPATH];
   PATH_ORIGINAL = new char[MAXPATH];
   PATH_ACTUAL   = new char[MAXPATH];
   strcpy(PATH_SISTEMA,"C:\\INTEGRA\\SYS\\ICONOS\\");
   // Guarda la trayectoria del sistema
   getcwd(PATH_ORIGINAL,MAXPATH);
   getcwd(PATH_ACTUAL,MAXPATH);


   // Definicion del objeto Pantalla Grafica
   Pg = new Pantalla_Grafica;
   Pg->Inicializa("C:\\INTEGRA\\SYS\\BGI\\",1,t_v,0,0); 

   // Definicion del objeto Nucleo
   NCO = new Nucleo;
   NCO->Inicializa();

   // Definicion del objeto ventana de trabajo
   Vt = new Ventana_iconos;
   // Abre la ventana de la aplicacion
   Vt->Define_colores(Blanco,Cian9,Azul9,Blanco); 
   Vt->Nombre_iconos("CLOSE.ICO","HELP.ICO");
   Vt->Define_ventana(tit,0,0, X_MAX,Y_MAX,1);
   // Dibuja la ventana con iconos
   Vt->Dibuja();
   // Dibuja la ventana de mensajes
   NCO->Dibuja();

   // Activa el mouse grafico
   C_dbl vent;
   vent.x1 = 0, vent.y1 = 0, vent.x2 = X_MAX-1, vent.y2 = Y_MAX-1;
   NCO->MInicializa(MOUSE_REQUERIDO,vent);
}


// Destruye las variables y procesos de la aplicaci¢n
void Aplicacion::Destruye(void)
{
   // Destructor del nucleo
   NCO->Destruye();
   // Destruye el objeto nucleo
   delete NCO;
   // Destruye la ventana de trabajo
   delete Vt;
   // Destruye al pantalla grafica
   Pg->Destruye();
   delete Pg;
   // Borra todos los archivos temporales
   remueve_archivos("*.$$$");
   // Libera las variables del sistema
   delete []PATH_SISTEMA;
   delete []PATH_ORIGINAL;
   delete []PATH_ACTUAL;
   delete []Texto_explicativo;
}


// Ejecuta y controla la aplicaci¢n
void Aplicacion::Ejecuta(const char *tit, const int t_v)
{
   // Inicializa las variables locales
   Inicializa(tit,t_v);
   // Proceso inicial de la aplicacion
   Proceso_inicial();
   // Indica que este activo el cambio de cursos en el mouse
   NCO->Ctrl_mouse(0);
   // Pantalla de presentacion
   Pantalla_presentacion();
   // Ciclo del prograna
   while(Programa_activo) {
      N_texto_explicativo = 0;

      // Administrador del sistema
      NCO->Administrador();

      // Revisa el estado de la ventana
      st = Vt->Itera();

      if(Vt->Icono_actual() == 1) N_texto_explicativo = 1;
      if(Vt->Icono_actual() == 2) N_texto_explicativo = 2;
      if(st) {
         if(st == 1) Tecla = ALT_X;  // Boton de salida
         if(st == 2) Tecla = ALT_H;  // Boton de ayuda
      }
      // Revisa si es necesario hacer alguna actividad
      if(Tecla != -1) {
         // Revisa si se requiere algun proceso
         switch(Tecla) {
            case ALT_H: // Visualiza la ayuda del sistema
            case F1:
               {
                  C_2I           vent;
                  unsigned int   selec;
                  vent.x = X_MAX, vent.y = 25;
                  Visualiza_texto(TXT8);
                  if(Opcion_menu(M_AYUDA_MENU,M_AYUDA_MENU_TEC,selec,vent)) {
                     Visualiza_texto(TXT8);
                     V_E_archivo(M_AYUDA_ARCHIVO[selec-1],M_AYUDA_MENU[selec-1],VISUALIZA,0);
                  }
               }
               break;
            case ALT_X:
               Visualiza_texto(TXT5);
               if(NCO->Opcion("","",TXT1) == 1) Programa_activo = 0;
               break;
            case CTRL_N_A: // Pantalla de presentacion
               Pantalla_presentacion();
               break;
            case CTRL_N_C: // Calculadora 
               Visualiza_texto(TXT9);
               Calculadora();
               break;
            case CTRL_N_D: // Redibuja la pantalla
               Dibuja();
               Redibuja_pantalla();
               break;
            case CTRL_N_E: // Editor de archivos
               Visualiza_texto(TXT7);               
               V_E_archivo("*.TXT",TXT3,EDITA,1);
               break;
            case CTRL_N_L: // Calendario 
               Visualiza_texto(TXT10);               
               Calendario(1,"");
               break;
            case CTRL_N_N: // Editor del archivo de notas
               Visualiza_texto(TXT11);               
               V_E_archivo("C:\\INTEGRA\\SYS\\NOTAS.NOT",TXT12,EDITA,0);
               break;
            case CTRL_N_P: // Protector de pantalla
               NCO->Protector_pantalla();
               break;
            case CTRL_N_S: // Visualiza el estado del sistema
               Visualiza_texto(TXT13);
               Pantalla_estado_sistema();
               break;
            case CTRL_N_T: // Visualiza la tabla ASCII
               Visualiza_texto(TXT14);
               Tabla_ascii();
               break;
            case CTRL_N_V: // Visualizador de archivos
               Visualiza_texto(TXT6);
               V_E_archivo("*.TXT",TXT2,VISUALIZA,1);
               break;
            case CTRL_N_W: // Graba la ventana completa
               NCO->Muestra_mouse(NO);
               graba_pantalla_grafica(0,0,X_MAX,Y_MAX,PANT);
               NCO->Muestra_mouse(SI);
               break;
            case CTRL_N_R: // Lee la ventana, previamente grabada
               if(n_archivos_directorios(PANT,FA_ARCH)) {
                  NCO->Muestra_mouse(NO);
                  lee_pantalla_grafica(0,0,PANT);
                  NCO->Muestra_mouse(SI);
               }
               break;
         }
      }

      // Controlador de la aplicacion (pueden haber mas de estos
      // para cada una de las aplicaciones corriedo simultaneamente
      Controlador();

      // Controla la visualizacion del texto explicativo
      if(M_Anterior != N_texto_explicativo) {
         NCO->Visualiza_texto(Texto_explicativo[N_texto_explicativo]);
         M_Activo = M_Anterior = N_texto_explicativo;
      }
   }
   // Revisa si es necesario desplegar mensaje de salida del sistema
   if(!SW_salida) {
      Programa_activo = 1;
      // Pantalla de terminacion
      Pantalla_terminacion();
   }
   // Proceso final de la aplicacion
   Proceso_final();
   // Destruye las variables locales
   Destruye();
}


// Dibuja la ventana de la aplicacion
void Aplicacion::Dibuja(void)
{
   // Dibuja la ventana con iconos
   Vt->Dibuja();
   // Visualiza la ventana de estado
   NCO->Dibuja();
   M_Anterior = 0;
}

// Visualizar la cadena en el area de mensajes
void Aplicacion::Visualiza_texto(const char *texto)
{
   NCO->Visualiza_texto(texto);
   M_Anterior = 999;
}




