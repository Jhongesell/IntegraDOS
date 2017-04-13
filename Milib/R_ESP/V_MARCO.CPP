// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.2-B.1


#include "v_marco.hpp"
#include "nucleo.hpp"
#include "cad_car.hpp"
#include "r_var.hpp"
#include "r_esp.hpp"

extern "C" {
   #include <graphics.h>
}


// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Maximo valor de la ventanade trabajo
extern int X_MAX;
extern int Y_MAX;



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                                                                                       //
//                            Rutinas correspondientes a la clase ventana con marco                      //
//                                                                                                       //
//                                                                                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////


// Constructor de ventana con marco
Ventana_marco::Ventana_marco(void)
{
   Mouse_dentro_ventana = 0, C_rec_inf = Gris9, C_rec_sup = Blanco, ((Ventana_marco*)this)->C_base_a = Cian9, C_base_n = Negro;
}


// Dibuja el marco de la ventana, Si t_b es (0) dibuja con el color c_rec_inf todo el recuadro, en caso contrario
// el recuadro superior lo hara con el color c_rec_inf y el superior con el color c_rec_sup
void Ventana_marco::Marco_ventana(const int t_b) const
{
   int cri = C_rec_inf, crs;
   // Dibuja el recuadro de la ventana
   crs = !t_b ? cri : C_rec_sup;
   // Dibuja el borde de la ventana
   Marco(0,0,Largo_X()-1,Largo_Y()-1,C_base_n,C_base_n);
   Marco(1,1,Largo_X()-2,Largo_Y()-2,cri,crs);
}


// Revisa si el mouse esta dentro de la ventana para activar esta
int Ventana_marco::Mouse_dentro(void) const
{
   int cl = C_base_n;
   int sw = 0;
   int st = Ventana::Mouse_dentro();

   // Revisa si la ventana esta activo o no
   if(st && !Mouse_dentro_ventana ) Mouse_dentro_ventana  = 1, sw = 1;
   if(!st && Mouse_dentro_ventana ) Mouse_dentro_ventana  = 0, sw = 1;
   // Dibuja la base de la ventana (tomando en cuenta si esta activa al ventana o no)
   if(sw) {
      if(Mouse_dentro_ventana) cl = C_base_a;
      Marco(0,0,Largo_X()-1,Largo_Y()-1,cl,cl);
   }
   return st;
}



// Define los colores de la ventana con marco
void Ventana_marco::Define_colores(const int c_v, const int c_r_i, const int c_r_s, const int c_b_a, const int c_b_n) const
{
   ((Ventana_marco*)this)->C_rec_inf = c_r_i, ((Ventana_marco*)this)->C_rec_sup = c_r_s, ((Ventana_marco*)this)->C_base_a = c_b_a, ((Ventana_marco*)this)->C_base_n = c_b_n;
   Ventana::Define_color_ventana(c_v);
}

// Dibuja la ventana con marco
void Ventana_marco::Dibuja(void) const
{
   Ventana::Dibuja();
   Ventana_marco::Marco_ventana(0);
}


