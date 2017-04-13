// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#include "vis_ayud.hpp"
#include "mouse.hpp"
#include "teclado.hpp"
#include "ctrl_mov.hpp"
#include "r_var.hpp"
#include "r_esp.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;


#ifdef _IDIOMA_ESPANOL_

   #define TXT1      "¨Desea grabar el archivo?"

   // Submenu de visualizacion
   static const char *M_visualizacion[] = {
      "Grabar como ...",
      "Imprimir",
      "Copia archivo al portapapeles",
      "0"
   };
   static const char *C_M_visualizacion = "GIA";

   // Submenu de edicion
   static const char *M_edicion[] = {
      "Grabar",
      "Leer",
      "Grabar como ...",
      "Imprimir",
      "Copia archivo a portapapeles",
      "Copia portapapeles al archivo",
      "0"
   };
   static const char *C_M_edicion = "GLCIAP";

#else

   #define TXT1      "Save file before closing?"

   // Submenu de visualizacion
   static const char *M_visualizacion[] = {
      "Save as ...",
      "Print",
      "Copy file to buffer",
      "0"
   };
   static const char *C_M_visualizacion = "SPF";

   // Submenu de edicion
   static const char *M_edicion[] = {
      "Save",
      "Read",
      "Save as ...",
      "Print",
      "Copy file to biffer",
      "Copy buffer to file",
      "0"
   };
   static const char *C_M_edicion = "SRAPFB";

#endif



// Parametros
void Vis_archivo::Parametros(const char *arch, const char *tex_exp, const int lgx, const int lgy, const int tp)
{
   unsigned int x1, y1, x2, y2;
   TP = tp;
   Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Cambia_tipo_letra_titulo(8,0,-1,0,0,0,0);
   Vt->Ventana_centrada(tex_exp,lgx,lgy,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
//   Vt->Limpia_recuadro(15,50,lgx-41,lgy-41,Blanco,Negro,Negro);
   Hor = new Traza_indicador;
   Ver = new Traza_indicador;
   Ver->Define_indicador(x2-40,y1+50,x2-40,y2-40,1);
   Ver->Dibuja();
   Hor->Define_indicador(x1+15,y2-40,x2-40,y2-40,0);
   Hor->Dibuja();
   if(TP) {
      Va = new Visualiza_archivo;
      Va->Parametros(x1+16,y1+51,x2-42,y2-42,arch);
    } else {
      Ed = new Edita_archivo;
      Ed->Parametros(x1+16,y1+51,x2-42,y2-42,arch);
   }
}


// Loop de control
void Vis_archivo::Loop(void)
{
   int sw, loop = 0;
   if(TP) Va->Revisualiza_pantalla();
    else Ed->Revisualiza_pantalla();
   while(!loop) {
      NCO->Administrador();
      // Barra controladora horizontal
      if(TP) sw = Ver->Controlador(Va->Linea_actual(),Va->Total_lineas(),Va->Lineas_desplegar());
       else sw = Ver->Controlador(Ed->Linea_actual(),Ed->Total_lineas(),Ed->Lineas_desplegar());
      if(sw == 1) Tecla = FLECHA_ARRIBA;
      if(sw == 2) Tecla = FLECHA_ABAJO;
      // Barra controladora vertical
      if(TP) sw = Hor->Controlador(Va->Caracter_actual(),Va->Caracteres_linea(),Va->Caracteres_desplegar());
       else sw = Hor->Controlador(Ed->Caracter_actual(),Ed->Caracteres_linea(),Ed->Caracteres_desplegar());
      if(sw == 1) Tecla = FLECHA_IZQUIERDA;
      if(sw == 2) Tecla = FLECHA_DERECHA;
      NCO->Muestra_mouse(SI);

      // Activa el menu de opciones
      if(NCO->Retorna_estado_mouse().button == BL) {
         unsigned int  selec;               // Indica la seleccion del menu de acceso rapido a funciones
         C_2I          vent;                // Cordenadas del menu de acceso rapido
         vent.y = NCO->Retorna_estado_mouse().y;
         vent.x = NCO->Retorna_estado_mouse().x;
         if(TP) {
            // Submenu del visualizador
            if(Va->Ventana::Mouse_dentro()) {
//////////////////////////////////////////////////////
// Falta poder controlar si hay menu para manejo de bloques
//////////////////////////////////////////////////////
               if(Opcion_menu(M_visualizacion,C_M_visualizacion,selec,vent)) {
                  switch(selec) {
                     case 1: Tecla = CTRL_F_W; break; // Grabar como ...
                     case 2: Tecla = CTRL_F_P; break; // Imprime el archivo
                     case 3: Tecla = CTRL_F_C; break; // Copia el archivo al portapapeles
                  }
               }
            }
          } else {
            // Submenu del editor
            if(Ed->Ventana::Mouse_dentro()) {
//////////////////////////////////////////////////////
// Falta poder controlar si hay menu para manejo de bloques
//////////////////////////////////////////////////////
               if(Opcion_menu(M_edicion,C_M_edicion,selec,vent)) {
                  switch(selec) {
                     case 1: Tecla = F2; break;        // Graba el archivo
                     case 2: Tecla = CTRL_K_R; break;  // Lee un archivo introduciendolo como bloque
                     case 3: Tecla = CTRL_F_W; break;  // Grabar como ...
                     case 4: Tecla = CTRL_F_P; break;  // Imprime el archivo
                     case 5: Tecla = CTRL_F_C; break;  // Copia el archivo al portapapeles
                     case 6: Tecla = CTRL_V; break;    // Copia el portapapeles al archivo
                  }
               }
            }
         }
      }

      // Controla movimiento y/o edicion
      if(TP) Va->Controlador(Tecla);
       else Ed->Controlador(Caracter,Tecla);
      // Revisa el estado del icono
      if(Vt->Itera() || !Programa_activo) loop = 1;
      NCO->Muestra_mouse(SI);

   }
   if(!TP) {
      if(Ed->Archivo_modificado()) {
         if(NCO->Opcion("",TXT1) == 1) Ed->Controlador(0,F2);
      }
   }
   if(TP) delete Va;
    else delete Ed;
   delete Hor;
   delete Ver;
   delete Vt;
}


void Vis_archivo::Lee_archivo(const char *arch)
{
   if(n_archivos_directorios(arch,FA_ARCH)) {
      if(TP) Va->Lee_archivo(arch,ADICIONA_ARCHIVO);
       else Ed->Lee_archivo(arch,ADICIONA_ARCHIVO);
   }
}

void Vis_archivo::Adiciona_linea(const char *cad)
{
   if(TP) Va->Adiciona_linea(1,cad,2);
    else Ed->Adiciona_linea(1,cad,2);
}



void Vis_archivo::Revisualiza(void)
{
   if(TP) Va->Revisualiza_pantalla();
    else Ed->Revisualiza_pantalla();
}

