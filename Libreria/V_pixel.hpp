// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifndef __V_PIXEL_HPP__
#define __V_PIXEL_HPP__

#include "definic.hpp"

// Definición del pixel
struct Definicion_pixel {
   bool pixel;
};


// Clase que manipila y controla un array de pixeles
class Arreglo_pixeles
{
   private:

      bool               Activo;        // Indica si esta activo el objeto
      int                Num_Y;         // Longitud de la matriz en Y
      int                Num_X;         // Longitud de la matrix en X
      C2D                Escala;        // Escala usada dentro de la ventana de pixeles
      Dimencion_Ventana  Dimenciones;   // Dimenciones de la ventana de trabajo
      int                pxx, pyx;      // Valores temporales
      int                p_x, p_y;
      int                xi1, xi2;
      int                xi, xf, yi, yf, x, y;
      bool               st;
      
      Definicion_pixel **Arreglo;       // Puntero al arreglo de pixeles
      Definicion_pixel  *prtArreglo;    // Puntero al arreglo de pixeles

   public:
                     // Constructor de la clase
                     Arreglo_pixeles(void)
                     { Activo = false; }
                     // Destructor de la clase
                    ~Arreglo_pixeles(void)
                     {Destruye();}
                     // Calcula la posición de un punto dentro de la ventana de pixeles
               bool  Calcula_punto(const long double x, const long double y, int &px, int &py)
                     {
                        st = true;
                        if ((y >= Dimenciones.Yi && y <= Dimenciones.Yf) && (x >= Dimenciones.Xi && x <= Dimenciones.Xf)) {
                           py = Num_Y - (((y - Dimenciones.Yi) * Escala.Y) + 1.0);
                           px = (((x - Dimenciones.Xi) * Escala.X));
                        } else st = false;
                        return st;
                     }
                     // Inicializa la clase
               void  Inicializa(const int pix_x, const int pix_y, const bool val, const Dimencion_Ventana v_dim);
                     // Asigna a un pixel del arreglo el valor indicado
               void  Asigna_valor(const int pix_x, const int pix_y, const bool val)
                     {
                        if (Activo && pix_x >= 0 && pix_x < Num_X && pix_y >= 0 && pix_y < Num_Y) Arreglo[pix_x][pix_y].pixel = val;
                     }
                     // Asigna a un pixel del arreglo el valor indicado
               void  Asigna_valor(const long double x, const long double y, const bool val)
                     {
                        if (Calcula_punto(x, y, pxx, pyx)) Asigna_valor(pxx, pyx, val);
                     }
                     // Destruye el contenido del arreglo
               void  Destruye(void);
                     // Retorna el valor del pixel indicado dentro del arreglo
               bool  Retorna_valor(const int pix_x, const int pix_y) const
                     {
                        if (Activo) {
                           if (pix_x >= 0 && pix_x < Num_X && pix_y >= 0 && pix_y < Num_Y) return (Arreglo[pix_x][pix_y].pixel);
                        }                  
                        return false;
                     }
                     // Asigna a todo el array el valor especificado
               void  Asigna_todo_array(const bool val) 
                     {
                        if (Activo) {
                           for (xi1 = 0; xi1 <  Num_X; xi1++) {
                              prtArreglo = &Arreglo[xi1][0];
                              for (xi2 = 0; xi2 <  Num_Y; xi2++) {
                                 prtArreglo->pixel = val;
                                 prtArreglo++;
                              }   
                           }
                        }
                     }
                     // Asigna linea
               void  Asigna_linea(const int xi, const int yi, const int xf, const int yf);
                     // Asigna linea
               void  Asigna_linea(const long double xi, const long double yi, const long double xf, const long double yf);
                     // Asigna un recuadro apartir del centro n pixeles
               void  Asigna_recuadro(const int pix_x, const int pix_y, const int num, const bool val)
                     {

                        xi = pix_x - num >= 0 ? pix_x - num : 0;
                        xf = pix_x + num < Num_X ? pix_x + num : Num_X -1;
       
                        yi = pix_y - num >= 0 ? pix_y - num : 0;
                        yf = pix_y + num < Num_Y ? pix_y + num : Num_Y -1;
                        for (y = yi; y <= yf; y ++) {
                             for (x = xi; x <= xf; x ++) {
                                  Arreglo[x][y].pixel = val;
                             } 
                        }
                     }
               
                     // Asigna un recuadro apartir del centro n pixeles
               void  Asigna_recuadro(const long double x, const long double y, const int num, const bool val)
                     {
                        if (Calcula_punto(x, y, p_x, p_y)) Asigna_recuadro(p_x, p_y, num, val);
                     }
                     // Cambia la dimension del al ventana
               void  Cambia_dimension(const Dimencion_Ventana v_dim, const bool val = false);
                     // Cambia la cantidad de pixeles en la ventana
               void  Cambia_cantidad_pixeles(const int pix_x, const int pix_y, const bool val, const Dimencion_Ventana v_dim);
};


// Notas:
//   (1) El array empieza en cero para los pixeles Y y X
//   (2) El numero maximo de elemetos depende de la memoria de la maquina

#endif



