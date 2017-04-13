// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


// Comportamientos de las clase MOUSE, TMOUSE, GMOUSE para control del mouse en grafico y texto


#include "mouse.hpp"
#include "r_esp.hpp"
#include "nucleo.hpp"


// Objeto global al nuclo del sistema
extern Nucleo *NCO;

#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "Esta aplicaci¢n requiere mouse"
   #define TXT2      "presione ESC para terminar"
#else
   #define TXT1      "This aplication require the mouse"
   #define TXT2      "press ESC to exit"
#endif


#define _MOUSE_REQUERIDO_



                  /////////////////////////////////////////////////////////////////
                  //                                                             //
                  //             Define los diferentes tipos de cursor           //
                  //                                                             //
                  /////////////////////////////////////////////////////////////////



// Cursor en forma de flecha
const Gcursor arrowcursor = {
  0,0,
  {0x3fff,0x1fff,0x0fff,0x07ff,
   0x03ff,0x01ff,0x00ff,0x007f,
   0x003f,0x00ff,0x01ff,0x10ff,
   0x30ff,0xf87f,0xf87f,0xfc3f},
  {0x0000,0x4000,0x6000,0x7000,
   0x7800,0x7c00,0x7e00,0x7f00,
   0x7f80,0x7e00,0x7c00,0x4600,
   0x0600,0x0300,0x0300,0x0180 }
};



// Cursor en forma de mano
const Gcursor handcursor = {
  4,0,
  {0xf3ff,0xe1ff,0xe1ff,0xe1ff,
   0xe001,0xe000,0xe000,0xe000,
   0x8000,0x0000,0x0000,0x0000,
   0x0000,0x0000,0x8001,0xc003},
  {0x0c00,0x1200,0x1200,0x1200,
   0x13fe,0x1249,0x1209,0x1209,
   0x7249,0x9001,0x9001,0x9001,
   0x8001,0x8001,0x4002,0x3ffc}
};


// Cursor para edicion
const Gcursor editcursor = {
  0,0,
  {0xffff,0xffff,0xf00f,0xfeff,
   0xfeff,0xfeff,0xfeff,0xfeff,
   0xfeff,0xfeff,0xfeff,0xfeff,
   0xfeff,0xf00f,0xffff,0xffff},
  {0x0000,0x0000,0x0000,0x0000,
   0x0000,0x0000,0x0000,0x0000,
   0x0000,0x0000,0x0000,0x0000,
   0x0000,0x0000,0x0000,0x0000}
};


/*
// Cursor en forma de flecha horizontal izquierda-derecha
const Gcursor leftrightcursor = {
  8,8,
  {0xffff,0xffff,0xfbdf,0xf3cf,
   0xe3c7,0xc003,0x8001,0x0000,
   0x8001,0xc003,0xe3c7,0xf3cf,
   0xfbdf,0xffff,0xffff,0xffff},
  {0x0000,0x0000,0x4020,0x0c30,
   0x1428,0x27e4,0x4002,0x8001,
   0x4002,0x27e4,0x1428,0x0c30,
   0x0420,0x0000,0x0000,0x0000}
};

// cursor en forma de flecha vertical arriba-abajo
const Gcursor updowncursor = {
  8,8,
  {0xfeff,0xfcff,0xf83f,0xf01f,
   0xe00f,0xc007,0xf83f,0xf83f,
   0xf83f,0xf83f,0xc007,0xe00f,
   0xf01f,0xf83f,0xfc7f,0xfeff},
  {0x0100,0x0280,0x0440,0x0820,
   0x1010,0x2c78,0x0440,0x0440,
   0x0440,0x0440,0x3c78,0x1010,
   0x0820,0x0440,0x0280,0x0100}
};
*/




               //////////////////////////////////////////////////////////////
               //                                                          //
               //                          Clase MOUSE                     //
               //                                                          //
               //////////////////////////////////////////////////////////////







// Revisa si existe el drive del MOUSE cargado en la computadora
unsigned int GMouse::Test_driver_exists(void) const
{
   void far * address;
   address = getvect(0x33);
   return ((address != 0) && (*(unsigned char far *) address != 0xcf));
}


// Inicializa el mouse
void GMouse::MInicializa(const unsigned int tp, const C_dbl lim) const
{
   ((GMouse*)this)->Limites    = lim;
   Driver_exists = Test_driver_exists();
   Mtrabajando = SI;
   if(tp == MOUSE_REQUERIDO) {
      if(!Driver_exists) NCO->Visualiza_error(1,"",TXT1,TXT2);
   }
   if(tp == MOUSE_NO_ACTIVO) Mtrabajando = NO;
   if(tp == MOUSE_OPCIONAL && !Driver_exists) Mtrabajando = NO;
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      // Reinicializa el mouse
      Mreset();
      // Define el limite del mouse
      Establece_limites(Limites);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}



// Reinicializa el mouse
Mresult *GMouse::Mreset(void) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Mview = 0;
      ((GMouse*)this)->Inreg.x.ax = 0;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
      ((GMouse*)this)->V_Mresult.present = Outreg.x.ax;
      ((GMouse*)this)->V_Mresult.buttons = Outreg.x.bx;
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
   return (&((GMouse*)this)->V_Mresult);
}


// Muestra u oculta el mouse (1) Mostrar, (0) Ocultar
void GMouse::Muestra_mouse(const unsigned int tp) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      // Cambia el tipo de mouse
      if(Tp_mouse != Tipo_mouse) Setcursor(Tp_mouse);
      // Mouestra o ouclta el mouse
      if(Mview != tp) {
         Mview = tp;
         ((GMouse*)this)->Inreg.x.ax = Mview ? 1 : 2;
         int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
      }
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}


// Define los limites minimo y maximo de X
void GMouse::Mxlimits(const int minx, const int maxx ) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 7;
      ((GMouse*)this)->Inreg.x.cx = minx;
      ((GMouse*)this)->Inreg.x.dx = maxx;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}


// Define los limites minimo y maximo de Y
void GMouse::Mylimits(const int miny, const int maxy) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 8;
      ((GMouse*)this)->Inreg.x.cx = miny;
      ((GMouse*)this)->Inreg.x.dx = maxy;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}


// Retorna la posicion del mouse
Mstatus GMouse::Mpos(void) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 3;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
      ((GMouse*)this)->V_Mstatus.button = Outreg.x.bx;
      ((GMouse*)this)->V_Mstatus.x = Outreg.x.cx;
      ((GMouse*)this)->V_Mstatus.y = Outreg.x.dx;
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
   return (V_Mstatus);
}


// Revisa si el mouse ha sido movido, con respecto a la ultima solicitud de estado
int GMouse::Ismoving(void) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 3;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
      if(V_Mstatus.x == Outreg.x.cx && V_Mstatus.y == Outreg.x.dx) return 0;
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
   return 1;
}


// Cambia la forma del mouse en la pantalla
void GMouse::Setcursor(const unsigned int tp) const
{
   // No cambia el tipo de mous
   if(Sw_mouse) return; 
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      if(tp != Tipo_mouse) {
         Gcursor ThisCursor = arrowcursor;
         switch(tp) {
           case 2:
             ThisCursor = handcursor;
             break;
           case 3:
             ThisCursor = editcursor;
             break;
//           case 4:
//             ThisCursor = leftrightcursor;
//             break;
//           case 5:
//             ThisCursor = updowncursor;
         }
         ((GMouse*)this)->Inreg.x.ax = 0x09;
         ((GMouse*)this)->Inreg.x.bx = ThisCursor.xkey;
         ((GMouse*)this)->Inreg.x.cx = ThisCursor.ykey;
         ((GMouse*)this)->Inreg.x.dx = FP_OFF(ThisCursor.ScreenMask);
         struct SREGS seg;
         seg.es     = FP_SEG(ThisCursor.CursorMask);
         int86x(0x33,&((GMouse*)this)->Inreg,&((GMouse*)this)->Outreg,&seg);
         ((GMouse*)this)->Tipo_mouse = tp;
      }
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}



/*
void GMouse::Mmoveto(int xaxis,int yaxis) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 4;
      ((GMouse*)this)->Inreg.x.cx = xaxis;
      ((GMouse*)this)->Inreg.x.dx = yaxis;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}


Mstatus GMouse::Mpressed(int button) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 5;
      ((GMouse*)this)->Inreg.x.bx = button;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
      ((GMouse*)this)->V_Mstatus.button = Outreg.x.ax;
      ((GMouse*)this)->V_Mstatus.flag = Outreg.x.bx;
      ((GMouse*)this)->V_Mstatus.x = Outreg.x.cx;
      ((GMouse*)this)->V_Mstatus.y = Outreg.x.dx;
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
   return (V_Mstatus);
}


Mstatus GMouse::Mreleased(int button) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 6;
      ((GMouse*)this)->Inreg.x.bx = button;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
      ((GMouse*)this)->V_Mstatus.button = Outreg.x.ax;
      ((GMouse*)this)->V_Mstatus.flag = Outreg.x.bx;
      ((GMouse*)this)->V_Mstatus.x = Outreg.x.cx;
      ((GMouse*)this)->V_Mstatus.y = Outreg.x.dx;
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
   return (V_Mstatus);
}




Mmovement* GMouse::Mmotion(void) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 11;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
      ((GMouse*)this)->V_Mmovement.x_count = _CX;
      ((GMouse*)this)->V_Mmovement.y_count = _DX;
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
   return (&((GMouse*)this)->V_Mmovement);
}




void GMouse::Mmove_ratio(int xsize,int ysize) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 15;
      ((GMouse*)this)->Inreg.x.cx = xsize;
      ((GMouse*)this)->Inreg.x.dx = ysize;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}


void GMouse::Mbox(int left,int top,int right,int bottom) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 16;
      ((GMouse*)this)->Inreg.x.cx = left;
      ((GMouse*)this)->Inreg.x.dx = top;
      ((GMouse*)this)->Inreg.x.si = right;
      ((GMouse*)this)->Inreg.x.di = bottom;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}


// Modifica la velocidad del mouse
void GMouse::Mspeed(int speed) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = 19;
      ((GMouse*)this)->Inreg.x.dx = speed;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}





void GMouse::Mlightpen(int set) const
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      ((GMouse*)this)->Inreg.x.ax = set ? 13 : 14;
      int86(0x33, &((GMouse*)this)->Inreg, &((GMouse*)this)->Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}

*/



                  /////////////////////////////////////////////////////////////////
                  //                                                             //
                  //               Solo para mouse en modo texto                 //
                  //                                                             //
                  /////////////////////////////////////////////////////////////////



#ifdef _MOUSE_TEXTO_
void TMouse::Mlightpen(int set)
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      Inreg.x.ax = set ? 13 : 14;
      int86(0x33, &Inreg, &Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}


// Selecciona el tipo de cursor en modo texto
void TMouse::Set_Cursor(int cursor_type,unsigned s_start,unsigned s_stop)
{
   // Revisa que el mouse este trabajando
   #ifndef _MOUSE_REQUERIDO_
   if(Mtrabajando) {
   #endif
      Inreg.x.ax = 10;
      Inreg.x.bx = cursor_type;
      Inreg.x.cx = s_start;
      Inreg.x.dx = s_stop;
      int86(0x33, &Inreg, &Outreg);
   #ifndef _MOUSE_REQUERIDO_
   }
   #endif
}
#endif

