//////////////////////////////////////////////////////////////
//                                                          //
//              I N T E G R A    V E R. 1.0                 //
//       (Analisador de sistemas din micos interativos)     //
//                                                          //
// Desarrollado para ambiente DOS en modo de video          //
// VGA y SVGA a 256 colores.                                //
//                                                          //
// Autores:                                                 //
//     Humberto Carrillo Calvet                             //
//     Antonio Carrillo Ledesma                             //
//     Luis Nava Fernandez                                  //
//                                                          //
// Desarrollado en:                                         //
//     Laboratorio de Dinamica no Lineal                    //
//     Facultad de Ciencias, UNAM, M‚xico D.F.              //
//     Tel‚fono 6 22-48-70                                  //
//     E-mail: dinamica@athena.fciencias.unam.mx            //
//                                                          //
// Propiedad intelectual, todos los derechos reservados     //
// conforme a la ley, registro en tr mite.                  //
// Revisi¢n  1.1-A                                          //
//                                                          //
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//                 CLASE DE CAMPO VECTORIAL                 //
//////////////////////////////////////////////////////////////


#include "campo_v.hpp"
#include "colores.hpp"
#include "Get.hpp"
#include "v_iconos.hpp"
#include "nucleo.hpp"
#include "combo_b.hpp"

extern "C" {
   #include <math.h>
}


// Definicion externa del objeto Nucleo
extern Nucleo *NCO;
 // Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;
// Controla el error matematico
extern int ERROR_MATEMATICO;
extern int VISUALIZA_ERROR_MATEMATICO;


#ifdef _IDIOMA_ESPANOL_

   #define TXT01   "CAMPO VECTORIAL" 
   #define TXT02   "Par metros:"
   #define TXT03   "Nombre:"
   #define TXT04   "Valor:"

   // Parametros del campo vectorial
   static const char *Parametros_campo_vectorial[] = {
      "NUMERO DE VECTORES (HORIZONTAL)",
      "NUMERO DE VECTORES (VERTICALES)",
      "LONGITUD DEL VECTOR (HORIZONTAL)",
      "LONGITUD DEL VECTOR (VERTICAL)",
      "0"
   };

   const char *C_Parametros_campo_vectorial = "NUVE";

#else
#endif





////////////////////////////////////////////////////////////////////////
//                    Configura el campo vectorial                    //
////////////////////////////////////////////////////////////////////////

void Campo_vectorial::Configura_campo_vectorial(void)
{
   unsigned int x1,x2,y1,y2;
   C_2I vent;
   unsigned int st = 0, actual_parm = 0, i = 0;
   double param[4];
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(TXT01,410,235,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Define_color_texto(Negro);
   Vt->Limpia_recuadro(15,50,330,190,Gris1,Blanco,Gris9);
   // visualiza el nombre de los metodos numericos
   Vt->Visualiza_texto(20,40,TXT02);
   Vt->Visualiza_texto(20,70,TXT03);
   Vt->Visualiza_texto(20,120,TXT04);

   vent.x = x1 + 20, vent.y = y1 + 85;

   Combo_box *Co1 = new Combo_box;
   Co1->Parametros(Parametros_campo_vectorial, vent,1,C_Parametros_campo_vectorial);
   Co1->Dibuja(Parametros_campo_vectorial);

   for(i = 0; i < 4; i++) param[i] = Parametros_camp_vect[Sistema_actual][i];

   Get *Gt = new Get;
   Gt->Parametros(param[actual_parm],vent.x,vent.y+55,25,0);
   Gt->Activa_get(1);
   Gt->Dibuja();

   Icono *Ic1 = new Icono;
   Icono *Ic2 = new Icono;

   Ic1->Define_icono("OK.ICO",x2-50,y1+70);
   Ic1->Dibuja();

   Ic2->Define_icono("CANCEL.ICO",x2-50,y1+130);
   Ic2->Dibuja();

   while(!st) {
      NCO->Administrador();
      if(Vt->Itera() || !Programa_activo || Tecla == ESC) break;
      Co1->Controla(Tecla,Parametros_campo_vectorial);
      if(Co1->Retorna_opcion() != actual_parm+1) {
         // Actualiza el valor anterior
         Gt->Cadena_editada(param[actual_parm]);
         // Obtiene el nuevo numero del parametro
         actual_parm = Co1->Retorna_opcion()-1;
         // Cambia el valor del parametro
         Gt->Cambia_cadena(param[actual_parm]);
         // Dibuja el parametro
         Gt->Dibuja();
      }
      Gt->Edita(Caracter,Tecla);
      if(Ic1->Oprimido()) st = 1;
      if(Ic2->Oprimido()) st = 2;
   }                    
   if(st == 1) {
      // Actualiza el valor anterior
      Gt->Cadena_editada(param[actual_parm]);
      // Valida y asigna el valor de los parametros
      for(i = 0; i < 4; i++) {
          if(i < 2) {
            if(param[i] > 1) Parametros_camp_vect[Sistema_actual][i] = param[i];
          } else Parametros_camp_vect[Sistema_actual][i] = param[i];
      }
   }
   delete Gt;
   delete Co1;
   delete Ic1;
   delete Ic2;
   delete Vt;
}



////////////////////////////////////////////////////////////////////////
//                       Dibuja el campo vectorial                    //
////////////////////////////////////////////////////////////////////////

void Campo_vectorial::Dibuja_campo_vectorial(const C_3D max, const C_3D min)
{
   unsigned int i;
   double Norma = 0, aux[2], aux2[2], t1, t2; 
   double ini[NUMERO_ECUACIONES+2], fin[NUMERO_ECUACIONES+2], aux1[NUMERO_ECUACIONES+2];

   // Inicializa valores
   for(i = 0; i < NUMERO_ECUACIONES+1; i++) ini[i] = fin[i] =  aux1[i] = _X[i] = 0;

   // Proyeccion XY
   aux[0] = (max.x - min.x) / Parametros_camp_vect[Sistema_actual][0];
   aux[1] = (max.y - min.y) / Parametros_camp_vect[Sistema_actual][1];

   // No visualiza los errores en el calculo numerico
   VISUALIZA_ERROR_MATEMATICO = 0;

   for(aux2[0] = min.x; aux2[0] < max.x; aux2[0] += aux[0]) {
      for(aux2[1] = min.y; aux2[1] < max.y; aux2[1] += aux[1]) {
         // Inicializa valores
         for(i = 0; i < NUMERO_ECUACIONES; i++) fin[i] = 0;
         t = 0;

         // Actualiza valores 
         if(Escenario[0][Sistema_actual].x == ind_no_avil) t = aux2[0];
         if(Escenario[0][Sistema_actual].y == ind_no_avil) t = aux2[1];
         _X[Escenario[0][Sistema_actual].x - 1] = ini[Escenario[0][Sistema_actual].x - 1] = aux2[0];
         _X[Escenario[0][Sistema_actual].y - 1] = ini[Escenario[0][Sistema_actual].y - 1] = aux2[1];

         // Calcula el nuevo valor inicial
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
            // Calculo con la ecuacion diferencial
            aux1[i] = (this->*funcion[Sistema_actual][i])();
            // Controla el error en las funciones matematicas
            if(ERROR_MATEMATICO) {
               ERROR_MATEMATICO = 0;
               continue;
            }
         }

         // Ajusta si esta involucrado el tiempo 
         if(Escenario[0][Sistema_actual].x == ind_no_avil) aux1[Escenario[0][Sistema_actual].x - 1] = 1.0;
         if(Escenario[0][Sistema_actual].y == ind_no_avil) aux1[Escenario[0][Sistema_actual].y - 1] = 1.0;

         Norma = (sqrt(pow(aux1[Escenario[0][Sistema_actual].x - 1],2) + (pow(aux1[Escenario[0][Sistema_actual].y - 1],2)))); 
         if(Norma == 0.0) {
            Dibuja_punto(ini,0,0,Rojo1);
            continue;
         }

         fin[Escenario[0][Sistema_actual].x - 1] = ini[Escenario[0][Sistema_actual].x - 1] + ((Parametros_camp_vect[Sistema_actual][2] * aux1[Escenario[0][Sistema_actual].x - 1]) / Norma) * 0.7 ;
         fin[Escenario[0][Sistema_actual].y - 1] = ini[Escenario[0][Sistema_actual].y - 1] + ((Parametros_camp_vect[Sistema_actual][3] * aux1[Escenario[0][Sistema_actual].y - 1]) / Norma) * 0.7 ;

         t1 = t2 = 0;
         if(Escenario[0][Sistema_actual].y == ind_no_avil || Escenario[0][Sistema_actual].x == ind_no_avil) {
            if(Escenario[0][Sistema_actual].x == ind_no_avil) t1 = ini[Escenario[0][Sistema_actual].x - 1], t2 = fin[Escenario[0][Sistema_actual].x - 1];
            if(Escenario[0][Sistema_actual].y == ind_no_avil) t1 = ini[Escenario[0][Sistema_actual].y - 1], t2 = fin[Escenario[0][Sistema_actual].y - 1];
         }
         Dibuja_linea(ini,fin,t1,t2,3,Negro);
         Cabeza_flecha(ini,fin);

      }
   }
   // Visualiza los errores en el calculo numerico
   VISUALIZA_ERROR_MATEMATICO = 1;
}


                               
////////////////////////////////////////////////////////////////////////
//               Cabeza de flecha del campo vectorial                 //
////////////////////////////////////////////////////////////////////////

void Campo_vectorial::Cabeza_flecha(const double ini[], const double fin[])
{
   double l, b, ang, a, ca, sa, m, t1, t2;
   double a1[NUMERO_ECUACIONES+2], a2[NUMERO_ECUACIONES+2];

   for(int i = 0; i < NUMERO_ECUACIONES+1; i++) a1[i] = a2[i] = ini[i];

   l = sqrt(pow(ini[Escenario[0][Sistema_actual].x - 1] - fin[Escenario[0][Sistema_actual].x - 1],2) + pow(ini[Escenario[0][Sistema_actual].y - 1] - fin[Escenario[0][Sistema_actual].y - 1],2)) / 3.0;
   b = 2.5 * l;
   if(fin[Escenario[0][Sistema_actual].x - 1] - ini[Escenario[0][Sistema_actual].x - 1]) {
      m = (fin[Escenario[0][Sistema_actual].y - 1] - ini[Escenario[0][Sistema_actual].y - 1]) / (fin[Escenario[0][Sistema_actual].x - 1] - ini[Escenario[0][Sistema_actual].x - 1]);
      ang = fin[Escenario[0][Sistema_actual].x - 1] < ini[Escenario[0][Sistema_actual].x - 1] ? atan(m) + M_PI : atan(m);
   } else ang = ini[Escenario[0][Sistema_actual].y - 1] < fin[Escenario[0][Sistema_actual].y - 1] ? M_PI_2 : - M_PI_2;

   a = sin(M_PI / 5.0) * l;
   ca = cos(ang);
   sa = sin(ang);

   a1[Escenario[0][Sistema_actual].x - 1] = ini[Escenario[0][Sistema_actual].x - 1] + b * ca - a * sa;
   a1[Escenario[0][Sistema_actual].y - 1] = ini[Escenario[0][Sistema_actual].y - 1] + b * sa + a * ca;
   a2[Escenario[0][Sistema_actual].x - 1] = ini[Escenario[0][Sistema_actual].x - 1] + b * ca + a * sa;
   a2[Escenario[0][Sistema_actual].y - 1] = ini[Escenario[0][Sistema_actual].y - 1] + b * sa - a * ca;

   t1 = t2 = 0;
   if(Escenario[0][Sistema_actual].y == ind_no_avil || Escenario[0][Sistema_actual].x == ind_no_avil) {
      if(Escenario[0][Sistema_actual].x == ind_no_avil) t1 = a1[Escenario[0][Sistema_actual].x - 1], t2 = fin[Escenario[0][Sistema_actual].x - 1];
      if(Escenario[0][Sistema_actual].y == ind_no_avil) t1 = a1[Escenario[0][Sistema_actual].y - 1], t2 = fin[Escenario[0][Sistema_actual].y - 1];
   }

   Dibuja_linea(a1,fin,t1,t2,3,Rojo1);

   t1 = t2 = 0;
   if(Escenario[0][Sistema_actual].y == ind_no_avil || Escenario[0][Sistema_actual].x == ind_no_avil) {
      if(Escenario[0][Sistema_actual].x == ind_no_avil) t1 = a2[Escenario[0][Sistema_actual].x - 1], t2 = fin[Escenario[0][Sistema_actual].x - 1];
      if(Escenario[0][Sistema_actual].y == ind_no_avil) t1 = a2[Escenario[0][Sistema_actual].y - 1], t2 = fin[Escenario[0][Sistema_actual].y - 1];
   }

   Dibuja_linea(a2,fin,t1,t2,3,Rojo1);
}

