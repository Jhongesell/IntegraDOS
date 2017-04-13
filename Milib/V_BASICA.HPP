// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __V_BASICA_HPP__
#define __V_BASICA_HPP__

#include "colores.hpp"
#include "mouse.hpp"
#include "r_var.hpp"

#define NO            0
#define SI            1
#define CERRAR        2
#define ABRIR         3
#define CON_FORMATO   0
#define SIN_FORMATO   1

class Ventana {

  private:

    char  C_F_A;                     // Almacena el color del fondo que se uso anteriormente
    char  C_T_A;                     // Almacena el color del texto que se uso anteriormente
    char  C_REF;                     // Indica el color con el que se se limpiara la ventana al salir
    char *ARCH_TMP;                  // Guarda el nombre del archivo que se usara para grabar la pantalla
    char *ARCH_TMP1;                 // Nombre de archivo temporal
    char *xcad;                      // Cadena temporal para visualizaci¢n de texto

                          // Inicializa las variables de la ventana
             void         Inicializar_var(void);

  protected:

    unsigned int TLt_hori:        1; // Almacena la horientacion de la letra
    unsigned int SALVAR_PANTALLA: 1; // Indica si se grabara la pantalla
    unsigned int VENTANA_VISIBLE: 1; // Indica si la ventana esta visible o no
    unsigned int VENTANA_ACTIVA:  1; // Indica si la ventana esta activa
    unsigned int MOUSE_DENTRO:    1; // Indica si el mouse esta dentro de la ventana o no
    char A_CAR, L_CAR;               // Ancho y largo del caracter a visualizar
    char N_car_visualizar;           // Numero de caracteres a visualizar
    char N_ren_visualizar;           // Numero de renglones a visualizar
    int  X1,Y1,X2,Y2;                // Limites de la ventana actual
    char C_VENT;                     // Almacena el color de la ventana
    char C_TEXT;                     // Almacena el color del texto a visualizar
    char C_FOND;                     // Almacena el color del fondo del texto a visualizar
    char TLt_tipo;                   // Almacena el tipo de Letra actual dentro del la ventana
    int  TLt_mulx;                   // Almacena la escala en X
    char TLt_divx;
    char TLt_muly;                   // Almacena la escala en Y
    char TLt_divy;
    char Inicia_X;                   // Indica donde iniciara la visualizacion dentro de la ventana
    char Inicia_Y;                   // Indica donde iniciara la visualizacion dentro de la ventana
    Mstatus  St_mouse;               // Indica el estado del mouse

  public:
                          // Rutinas constructora y destuctora de la clase ventana
                          Ventana(void);
                         ~Ventana();

                          // Define la ventana y su color
    virtual          void Define_ventana(const int x1, const int y1, const int x2, const int y2, const int cl = Blanco) const;
                          // Define una ventana centrada de longitud y color dados
    virtual          void Ventana_centrada(const int lg_x, const int lg_y, const int cl, const int inc_x=0, const int inc_y=0) const;
                          // Centra texto dentro de la ventana
                     void Centra_texto(const unsigned int y, const char *texto) const;
                          // Visualiza una cadena dentro de la ventana
                     void Visualiza_texto(const unsigned int x,const unsigned int y, const char *texto, const int t_v = 0, const int lg_max = 0) const;
                          // Visualiza un caracter dentro de la ventana
                     void Visualiza_caracter(const unsigned int x,const unsigned int y, const char car, const int cl, const int t_v = 1) const;
                          // Activa grabar ventana
                     void Activa_graba_ventana(const int tp) const;
                          // Indica el color de la ventana al cerrar
                     void Activa_color_refresco(const int col) const
                          {((Ventana*)this)->C_REF = col;}
                          // Actual posicion dentro de la ventana
                     void Actual_pos_ventana(unsigned int &x1, unsigned int &y1, unsigned int &x2, unsigned int &y2) const
                          {x1 = X1, y1 = Y1, x2 = X2, y2 = Y2;}
                          // Mover la actual ventana una determinada cantidad de pixeles
    virtual          void Mover_ventana(const int x, const int y) const;
    virtual          void Mover_ventana_n_pixeles(const int x, const int y) const;
                          // Indica el nuevo tama¤o de la ventana
    virtual          void Cambiar_tamano(const int x1, const int y1, const int x2, const int y2) const;
                          // Retorna el largo en X de la ventana
                     int  Largo_X(void) const
                          {return ((X2-X1)+1);}
                          // Retorna el largo en Y de la ventana
                     int  Largo_Y(void) const
                          {return ((Y2-Y1)+1);}
                          // Retorna el largo del caracter actual
                     int  Largo_caracter(void) const
                          {return L_CAR;}
                          // Retorna el ancho del caracter actual
                     int  Ancho_caracter(void) const
                          {return A_CAR;}
                          // Indica si el mouse esta dentro de la ventana
    virtual          int  Mouse_dentro(void) const;
                          // Define el color de fondo al visualizar texto
                     void Define_color_fondo(const int cl) const
                          {((Ventana*)this)->C_FOND = cl;}
                          // Define el color del texto a visualizar
                     void Define_color_texto(const int cl) const
                          {((Ventana*)this)->C_TEXT = cl;}
                          // Define el color del fondo y del texto a visulizar
                     void Define_color(const int cfd, const int ctx) const
                          {((Ventana*)this)->C_FOND = cfd, ((Ventana*)this)->C_TEXT = ctx;}
                          // Define el color de la ventana
                     void Define_color_ventana(const int cl) const
                          {((Ventana*)this)->C_FOND = ((Ventana*)this)->C_VENT = cl;}
                          // Indica el actual color del texto
                     char Actual_color_texto(void) const
                          {return C_TEXT;}
                          // Indica el actual color de fondo del texto
                     char Actual_color_fondo(void) const
                          {return C_FOND;}
                          // Indica el actual color de la ventana
                     char Actual_color_ventana(void) const
                          {return C_VENT;}
                          // Define el tama¤o de la letra a visualizar en la ventana
                     void Tamano_letra(const int lg_x, const int lg_y) const;
                          // Define el tipo de letra, horientacion y tama¤o
                     void Define_tipo_letra(const int tipo, const int horient = 0, const int multx = 1, const int divx = 0, const int multy = 0, const int divy = 0) const;
                          // Limpia una porcion de la ventana
    virtual          void Limpia_recuadro(const int x1, const int y1, const int x2, const int y2, const int cl) const;
                          // Dibuja la ventana
    virtual          void Dibuja(void) const
                          {Limpia_recuadro(0,0,Largo_X()-1,Largo_Y()-1,C_VENT);}
                          // Oculta la ventana o revisualiza esta
                     void Oculta_ventana(const int tp) const;
                          // Activa o desactiva la ventana
    virtual          void Ventana_activa(const int tp) const
                          {((Ventana*)this)->VENTANA_ACTIVA = tp;}
                          // Retorna el estado Ventana (Activo o no)
    virtual unsigned int  Retorna_ventana_activa(void) const
                          {return VENTANA_ACTIVA;}
                          // Retorna el inicion del area de trabajo en X
                     int  Retorna_inicio_X(void) const
                          {return Inicia_X;}
                          // Retorna el inicion del area de trabajo en Y
                     int  Retorna_inicio_Y(void) const
                          {return Inicia_Y;}
                          // Graba la actual ventana, si tp es (0) Imagen con formato (1) imagen sin formato
                     void Graba_ventana(const char *arch, const int tp) const;
                          // Lee una imagen a la actual ventana, si tp es (0) Imagen con formato (1) imagen sin formato
                     void Lee_ventana(const char *arch, const int tp) const;
                          // Dibuja un marco dentro de la ventana
                     void Marco(const int x1, const int y1, const int x2, const int y2, const int c_r_i, const int c_r_s) const;
                          // Retorna la posicion del mouse dentro de la ventana
   virtual           int  Posicion_mouse(int &x, int &y) const;
                          // Revisa si el boton del mouse es oprimido dentro de la ventana
   virtual unsigned  int  Presionado(const unsigned int boton) const;
                          // Limpia y marca el recuadro
   virtual           void Limpia_recuadro(const int x1, const int y1, const int x2, const int y2,const int cfd, const int c_r_i, const int c_r_s) const;

                          // Selecciona una porcion de la ventana, regresando las coordenadas de esta area
                          // Retorna (1) si se selecciono correctamente
                          //         (0) si el usuario cancelo la seleccion por medio de la tecla ESC
//                     int  Selecciona_porcion_ventana(C_dbl &vent) const;
};

/*

Clase Ventana:

   La clase VENTANA permite definir una ventana de trabajo, visualizar
   texto dentro de ella, cambiar el tama¤o y tipo de la letra a
   visualizar, mover esta, cambiar el tamano, grabar la imagen existente
   antes de abrir la ventana y activar la ventana.

   Forma de uso:
      Primero defina el objeto:
         Ya sea dinamico o estatico
      Despues defina:
         Activa_graba_ventana
            Por omicion esta apagado, si decea grabarla activarlo con 1.
            (recomendable)
         Activa_color_refresco
            Por omicion no esta activada, activela con el color de el
            fondo antes de activar la ventana (Si no hay texto entes
            es mejor de la ventana usar esta)

      Despues defina:
         Define_ventana
            Define la ventana en una determinada area de la
            ventana dada por X1,Y1,X1,Y2
         Centra_texto
            Define una ventana centrada indicando la longitud
            de esta pormedio de longitud en X y Y

      Despues
         Dibuja
            Dibuja la venatana

      Despues puede utilizar todos los demas comportamientos segun sean
      las necesidades del programa. al destruirse el objeto restaura la
      ventana original (si se grabo esta) o redibuja con el color de
      refresco (si este fue activado)



ejemplo:

   Ventana *Vpresent = new Ventana;
   Vpresent->Activa_graba_ventana(1);
   Vpresent->Ventana_centrada(480,300);
   Vpresent->Dibuja();
   Vpresent->Define_color_texto(Negro);
   Vpresent->Define_tipo_letra(4,0,2,2,2,2);
   Vpresent->Centra_texto(60,NOMBRE_SISTEMA);
   Vpresent->Define_color_texto(Azul9);
   Vpresent->Define_tipo_letra(5,0,1,3,1,3);
   sprintf(xcad,"Version: %s  @",VERSION_SISTEMA);
   Vpresent->Centra_texto(110,xcad);
   Vpresent->Define_color_texto(Azul1);
   Vpresent->Define_tipo_letra(7,0,1,3,1,3);
   Vpresent->Centra_texto(140,DESCRIPCION1);
   Vpresent->Centra_texto(155,DESCRIPCION2);
   Vpresent->Centra_texto(170,DESCRIPCION3);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   Vpresent->Define_color_texto(Negro);
   Vpresent->Visualiza_texto(20,220,"Authors: Humberto Carrillo");
   Vpresent->Visualiza_texto(20,230,"         Luis Nava");
   Vpresent->Visualiza_texto(20,240,"         Antonio Carrillo");
   Vpresent->Define_tipo_letra(2,0,-2,0,0,0);
   Vpresent->Define_color_texto(Rojo1);
   sprintf(xcad,"Fecha de compilaci¢n: %s Hora: %s",__DATE__,__TIME__);
   Vpresent->Visualiza_texto(20,280,xcad);
   sprintf(xcad,"(@) Todos los derechos reservados, registro en t rmite %s ",__DATE__);
   Vpresent->Visualiza_texto(20,290,xcad);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   while(Programa_activo && Tecla != ENTER && Tecla != ESC) Administrador();
   delete Vpresent;

*/



#endif

