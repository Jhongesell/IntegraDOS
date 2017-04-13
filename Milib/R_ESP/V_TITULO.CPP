// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.2-C.1



#include "v_titulo.hpp"
#include "cad_car.hpp"
#include "r_var.hpp"
#include "r_esp.hpp"

extern "C" {
   #include <string.h>
   #include <graphics.h>
}


extern int X_MAX;
extern int Y_MAX;




///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                                                                                       //
//                            Rutinas correspondientes a la clase ventana con titulo                     //
//                                                                                                       //
//                                                                                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////


// Contructor de la ventana de titulo
Ventana_titulo::Ventana_titulo(void)
{
   // Inicializaci¢n de datos
   Titulo = NULL;
   C_fdo = Gris1, C_tit_act = Cian9, C_tit_no_act = Azul9, C_txt = Blanco;
   pt_x1 = pt_y1 = pt_x2 = 0, pt_y2 = 23; 
   Lt_xtipo = Lt_xhori = 0;
   Lt_xdivx = Lt_xdivy = 1;
   Lt_xmuly = 2;
   Lt_xmulx = -2;
   Tit_centrado = 1;
}


// Destructor de la ventana de  titulo
Ventana_titulo::~Ventana_titulo()
{
   delete []Titulo;
}


// Define la ventana con titulo
void  Ventana_titulo::Define_ventana(const char *tit, const int x1, const int y1, const int x2, const int y2) const
{
   // Define el titulo
   Define_titulo(tit);
   // Pasa los parametros  de la ventana
   Ventana_marco::Define_ventana(x1,y1,x2,y2,C_fdo);
   ((Ventana_titulo*)this)->Inicia_X = 2, ((Ventana_titulo*)this)->Inicia_Y = 23;
}


// Define la ventana de titulo centrada
void Ventana_titulo::Ventana_centrada(const char *tit, const int lgx, const int lgy, const int inc_x, const int inc_y) const
{
   // Define el titulo
   Define_titulo(tit);
   // Pasa los parametros  de la ventana
   Ventana_marco::Ventana_centrada(lgx,lgy,C_fdo,inc_x,inc_y);
   ((Ventana_titulo*)this)->Inicia_X = 1, ((Ventana_titulo*)this)->Inicia_Y = 21;
}

void Ventana_titulo::Define_titulo(const char *tit) const
{
   if(Titulo) delete []Titulo;
   ((Ventana_titulo*)this)->Titulo = new char[strlen(tit)+1];
   s_trim(tit,Titulo);
}


// Dibuja el titulo de la ventana
void Ventana_titulo::Dibuja_titulo(void) const
{
   char c_tit = C_tit_act;
   // Limpia el recuadro del titulo
   if(!VENTANA_ACTIVA)  c_tit = C_tit_no_act;
   Ventana_marco::Limpia_recuadro(pt_x1+2,pt_y1+2,Largo_X() - (pt_x2+3),pt_y2+2,c_tit,Gris9,Blanco);   // Cambia el tipo de letra
//   Ventana_marco::Define_tipo_letra(Lt_xtipo,Lt_xhori,Lt_xmulx,Lt_xdivx,Lt_xmuly,Lt_xdivy);
   // Cambia el color para el titulo
//   Ventana_marco::Define_color(C_tit,Negro);
   // Centra el texto del titulo
//   if(Tit_centrado) {
//      Ventana_marco::Centra_texto(((pt_y2 - textheight("W")) / 2)-1,Titulo);
//      Ventana_marco::Define_color(C_tit,C_txt);
//      Ventana_marco::Centra_texto(((pt_y2 - textheight("W")) / 2)+1,Titulo);
//    } else {
//      Ventana_marco::Visualiza_texto(9,((pt_y2 - textheight("W")) / 2)-1, Titulo, 0);
      // Sombra del titulo
      Ventana_marco::Define_color(c_tit,Negro);
      Ventana_marco::Visualiza_texto(10,((pt_y2 - textheight("W")) / 2)+4,Titulo);
      // Titulo
      Ventana_marco::Define_color(c_tit,C_txt);
      Ventana_marco::Visualiza_texto(9,((pt_y2 - textheight("W")) / 2)+3,Titulo);
//   }
   Marco(2,pt_y2+3,Largo_X()-3,Largo_Y()-3,Gris9,Blanco);
   // Regresa al tipo de letra anterior
//   Ventana_marco::Define_tipo_letra(0,0,-1,0,0,0);
   // Activa el color de fondo de la ventana
   Ventana_marco::Define_color_fondo(C_fdo);
}




