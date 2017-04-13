// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "bvisedi.hpp"
#include "man_arch.hpp"
//#include "cad_car.hpp"
#include "teclado.hpp"
#include "r_m_c.hpp"
#include "nucleo.hpp"
#include "r_esp.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;

extern "C" {
   #include <graphics.h>
   #include <dir.h>
   #include <string.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "GRABAR BLOQUE"
   #define TXT5      "GRABA COMO ..."
   #define TXT6      "EDITOR TEXTO"
#else
   #define TXT1      "SAVE BLOCK"
   #define TXT5      "SAVE AS ..."
   #define TXT6      "TEXT EDIT"
#endif

static const char *Archivo_impresion = "ARCH_PRN.$$$";


// Constructor de la clase base del visualizador y del editor
Base_vis_edi::Base_vis_edi(void)
{
   Mod_arch = p_ver = p_hor = Inicio_block = Fin_block = C_fdo = 0;
   Sw_grabar = 1;
   Block_activado = 0;
   Ctrl_mov_ver = new Controlador_movimiento;
   Ctrl_mov_hor = new Controlador_movimiento;
   Lista2l      = new Lista2ligada;
   text1        = new char[Cadena_maxima_seg];
   text2        = new char[Cadena_maxima_seg];
   text3        = new char[Cadena_maxima_seg];
   xcad         = new char[Cadena_maxima_seg];
   Archivo      = new char[MAXPATH];
   for(i = 0; i < 9; i++) Marca[i] = 0;
}


// Destructor de la clase base del visualizador y del editor
Base_vis_edi::~Base_vis_edi()
{
   delete []text1;
   delete []text2;
   delete []text3;
   delete []xcad;
   delete []Archivo;
   delete Ctrl_mov_ver;
   delete Ctrl_mov_hor;
   delete Lista2l;
}


// Visualiza_archivo el archivo dentro de la pantalla
void Base_vis_edi::Visualiza_texto(void)
{
   // Revisa si es necesario revisualizar la ventana de texto
   if(p_ver != Ctrl_mov_ver->Elemento_inicial_paneo() || p_hor != Ctrl_mov_hor->Elemento_inicial_paneo()) {
      p_ver = Ctrl_mov_ver->Elemento_inicial_paneo();
      p_hor = Ctrl_mov_hor->Elemento_inicial_paneo();
      // Selecciona el color del texto
      for(i = 0; i < Ctrl_mov_ver->Numero_elementos_paneo(); i++) {
         // Revisa con que color se visualiza el fondo
         if(p_ver+i >= Inicio_block && p_ver+i <= Fin_block) C_fdo = Cian9, C_tex = Blanco;
          else C_fdo = Actual_color_fondo(), C_tex = Negro;
         // Limpia la porcion donde se desplegara el texto
         Ventana::Limpia_recuadro(2,5+(i*L_CAR),Largo_X()-4,((i+1)*L_CAR)+4,C_fdo);
         // Visualiza la linea de texto de acuerdo con el paneo existente
         pt_text = Lista2l->Contenido_nodo(p_ver+i);
         if(pt_text) {
            setcolor(C_tex);
            Substr(pt_text,(p_hor - 1),N_car_visualizar,text1);
            outtextxy(X1+5,Y1+5+(i*L_CAR),text1);
         }
      }
   }
   Visualiza_cursor(1);
}


// Controla el movimiento dentro del texto
void Base_vis_edi::Controla_movimiento(const int tc)
{
   // Posicionamiento
   switch(tc) {
     case FLECHA_ABAJO__HOME:
       Ctrl_mov_ver->Vertical(FLECHA_ABAJO);
       pt_text = Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual());
       Ctrl_mov_hor->Horizontal(HOME,pt_text);
       break;
     case FLECHA_ARRIBA__END:
       Ctrl_mov_ver->Vertical(FLECHA_ARRIBA);
       pt_text = Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual());
       Ctrl_mov_hor->Horizontal(END,pt_text);
       break;
     case FLECHA_DERECHA: // Siguiente caracter
       Ctrl_mov_hor->Horizontal(tc,"");
       break;
     case CTRL_FLECHA_DERECHA:  // Siguiente palabra
       if(Ctrl_mov_hor->Horizontal(tc,pt_text)) {
          Ctrl_mov_ver->Vertical(FLECHA_ABAJO);
          pt_text = Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual());
          Ctrl_mov_hor->Horizontal(HOME,pt_text);
       }
       break;
     case FLECHA_IZQUIERDA: // Caracter anterior
       Ctrl_mov_hor->Horizontal(tc,"");
       break;
     case CTRL_FLECHA_IZQUIERDA:  // Anterior palabra
       if(Ctrl_mov_hor->Horizontal(tc,pt_text)) {
          Ctrl_mov_ver->Vertical(FLECHA_ARRIBA);
          pt_text = Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual());
          Ctrl_mov_hor->Horizontal(END,pt_text);
       }
       break;
     case HOME: // Inicio de linea
       Ctrl_mov_hor->Horizontal(tc,"");
       break;
     case END:  // Fin de linea
       Ctrl_mov_hor->Horizontal(tc,pt_text);
       break;
     case TAB:  // Abanza la longitud del tabulador
       Ctrl_mov_hor->Horizontal(tc,"");
       break;
     case SHIFT_TAB:  // Retrocede la longitud del tabulador
       Ctrl_mov_hor->Horizontal(tc,"");
       break;
     // Proceso sobre el archivo
     case CTRL_F_P: // Imprimir archivo
        Graba_archivo(Archivo_impresion,GRABA_ARCHIVO_COMPLETO);
        NCO->Parametros_Administrador_impresion(Archivo_impresion,LPT1);
        break;
     case CTRL_F_W: // Grabar como
        if(Sw_grabar) {
// Quitar estas lineas
//           char xarch[100];
//           strcpy(xarch,Archivo);
//           if(Retorna_archivo_seleccionado(xarch,0,TXT5)) {
//              Graba_archivo(xarch,GRABA_ARCHIVO_COMPLETO);
//           }
           strcpy(xcad,Archivo);
           if(Retorna_archivo_seleccionado(xcad,0,TXT5)) {
              Graba_archivo(xcad,GRABA_ARCHIVO_COMPLETO);
           }
        }
        break;
     case CTRL_F_C: // Copia el archivo al porta papeles
        NCO->Crea_porta_papeles(Total_lineas() + 1);
        for(i = 0; i < (Total_lineas() + 1); i++) {
            pt_text = Lista2l->Contenido_nodo(i+1);
            NCO->Inserta_porta_papeles(pt_text);
        }
        break;
     // Proceso de posicionamiento
     case CTRL_Q_R:
       Posicionarse(1,1);
       break;
     case CTRL_Q_C:
       Posicionarse(Ctrl_mov_ver->Numero_total_elementos(),1);
       break;
     case CTRL_Q_1: // Poner marca
     case CTRL_Q_2:
     case CTRL_Q_3:
     case CTRL_Q_4:
     case CTRL_Q_5:
     case CTRL_Q_6:
     case CTRL_Q_7:
     case CTRL_Q_8:
     case CTRL_Q_9:
       Marca[tc - CTRL_Q_1] = Ctrl_mov_ver->Elemento_actual();
       break;
     case CTRL_K_1: // Posicionarse en la marca
     case CTRL_K_2:
     case CTRL_K_3:
     case CTRL_K_4:
     case CTRL_K_5:
     case CTRL_K_6:
     case CTRL_K_7:
     case CTRL_K_8:
     case CTRL_K_9:
       if(Marca[tc - CTRL_K_1]) Posicionarse(Marca[tc - CTRL_K_1],1);
       break;
     case CTRL_Q_B:  // Inicio de block
       if(Block_activado) Posicionarse(Inicio_block,1);
       break;
     case CTRL_Q_K:  // Fin de block
       if(Block_activado) Posicionarse(Fin_block,1);
       break;
     case CTRL_O_N:  // Ir a la linea
       break;
     // Marcar un bloque
     case F7:
        // Inicio de BLOCK
        Inicio_block = Ctrl_mov_ver->Elemento_actual();
        p_ver = 0;
        break;
     case F8:
        // Fin de BLOCK
        Fin_block = Ctrl_mov_ver->Elemento_actual();
        p_ver = 0;
        break;
     // Proceso en Bloque
     case CTRL_K_P: // Imprime un bloque
        if(Block_activado) {
           Graba_archivo(Archivo_impresion,GRABA_BLOQUE);
           NCO->Parametros_Administrador_impresion(Archivo_impresion,LPT1);
        }
        break;
     case CTRL_C: // Copia bloque al porta papeles
        if(Block_activado) {
           unsigned int longitud = Fin_block - Inicio_block + 1;
           NCO->Crea_porta_papeles(longitud);
           for(i = 0; i < longitud; i++) {
               pt_text = Lista2l->Contenido_nodo(Inicio_block + i);
               NCO->Inserta_porta_papeles(pt_text);
           }
        }
        break;
     case CTRL_K_W: // Graba un bloque
        if(Block_activado) {
// Quitar estas lineas
//           char xarch[MAXPATH];
//           if(Retorna_archivo_seleccionado(xarch,0,TXT1)) Graba_archivo(xarch,GRABA_BLOQUE);
           if(Retorna_archivo_seleccionado(xcad,0,TXT1)) Graba_archivo(xcad,GRABA_BLOQUE);
        }
        break;
     // Busqueda y remplazo
     case CTRL_Q_F: // Buscar una cadena
        break;
     case CTRL_L:   // Buscar la siguiente
        break;
     case CTRL_Q_A: // Busqueda y remplazo
        break;
     case CTRL_R:   // Remplaza la siguiente
        break;
     default:
       Ctrl_mov_ver->Vertical(tc);
   }
   if(Fin_block >= Inicio_block) Block_activado = 1;
    else Block_activado = 0;
}



// Visualizacion del cursor
void Base_vis_edi::Visualiza_cursor(const int tp)
{
   int x = X1+4+((Ctrl_mov_hor->Actual_elemento_paneo() - 1)*A_CAR);
   int y = Y1+7+((Ctrl_mov_ver->Actual_elemento_paneo() - 1)*L_CAR);
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   // Selecciona color del cursor
   if(tp) setcolor(Rojo1);
    else setcolor(C_fdo);
   outtextxy(x,y,"_");
}


// Lee el archivo o el block
void Base_vis_edi::Lee_archivo(const char *arch, const int tp)
{
   unsigned int lin_leida = 0;
   Inicio_block = Fin_block = 0;
   Mod_arch = 1;
   // Abre el archivo a leer
   Manipulador_archivos *Leer = new Manipulador_archivos;
   Leer->Parametros(arch,LEE_ARCHIVO,Cadena_maxima,TXT6);
   // Lectura del Block
   if(tp == LEE_BLOQUE) {
      Inicio_block = Ctrl_mov_ver->Elemento_actual();
    } else {
      if(tp == LEE_NUEVO_ARCHIVO) {
         Borra_contenido_editor();
         Mod_arch = 0;
      }
   }
   while(Leer->Retorna_estado() != 2) {
      // Lee la linea de text1o
      Leer->Lee_linea(text1);
      // La adiciona en la lista
      Lista2l->Adicionar(Inicio_block+lin_leida,text1,tp);
      lin_leida++;
   }
   if(tp == LEE_BLOQUE) Fin_block = Inicio_block + lin_leida;
   // Actualiza datos en la lista doblemente ligada
   Actualiza_datos();
   delete Leer;
}


// Graba el archivo o el block
void Base_vis_edi::Graba_archivo(const char *arch, const int tp)
{
   unsigned int Posicion_inicial = 1, Posicion_final = Total_lineas();
   // Actualiza datos en la lista doblemente ligada
   Actualiza_datos();
   // Abre el archivo a grabar
   Manipulador_archivos *Graba = new Manipulador_archivos;
   Graba->Parametros(arch,GRABA_ARCHIVO,Cadena_maxima,TXT6);
   // Graba solamente el block indicado
   if(tp == GRABA_BLOQUE) Posicion_inicial = Inicio_block, Posicion_final   = Fin_block;
   while(Posicion_inicial <= Posicion_final) {
      // Graba el contenido del nodo
      pt_text = Lista2l->Contenido_nodo(Posicion_inicial);
      Graba->Graba_linea(pt_text);
      // Se posiciona en el siguiente nodo
      Posicion_inicial++;
   }
   Mod_arch = 0;
   delete Graba;
}


// Parametros del visualizador de archivos
void Base_vis_edi::Parametros(const int x1, const int y1, const int x2, const int y2, const char *arch)
{
   Define_ventana(x1,y1,x2,y2);
   Oculta_ventana(ABRIR);
   Tamano_letra(A_CAR,L_CAR+4);
   // Revisa que exista el nombre del archivo
   strcpy(Archivo,arch);
   if(Archivo[0]) {
      if(n_archivos_directorios(Archivo,FA_ARCH)) Lee_archivo(Archivo,LEE_NUEVO_ARCHIVO);
       else Adiciona_linea(1,"",1);
   }
   Ctrl_mov_ver->Parametros(Lista2l->Retorna_total_nodos(),N_ren_visualizar,"","");
   Ctrl_mov_hor->Parametros(Cadena_maxima,N_car_visualizar,"","");
   if(Archivo[0]) {
      Posicionarse(1,1);
      Visualiza_texto();
   }
}

// Actualiza datos del editor
void Base_vis_edi::Actualiza_datos(void)
{
   Lista2l->Actualiza_datos();
   Ctrl_mov_ver->Fija_total_elementos(Lista2l->Retorna_total_nodos());
}


// Adiciona una linea a la lista
// si TP es
//    (1)  Se adiciona al inicio de la lista
//    (2)  Se adiciona al final de la lista
void Base_vis_edi::Adiciona_linea(const unsigned int nl, const char *txt, const int tp)
{
   Lista2l->Adicionar(nl,txt,tp);
   Actualiza_datos();
   p_ver = 0;
}


// Se posicion el la linea indicada de la lista
void Base_vis_edi::Posicionarse(const unsigned int nl, const unsigned int nc)
{
   Fija_caracter_actual(nc);
   Fija_linea_actual(nl);
}

// Borra el contenido del editor
void Base_vis_edi::Borra_contenido_editor(void)
{
   Lista2l->Borra_todos_nodos();
   Actualiza_datos();
   Mod_arch = 0;
}


// Revisualiza la pantalla
void Base_vis_edi::Revisualiza_pantalla(void)
{
   p_ver = 0;
   Ventana_marco::Dibuja();
   Visualiza_texto();
}

