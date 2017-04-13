// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.3-C.1



#include "v_iconos.hpp"
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
//          Rutinas correspondientes a la clase ventana con titulo y con iconos                          //
//                                                                                                       //
//                                                                                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////


Ventana_iconos::Ventana_iconos(void)
{
   Tipo = 1;
   Nico1 = Nico2 = Nico3 = NULL;
   Ico1 = Ico2 = Ico3 = NULL;
   Nombre_iconos("CLOSE.ICO","MAXIM.ICO","MINIM.ICO");
}



// Destructor de la ventana con iconos
Ventana_iconos::~Ventana_iconos()
{
   delete []Nico1;
   delete []Nico2;
   delete []Nico3;
   delete Ico1;
   delete Ico2;
   delete Ico3;
}


// Copia el nombre de los iconos
void Ventana_iconos::Nombre_iconos(const char *n1, const char *n2, const char *n3) const
{
   delete []Nico1;
   delete []Nico2;
   delete []Nico3;
   ((Ventana_iconos*)this)->Nico1 = new char[strlen(n1)+1];
   strcpy(Nico1,n1);
   ((Ventana_iconos*)this)->Nico2 = new char[strlen(n2)+1];
   strcpy(Nico2,n2);
   ((Ventana_iconos*)this)->Nico3 = new char[strlen(n3)+1];
   strcpy(Nico3,n3);
}


// Define y abre la ventana
void  Ventana_iconos::Define_ventana(const char *tit, const int x1, const int y1, const int x2, const int y2, const unsigned int tp) const
{
   // Define el tipo de ventana
   ((Ventana_iconos*)this)->Tipo = tp;
   // Define la ventana
   Ventana_titulo::Define_ventana(tit,x1,y1,x2,y2);
   // Define los Iconos
   Carga_iconos();
}


// Define y abre la ventana centrada
void Ventana_iconos::Ventana_centrada(const char *tit, const int lgx, const int lgy, const unsigned int tp, const int inc_x, const int inc_y) const
{
   // Define el tipo de ventana
   ((Ventana_iconos*)this)->Tipo = tp;
   // Define la ventana
   Ventana_titulo::Ventana_centrada(tit,lgx,lgy,inc_x,inc_y);
   // Define los Iconos
   Carga_iconos();
}


// Cambia el tamano de la ventana
void Ventana_iconos::Cambiar_tamano(const int x1, const int y1, const int x2, const int y2) const
{ 
   // Reedibuja los iconos el la posici¢n ya establecida dentro de la ventana
   Ico1->Mover_ventana(x2-24,y1+4);
   if(Tipo) Ico2->Mover_ventana(x2-46,y1+4);
   if(Tipo == 2) Ico3->Mover_ventana(x2-68,y1+4);
   Ventana_titulo::Cambiar_tamano(x1,y1,x2,y2);
   Ventana_iconos::Dibuja_iconos();
}


// Carga la definicion de los Iconos
void Ventana_iconos::Carga_iconos(void) const
{
   // Definicion del icono primero
   ((Ventana_iconos*)this)->Ico1 = new Icono;
   Ico1->Define_icono(Nico1,X2-24,Y1+4,20,19);
   // Definicion del icono segundo
   if(Tipo) { 
      ((Ventana_iconos*)this)->Ico2 = new Icono;
      Ico2->Define_icono(Nico2,X2-46,Y1+4,20,19);
   }
   if(Tipo == 2) {
      ((Ventana_iconos*)this)->Ico3 = new Icono;
      Ico3->Define_icono(Nico3,X2-68,Y1+4,20,19);
   }
}


// Dibula la ventana con los iconos correspondientes
void Ventana_iconos::Dibuja_iconos(void) const
{
   // Reedibuja los iconos el la posici¢n ya establecida dentro de la ventana
   Ico1->Dibuja();
   if(Tipo) Ico2->Dibuja();
   if(Tipo == 2) Ico3->Dibuja();
}


// Indica el estado de los Iconos de la ventana
unsigned int Ventana_iconos::Itera(void) const
{
  ((Ventana_iconos*)this)->IconoActual = ((Ventana_iconos*)this)->IconoPresionado = 0;
  // Revisa el primer icono (salida)
  if(Ico1->Oprimido()) ((Ventana_iconos*)this)->IconoPresionado = 1;
  if(Ico1->Mouse_dentro()) ((Ventana_iconos*)this)->IconoActual = 1;
  // Revisa el segudo icono
  if(Tipo) {
     if(Ico2->Oprimido()) ((Ventana_iconos*)this)->IconoPresionado = 2;
     if(Ico2->Mouse_dentro()) ((Ventana_iconos*)this)->IconoActual = 2;
  }
  if(Tipo == 2) {
     if(Ico3->Oprimido()) ((Ventana_iconos*)this)->IconoPresionado = 3;
     if(Ico3->Mouse_dentro()) ((Ventana_iconos*)this)->IconoActual = 3;
  }
  // Revisa si el mouse esta dentro de la ventana resaltando el recuadro
  Ventana_marco::Mouse_dentro();
  return IconoPresionado;
}

