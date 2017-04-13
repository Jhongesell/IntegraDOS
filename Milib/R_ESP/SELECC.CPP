// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "selecc.hpp"


// Define los elementos del boton
void Seleccionador::Define_boton(const int x, const int y, const int tp, const int lg, const int cl) const
{
   ((Seleccionador*)this)->Boton_activo = tp;
   Boton::Abrir(x,y,lg,lg,cl);
}


void Seleccionador::Dibuja(void) const
{
   // Visualiza el boton
   Ventana_marco::Dibuja();
   // Dibuja el indicador de activado
   if(Boton_activo) Visualiza_caracter(1,1,254,0);
}


