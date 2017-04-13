// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "m_proy.hpp"
#include "nucleo.hpp"
#include "r_var.hpp"
#include "cad_car.hpp"
#include "r_esp.hpp"
#include "teclado.hpp"

// Definicion externa del objeto Nucleo
extern Nucleo *NCO;
// Indica cual texto explicativo a visualizar
extern unsigned int N_texto_explicativo;

extern "C" {
   #include <string.h>
   #include <stdio.h>
   #include <dir.h>
   #include <io.h>
}

// Rutina utilizada para el sorteo de archivos o directorios
extern int Rutina_comparacion(const void *cad1, const void *cad2);

#ifdef _IDIOMA_ESPANOL_

   #define TXT1      "DESCRIPCION DEL PROYECTO"
   #define TXT2      "PROYECTOS"
   #define TXT3      "PROYECTO NUEVO"
   #define TXT4      "Nombre del proyecto"
   #define TXT6      "¨Desea borra el actual proyecto?"
   #define TXT7      "DUPLICA EL PROYECTO"
   #define TXT8      "Nombre actual:"
   #define TXT9      "Nombre nuevo:"
   #define TXT10     "RENOMBRA EL PROYECTO"
   #define TXT11     "¨Desea borra el actual subdirectorio?"

   // Submenu de proyectos (Archivos y subdirectorios)
   static const char *M_proyectos[] = {
      "Nuevo Proyecto",
      "Carga Proyecto",
      "Borra Proyecto",
      "Duplica Proyecto",
      "Renombra Proyecto",
      "Imprime Proyecto",
      "Genera Ejecutable",
      "Cambiar Directorio",
      "Nuevo Directorio",
      "Borrar Directorio",
      "0"
   };

   static const char *C_M_proyectos = "NCBDRIGAUO";

   // Submenu de proyectos (Descripci¢n)
   static const char *M_D_proyectos[] = {
      "Edita Descripci¢n",
      "Grabar Como ...",
      "Imprime Descripci¢n",
      "Copia al Portapapeles",
      "0"
   };

   static const char *C_M_D_proyectos = "EGIC";

#else

   #define TXT1      "PROYECT DESCRIPTION"
   #define TXT2      "PROYECTS"
   #define TXT3      "NEW PROYECT"
   #define TXT4      "Name of proyect"
   #define TXT6      "Do want delete the current proyect?"
   #define TXT7      "DUPLICATE THE PROYECT"
   #define TXT8      "Current name:"
   #define TXT9      "New name:"
   #define TXT10     "RENAME THE PROYECT"
   #define TXT11     "Do want delete the current directory?"

   // Submenu de proyectos (Archivos y subdirectorios)
   static const char *M_proyectos[] = {
      "New Proyect",
      "Load Proyect",
      "Delete Proyect",
      "Copy Proyect",
      "Rename Proyect",
      "Print Proyect",
      "Meke Exes",
      "Change Directory",
      "New Directory",
      "Delete Directory",
      "0"
   };

   static const char *C_M_proyectos = "NLDCRPMHWT";


   static const char *M_D_proyectos[] = {
      "Edit Description",
      "Save as ...",
      "Print Description",
      "Copy to buffer",
      "0"
   };

   static const char *C_M_D_proyectos = "ESPC";

#endif



// Constructor para la seleccion del proyecto a trabajar
void Selecciona_proyecto::Parametros(const int x1,const int y1,const int x2,const int y2)
{
   Ctrl_estructura::Libera_memoria();
   X1 = x1, Y1 = y1, X2 = x2, Y2 = y2;
   Ipresionado = 0;
   Archivos = NULL;
   Arr_menu = NULL;
   // Define los iconos a mostrar
   Iconos = new Icono [8];
   Iconos[0].Define_icono("NUEVO.ICO",10,28);
   Iconos[1].Define_icono("LOAD.ICO",60,28);
   Iconos[2].Define_icono("BASURERO.ICO",110,28);
   Iconos[3].Define_icono("COPY.ICO",160,28);
   Iconos[4].Define_icono("RENAME.ICO",210,28);
   Iconos[5].Define_icono("PRINTER.ICO",260,28);
   Iconos[6].Define_icono("EDITA.ICO",310,28);
   Iconos[7].Define_icono("MAKE.ICO",360,28);

   Niconos = 8;
   // Apertura de la ventana descripcion
   VDC = new Ventana_titulo;
   VDC->Define_ventana(TXT1,X1+250, Y1+80, X2-40, Y2-70);
   // Ventana indicadora de la ventana de descripci¢n
   VDCID = new Traza_indicador;
   VDCID->Define_indicador(X2-86, Y1+110, X2-86, Y2-80);

   // Parametros para la visualizacion de la descripcion
   Va = new Visualiza_archivo;
   Va->Parametros(X1+261,Y1+111,X2-86,Y2-81,"");
   // Apertura de la ventana de proyectos
   VMA = new Ventana_titulo;
   VMA->Define_ventana(TXT2,X1+40, Y1+80, X1+220, Y2-70);
   // Ventana indicadora de la ventana de proyectos
   VMAID = new Traza_indicador;
   VMAID->Define_indicador(X1+176, Y1+110, X1+176, Y2-80);
   // Defina la ventana del menu
   M1 = new Menu;
   M1->Define_ventana_menu(X1+50, Y1+110, X1+175, Y2-80,Blanco);
   Carga_proyectos();
   Visualiza();
}



// Carga los proyectos a seleccionar
void Selecciona_proyecto::Carga_proyectos(void)
{
   // Variables generales
   char xarch[MAXPATH];
   char tmpbusq[MAXPATH];
   unsigned int i1, i, xi;
   // Revisa el numero de proyectoas a cargar
   sprintf(tmpbusq,"%s*.PRY",PATH_PROYECTOS);
   Num_files = n_archivos_directorios(tmpbusq,FA_ARCH);
   char tmparch[MAXPATH];
   sprintf(tmparch,"%s*.*",PATH_PROYECTOS);
   Num_files += n_archivos_directorios(tmparch,FA_DIREC);
   Opcion_actual_menu = 0;
   // Libera memoria
   if(Archivos) delete [] Archivos;
   if(Arr_menu) delete [] Arr_menu;
   // Inicializacion de arreglo de archivos
   Archivos = new char [Num_files*15];
   Arr_menu = new const char *[Num_files+3];
   // Carga el nombre de los archivos
   for(i = 0; i < n_archivos_directorios(tmpbusq,FA_ARCH); i++) {
      // Obtiene el nombre del archivo
      nombre_archivo_directorio(tmpbusq, FA_ARCH,!i,xarch);
      // Copia el nombre de archivo al arreglo temporal sin extencion
      for(xi = 0; xi < 14; xi++) {
          if(xarch[xi] == '.') break;
          Archivos[i*14+xi] = xarch[xi];
      }
      Archivos[i*14+xi] = 0;
      // Asigna el puntero del nombre de archivo al arreglo de punteros
      Arr_menu[i] = &Archivos[i*14];
   }
   // Carga el nombre de los directorios
   for(i1 = 0; i1 < n_archivos_directorios(tmparch,FA_DIREC); i1++) {
      // Obtiene el nombre del archivo
      nombre_archivo_directorio(tmparch,FA_DIREC,!i1,xarch);
      Archivos[(i+i1)*14] = '[';
      // Copia el nombre del al arreglo temporal
      for(xi = 0; xi < 11; xi++) {
          Archivos[(i+i1)*14+xi+1] = xarch[xi];
      }
      Archivos[(i+i1)*14+xi+1] = ']';
      Archivos[(i+i1)*14+xi+2] = 0;
      // Asigna el puntero del nombre de archivo al arreglo de punteros
      Arr_menu[i+i1] = &Archivos[(i+i1)*14];
   }

   // Sortea los nombres de los archivos
   qsort(Archivos,Num_files,14,Rutina_comparacion);
   // Parametros para el menu
   M1->Parametros_menu(Arr_menu,"","",Num_files);
   if(Num_files) {
      Lee_proyecto(Arr_menu[Opcion_actual_menu]);
   }
   Carga_descripcion_proyectos();
}

// Carga la descriopcion del proyecto
void Selecciona_proyecto::Carga_descripcion_proyectos(void)
{
   if(Informacion) {
      unsigned int i;
      Va->Borra_contenido_editor();
      if(Informacion->N_e_Descripcion) {
         for(i = 0; i < Informacion->N_e_Descripcion; i++) Va->Adiciona_linea(1,Informacion->Descripcion[i],2);
      } else Va->Adiciona_linea(0,"",2);
   } else Va->Adiciona_linea(0,"",2);
   Va->Posicionarse(1,1);
}



// Visualiza iconos asi como ventanas de trabajo
void Selecciona_proyecto::Visualiza(void)
{
   // Dibuja a los iconos
   for(int i = 0; i < Niconos; i++) Iconos[i].Dibuja();
   // Dibuja menu de seleccion de proyectos
   VMA->Dibuja();
   VMAID->Dibuja();
   M1->Dibuja();
   // Dibuja la visualizacion de la descripci¢n
   VDC->Dibuja();
   VDCID->Dibuja();
   Va->Revisualiza_pantalla();
}


// Controlador del objeto seleccion del proyecto
void Selecciona_proyecto::Controlador(char caracter, int tecla)
{
   int opc,i;
   unsigned int sw = 0;
   Ipresionado = 0;
   // Teclas de accion rapida
   switch(tecla) {
      case INSERT:
      case CTRL_N: // Nuevo
        Ipresionado = 1, tecla = -1;
        break;
      case CTRL_L: // Cargar proyecto
        Ipresionado = 2, tecla = -1;
        break;
      case DEL:    // Borra proyecto
      case CTRL_Y:
        Ipresionado = 3, tecla = -1;
        break;
      case CTRL_C: // Duplica proyecto
        Ipresionado = 4, tecla = -1;
        break;
      case CTRL_R: // Renombra proyecto
        Ipresionado = 5, tecla = -1;
        break;
      case CTRL_P: // Imprime proyecto
        Ipresionado = 6, tecla = -1;
        break;
      case CTRL_D: // Edita descripcion
        Ipresionado = 7, tecla = -1;
        break;
      case CTRL_M: // Crea el archivo exe
        Ipresionado = 8, tecla = -1;
   }
   // Actualiza el indicador de posici¢n de la descripci¢n
   opc = VDCID->Controlador(Va->Linea_actual(),Va->Total_lineas(),Va->Lineas_desplegar());
   if(opc == 1) tecla = FLECHA_ARRIBA;
   if(opc == 2) tecla = FLECHA_ABAJO;

   // Activa el menu de opciones
   if(NCO->Retorna_estado_mouse().button == BL) {
      vent.y = NCO->Retorna_estado_mouse().y;
      vent.x = NCO->Retorna_estado_mouse().x;
      // Menu de proyectos
      if(VMA->Ventana::Mouse_dentro()) {
         if(Opcion_menu(M_proyectos,C_M_proyectos,selec,vent)) Ipresionado = selec > 6 ? selec + 1 : selec;
      }
      // Menu de descripcion del proyecto
      if(VDC->Ventana::Mouse_dentro()) {
         if(Opcion_menu(M_D_proyectos,C_M_D_proyectos,selec,vent)) {
            switch(selec) {
               case 1: Ipresionado = Num_files ? 7 : 1; break;
               case 2: tecla = CTRL_F_W; break; // Grabar como ...
               case 3: tecla = CTRL_F_P; break; // Imprime el archivo
               case 4: tecla = CTRL_F_C; break; // Copia el archivo al portapapeles
            }
         }
      }
   }


   // Controla el movimiento de visualizacion del archivo
   if(VDC->Mouse_dentro()) Va->Controlador(tecla);

   // Actualiza el indicador de posici¢n de la selecci¢n de archivos
   opc = VMAID->Controlador(M1->Opcion_actual_menu(),M1->Retorna_total_elementos(),M1->Retorna_elementos_paneo());
   if(opc == 1) tecla = FLECHA_ARRIBA;
   if(opc == 2) tecla = FLECHA_ABAJO;
   // Controla la seleccion del menu
   if(VMA->Mouse_dentro()) M1->Controlador_menu(caracter,tecla,sw);
   // si existen proyectos, realiza su edicion
   if(sw) Ipresionado = Num_files ? 2: 1;
   // Revisa el comportamiento de los iconos
   for(i = 0; i < Niconos; i++) {
       // Revisa si algun icono fue presionado
       if(Iconos[i].Oprimido()) Ipresionado = i + 1;
       // Revisa sobre que icono esta el mouse
       if(Iconos[i].Mouse_dentro()) N_texto_explicativo = i + 5;
   }
   // Revisa si el mouse esta dentro de las ventana
   if(VMA->Mouse_dentro()) N_texto_explicativo = 3;
   if(VDC->Mouse_dentro()) N_texto_explicativo = 4;
   // Carga la descripcion del proyecto
   if(M1->Opcion_actual_menu()-1 != Opcion_actual_menu) {
      Grabar(1);
      Opcion_actual_menu = (int) M1->Opcion_actual_menu()-1;
      Lee_proyecto(Arr_menu[Opcion_actual_menu]);
      Carga_descripcion_proyectos();
      Va->Revisualiza_pantalla();
   }
   // Indica que se edite la descripcion del sistema
   if(Va->Ventana::Mouse_dentro() && NCO->Retorna_estado_mouse().button == BR) Ipresionado = Num_files ? 7 : 1;
}


// Proyecto nuevo
int Selecciona_proyecto::Nuevo(void)
{
   int i, st = 1;
   char cad[80];
   strcpy(cad,"NONAME.PRY");
   if(Captura_cadena(TXT3,TXT4,cad,20,1) == 1) {
      char xcad1[80];
      Manipulador_archivos ma;
      ma.Cambia_ext_path(cad,"PRY",xcad1);
      if(Valida_nombre_proyecto(xcad1)) {
         // Borra el contenido del proyecto
         Borra_proyecto();
         // Solicita espacio para el proyecto
         Informacion = new Informacion_proyecto;
         Informacion->Modificada = 1;
         Informacion->Nombre = NULL;
         Informacion->N_e_Descripcion = 0;
         Informacion->Descripcion = NULL;
         Informacion->Numero_sistemas = 0;
         // Inicializa la estructura que contendra a los sistemas
         Informacion->Inf_sistema = new Informacion_sistema *[NUMERO_SISTEMAS];
         for(i = 0; i < NUMERO_SISTEMAS; i++) Informacion->Inf_sistema[i] = NULL;
         // Solicita espacio para el nombre del proyecto
         Informacion->Nombre = new char[strlen(xcad1)+1];
         // Copia el nombre del proyecto
         strcpy(Informacion->Nombre,xcad1);
         st = 0;
      }
   }
   return st;
}





// Borra el proyecto o directorio actual
void Selecciona_proyecto::Borrar(void)
{
   // Descarta si el archivo es nulo
   if(Arr_menu[Opcion_actual_menu][0] == 32) return;
   char xnombre[MAXPATH];
   if(Arr_menu[Opcion_actual_menu][0] == '[') {
      // Remueve el directorio especificado
      char temp[MAXPATH];
      strcpy(temp,Arr_menu[Opcion_actual_menu]);
      temp[0] = 32, temp[12] = 32;
      s_trim(temp);
      sprintf(xnombre,"%s%s",PATH_PROYECTOS,temp);
      if(NCO->Opcion(TXT11,Arr_menu[Opcion_actual_menu]) == 1) rmdir(xnombre);
    } else {
      // Remueve el archivo especificado
      sprintf(xnombre,"%s%s.PRY",PATH_PROYECTOS,Arr_menu[Opcion_actual_menu]);
      if(NCO->Opcion(TXT6,Arr_menu[Opcion_actual_menu]) == 1) unlink(xnombre);  
   }
   Carga_proyectos();
}


// Duplica el proyecto actual
void Selecciona_proyecto::Duplicar(void)
{
   char cad1[80], cad2[80];
   strcpy(cad1,Arr_menu[Opcion_actual_menu]);
   strcpy(cad2,Arr_menu[Opcion_actual_menu]);
   if(Arr_menu[Opcion_actual_menu][0] == 32) return;
   if(Captura_cadenas(TXT7,TXT8,cad1,TXT9,cad2,20,0) == 1) {
      char xcad[80];
      Manipulador_archivos ma;
      ma.Cambia_ext_path(cad2,"PRY",xcad);
      if(Valida_nombre_proyecto(xcad)) {
         char xnombre1[MAXPATH];
         sprintf(xnombre1,"%s%s.PRY",PATH_PROYECTOS,cad1);
         char xnombre2[MAXPATH];
         sprintf(xnombre2,"%s%s",PATH_PROYECTOS,xcad);
         Manipulador_archivos ma;
         ma.Copia_archivo(xnombre1,xnombre2); // Duplica el proyecto actual
         Carga_proyectos();
      }
   }
}


// Renombra el proyecto actual
void Selecciona_proyecto::Renombrar(void)
{
   if(Arr_menu[Opcion_actual_menu][0] == 32) return;
   char cad1[80], cad2[80];
   strcpy(cad1,Arr_menu[Opcion_actual_menu]);
   strcpy(cad2,Arr_menu[Opcion_actual_menu]);
   if(Arr_menu[Opcion_actual_menu][0] == 32) return;
   if(Captura_cadenas(TXT10,TXT8,cad1,TXT9,cad2,20,0) == 1) {
      char xcad[80];
      Manipulador_archivos ma;
      ma.Cambia_ext_path(cad2,"PRY",xcad);
      if(Valida_nombre_proyecto(cad2)) {
         char xnombre1[MAXPATH];
         sprintf(xnombre1,"%s%s.PRY",PATH_PROYECTOS,cad1);
         char xnombre2[MAXPATH];
         sprintf(xnombre2,"%s%s",PATH_PROYECTOS,xcad);
         rename(xnombre1,xnombre2); // Renombra el actual proyecto
         Carga_proyectos();
      }
   }
}




