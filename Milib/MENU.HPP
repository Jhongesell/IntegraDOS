// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __MENU_HPP__
#define __MENU_HPP__

#include "v_marco.hpp"
#include "ctrl_mov.hpp"



class Menu: public Ventana_marco
{
  private:
    long xNea;

    // Objeto controlador de movimiento
    unsigned int             Tipo_menu: 1;  // Indica el tipo de menu (1) Vertical,  (0) horizontal
    unsigned int             Posx;          // Indica la posici¢n horizontal donde iniciara la visualizaci¢n del menu
    unsigned int             Posy;          // Indica la posici¢n vertical donde iniciara la visualizaci¢n del menu
//    unsigned int             Elem_pan;      // Indica el numero de elementos del paneo
    char                     Lgop;          // Indica la longitud de la opci¢n (largo de esta)
    char                     Cfdn;          // Indica el color del fondo en los elementos no activados
    char                     Cfda;          // Indica el color del fondo en el elemento activado
    char                     Cfdd;          // Indica el color del fondo en los elementos descartados
    char                     Ctxn;          // Indica el color del texto en los elementos no activados
    char                     Ctxa;          // Indica el color del texto en el elemento activado
    char                     Ctxd;          // Indica el color del texto en los elementos descartados
    Controlador_movimiento  *Ctrl_mov;      // Controla el movimiento 
    const char             **Texto_menu;    // Puntero al texto del menu


                  // Indica la posicion del mouse dentro del menu
    void          Posicion_mouse(unsigned int &pos) const;
                  // Selecciona un elemento del menu con el mouse
    unsigned int  Seleccion_mouse(unsigned int &pos) const;


  public:
                  // Constructor de la clase Menu
                  Menu(void);
                  // Destructor de la clase Menu
                 ~Menu();

                  // Define la ventana del menu
    void          Define_ventana_menu(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const char c_f) const;
                  // Carga los parametros del menu y visualiza este
    void          Parametros_menu(const char **text, const char *opc, const char *opcd, const unsigned int t_e, const unsigned int t_m = 1) const;
                  // Dibuja la ventana asi como el menu
    void          Dibuja(void) const
                  {Ventana_marco::Dibuja(); Visualiza_menu();}
                  // Visualiza el menu
    void          Visualiza_menu(void) const;
                  // Controla al menu usando el teclado y el mouse
    unsigned int  Controlador_menu(const char car,const int t_c, unsigned int &pos) const;
                  // Indica la opcion actual dentro del menu
    unsigned int  Opcion_actual_menu(void) const
                  {return Ctrl_mov->Elemento_actual();}
                  // Fija el actual elemento del menu
    void          Fija_elemento_actual(const unsigned int elem_act) const
                  {Ctrl_mov->Fija_elemento_actual(elem_act);}
                  // Indica la posicion del menu
    void          Fija_posicion_menu(const int x, const int y) const
                  {((Menu*)this)->Posx = x, ((Menu*)this)->Posy = y;}
                  // Retorna el numero de elementos de paneo
    unsigned int  Retorna_elementos_paneo(void) const
                  {return Ctrl_mov->Numero_elementos_paneo();}
                  // Retorna el numero total de elementos
    unsigned int  Retorna_total_elementos(void) const
                  {return Ctrl_mov->Numero_total_elementos();}
};

#endif

