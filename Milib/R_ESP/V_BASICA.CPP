// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.4-D.1



#include "v_basica.hpp"
#include "nucleo.hpp"
#include "cad_car.hpp"
#include "r_var.hpp"
#include "r_esp.hpp"

extern "C" {
   #include <string.h>
   #include <graphics.h>
   #include <conio.h>
}


// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Dimenciones de la ventana
extern int X_MAX;
extern int Y_MAX;



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//                                                                                                       //
//                            Rutina correspondiente a la clase base ventana                             //
//                                                                                                       //
//                                                                                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////




// Constructor de la clase ventana
Ventana::Ventana(void)
{
   // Inicializa las variables internas de la ventana
   ARCH_TMP = ARCH_TMP1 =  NULL;
   xcad = new char[200];
   // Por omisi¢n la longitud de la ventana es toda la pantalla
   X1 = Y1 = 0, X2 = X_MAX, Y2 = Y_MAX;
   // Indica que no salvara la pantalla grafica por omicion
   SALVAR_PANTALLA = 0, C_REF = 255;
   // Define los colores de la ventana
   C_F_A = C_FOND = C_VENT = Blanco, C_T_A = C_TEXT = Negro;
   // Indica el largo y ancho del caracter activo para visualizar
   Define_tipo_letra(0,0,-1,0,0,0);
   // Ventana activa
   VENTANA_VISIBLE = VENTANA_ACTIVA = 1;
}

// Destructor de la clase ventana
Ventana::~Ventana()
{
   Oculta_ventana(CERRAR);
   // Remueve el archivo temporal
   if(SALVAR_PANTALLA) unlink(ARCH_TMP);
   delete []ARCH_TMP;
   delete []ARCH_TMP1;
   delete []xcad;
}

// Activa el grabar ventana
void Ventana::Activa_graba_ventana(const int tp) const
{
   delete []ARCH_TMP;
   ((Ventana*)this)->SALVAR_PANTALLA = tp;
   ((Ventana*)this)->ARCH_TMP = NULL;
   if(SALVAR_PANTALLA) {
      ((Ventana*)this)->ARCH_TMP = new char[80];
      Nombre_archivo_temporal(ARCH_TMP);
   }
}


// Define las coordenadas de la ventana
void Ventana::Define_ventana(const int x1, const int y1, const int x2, const int y2, const int cl) const
{
   #ifdef __VALIDA__
   // Revisa y valida los parametros de la ventana
   if(y1 > Y_MAX || y2 > Y_MAX || x1 > x2 || y1 > y2) ctrl_err(1,"VENTANA");
   #endif
   ((Ventana*)this)->X1 = x1, ((Ventana*)this)->Y1 = y1, ((Ventana*)this)->X2 = x2, ((Ventana*)this)->Y2 = y2, ((Ventana*)this)->C_FOND = ((Ventana*)this)->C_VENT = cl;
   Oculta_ventana(ABRIR);
   ((Ventana*)this)->Inicia_X = ((Ventana*)this)->Inicia_Y = 1;
}


// Define una ventana centrada en la pantalla
void Ventana::Ventana_centrada(const int lg_x, const int lg_y, const int cl, const int inc_x, const int inc_y) const
{
   #ifdef __VALIDA__
   //Revisa si los valores so correctos
   if(lg_x > X_MAX || lg_y > Y_MAX) ctrl_err(1,"VENTANA");
   #endif
   // Calcula la posicion de la ventana
   int x = (X_MAX - lg_x + 1) / 2, y = (Y_MAX - lg_y + 1) / 2;
   // Define la posici¢n de la ventana
   Define_ventana(x+inc_x,y+inc_y,x+lg_x+inc_x,y+lg_y+inc_y,cl);
}

// Define el tama¤o de la letra a visualizar en la ventana
void Ventana::Tamano_letra(const int lg_x, const int lg_y) const
{
   ((Ventana*)this)->A_CAR = lg_x, ((Ventana*)this)->L_CAR = lg_y;
   ((Ventana*)this)->N_car_visualizar = (Largo_X() - 10) / A_CAR;
   ((Ventana*)this)->N_ren_visualizar = (Largo_Y() - 10) / L_CAR;
}

// Mueve la ventana a las coordenadoas indicadas en X e Y
void Ventana::Mover_ventana(const int x, const int y) const
{
   #ifdef __VALIDA__
   // Valida los parametros de movimiento de la ventana
   if((x > 0 && X2 + x > X_MAX) || (x < 0 && X1 + x < 0) || (y > 0 && Y2 + y >= Y_MAX) || (y < 0 && Y1 + y < 0)) ctrl_err(1,"VENTANA");
   #endif
   // Graba y oculta la ventana
   Oculta_ventana(SI);
   // Posicionarse en la nueva ventana
   ((Ventana*)this)->X2 = x + Largo_X()-1, ((Ventana*)this)->Y2 = y + Largo_Y()-1, ((Ventana*)this)->X1 = x, ((Ventana*)this)->Y1 = y;
   // Restaura la ventana
   Oculta_ventana(NO);
}



// Mueve la ventana la candidad de pixeles indicade en X e Y
void Ventana::Mover_ventana_n_pixeles(const int x, const int y) const
{
   #ifdef __VALIDA__
   // Valida los parametros de movimiento de la ventana
   if((x > 0 && X2 + x > X_MAX) || (x < 0 && X1 + x < 0) || (y > 0 && Y2 + y >= Y_MAX) || (y < 0 && Y1 + y < 0)) ctrl_err(1,"VENTANA");
   #endif
   // Graba y oculta la ventana
   Oculta_ventana(SI);
   // Posicionarse en la nueva ventana
   ((Ventana*)this)->X2 += x, ((Ventana*)this)->X1 += x, ((Ventana*)this)->Y2 += y, ((Ventana*)this)->Y1 += y;
   // Restaura la ventana
   Oculta_ventana(NO);
}


// Cambia el tama¤o de la ventana actual redibujando esta
void Ventana::Cambiar_tamano(const int x1, const int y1, const int x2, const int y2) const
{
   // Restaurar la pantalla original
   Oculta_ventana(CERRAR);
   // Ajuste del tama¤o de la ventana
   ((Ventana*)this)->X1 = x1, ((Ventana*)this)->Y1 = y1, ((Ventana*)this)->X2 = x2, ((Ventana*)this)->Y2 = y2;
   // Abre la ventana con el nuevo tama¤o
   Oculta_ventana(ABRIR);
}



// Limpia el recuadro especificado
void Ventana::Limpia_recuadro(const int x1, const int y1, const int x2, const int y2, const int cl) const
{
   // No muestra el mouse
   NCO->Muestra_mouse(NO);
   // Limpia el recuadro indicado con el color indicado
   setfillstyle(1,cl);
   bar(X1+x1,Y1+y1,X1+x2,Y1+y2);
}



// Visualiza el texto centrado dentro de la ventana actual
void Ventana::Centra_texto(const unsigned int y, const char *texto) const
{
   int x = 0, lg = 0, i = 0;
   // Calcula la longitud de la cadena, Revisa si tiene codigo para el control de colores edn una cadena
   while(texto[i]) {
      if(texto[i] == 127) i ++;
      else xcad[lg] = texto[i], i ++, lg ++;
   }
   xcad[lg] = 0;
   // Calcula la posici¢n donde sera visualizado el texto
   x = ((Largo_X() - textwidth(xcad)) / 2) + 0.51;
   x = x > 1 ? x : 0;
   // Visualiza el texto
   Visualiza_texto(x,y,texto);
}




// Visualiza el texto indicado en la ventana actual
void Ventana::Visualiza_texto(const unsigned int x, const unsigned int y, const char *texto, const int t_v, const int lg_max) const
{
   // No muestra el mouse
   NCO->Muestra_mouse(NO);

   strcpy(xcad,texto);
   // Recorta el texto a la longitud en pixeles especificado
   if(lg_max) {
      int i = textwidth(xcad);
      while(i > lg_max ) {
         i = strlen(xcad);
         xcad[i-1] = 0;
         i = textwidth(xcad);
      }
   }
   // Limpia primero el fondo sobre el que se visualizara el texto
   if(t_v) {
      setfillstyle(1,C_FOND);
      bar(X1+x,Y1+y,X1+x+(textwidth(xcad)),Y1+y+L_CAR);
   }
   setcolor(C_TEXT);
   int i, xi = 0;
   for(i = 0; i < strlen(xcad); i++) {
      if(texto[i] == 127) outtextxy(X1+x+xi,Y1+y+1,"_");
       else xi += A_CAR;
   }
   s_nocar(xcad,127,1);
   outtextxy(X1+x,Y1+y,xcad);
}
/*
   // Define el tipo de letra
   // Copia el texto a la cadena de trabajo
   strcpy(xcad,texto);
   // Inicializa variables
   int pos = 0, xpos = 0, lg = 0, lg_t = 0, lg_r = 0, xcf = 0, xct = 0;
   // Validar el ancho y largo de la ventana
   if(y > Largo_Y() && x > Largo_X()) return;
   // Revisa si el texto excede la longitud de la ventana
//      if((x + (lg_t * A_CAR)) > Largo_X()) {
      // Trunca el texto al ancho de la ventana
//         int xlg = (Largo_X() - x) / A_CAR;
//         lg_r = 0, lg_t = 0;
//         while(*(xcad+lg_r)) {
//            if(lg_t >= xlg) break;
//            if(*(xcad+lg_r) == 127) lg_r += 5;
//             else lg_t++, lg_r++;
//         }
//         xcad[lg_r] = 0;
//      }
   // Obtiene la primera posici¢n donde esta el cambio de color si existe
   while(*(xcad+pos)) {
     if(*(xcad+pos) == 127) break;
     pos++;
   }
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   // Inicializa la visualizacion de la cadena
   if(*(xcad+pos) != 127) {
      // Checa si se sobreescribe el caracter o limpia antes de escribirlo
      setcolor(C_TEXT);
      outtextxy(X1+x,Y1+y,xcad);
    } else {
      if(pos) {
         s_substr(xcad,0,pos,xxcad);
         // Checa si se sobreescribe el caracter o limpia antes de escribirlo
         if(t_v) {
            setfillstyle(1,C_FOND);
            bar(X1+x,Y1+y,X1+x+(pos*A_CAR),Y1+y+L_CAR);
         }
         setcolor(C_TEXT);
         outtextxy(X1+x,Y1+y,xxcad);
         lg = pos;
      }
      // Visualiza la cadena en porciones de distinto color
      while(pos < lg_r) {
         // Obtiene el color del texto
//         xct = (((int) xcad[pos+1] - 48) * 10) + ((int) xcad[pos+2] - 48);
         // Obtiene el color del fondo
//         xcf = (((int) xcad[pos+3] - 48) * 10) + ((int) xcad[pos+4] - 48);
         // Controla los codigos de color si este es (99), Asume el color existente anteriormente
//         if(xcf == 99) C_FOND = C_F_A;
//          else if(xcf != 98) C_F_A = C_FOND, C_FOND = xcf;
//         if(xct == 99) C_TEXT = C_T_A;
//          else if(xct != 98) C_T_A = C_TEXT, C_TEXT = xct;
         // Obtiene la porci¢n del texto
         pos ++, xpos = 0;
         while(xcad[pos + xpos] && xcad[pos + xpos] != 127) xpos++;
         // Visualiza la porci¢n indicada de la cadena
         // Checa si se sobreescribe el caracter o limpia antes de escribirlo
         if(t_v) {
            setfillstyle(1,C_FOND);
            bar(X1+x+(lg*A_CAR),Y1+y,X1+x+((lg+xpos)*A_CAR),Y1+y+L_CAR);
         }
         setcolor(C_TEXT);
         s_substr(xcad,pos,xpos,xxcad);
         outtextxy(X1+x+(lg*A_CAR),Y1+y,xxcad);
         // Actualiza posici¢n dentro del texto
         lg += xpos, pos += xpos;
      }
   }
*/




// Visualiza el caracter indicado en la ventana actual
void Ventana::Visualiza_caracter(const unsigned int x, const unsigned int y, const char car, const int cl, const int t_v) const
{
   // Define el tipo de letra
   Define_tipo_letra(TLt_tipo,TLt_hori,TLt_mulx,TLt_divx,TLt_muly,TLt_divy);
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   // Validar el ancho y largo de la ventana
   if(y < Largo_Y() && x < Largo_X()) {
      xcad[0] = 219, xcad[1] = 0;
      // Checa si se sobreescribe el caracter o limpia antes de escribirlo
      if(t_v) {
         // Indica que limpie antes de escribir
         setcolor(C_FOND);
         outtextxy(X1+x,Y1+y,xcad);
      }
      // Visualiza el caracter
      xcad[0] = car;
      setcolor(cl);
      outtextxy(X1+x,Y1+y,xcad);
   }
}


// Define el tipo de letra, horientacion y tama¤o
// Si multx es (-1) Asume 8  x 8  pixeles
//             (-2) Asume 16 x 16 pixeles
void Ventana::Define_tipo_letra(const int tipo, const int horient, const int multx, const int divx, const int multy, const int divy) const
{
   int tamano;
   ((Ventana*)this)->TLt_tipo = tipo, ((Ventana*)this)->TLt_hori = horient, ((Ventana*)this)->TLt_mulx = multx, ((Ventana*)this)->TLt_divx = divx, ((Ventana*)this)->TLt_muly = multy, ((Ventana*)this)->TLt_divy = divy;
   tamano = multx < 0 ? multx * -1: 0;
   // Selecciona el tipo de letra horientacion y tama¤o
   settextstyle(tipo,horient,tamano);
   // Modifica el tama¤o del tipo de letra seleccionado
   if(!tamano) setusercharsize(multx,divx,multy,divy);
   Tamano_letra(textwidth("W"),textheight("W"));
}


// Oculta la ventana o revisualiza esta
void Ventana::Oculta_ventana(const int tp) const
{
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   switch(tp) {
      case SI:
         delete []ARCH_TMP1;
         ((Ventana*)this)->ARCH_TMP1 = new char[80];
         Nombre_archivo_temporal(ARCH_TMP1);
         // Graba la pantalla grafica correspondiente a la actual ventana
         graba_pantalla_grafica(X1,Y1,X2,Y2,ARCH_TMP1);
      case CERRAR:
         // Revisa si se restaura la ventana de un archivo
         if(SALVAR_PANTALLA) lee_pantalla_grafica(X1,Y1,ARCH_TMP);
         if(C_REF < 255) Limpia_recuadro(0,0,Largo_X()-1,Largo_Y()-1,C_REF);
         break;
      case NO:
      case ABRIR:
         // Graba la pantalla grafica correspondiente a la actual ventana
         if(SALVAR_PANTALLA) graba_pantalla_grafica(X1,Y1,X2,Y2,ARCH_TMP);
         // Lee la pantalla grafica correspondiente a la actual ventana
         if(!tp) lee_pantalla_grafica(X1,Y1,ARCH_TMP1);
   }
}


// Retorna si el mouse esta dentro de la ventana o no
int Ventana::Mouse_dentro(void) const
{
   ((Ventana*)this)->MOUSE_DENTRO = 0;
   // Lee el estado del mouse
   ((Ventana*)this)->St_mouse = NCO->Retorna_estado_mouse();
   // Revisa si esta dentro de la ventana
   if(St_mouse.x >= X1 && St_mouse.y >= Y1 && St_mouse.x <= X2 && St_mouse.y <= Y2) MOUSE_DENTRO = 1;
   return MOUSE_DENTRO;
}

// Retorna la posicion del mouse dentro de la ventana
int Ventana::Posicion_mouse(int &x, int &y) const
{
   Ventana::Mouse_dentro();
   x = St_mouse.x - X1, y = St_mouse.y - Y1;
   return MOUSE_DENTRO;
}

// Revisa si se presione un boton dado del mouse sobre la ventana
unsigned int Ventana::Presionado(const unsigned int boton) const
{
   // Revisa si el mouse esta adentro de la ventana
   Mouse_dentro();
   // Revisa si se presiona el boton indicado dentro de la ventana
   if(MOUSE_DENTRO && St_mouse.button == boton) return 1;
   return 0;
}


// Dibuja un marco dentro de la ventana con los vertices X1,Y1,X2,Y2 , El recuadro superior izquierdo del color C_R_S
// el recudro inferior derecho con el color C_R_I
void Ventana::Marco(const int x1, const int y1, const int x2, const int y2, const int c_r_i, const int c_r_s) const
{
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   // Dibuja el recuadro
   setcolor(c_r_s);
   rectangle(X1+x1,Y1+y1,X1+x2,Y1+y2);
   // Dibuja el recuadro inferior
   if(c_r_s != c_r_i) {
      setcolor(c_r_i);
      line(X1+x1,Y1+y2,X1+x2,Y1+y2);
      line(X1+x2,Y1+y1,X1+x2,Y1+y2);
   }
}

// Limpia y marca el recuadro
void Ventana::Limpia_recuadro(const int x1, const int y1, const int x2, const int y2,const int cfd, const int c_r_i, const int c_r_s) const
{
   Limpia_recuadro(x1,y1,x2,y2,cfd);
   Marco(x1,y1,x2,y2,c_r_i,c_r_s);
}




// Graba la actual ventana, si tp es (0) Imagen con formato (1) imagen sin formato
void Ventana::Graba_ventana(const char *arch, const int tp) const
{   
   // No muestra el mouse
   NCO->Muestra_mouse(NO);
   if(tp) Graba_imagen_sin_formato(arch,X1,Y1,X2,Y2);
    else graba_pantalla_grafica(X1,Y1,X2,Y2,arch);
}

// Lee una imagen a la actual ventana, si tp es (0) Imagen con formato (1) imagen sin formato
void Ventana::Lee_ventana(const char *arch, const int tp) const
{
   // No muestra el mouse
   NCO->Muestra_mouse(NO);
   if(tp) Lee_imagen_sin_formato(arch,X1,Y1,X2,Y2);
    else lee_pantalla_grafica(X1,Y1,arch);
}



/*
// Selecciona una porcion de la ventana, regresando las coordenadas de esta area
// Retorna (1) si se selecciono correctamente
//         (0) si el usuario cancelo la seleccion por medio de la tecla ESC
int Ventana::Selecciona_porcion_ventana(C_dbl &vent) const
{
   int xx,xy, st = 1;
   // Posicion del mouse
   Posicion_mouse(vent.x1,vent.y1);
   // Calcula los limites de la ventana de trabajo
   C_dbl lim;
   lim.x1 = X1 + Inicia_X, lim.x2 = X2 - Inicia_X, lim.y1 = Y1 + Inicia_Y, lim.y2 = Y2 - Inicia_X;
   // Restringe los limites del mouse a la ventana
   NCO->Establece_limites(lim);
   // ajusta la posicion del mouse para estar dentro del area de trabajo
   if(St_mouse.x >= lim.x1 && St_mouse.x <= lim.x2 && St_mouse.y >= lim.y1 && St_mouse.y <= lim.y2) Posicion_mouse(vent.x1,vent.y1);
    else vent.x1 = Inicia_X, vent.y1 = Inicia_Y;
   // Restringe los limites del mouse a la ventana
   delay(200);
   NCO->Administrador();
   // Obtiene la posicion del mouse sobre la ventana
   Posicion_mouse(vent.x2,vent.y2);
   // Apaga el mouse
   NCO->Muestra_mouse(NO);
   // Dibuja el area seleccionada
   setcolor(Amarillo);
   setwritemode(1);
   rectangle(X1+vent.x1,Y1+vent.y1,X1+vent.x2,Y1+vent.y2);
   setwritemode(0);
   xx = vent.x2, xy = vent.y2;
   // Ciclo de seleccion
   while(Tecla != ESC && Programa_activo) {
      NCO->Administrador();
      // Obtiene la posicion del mouse sobre la ventana
      Posicion_mouse(xx,xy);
      if(xx != vent.x2 || xy != vent.y2) {
         // Apaga el mouse
         NCO->Muestra_mouse(NO);
         // Dibuja el area seleccionada
         setcolor(Amarillo);
         setwritemode(1);
         rectangle(X1+vent.x1,Y1+vent.y1,X1+vent.x2,Y1+vent.y2);
         vent.x2 = xx, vent.y2 = xy; 
         rectangle(X1+vent.x1,Y1+vent.y1,X1+vent.x2,Y1+vent.y2);
         setwritemode(0);
      }
      // Termina al seleccionar el area deseada
      if(Presionado(BR)) break;
   }
   NCO->Muestra_mouse(NO);
   // Quita la marca del area seleccionada
   setcolor(Amarillo);
   setwritemode(1);
   rectangle(X1+vent.x1,Y1+vent.y1,X1+vent.x2,Y1+vent.y2);
   setwritemode(0);
   // Revisa si se descarto la seleccion
   if(Tecla == ESC) st = 0;
   // Restablece los limites del mouse
   NCO->Restaura_limites();
   // retardo temporal
   delay(200);
   if(vent.x1 > vent.x2)  xx = vent.x1, vent.x1 = vent.x2, vent.x2 = xx;
   if(vent.y1 > vent.y2)  xx = vent.y1, vent.y1 = vent.y2, vent.y2 = xx;
   if(vent.y1 == vent.y2 || vent.x1 == vent.x2) st = 0;
   return st;
}
*/

