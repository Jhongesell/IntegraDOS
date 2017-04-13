// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef ___BVISEDI_HPP__
#define ___BVISEDI_HPP__



#include "v_marco.hpp"
#include "ctrl_mov.hpp"
#include "..\libreria\lista2l.hpp"
#include "..\libreria\cadenas.hpp"


#define LEE_NUEVO_ARCHIVO      2
#define ADICIONA_ARCHIVO       2
#define LEE_BLOQUE             0
#define GRABA_ARCHIVO_COMPLETO 1
#define GRABA_BLOQUE           0


class Base_vis_edi: public Ventana_marco, public Cadenas
{
  protected:
      unsigned int            Mod_arch:        1; // Indica si el archivo fue modificado o no
      unsigned int            Sw_grabar:       1; // Indica si se trabaja con el editor o solo con edicion
      unsigned int            Block_activado:  1; // Indica si el block esta activado o no 
      char                    C_fdo;              // Indica el color de fondo a visualizar
      char                    C_tex;              // Indica el color del texto a visualizar
      unsigned int            p_ver, p_hor;       // Indicador de paneo vertical y horizontal
      unsigned int            Marca[9];           // Indica el posicion de la marca
      unsigned int            Inicio_block;       // Indica el inicio del block
      unsigned int            Fin_block;          // Indica el fin del block
      unsigned int            i;                  // Variables de uso general
      int                     xtc;                // Variables de uso general
      char                   *text1;              // Puntero al texto para varios usos
      char                   *text2;
      char                   *text3;
      char                   *xcad;
      const char             *pt_text;            // Puntero al texto del editor
      char                   *Archivo;            // Nombre de archivo

      Controlador_movimiento *Ctrl_mov_ver;
      Controlador_movimiento *Ctrl_mov_hor;
      Lista2ligada           *Lista2l;

  public:

            Base_vis_edi(void);
           ~Base_vis_edi();

            // Parametros para la ventana de trabajo del editor y/o visualizador
      void  Parametros(const int x1, const int y1, const int x2, const int y2, const char *arch);
            // Controla el movimiento dentro del texto
      void  Controla_movimiento(const int tc);
            // Visualiza_archivo el archivo dentro de la pantalla
      void  Visualiza_texto(void);
            // Actualiza los datos dentro de la lista doblemente ligada
      void  Actualiza_datos(void);
            // Lee el archivo
      void  Lee_archivo(const char *arch, const int tp);
            // Graba el archivo
      void  Graba_archivo(const char *arch, const int tp);
            // Visualizacion del cursor
      void  Visualiza_cursor(const int tp);
            // Adiciona una linea a la lista
            //    (1)  Se adiciona al inicio de la lista
            //    (2)  Se adiciona al final de la lista
      void  Adiciona_linea(const unsigned int nl, const char *txt, const int tp);
            // Retorna el contenido de la linea
      const char *Retorna_contenido_linea(const unsigned int nl) const
            {return Lista2l->Contenido_nodo(nl);}
            // Borra el contenido del editor
      void  Borra_contenido_editor(void);
            // Retorna la longitud de el contenido de la linea
      unsigned int  Retorna_longitud_linea(const unsigned int nl) const
            {return (Lista2l->Retorna_longitud_nodo(nl));}
            // Se posicion el la linea indicada de la lista
      void  Posicionarse(const unsigned int nl, const unsigned int nc);
            // Retorna la linea actual
      unsigned int  Linea_actual(void) const
            {return Ctrl_mov_ver->Elemento_actual();}
            // Retorna el tola de lineas
      unsigned int  Total_lineas(void) const
            {return Ctrl_mov_ver->Numero_total_elementos();}
            // Numero de lineas a desplegar
      unsigned int Lineas_desplegar(void) const
            {return Ctrl_mov_ver->Numero_elementos_paneo();}
            // Indica el caracter actual de la linea
      unsigned int  Caracter_actual(void) const
            {return Ctrl_mov_hor->Elemento_actual();}
            // Indica la cantidad maxima de caracteres por linea
      unsigned int  Caracteres_linea(void) const
            {return Ctrl_mov_hor->Numero_total_elementos();}
            // Indica el numero de caracteres a desplegar por linea
      unsigned int  Caracteres_desplegar(void) const
            {return Ctrl_mov_hor->Numero_elementos_paneo();}
      void  Revisualiza_pantalla(void);
      int   Archivo_modificado(void) const
            {return Mod_arch;}
      void  Fija_caracter_actual(const unsigned int car_act) const
            {Ctrl_mov_hor->Fija_elemento_actual(car_act);}
      void  Fija_linea_actual(const unsigned int lin_act) const
            {Ctrl_mov_ver->Fija_elemento_actual(lin_act);}
};

#endif
