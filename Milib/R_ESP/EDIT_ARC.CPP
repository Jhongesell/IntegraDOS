// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "edit_arc.hpp"
#include "teclado.hpp"
#include "r_m_c.hpp"
#include "r_var.hpp"
#include "r_esp.hpp"
#include "s_arch.hpp"
#include "nucleo.hpp"


extern "C" {
   #include <string.h>
   #include <dir.h>
}

// Objeto global al nuclo del sistema
extern Nucleo *NCO;

#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "LEE BLOQUE"
   #define TXT3      "¨Desea Grabar el archivo?"
   #define TXT4      "LEE ARCHIVO"
   #define TXT5      "GRABA COMO"
   #define TXT6      "¨Desea borrar el contenido del editor?"
   #define TXT7      "GRABAR"
#else
   #define TXT1      "READ BLOCK"
   #define TXT3      "Save file before closing?"
   #define TXT4      "LOAD FILE"
   #define TXT5      "SAVE AS"
   #define TXT6      "Do you want to delete editor content?"
   #define TXT7      "SAVE"
#endif



// Controlador del visualizador de archivos
void Edita_archivo::Controlador(const char car, const int tc)
{
   // Cambia el tipo de cursor del mouse
   if(Ventana::Mouse_dentro()) NCO->Selecciona_tipo_mouse(3);
   // Si no hay accion a realizar en tecla retorna
   if(tc == -1) return;
   // Quita cursor
   Visualiza_cursor(NO);

   xtc = Edita_linea(car,tc);
   if(xtc == -1) Controla_movimiento(tc);
    else {
      p_hor = 0, Mod_arch = 1;
      Controla_movimiento(xtc);
   }
   Visualiza_texto();
}



// Edita la actual linea
int Edita_archivo::Edita_linea(const char car, const int tc)
{
  char xarch[MAXPATH]; // Nombre temporal del archivo
  int sw = -1, i = 0; 
  unsigned int pos =  Ctrl_mov_hor->Elemento_actual() - 1;
  // Retorna el texto de la linea actual
  pt_text = Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual());
  s_r_t(pt_text,Cadena_maxima,32,xcad);
  if(car) {
     if(NCO->Estado_Insert()) s_ins_car(xcad,pos,car);
      else xcad[pos] = car;
     Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual());
     return FLECHA_DERECHA;
  }
  switch(tc) {
     case CTRL_Y: // Borra la linea actual
        if(Ctrl_mov_ver->Elemento_actual() >= Ctrl_mov_ver->Numero_total_elementos()) {
           Lista2l->Cambia_contenido_nodo("",Ctrl_mov_ver->Elemento_actual());
         } else {
           Lista2l->Borra_nodo(Ctrl_mov_ver->Elemento_actual());
           Actualiza_datos();
        }
        sw = HOME;
        break;
     case DEL: // Borra el caracter actual  recorriendo el texto
        if(Lista2l->Retorna_longitud_nodo(Ctrl_mov_ver->Elemento_actual()) > pos) {
           Borra_caracter(xcad,pos);
           Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual());
         } else {
           if(Ctrl_mov_ver->Elemento_actual() < Ctrl_mov_ver->Numero_total_elementos()) {
              // Sube la siguiente linea a la actual
              xcad[pos] = 0;
              pt_text = Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual()+1);
              strcat(xcad,pt_text);
              Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual());
              Lista2l->Borra_nodo(Ctrl_mov_ver->Elemento_actual()+1);
              Actualiza_datos();
           }
        }
        sw = 0;
        break;
     case BACKSPACE: // Borra el caracter anterior
        if(pos) {
           Borra_caracter(xcad,pos-1);
           Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual());
           sw = FLECHA_IZQUIERDA;
         }else {
           if(Ctrl_mov_ver->Elemento_actual() > 1) {
              // Fija la columna actual
              Ctrl_mov_hor->Fija_elemento_actual(strlen(pt_text)+1);
              // Sube la cadena del nodo actual al anterior nodo
              strcpy(xcad,Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual()-1));
              Ctrl_mov_hor->Fija_elemento_actual(strlen(xcad)+1);
              strcat(xcad,pt_text);
              Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual()-1);
              Lista2l->Borra_nodo(Ctrl_mov_ver->Elemento_actual());
              Actualiza_datos();
              sw = FLECHA_ARRIBA;
           }
        }
        break;
     case CTRL_T: // Borra la palabra siguiente
        s_borra_palabra_sig(xcad,pos);
        Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual());
        sw = 0;
        break;
     case TAB: // Inserta un tabulador
        for(i = 0; i < TABULADOR; i++) {
           if(NCO->Estado_Insert()) s_ins_car(xcad,pos,32);
            else xcad[pos+i] = 32;
        }
        Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual());
        sw = TAB;
        break;
     case CTRL_V: // Copia el contenido del porta papeles al editor  
        for(i = 0; i < NCO->Retorna_total_elementos(); i ++) {
            Lista2l->Adicionar(Ctrl_mov_ver->Elemento_actual()+1+i,NCO->Retorna_cadena_porta_papeles(i),0);
        }
        Actualiza_datos();
        Revisualiza_pantalla();
        break;
     case CTRL_K_C: // Copia un bloque
        break;
     case CTRL_K_V: // Mueve un bloque
        break;
     case CTRL_K_Y: // Borra un bloque
        break;
     case CTRL_K_R: // Lee un bloque
        if(Retorna_archivo_seleccionado(xarch,1,TXT1)){
           if(n_archivos_directorios(xarch,FA_ARCH)) Lee_archivo(xarch,LEE_BLOQUE);
        }
        Revisualiza_pantalla();
        break;
     // Proceso en archivo
     case F2:       // Graba el archivo
        if(Sw_grabar) Graba_archivo(Archivo,GRABA_ARCHIVO_COMPLETO);
        break;
     case F3:       // Lee el archivo
        if(Sw_grabar) {
           if(Mod_arch){
              if(NCO->Opcion("",TXT3) == 1) Graba_archivo(Archivo,GRABA_ARCHIVO_COMPLETO);
           }
           if(Retorna_archivo_seleccionado(Archivo,1,TXT4)) {
              if(n_archivos_directorios(Archivo,FA_ARCH)) Lee_archivo(Archivo,LEE_NUEVO_ARCHIVO);
           }
           Revisualiza_pantalla();
        }
        break;
     case CTRL_F_N: // Archivo nuevo
        if(Sw_grabar) {
           if(NCO->Opcion("",TXT6) == 1) Borra_contenido_editor();
           Revisualiza_pantalla();
        }
        break;
     case ENTER: // Realiza la insercion de una nueva linea
        if(NCO->Estado_Insert()) {
           // Trunca la linea de texto a la posicion donde se encuantra el cursor
           xcad[pos] = 0;
           // Revisa si hay que adicionar texto al nodo
           if(Lg_cadena_sin_espacios_final(pt_text) >= pos) Substr(pt_text,pos,Cadena_maxima,text1);
            else strcpy(text1,"");
           // Cambia el contenido del nodo
           Lista2l->Cambia_contenido_nodo(xcad,Ctrl_mov_ver->Elemento_actual());
           Lista2l->Adicionar(Ctrl_mov_ver->Elemento_actual()+1,text1,0);
           Actualiza_datos();
        }
        sw = FLECHA_ABAJO__HOME;
  }
  return sw;
}






