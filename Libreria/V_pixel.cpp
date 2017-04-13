// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV7
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en trámite 1999-2000
// Revisión  1.1-A


#include "V_pixel.hpp"

// Inicializa el arreglo de pixeles con el valor VAL
void Arreglo_pixeles::Inicializa(const int pix_x, const int pix_y, const bool val, const Dimencion_Ventana v_dim)
{
   if (Activo) return;
   // Declara el arreglo para los renglones
   Arreglo = new Definicion_pixel *[pix_x];
   // Declara el arreglo para las columnas dentro de cada renglon
   for (int i1 = 0; i1 < pix_x; i1++) Arreglo[i1] = new Definicion_pixel [pix_y];
   Num_X = pix_x, Num_Y = pix_y;
   Activo = true;
   Cambia_dimension(v_dim, val);
}

// Cambia la cantidad de pixeles en la ventana de pixeles
void  Arreglo_pixeles::Cambia_cantidad_pixeles(const int pix_x, const int pix_y, const bool val, const Dimencion_Ventana v_dim)
{
   Destruye();
   Inicializa(pix_x, pix_y, val, v_dim);
}

// Cambia la dimension de la ventana de trabajo
void Arreglo_pixeles::Cambia_dimension(const Dimencion_Ventana v_dim, const bool val)
{
   // Dimenciones de la ventana de trabajo
   Dimenciones = v_dim;
   // Escala para trabajar en la ventana de pixeles
   Escala.X = (Num_X -1) / (Dimenciones.Xf - Dimenciones.Xi); 
   Escala.Y = (Num_Y -1) / (Dimenciones.Yf - Dimenciones.Yi); 
   Asigna_todo_array(val);
}


// Destruye el contenido del arreglo
void Arreglo_pixeles::Destruye(void)
{
   if (!Activo) return;
   // Destruye el arreglo 
   for (int i1 = 0; i1 < Num_X; i1++) delete []Arreglo[i1];
   delete []Arreglo;
   Activo = false;
}

// Asigna linea
void Arreglo_pixeles::Asigna_linea(const int xi, const int yi, const int xf, const int yf)
{
     int min_x, max_x, min_y, max_y, xxi, xyi, xxf, xyf, py;
     long double m;
     int i;

     xxi = xi, xxf = xf, xyi = yi, xyf = yf;
     min_x = xxi < xxf ? xxi: xxf;
     max_x = xxf > xxi ? xxf: xxi;
     min_y = xyi < xyf ? xyi: xyf;
     max_y = xyf > xyi ? xyf: xyi;
     // Linea vertical
     if (xxi == xxf) {
        for (i = min_y; i <= max_y; i++) Asigna_valor(xi, i, true);
     }
     // Linea horizontal
     if (xyi == xyf) {
        for (i = min_x; i <= max_x; i++) Asigna_valor(i, yi, true);
     }
     // Linea en general
     if (xxi != xxf && xyi != xyf) {
        m = (xyf - xyi) / (long double) (xxf - xxi);
        for (i = min_x; i <= max_x; i++) {
            py = m * (i - xxf) + xyf;
            Asigna_valor(i, py, true);
        }
     }
}

// Asigna linea
void Arreglo_pixeles::Asigna_linea(const long double xi, const long double yi, const long double xf, const long double yf)
{
   int p1, p2, p3, p4;
///////////////////////////////////////////////////////////////////////////////   
//#error Ajustar los valores para generar una linea truncada a la ventana
   if (!Calcula_punto(xi, yi, p1, p2)) return;
   if (!Calcula_punto(xf, yf, p3, p4)) return;
///////////////////////////////////////////////////////////////////////////////   
   Asigna_linea(p1, p2, p3, p4);
}


