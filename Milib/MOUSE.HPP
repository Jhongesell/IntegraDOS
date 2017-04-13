// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

#include "definic.hpp"

extern "C" {
  #include <dos.h>
}


#define SI   1
#define NO   0
#define MOUSE_REQUERIDO  1
#define MOUSE_NO_ACTIVO  2
#define MOUSE_OPCIONAL   3
#define BR   1
#define BM   3
#define BL   2



typedef struct {
  int present, buttons;
} Mresult;

typedef struct {
  int button, flag, x, y;
} Mstatus;

// Definicion del mouse en el modo grafico
typedef struct {
  unsigned int xkey, ykey, ScreenMask[16], CursorMask[16];
} Gcursor;



// Clase  MOUSE en modo grafico
class GMouse {

   private:

      unsigned  int  Driver_exists: 1;       // Esta el driver del mouse presente o no
      unsigned  int  Mtrabajando:   1;       // Indica si en mouse esta trabajando
		unsigned  int  Mview:         1;       // Esta el MOUSE visible o no
      unsigned  int  Sw_mouse:      1;
      unsigned  int  Tp_mouse:      3;       // Indica el tipo de mouse activo
      unsigned  int  Tipo_mouse:    3;       // Indica el tipo de mouse a mostrarse
      C_dbl          Limites;                // Indica los limites del mouse
      union REGS     Inreg, Outreg;          // Estructuras para el manejo del MOUSE

      Mstatus        V_Mstatus;
      Mresult        V_Mresult;

                     // Establece los limites para X
		void           Mxlimits(const int minx,const int maxx) const;
                     // Establece los limites para Y
		void           Mylimits(const int miny,const int maxy) const;
                     // Selecciona el tipo de cursor del mouse
      void           Setcursor(const unsigned int tp) const;


                     // Revisa si existe el driver del mouse
      unsigned int   Test_driver_exists(void) const;

	public:

                     GMouse(void)
                     {Tp_mouse = 1, Tipo_mouse = 1, Sw_mouse = 1;}

                     // Inicializa el mouse
      void           MInicializa(const unsigned int tp, const C_dbl lim) const;

		Mresult       *Mreset(void) const;
		Mstatus        Mpos(void) const;
                     // Inidca si se visualiza el mouse o no
		void           Muestra_mouse(const unsigned int tp) const;
                     // Establece nuevos limites para el mouse
      void           Establece_limites(const C_dbl lim) const
                     {Mxlimits(lim.x1,lim.x2); Mylimits(lim.y1,lim.y2);}
                     // Restablece los limites del mouse
      void           Restaura_limites(void) const
                     {Establece_limites(Limites);}
                     // Revisa si el mouse ha sido movido
      int            Ismoving(void) const;
                     // Retorna el estado del mouse
      Mstatus        Retorna_estado_mouse(void) const
                     {return V_Mstatus;}
                     // Retorna el estado del mouse
      Mresult        Retorna_Mresult(void) const
                     {return V_Mresult;}
                     // Controla el tipo de mouse 
      void           Selecciona_tipo_mouse(const unsigned int tp)
                     {Tp_mouse = tp;}
                     // Controla el inicio del cambio de forma del mouse
      void           Ctrl_mouse(const unsigned int tp)
                     {Sw_mouse = tp;}


/*
//////////////////////////////////////////////////////////////
// No se usan en este momento estas rutinas
//////////////////////////////////////////////////////////////

typedef struct {
  int x_count, y_count;
} Mmovement;


      Mstatus        Mpressed(int button) const;
      Mstatus        Mreleased(int button) const;
      Mmovement      V_Mmovement;
                     // Permite posicionar el mouse en la posicion X y Y
		void           Mmoveto(int x, int y) const;
                     // Retorna el movimiento del mouse
      Mmovement      Retorna_Mmovement(void) const
                     {return V_Mmovement;}
                     // Rutinas validas solo para modo grafico
		void           Mlightpen(int set) const;
		void           Mmove_ratio(int xsize,int ysize) const;
		void           Mspeed(int speed) const;
		void           Mbox(int left,int top,int righ,int bottom) const;
		Mmovement     *Mmotion(void) const;
*/
};






#ifdef _MOUSE_TEXTO_
// Clase para mouse texto
class TMouse : public Mouse {
    public:
      void       Set_Cursor(int cursor_type,unsigned s_start,unsigned s_stop);
      void       Mlightpen(int set);
};

typedef struct {
  unsigned flag, button, xaxis, yaxis;
} mouse_event;
#endif





#endif


