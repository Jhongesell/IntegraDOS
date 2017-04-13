// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#include "v_grafic.hpp"
#include "r_var.hpp"
#include "nucleo.hpp"
#include "teclado.hpp"

extern "C" {
   #include <graphics.h>
   #include <math.h>
   #include <string.h>
}


// Definicion externa del objeto Nucleo
extern Nucleo *NCO;


/////////////////////////////////////////////////////////////////////
//                   Ventana de Graficaci¢n                        //
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
// Inicializa la clase                                     //
/////////////////////////////////////////////////////////////
void Ventana_graficacion::Inicializa(void) const
{
   // Fija la posicion dentro de los ejes (real)
   ((Ventana_graficacion*)this)->C_reales.x = ((Ventana_graficacion*)this)->C_reales.y = ((Ventana_graficacion*)this)->C_reales.z = 0.0;

   // Fija la dimenciones de los ejes (real)
   ((Ventana_graficacion*)this)->Maximo.x = ((Ventana_graficacion*)this)->Maximo.y = ((Ventana_graficacion*)this)->Maximo.z = 10;
   ((Ventana_graficacion*)this)->Minimo.x = ((Ventana_graficacion*)this)->Minimo.y = ((Ventana_graficacion*)this)->Minimo.z = -10;
   Tipo_ventana = 1;  // Ventana tipo ejes coordenados
   ((Ventana_graficacion*)this)->V_pos_ini.x = 30, ((Ventana_graficacion*)this)->V_pos_ini.y = -140;
}


/////////////////////////////////////////////////////////////////////
//            Base de la Ventana de ejes coordenados               //
/////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////
// Inicializa la clase                                     //
/////////////////////////////////////////////////////////////
void B_Ventana_ejes_coordenados::Inicializa(const int t_proy, const double factor) const
{
   // indica que se trabajara en coordenadas rectangulares
   Rectangulares = 1;
   Ventana_graficacion::Inicializa();

   ((B_Ventana_ejes_coordenados*)this)->Factor = factor;

   // Ejes en tres dimenciones
   ((B_Ventana_ejes_coordenados*)this)->Proyeccion = t_proy;
   ((B_Ventana_ejes_coordenados*)this)->TETA = M_PI / 13, ((B_Ventana_ejes_coordenados*)this)->FI = 9.0 * M_PI / 20.0;
   // Fija las escalas
   Fija_escalas();
   Calcula_valores();
}

/////////////////////////////////////////////////////////////
// Retorna la posicion del mouse dentro de la ventana de   //
// graficacion                                             //
/////////////////////////////////////////////////////////////
int B_Ventana_ejes_coordenados::Posicion_mouse(C_2D &pos) const
{
   int x, y;
   Ventana::Posicion_mouse(x,y);
   pos.x = ((double) ((X1 + x) - Centro.x) / Escala.x);
   pos.y = ((double) (Centro.y - (y + Y1)) / Escala.y);
   return MOUSE_DENTRO;
}


//////////////////////////////////////////////////////////////////
//             Fija la escala dentro de la ventana              //
//////////////////////////////////////////////////////////////////

void B_Ventana_ejes_coordenados::Fija_escalas(void) const
{
   // Valores numericos para las proyecciones
   ((B_Ventana_ejes_coordenados*)this)->Centro.x   = X1 + Retorna_inicio_X() + ((Largo_X() - Retorna_inicio_X()) / 2.0);
   ((B_Ventana_ejes_coordenados*)this)->Centro.y   = Y1 + Retorna_inicio_Y() + ((Largo_Y() - Retorna_inicio_Y()) / 2.0);
   C_3D xesc;
   xesc.x = Maximo.x > - Minimo.x ? Maximo.x * 2.0: Minimo.x * (-2.0);
   xesc.y = Maximo.y > - Minimo.y ? Maximo.y * 2.0: Minimo.y * (-2.0);
   xesc.z = Maximo.z > - Minimo.z ? Maximo.z * 2.0: Minimo.z * (-2.0);
   int min = 0;
   // Tomo las escalas con referencia al tama¤o del lado mas peque¤o de la ventana
   min = (Largo_X() - Retorna_inicio_X()) < (Largo_Y() - Retorna_inicio_Y()) ? (Largo_X() - Retorna_inicio_X()) : (Largo_Y() - Retorna_inicio_Y());
   min *= Factor;
   ((B_Ventana_ejes_coordenados*)this)->Escala.x = (xesc.x == 0.0 ? 0.0 : min / xesc.x);
   ((B_Ventana_ejes_coordenados*)this)->Escala.y = (xesc.y == 0.0 ? 0.0 : min / xesc.y);
   ((B_Ventana_ejes_coordenados*)this)->Escala.z = (xesc.z == 0.0 ? 0.0 : min / xesc.z);
}


/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
void B_Ventana_ejes_coordenados::Calcula_valores(void) const
{
   ((B_Ventana_ejes_coordenados*)this)->ST = sin(TETA);
   ((B_Ventana_ejes_coordenados*)this)->CT = cos(TETA);
   ((B_Ventana_ejes_coordenados*)this)->SF = sin(FI);
   ((B_Ventana_ejes_coordenados*)this)->CF = cos(FI);
   ((B_Ventana_ejes_coordenados*)this)->x1 = CT * CF, ((B_Ventana_ejes_coordenados*)this)->x2 = ST * CF, ((B_Ventana_ejes_coordenados*)this)->x3 = CT * SF, ((B_Ventana_ejes_coordenados*)this)->x4 = ST * SF;
}


// Dibuja un punto
void B_Ventana_ejes_coordenados::Dibuja_punto(const C_3D aux, const char col, const int tp) const
{
   ((B_Ventana_ejes_coordenados*)this)->xaux1 = aux;
   if(!Rectangulares) {
      if(xaux1.x == 0.0 || xaux1.y == 0.0) return;
      ((B_Ventana_ejes_coordenados*)this)->xaux1.x = aux.x * cos(aux.y);
      ((B_Ventana_ejes_coordenados*)this)->xaux1.y = aux.x * sin(aux.y);

   }
   // Controla que no se salga de la region de dibujo
   if(tp) {
      if(xaux1.x > Maximo.x || xaux1.x < Minimo.x || xaux1.y > Maximo.y || xaux1.y < Minimo.y || xaux1.z > Maximo.z || xaux1.z < Minimo.z) return;
   }
   if(Proyeccion) {
      if(Proyecciones_2D(xaux1,((B_Ventana_ejes_coordenados*)this)->taux1)) return;
    } else {
      if(Proyecciones_3D(xaux1,((B_Ventana_ejes_coordenados*)this)->taux1)) return;
   }
   setcolor(col);
   putpixel(taux1.x,taux1.y,col);
}


// Dibuja una linea
void B_Ventana_ejes_coordenados::Dibuja_linea(const C_3D aux1,const C_3D aux2, const char col, const int tp) const
{
   ((B_Ventana_ejes_coordenados*)this)->xaux1 = aux1;
   ((B_Ventana_ejes_coordenados*)this)->xaux2 = aux2;
   if(!Rectangulares) {
      if(xaux1.x == 0.0 || xaux2.x == 0.0 || xaux1.y == 0.0 || xaux2.y == 0.0) return;
      ((B_Ventana_ejes_coordenados*)this)->xNorma  = sqrt((xaux1.x * xaux1.x) +  (xaux1.y * xaux1.y));
      ((B_Ventana_ejes_coordenados*)this)->xTeta   = atan2(xaux1.y, xaux1.x);
      ((B_Ventana_ejes_coordenados*)this)->xaux1.x = xNorma * cos(xTeta);
      ((B_Ventana_ejes_coordenados*)this)->xaux1.y = xNorma * sin(xTeta);

      ((B_Ventana_ejes_coordenados*)this)->xNorma  = sqrt((xaux2.x * xaux2.x) +  (xaux2.y * xaux2.y));
      ((B_Ventana_ejes_coordenados*)this)->xTeta   = atan2(xaux2.y, xaux2.x);
      ((B_Ventana_ejes_coordenados*)this)->xaux2.x = xNorma * cos(xTeta);
      ((B_Ventana_ejes_coordenados*)this)->xaux2.y = xNorma * sin(xTeta);
   }
   // Controla que no se salga de la region de dibujo
   if(tp) {
      if(xaux1.x > Maximo.x || xaux1.x < Minimo.x || xaux1.y > Maximo.y || xaux1.y < Minimo.y || xaux1.z > Maximo.z || xaux1.z < Minimo.z) return;
      if(xaux2.x > Maximo.x || xaux2.x < Minimo.x || xaux2.y > Maximo.y || xaux2.y < Minimo.y || xaux2.z > Maximo.z || xaux2.z < Minimo.z) return;
   }
   if(Proyeccion) {
      if(Proyecciones_2D(xaux1,((B_Ventana_ejes_coordenados*)this)->taux1)) return;
      if(Proyecciones_2D(xaux2,((B_Ventana_ejes_coordenados*)this)->taux2)) return;
    } else {
      if(Proyecciones_3D(xaux1,((B_Ventana_ejes_coordenados*)this)->taux1)) return;
      if(Proyecciones_3D(xaux2,((B_Ventana_ejes_coordenados*)this)->taux2)) return;
   }
   setcolor(col);
   line(taux1.x,taux1.y,taux2.x,taux2.y);
}


/*
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
void B_Ventana_ejes_coordenados::Acercamiento(void) const
{

   double xx;
   C_2I v_max, v_min;
   C_dbl vent;
   if(Selecciona_porcion_ventana(vent)) {
      v_max.x = (X1 + vent.x1) - Centro.x;
      v_max.y = Centro.y - (vent.y1 + Y1);
      v_min.x = (vent.x2 + X1) - Centro.x;
      v_min.y = Centro.y - (vent.y2 + Y1);

      C_3D xv_max, xv_min;

      xv_max.x = (double) v_max.x / Escala.x;
      xv_min.x = (double) v_min.x / Escala.x;
      xv_max.y = (double) v_max.y / Escala.y;
      xv_min.y = (double) v_min.y / Escala.y;

      if(xv_max.x < xv_min.x)  xx = xv_max.x, xv_max.x = xv_min.x, xv_min.x = xx;
      if(xv_max.y < xv_min.y)  xx = xv_max.y, xv_max.y = xv_min.y, xv_min.y = xx;

      double dif = fabs(xv_max.x - xv_min.x) > fabs(xv_max.y - xv_min.y) ? fabs(xv_max.x - xv_min.x) : fabs(xv_max.y - xv_min.y);
      xv_min.x = xv_max.x - dif;
      xv_min.y = xv_max.y - dif;

      Fija_dimenciones(xv_max,xv_min);
   }

}
*/

/////////////////////////////////////////////////////////////
// Mueve el origen del eje coordenado                      //
/////////////////////////////////////////////////////////////
void B_Ventana_ejes_coordenados::Mover_origen(const int tp) const
{
   if(!Tipo_ventana) return; // no mueve origen
   switch(tp) {
     case FLECHA_ARRIBA:    // Hacia arriba
       if(Centro.y > Y1 + 3) ((B_Ventana_ejes_coordenados*)this)->Centro.y--;
       if(Revisa_graficacion()) ((B_Ventana_ejes_coordenados*)this)->Centro.y++;;
       break;
     case FLECHA_ABAJO:     // Hacia abajo
       if(Centro.y < Y2) ((B_Ventana_ejes_coordenados*)this)->Centro.y++;
       if(Revisa_graficacion()) ((B_Ventana_ejes_coordenados*)this)->Centro.y--;;
       break;
     case FLECHA_IZQUIERDA: // Hacia la izquierda
       if(Centro.x > X1) ((B_Ventana_ejes_coordenados*)this)->Centro.x--;
       if(Revisa_graficacion()) ((B_Ventana_ejes_coordenados*)this)->Centro.x++;;
       break;
     case FLECHA_DERECHA:   // Hacia la derecha
       if(Centro.x < X2) ((B_Ventana_ejes_coordenados*)this)->Centro.x++;
       if(Revisa_graficacion()) ((B_Ventana_ejes_coordenados*)this)->Centro.x--;;
       break;
   }
}


/////////////////////////////////////////////////////////////
// Acercar / alejer la ventana                             //
/////////////////////////////////////////////////////////////
void B_Ventana_ejes_coordenados::Acercar_alejar(const char tp) const
{
   switch(tp) {
      case '+': // Acerca los ejes coordenados
        ((B_Ventana_ejes_coordenados*)this)->Escala.x *= 1.01, ((B_Ventana_ejes_coordenados*)this)->Escala.y *= 1.01, ((B_Ventana_ejes_coordenados*)this)->Escala.z *= 1.01;
        if(!Revisa_graficacion()) break;

      case '-': // Aleja los ejes coordenados
        ((B_Ventana_ejes_coordenados*)this)->Escala.x *= 0.99, ((B_Ventana_ejes_coordenados*)this)->Escala.y *= 0.99, ((B_Ventana_ejes_coordenados*)this)->Escala.z *= 0.99;
        break;
   }
}


// Calcula la proyeccion de un punto de 2D a la ventana
int   B_Ventana_ejes_coordenados::Proyecciones_2D(const C_3D pt_r, C_2I &pt_p) const
{
  // Revisa el tipo de ventana
  if(Tipo_ventana) {
     // Ventana de ejes coordenados
     ((B_Ventana_ejes_coordenados*)this)->pt_x = (pt_r.x  * Escala.x) + Centro.x;
     ((B_Ventana_ejes_coordenados*)this)->pt_y =  Centro.y - (pt_r.y * Escala.y);
   } else {
     // Descarta graficaciones por fuera de los rangos de visualizacion
     if(pt_r.x > Maximo.x || pt_r.x < Minimo.x || pt_r.y > Maximo.y || pt_r.y < Minimo.y) return 1;
     // Ventana de caja
     ((B_Ventana_ejes_coordenados*)this)->pt_x = (pt_r.x - Minimo.x) * Escala.x + V_pos_ini.x;
     ((B_Ventana_ejes_coordenados*)this)->pt_y = (Centro.y * 2) - ((pt_r.y - Minimo.y) * Escala.y) + V_pos_ini.y;
  }
  if(fabs(pt_x) > 32000.00 || fabs(pt_y) > 32000.00) return 1;
  pt_p.x = (int) pt_x;
  pt_p.y = (int) pt_y;
  if(pt_p.x > (X1 + Retorna_inicio_X()) &&  pt_p.x < (X1 + Largo_X() -3) && pt_p.y > (Y1 + Retorna_inicio_Y()+2) && pt_p.y < (Y1 + Largo_Y() - 3)) return 0;
  return 1;
}



// Calcula la proyeccion de un punto de 3D a la ventana
int B_Ventana_ejes_coordenados::Proyecciones_3D(const C_3D pt_r, C_2I &pt_p) const
{
  // Revisa el tipo de ventana
  if(Tipo_ventana) {
     // Ventana de ejes coordenados
     ((B_Ventana_ejes_coordenados*)this)->aux3 = pt_r;
     ((B_Ventana_ejes_coordenados*)this)->aux3.x *= Escala.x, ((B_Ventana_ejes_coordenados*)this)->aux3.y *= Escala.y, ((B_Ventana_ejes_coordenados*)this)->aux3.z *= Escala.z;
     ((B_Ventana_ejes_coordenados*)this)->aux4.x = (aux3.z * ST) + (aux3.x * CT);
     ((B_Ventana_ejes_coordenados*)this)->aux4.y = (aux3.z * x1) - (aux3.x * x2) + (aux3.y * SF);
     ((B_Ventana_ejes_coordenados*)this)->aux4.z = (aux3.z * x3) - (aux3.x * x4) - (aux3.y * CF) + 770;
     ((B_Ventana_ejes_coordenados*)this)->pt_x = ((800 * aux4.x) / aux4.z) + Centro.x;
     ((B_Ventana_ejes_coordenados*)this)->pt_y = Centro.y - ((800 * aux4.y) / aux4.z) + 5;
    } else {
     // Descarta graficaciones por fuera de los rangos de visualizacion
     if(pt_r.x > Maximo.x || pt_r.x < Minimo.x || pt_r.y > Maximo.y || pt_r.y < Minimo.y || pt_r.z > Maximo.z || pt_r.z < Minimo.z) return 1;
     // Ventana de caja 
     ((B_Ventana_ejes_coordenados*)this)->aux3 = pt_r;
     ((B_Ventana_ejes_coordenados*)this)->aux3.x *= Escala.x, ((B_Ventana_ejes_coordenados*)this)->aux3.y *= Escala.y, ((B_Ventana_ejes_coordenados*)this)->aux3.z *= Escala.z;
     ((B_Ventana_ejes_coordenados*)this)->aux4.x = (aux3.z * ST) + (aux3.x * CT);
     ((B_Ventana_ejes_coordenados*)this)->aux4.y = (aux3.z * x1) - (aux3.x * x2) + (aux3.y * SF);
     ((B_Ventana_ejes_coordenados*)this)->aux4.z = (aux3.z * x3) - (aux3.x * x4) - (aux3.y * CF) + 770;
     ((B_Ventana_ejes_coordenados*)this)->pt_x = ((600 * aux4.x) / aux4.z) + (Centro.x -100);
     ((B_Ventana_ejes_coordenados*)this)->pt_y = (Centro.y - 30) - ((600 * aux4.y) / aux4.z) + 5;

  }
  if(fabs(pt_x) > 32000.00  || fabs(pt_y) > 32000.00) return 1;
  pt_p.x = (int) pt_x;
  pt_p.y = (int) pt_y;

  if(pt_p.x > (X1 + Retorna_inicio_X()) &&  pt_p.x < (X1 + Largo_X() -3) && pt_p.y > (Y1 + Retorna_inicio_Y()+2) && pt_p.y < (Y1 + Largo_Y() - 3)) return 0;
  return 1;
}




/////////////////////////////////////////////////////////////
// Rota ejes coordenados                                   //
/////////////////////////////////////////////////////////////
void B_Ventana_ejes_coordenados::Rota_ejes(const int tp) const
{
//////////////////////////////////////////
// Tener cuidado cuendo es un cubo
//////////////////////////////////////////
   int ang = 0;
   double rad = 0;
   switch(tp) {
      case PGDN:
      case PGUP:
      case FLECHA_ARRIBA:     // Rota hacia arriba
        ang = 1, rad = 0.01;
        break;
      case FLECHA_ABAJO:      // Rota hacia abajo
        ang = 1, rad = -0.01;
        break;
      case FLECHA_IZQUIERDA:  // Rota hacia la izquierda
        rad = 0.01;
        break;
      case FLECHA_DERECHA:    // Rota hacia la derecha
        rad = -0.01;
   }
   if(ang) ((B_Ventana_ejes_coordenados*)this)->FI += rad;
    else ((B_Ventana_ejes_coordenados*)this)->TETA += rad;
   Calcula_valores();
}





///////////////////////////////////////////////////////////////
//                                                           //
//  Ventana con cursor                                       //
//                                                           //
///////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// Inicializa la clase                                     //
/////////////////////////////////////////////////////////////
void V_cursor::Inicializa(void) const
{
   // Define al puntor que contiene la imagen debajo del cursor
   unsigned int i = imagesize(0,0,7,7);
   ((V_cursor*)this)->Cursor = new char [i+10];
   // Fija la posicion del cursor
   ((V_cursor*)this)->Pos_ant.x = ((V_cursor*)this)->Pos_ant.y = 0;
   sw_cursor = 0;
}



/////////////////////////////////////////////////////////////
// Controla la visualizacion del cursor                    //
/////////////////////////////////////////////////////////////
void V_cursor::Visualiza_cursor(const int tp) const
{
   // Hace no visible el mouse
   NCO->Muestra_mouse(NO);
   if(tp) { // Muestra el cursor 
      if(Proyeccion) {
         if(Proyecciones_2D(C_reales,((V_cursor*)this)->Pos_tmp)) return;
       } else {
         if(Proyecciones_3D(C_reales,((V_cursor*)this)->Pos_tmp)) return;
      }
      ((V_cursor*)this)->Pos_ant = Pos_tmp;
      if(!sw_cursor) getimage(Pos_ant.x,Pos_ant.y,Pos_ant.x+7,Pos_ant.y+7,Cursor);
      setcolor(Magenta1);
      line(Pos_ant.x,Pos_ant.y,Pos_ant.x+7,Pos_ant.y+7);
      line(Pos_ant.x,Pos_ant.y,Pos_ant.x+5,Pos_ant.y);
      line(Pos_ant.x,Pos_ant.y,Pos_ant.x,Pos_ant.y+5);
      sw_cursor = 1;
   } else { // Oculta el cursor
      if(sw_cursor) putimage(Pos_ant.x,Pos_ant.y,Cursor,COPY_PUT);
      sw_cursor = 0;
   }
}





/////////////////////////////////////////////////////////////
//                                                         //
// Ventana con ejes coordenados en 2D y 3D                 //
//                                                         //
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// Inicializa la clase                                     //
/////////////////////////////////////////////////////////////
void V_ejes_coordenados::Inicializa(const int t_proy, const double factor) const
{
   // Inicializa la ventana de graficacion en 3D
   B_Ventana_ejes_coordenados::Inicializa(t_proy,factor);
   // Indica que visualize el nombre de los ejes
   Visualiza_nombre_eje = 1;
   ((V_ejes_coordenados*)this)->Nombre_eje_X = NULL;
   ((V_ejes_coordenados*)this)->Nombre_eje_Y = NULL;
   ((V_ejes_coordenados*)this)->Nombre_eje_Z = NULL;
   // Fija el color de los ejes coordenados
   ((V_ejes_coordenados*)this)->Color_eje.x = Rojo1;     
   ((V_ejes_coordenados*)this)->Color_eje.y = Verde9;    
   ((V_ejes_coordenados*)this)->Color_eje.z = Azul1;     
}


/////////////////////////////////////////////////////////////
// Dibuja los ejes coordenados en 2D                       //
/////////////////////////////////////////////////////////////
void V_ejes_coordenados::Dibuja_ejes_2D(const int tp) const
{
   NCO->Muestra_mouse(NO);
   int cl,i, sw = 0;
   C_3D pos, ctr;
   C_2I pt, ct;
   if(Tipo_ventana) {
      pos.x = 0.0, pos.y = 0.0, pos.z = 0.0;
      ctr = pos;
      // Eje X
      Proyecciones_2D(pos,ct);
      for(i = 0; i < 2;++i) {
         setlinestyle(1-i,0,i);
         pos.x = i ? Maximo.x : Minimo.x;
         cl = tp ? Color_eje.x : Actual_color_fondo();
         Dibuja_linea(ctr,pos,cl,1);
         if(Visualiza_nombre_eje) {
            sw = !Proyecciones_2D(pos,pt) ? 1 : 0;
            if(sw) {
               // Indica el nombre del eje coordenado
               if(!i) {
                  outtextxy(pt.x-15,pt.y-10,"-");
                  outtextxy(pt.x-5,pt.y-10,Nombre_eje_X);
                } else {
                  outtextxy(pt.x+5,pt.y-10,Nombre_eje_X);
               }
            }
         }
         pos.x = 0.0;
         // Eje Y
         pos.y = i ? Maximo.y : Minimo.y;
         Proyecciones_2D(pos,pt);
         cl = tp ? Color_eje.y : Actual_color_fondo();
         Dibuja_linea(ctr,pos,cl,1);
         // Indica el nombre del eje coordenado
         if(Visualiza_nombre_eje) {
            sw = !Proyecciones_2D(pos,pt) ? 1 : 0;
            if(sw) {
               if(!i) {
                  outtextxy(pt.x-15,pt.y+5,"-");
                  outtextxy(pt.x-5,pt.y+5,Nombre_eje_Y);
                } else {
                  outtextxy(pt.x+5,pt.y-13,Nombre_eje_Y);
               }
            }
         }
         pos.y = 0.0;
      }
   } else {
/////////////////////
// Poder ocultar el recuadro superior derecho del recuadro
/////////////////////
      pos.x= Minimo.x; pos.y= Maximo.y;
      Proyecciones_2D(pos,pt);
      ctr.x= Maximo.x; ctr.y= Minimo.y;
      Proyecciones_2D(ctr,ct);
      setcolor(tp ? Color_eje.x : Actual_color_fondo());
      rectangle(pt.x-1,pt.y-1,ct.x+1,ct.y+1);
      // Nombre del eje X
      outtextxy(pt.x-1,ct.y+13,Nombre_eje_X);
      setcolor(tp ? Color_eje.y : Actual_color_fondo());
      line(pt.x-1,pt.y-1,pt.x-1,ct.y+1);
      line(ct.x+1,pt.y-1,ct.x+1,ct.y+1);
      // Nombre del eje Y
      settextstyle(0,1,1);
      outtextxy(pt.x-10,ct.y-textwidth(Nombre_eje_Y),Nombre_eje_Y);
      settextstyle(0,0,1);
   }
}


/////////////////////////////////////////////////////////////
// Dibuja los ejes coordenados en 3D                       //
/////////////////////////////////////////////////////////////
void V_ejes_coordenados::Dibuja_ejes_3D(const int tp) const
{
   NCO->Muestra_mouse(NO);
   int cl,i, sw = 0;
   C_3D pos, ctr;
   C_2I pt, ct;
   if(Tipo_ventana) {
      pos.x = 0, pos.y = 0, pos.z = 0;
      ctr = pos;
      // Eje X
      Proyecciones_3D(pos,ct);
      for(i = 0; i < 2;++i) {
         setlinestyle(1-i,0,i);
         pos.x = i ? Maximo.x : Minimo.x;
         cl = tp ? Color_eje.x : Actual_color_fondo();
         Dibuja_linea(ctr,pos,cl,1);
         if(Visualiza_nombre_eje) {
            sw = !Proyecciones_3D(pos,pt) ? 1 : 0;
            if(sw) {
               // Indica el nombre del eje coordenado
               if(!i) {
                  outtextxy(pt.x-15,pt.y-10,"-");
                  outtextxy(pt.x-5,pt.y-10,Nombre_eje_X);
                } else {
                  outtextxy(pt.x+5,pt.y-10,Nombre_eje_X);
               }
            }
         }
         pos.x = 0.0;
         // Eje Y
         pos.y = i ? Maximo.y : Minimo.y;
         Proyecciones_3D(pos,pt);
         cl = tp ? Color_eje.y : Actual_color_fondo();
         Dibuja_linea(ctr,pos,cl,1);
         // Indica el nombre del eje coordenado
         if(Visualiza_nombre_eje) {
            sw = !Proyecciones_3D(pos,pt) ? 1 : 0;
            if(sw) {
               if(!i) {
                  outtextxy(pt.x-15,pt.y+5,"-");
                  outtextxy(pt.x-5,pt.y+5,Nombre_eje_Y);
                } else {
                  outtextxy(pt.x+5,pt.y-13,Nombre_eje_Y);
               }
            }
         }
         pos.y = 0.0;
         // Eje Z
         pos.z = i ? Maximo.z : Minimo.z;
         Proyecciones_3D(pos,pt);
         cl = tp ? Color_eje.z : Actual_color_fondo();
         Dibuja_linea(ctr,pos,cl,1);
         // Indica el nombre del eje coordenado
         if(Visualiza_nombre_eje) {
            sw = !Proyecciones_3D(pos,pt) ? 1 : 0;
            if(sw) {
               if(!i) {
                  outtextxy(pt.x-15,pt.y+5,"-");
                  outtextxy(pt.x-5,pt.y+5,Nombre_eje_Z);
                } else {
                  outtextxy(pt.x+5,pt.y-13,Nombre_eje_Z);
               }
            }
         }
         pos.z = 0.0;
      }

   } else {

/////////////////////
// Poder ocultar el recuadro superior derecho del recuadro
/////////////////////
      // Cara tracera
      pos.x= Minimo.x; pos.y= Maximo.y, pos.z = Maximo.z;
      Proyecciones_3D(pos,pt);
      ctr.x= Maximo.x; ctr.y= Minimo.y, ctr.z = Maximo.z;
      Proyecciones_3D(ctr,ct);
      // Apaga el mouse
      setcolor(tp ? Color_eje.x : Actual_color_fondo());
      rectangle(pt.x-1,pt.y-1,ct.x+1,ct.y+1);
      setcolor(tp ? Color_eje.y : Actual_color_fondo());
      line(pt.x-1,pt.y-1,pt.x-1,ct.y+1);
      line(ct.x+1,pt.y-1,ct.x+1,ct.y+1);

      // Cara de enfrente
      pos.x= Minimo.x; pos.y= Maximo.y, pos.z = Minimo.z;
      Proyecciones_3D(pos,pt);
      ctr.x= Maximo.x; ctr.y= Minimo.y, ctr.z = Minimo.z;
      Proyecciones_3D(ctr,ct);
      setcolor(tp ? Color_eje.x : Actual_color_fondo());
      rectangle(pt.x-1,pt.y-1,ct.x+1,ct.y+1);
      // Nombre del eje X
      outtextxy(pt.x-1,ct.y+13,Nombre_eje_X);
      setcolor(tp ? Color_eje.y : Actual_color_fondo());
      line(pt.x-1,pt.y-1,pt.x-1,ct.y+1);
      line(ct.x+1,pt.y-1,ct.x+1,ct.y+1);
      // Nombre del eje Y
      settextstyle(0,1,1);
      outtextxy(pt.x-10,ct.y-textwidth(Nombre_eje_Y),Nombre_eje_Y);
      settextstyle(0,0,1);

      // Lineas que unen las caras
      setcolor(tp ? Color_eje.z : Actual_color_fondo());

      pos.x= Minimo.x; pos.y= Minimo.y, pos.z = Minimo.z;
      Proyecciones_3D(pos,pt);
      ctr.x= Minimo.x; ctr.y= Minimo.y, ctr.z = Maximo.z;
      Proyecciones_3D(ctr,ct);
      line(pt.x,pt.y,ct.x,ct.y);


      pos.x= Minimo.x; pos.y= Maximo.y, pos.z = Minimo.z;
      Proyecciones_3D(pos,pt);
      ctr.x= Minimo.x; ctr.y= Maximo.y, ctr.z = Maximo.z;
      Proyecciones_3D(ctr,ct);
      line(pt.x,pt.y,ct.x,ct.y);



      pos.x= Maximo.x; pos.y= Maximo.y, pos.z = Minimo.z;
      Proyecciones_3D(pos,pt);
      ctr.x= Maximo.x; ctr.y= Maximo.y, ctr.z = Maximo.z;
      Proyecciones_3D(ctr,ct);
      line(pt.x,pt.y,ct.x,ct.y);


      pos.x= Maximo.x; pos.y= Minimo.y, pos.z = Minimo.z;
      Proyecciones_3D(pos,pt);
      ctr.x= Maximo.x; ctr.y= Minimo.y, ctr.z = Maximo.z;
      Proyecciones_3D(ctr,ct);
      line(pt.x,pt.y,ct.x,ct.y);

   }
}





///////////////////////////////////////////////////////////////
//                                                           //
// Ventana de graficacion en 2D y 3D con cursor y ejes       //
// cordenados                                                //
//                                                           //
///////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
// Revisa que la zona de graficacion este dentro de la     //
// ventana                                                 //
/////////////////////////////////////////////////////////////

int V_graficacion::Revisa_graficacion(void) const
{
   C_3D pt_r;
   C_2I pt_v;
   int i;
   double xdim[6][3];
   for(i = 0; i < 6; i++) xdim[i][0] = xdim[i][1] = xdim[i][2] = 0;
   xdim[0][0] = Maximo.x * 1.14, xdim[1][1] = Maximo.y * 1.14, xdim[2][2] = Maximo.z * 1.14;
   xdim[3][0] = Minimo.x * 1.18, xdim[4][1] = Minimo.y * 1.18, xdim[5][2] = Minimo.z * 1.18;
   if(!Tipo_ventana) { // Para caja o cubo
      xdim[0][0] = Maximo.x, xdim[1][1] = Maximo.y, xdim[2][2] = Maximo.z;
      xdim[3][0] = Minimo.x, xdim[4][1] = Minimo.y, xdim[5][2] = Minimo.z;
   }
   for(i = 0; i < 6; i++) {
      pt_r.x = xdim[i][0], pt_r.y = xdim[i][1], pt_r.z = xdim[i][2];
      if(Proyeccion) {
         if(Proyecciones_2D(pt_r, pt_v)) return 1;
       } else {
         if(Proyecciones_3D(pt_r, pt_v)) return 1; 
      }
   }
   return 0;
}       




