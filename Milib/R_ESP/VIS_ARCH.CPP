// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "vis_arch.hpp"
#include "teclado.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;


// Controlador del visualizador de archivos
void Visualiza_archivo::Controlador(const int tc)
{
   // Cambia el tipo de cursor del mouse
   if(Ventana::Mouse_dentro()) NCO->Selecciona_tipo_mouse(3);
   // Si no hay accion a realizar en tecla retorna
   if(tc == -1) return;
   // Oculta el cursor
   Visualiza_cursor(0);
   pt_text = Lista2l->Contenido_nodo(Ctrl_mov_ver->Elemento_actual());
   // Controla el movimineto dentro del visualizador
   if(tc == ENTER) Controla_movimiento(FLECHA_ABAJO__HOME);
    else Controla_movimiento(tc);
   Visualiza_texto();
}





