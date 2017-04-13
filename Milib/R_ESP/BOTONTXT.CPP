// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "botontxt.hpp"
extern "C" {
   #include <graphics.h>
}



// Define los elementos del boton
void Boton_texto::Define_boton(const char *nombre,const int x, const int y, const int lgx, const int lgy, const int clf, const int clt) const
{
   ((Boton_texto*)this)->Nombre = new char[strlen(nombre)+1];
   strcpy(Nombre,nombre);
   Boton::Abrir(x,y,lgx,lgy,clf);
   Boton::Define_color(clf,clt);
}

// Permite cambiar el nombre del boton
void Boton_texto::Define_nombre(const char *nombre) const
{
   delete []Nombre;
   ((Boton_texto*)this)->Nombre = new char[strlen(nombre)+1];
   strcpy(Nombre,nombre);
}


// Dibuja la estructura del boton
void Boton_texto::Dibuja(void) const
{
   Ventana_marco::Dibuja();
   // Visualiza el texto en el boton
   Boton::Centra_texto((Boton::Largo_Y() - textheight("W") + 1)/2,Nombre);
   Dibuja_indicador();
}


// Activa o desactiva el indicador de boton activo
void Boton_texto::Activar(const int tp) const
{
   if(Boton_activo != tp) {
      ((Boton_texto*)this)->Boton_activo = tp;
      // Dibuja el borde indicador de boton activado
      Dibuja_indicador();
   }
}


// Dibuja el borde indicador de boton activo
void Boton_texto::Dibuja_indicador(void) const
{
   // Dibuja el marco del boton
   Boton::Marco_ventana(1);
   int cl = Boton::Actual_color_ventana();
   if(Boton_activo) cl = Boton::Actual_color_texto();
   setlinestyle(4,34952u,1);
   Boton::Marco(5,5,Boton::Largo_X()-5,Boton::Largo_Y()-5,cl,cl);
   setlinestyle(0,1,1);
}
