// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.01-D

#include "s_arch.hpp"
#include "ctrl_tec.hpp"
#include "nucleo.hpp"
#include "cad_car.hpp"

extern "C" {
   #include <dir.h>
}


// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;
// Path original del sistema
extern char *PATH_ORIGINAL;
// Path actual
extern char *PATH_ACTUAL;



#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "Nombre de archivo:"
   #define TXT2      "Directorios:"
   #define TXT3      "Unidades:"
#else
   #define TXT1      "File name:"
   #define TXT2      "Directories:"
   #define TXT3      "Drivers:"
#endif




// Rutina utilizada para el sorteo de archivos o directorios
int Rutina_comparacion(const void *cad1, const void *cad2)
{
   return(strncmp((char *)cad1,(char *)cad2,80));
}


// Costructor de la clase
Selecciona_muestra_archivos::Selecciona_muestra_archivos(void)
{
   // inicializa variables
   Trayectoria = new char[MAXPATH];
   Archivo     = new char[MAXPATH];
   VT          = new Ventana_iconos;
   Ia          = new Traza_indicador;
   Ma          = new Menu;
   Id          = new Traza_indicador;
   Md          = new Menu;
   Iu          = new Traza_indicador;
   Mu          = new Menu;
   Gt          = new Get;
   Iconos      = new Icono[2];
   Archivos = Directorios = Unidades = NULL;
   Arr_menu1 = Arr_menu2 = Arr_menu3 = NULL;
}


// Destructor de la clase
Selecciona_muestra_archivos::~Selecciona_muestra_archivos()
{
   cambia_directorio(PATH_ORIGINAL);
   delete []Archivo;
   delete []Trayectoria;
   delete []Archivos;
   delete []Directorios;
   delete []Unidades;
   delete []Arr_menu1;
   delete []Arr_menu2;
   delete []Arr_menu3;
   delete []Iconos;
   delete Ma;
   delete Md;
   delete Mu;
   delete Ia;
   delete Id;
   delete Iu;
   delete Gt;
   delete VT;
}

// Parametros de la clase
void Selecciona_muestra_archivos::Parametros(const char *arch, const char *tit)
{
   unsigned int X1,Y1,X2,Y2;  // Posicion de la ventana
   Trayectoria_original = arch;
   // Ventana de trabajo
   VT->Activa_graba_ventana(1);
   VT->Ventana_centrada(tit,450,350,0);
   VT->Actual_pos_ventana(X1,Y1,X2,Y2);
   // Indicador de posicion de los archivos
   Ia->Define_indicador(X1+151,Y1+100,X1-151,Y2-20);
   // Menu de los archivos
   Ma->Define_ventana_menu(X1+20,Y1+100,X1+150,Y2-20,Blanco);
   // Indicador de posicion de los directorios
   Id->Define_indicador(X1+351,Y1+100,X1-351,Y2-100);
   // Menu de los directorios
   Md->Define_ventana_menu(X1+200,Y1+100,X1+350,Y2-100,Blanco);
   // Indicador de posicion de las unidades
   Iu->Define_indicador(X1+351,Y2-80,X1-351,Y2-20);
   // Menu de las unidades
   Mu->Define_ventana_menu(X1+200,Y2-80,X1+350,Y2-20,Blanco);


   // Get
   strcpy(Trayectoria,"*.*");
   s_r_t(Trayectoria,80,32);
   Gt->Parametros(Trayectoria,X1+20,Y1+65,17);

   // Iconos
   Iconos[0].Define_icono("OK.ICO",X2-50,Y1+100);
   Iconos[1].Define_icono("CANCEL.ICO",X2-50,Y1+170);
   // Visualiza la estructura
   VT->Dibuja();
   VT->Marco(10,50,185,VT->Largo_Y()-10,Blanco,Gris9);
   VT->Marco(187,50,VT->Largo_X()-60,VT->Largo_Y() -10,Blanco,Gris9);
   VT->Define_color_texto(Negro);
   VT->Visualiza_texto(20,55,TXT1);
   VT->Visualiza_texto(200,85,TXT2);
   VT->Visualiza_texto(200,VT->Largo_Y()-90,TXT3);
   Iconos[0].Dibuja();
   Iconos[1].Dibuja();
   Fija_seleccion(arch);
   Cambia_path(arch);
}




// Fija selecion de archivo
void Selecciona_muestra_archivos::Fija_seleccion(const char *arch)
{
   // Libera memoria
   delete []Archivos;
   delete []Directorios;
   delete []Unidades;
   delete []Arr_menu1;
   delete []Arr_menu2;
   delete []Arr_menu3;

   strcpy(Trayectoria,arch);
   s_r_t(Trayectoria,80,32);
   char xcad[80];
   int i, xi;

   // Archivos
   // Numero de archivos con ese path localizados
   Num_files = n_archivos_directorios(Trayectoria,FA_ARCH);
   Archivos = new char[14*(Num_files+1)];
   // Inicializacion de arreglo de archivos
   Arr_menu1 = new const char *[Num_files];
   // Carga el nombre de los archivos
   for(i = 0; i < Num_files; i++) {
      // Obtiene el nombre del archivo
      nombre_archivo_directorio(Trayectoria,FA_ARCH,!i,xcad);
      // Copia el nombre de archivo al arreglo temporal
      for(xi = 0; xi < 14; xi++) Archivos[i*14+xi] = xcad[xi];
      Archivos[i*14+13] = 0;
      // Asigna el puntero del nombre de archivo al arreglo de punteros
      Arr_menu1[i] = &Archivos[i*14];
   }
   // Sortea los nombres de los archivos
   qsort(Archivos,Num_files,14,Rutina_comparacion);
   // Asigna los parametros al menu
   Ma->Parametros_menu(Arr_menu1,"","",Num_files);

   // Directorios
   // Numero de directorios con ese path localizados
   Num_direc = n_archivos_directorios("*.*",FA_DIREC);
   Directorios = new char[14*(Num_direc+1)];
   // Inicializacion de arreglo de directorios
   Arr_menu2 = new const char *[Num_direc];
   // Carga el nombre de los directorios
   for(i = 0; i < Num_direc; i++) {
      // Obtiene el nombre del directorio
      nombre_archivo_directorio("*.*",FA_DIREC,!i,xcad);
      // Copia el nombre de directorio al arreglo temporal
      for(xi = 0; xi < 14; xi++) Directorios[i*14+xi] = xcad[xi];
      Directorios[i*14+13] = 0;
      // Asigna el puntero del nombre de directorio al arreglo de punteros
      Arr_menu2[i] = &Directorios[i*14];
   }
   // Sortea los nombres de los directorios
   qsort(Directorios,Num_direc,14,Rutina_comparacion);
   // Asigna los parametros al menu
   Md->Parametros_menu(Arr_menu2,"","",Num_direc);

   // Unidades
   // Numero de unidades
   Num_unidades = numero_drives_logicos(10);
   Unidades = new char[14*(Num_unidades+1)];
   // Inicializacion de arreglo de unidades
   Arr_menu3 = new const char *[Num_unidades];
   // Carga el nombre de las unidades
   for(i = 0; i < Num_unidades; i++) {
      // Obtiene el nombre del archivo
      nombre_drive_logico(!i,10,xcad);
      // Copia el nombre de archivo al arreglo temporal
      for(xi = 0; xi < 14; xi++) Unidades[i*14+xi] = xcad[xi];
      Unidades[i*14+13] = 0;
      // Asigna el puntero del nombre de archivo al arreglo de punteros
      Arr_menu3[i] = &Unidades[i*14];
   }
   // Sortea los nombres de las unidades
   qsort(Unidades,Num_unidades,14,Rutina_comparacion);
   // Asigna los parametros al menu
   Mu->Parametros_menu(Arr_menu3,"","",Num_unidades);


   // GET
   Gt->Cambia_cadena(Trayectoria);
   Gt->Activa_get(1);
}



// Ciclo de trabajo, si TP es (0) No es necesario que el archivo exista
//                            (1) Es necesario que el archivo exista
int Selecciona_muestra_archivos::Ciclo(const unsigned int tp)
{
   int v_direc = 1, ciclo = 1;
   char xcad[90];
   st = 0;
   unsigned int sw = 0;
   int opc, get_act = 1;

   while(ciclo) {
      NCO->Administrador();
      if(!Programa_activo) break;
      if(VT->Itera()) break;
      // Get
      Gt->Edita(Caracter,Tecla);
      if(!Ma->Mouse_dentro() && !Md->Mouse_dentro() && !Mu->Mouse_dentro()) {
         if(get_act) {
            Gt->Marco(0,0,Gt->Largo_X()-1,Gt->Largo_Y()-1,Verde1,Verde1);
            get_act = 0;
         }
         if(Tecla == ENTER) {
            Gt->Cadena_editada(xcad);
            Cambia_path(xcad);
            v_direc = 1;
         }
       } else {
        if(!get_act) Gt->Marco(0,0,Gt->Largo_X()-1,Gt->Largo_Y()-1,Negro,Negro);
        get_act = 1;
      }

      // Controlador de menu de archivos
      opc = Ia->Controlador(Ma->Opcion_actual_menu(),Ma->Retorna_total_elementos(),Ma->Retorna_elementos_paneo());
      if(Ma->Mouse_dentro() || opc) {
         if(opc == 1) Tecla = FLECHA_ARRIBA;
         if(opc == 2) Tecla = FLECHA_ABAJO;
         // Controla el movimiento del menu
         Ma->Controlador_menu(Caracter,Tecla,sw);
         // Selecciona un archivo
         if(sw) {
            strcpy(xcad,Arr_menu1[(int) sw-1]);
            Fija_seleccion(xcad);
            Dibuja();
         }
      }

      // Controlador de menu de directorios
      opc = Id->Controlador(Md->Opcion_actual_menu(),Md->Retorna_total_elementos(),Md->Retorna_elementos_paneo());
      if(Md->Mouse_dentro() || opc) {
         if(opc == 1) Tecla = FLECHA_ARRIBA;
         if(opc == 2) Tecla = FLECHA_ABAJO;
         // Controla el movimiento del menu
         Md->Controlador_menu(Caracter,Tecla,sw);
         if(sw) {
            // Cambia la actual trayectoria por la especificada en PATH
            cambia_directorio(Arr_menu2[(int) sw-1]);
            Gt->Cadena_editada(xcad);
            Fija_seleccion(xcad);
            Dibuja();
            v_direc = 1;
         }
      }

      // Controlador de menu de unidades
      opc = Iu->Controlador(Mu->Opcion_actual_menu(),Mu->Retorna_total_elementos(),Mu->Retorna_elementos_paneo());
      if(Mu->Mouse_dentro() || opc) {
         if(opc == 1) Tecla = FLECHA_ARRIBA;
         if(opc == 2) Tecla = FLECHA_ABAJO;
         // Controla el movimiento del menu
         Mu->Controlador_menu(Caracter,Tecla,sw);
         if(sw) {
            // Cambia la actual unida por la especificada en el PATH (solo toma el primer caracter como unida)
            char unid[3];
            unid[0] = Arr_menu3[(int) sw-1][2], unid[1] = 0;
            cambia_unidad(unid);
            Gt->Cadena_editada(xcad);
            Fija_seleccion(xcad);
            Dibuja();
            v_direc = 1;
         }
      }


      // Primer boton [OK]
      if(Iconos[0].Oprimido()) {
         // Devuelve el nombre de archivo asi como la trayectoria de este
         Gt->Cadena_editada(xcad);
         s_trim(xcad);
         if(tp && !n_archivos_directorios(xcad,FA_ARCH)) continue;
         // Junta la trayectoria con el nombre del archivo
///////////////////////////////////////////
//         if(s_busca_caracteres(xcad,"\\:") == -1) sprintf(Archivo,"%s%c%s",PATH_ACTUAL,92,xcad);
         sprintf(Archivo,"%s%c%s",PATH_ACTUAL,92,xcad);
///////////////////////////////////////////
         int i = 0, xi = 0;
         // Quita la doble diagonal
         while(Archivo[i]) {
            if(Archivo[i] != 92 || (Archivo[i] == 92 && Archivo[i+1] != 92)) xcad[xi] = Archivo[i], xi++;
            i++;
         }
         xcad[xi] = 0;
         strcpy(Archivo,xcad);
         // Valida el nobre de archivo
         if(!Valida_nombre_archivo(Archivo)) st = 1, ciclo = 0;         
      }

      // Segundo boton [Cancel]
      if(Iconos[1].Oprimido()) ciclo = 0;

      // Visualiza el nombre del directorio
      if(v_direc) {
         VT->Limpia_recuadro(200,55,VT->Largo_X()-61,80,Gris1,Gris1,Gris1);
         getcwd(PATH_ACTUAL,80);
         char xcad[80];
         s_rsubstr(PATH_ACTUAL,24,xcad);
         VT->Visualiza_texto(200,70,xcad);
         v_direc = 0;
         // Obtiene el nombre de la unidad actual
         struct ffblk est_arch;
         findfirst("*.*",&est_arch,FA_LABEL);
         s_nocar(est_arch.ff_name,'.',1);
         // Visualiza el nombre de la unidad
         VT->Visualiza_texto(200,55,est_arch.ff_name);
      }
   }
   return st;
}


// Cambia al path indicado
void Selecciona_muestra_archivos::Cambia_path(const char *path)
{
   char xcad[MAXPATH];
   int id;
   s_trim(path,xcad);
   // Revisa si es un archivo o directorio
   if(n_archivos_directorios(xcad,FA_ARCH)) {
      // Revisa si exite trayectoria o es solo el archivo
      if(s_bus_ult_car(xcad,'\\',id)) {  
         // Separa la trayectoria del archivo
         char xtray[MAXPATH], xarch[MAXPATH];
         s_substr(xcad,0,id,xtray);
         s_substr(xcad,id+1,100,xarch);
         cambia_directorio(xtray);
         Fija_seleccion(xarch);
      } else Fija_seleccion(xcad); // Asume solo archivo
    } else {
      // Es un subdirectorio
      cambia_directorio(xcad);
      Fija_seleccion(Trayectoria_original);
   }
   Dibuja();
}


// Dibuja la ventana del seleccionador de archivos
void Selecciona_muestra_archivos::Dibuja(void)
{
   Ia->Dibuja();
   Id->Dibuja();
   Iu->Dibuja();
   Ma->Dibuja();
   Md->Dibuja();
   Mu->Dibuja();
   Gt->Dibuja();
}
