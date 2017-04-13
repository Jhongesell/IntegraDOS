// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void Calendario(const int tp,const char *fecha);
  // Visualiza una ventana en la cual muestra el calendario del mes indicado por la cadena
  // de fecha pasada como parametro si tp es cero, si tp es distinto de cero asume la fecha
  // actual del sistema



#include "v_iconos.hpp"
#include "botontxt.hpp"
#include "r_var.hpp"
#include "cad_car.hpp"
#include "fechas.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;


#ifdef _IDIOMA_ESPANOL_
   #define TXT1     "CALENDARIO"
   #define TXT2     "Anterior"
   #define TXT3     "Siguiente"
   #define TXT4     "%s de %d"
   #define TXT5     "D  L  M  M  J  V  S"
   #define Idioma   1 //  Espa¤ol
#else
   #define TXT1     "CALENDAR"
   #define TXT2     "Previous"
   #define TXT3     "Next"
   #define TXT4     "%s of %d"
   #define TXT5     "S  M  T  W  T  F  S"
   #define Idioma   0 //  Ingles
#endif




void Calendario(const int tp,const char *xfecha)
{
   unsigned int x1,y1,x2,y2,db = 1;
   unsigned int dias[50], n_md,ini,i,xi,xx;
   unsigned int mes, ano;
   Fechas f;
   char xcad[50], xcad1[50], xcad2[50], fecha[30];
   long  ct = 0, e_a = 1, st = 0;
   unsigned long jul = 0;
   if(tp) f.Fecha_hoy(fecha);
    else strcpy(fecha,xfecha);

   mes = f.Numero_mes(fecha);
   ano = f.Numero_ano(fecha);

   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Rojo1,Gris9,Blanco);
   Vt->Cambia_tipo_letra_titulo(8,0,-1,0,0,0);
   Vt->Ventana_centrada(TXT1,300,260,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Limpia_recuadro(10,45,290,180,Blanco,Negro,Negro);

   // Definicion del primer boton
   Boton_texto *Bt1 = new Boton_texto;
   Bt1->Define_boton(TXT2,x1 + 30,y2 - 30);
   Bt1->Dibuja();
   // Definicion del segundo boton
   Boton_texto *Bt2 = new Boton_texto;
   Bt2->Define_boton(TXT3,x2 -114,y2 - 30);
   Bt2->Dibuja();

   // Visualiza la fecha actual
   Vt->Define_color(Blanco,Rojo9);
   f.Fecha_escrita(fecha,xcad,1,Idioma);
   Vt->Centra_texto(190,xcad);
   while(!ct) {
        if(db) {
           // Visualiza la ventana donde aparecera el calendario
           Vt->Limpia_recuadro(13,48,287,177,Blanco,Blanco,Blanco);
           // Visualiza el mes  y el a¤o
           f.Mes_letra(mes,xcad1,Idioma); 
           sprintf(xcad,TXT4,xcad1,ano);
           Vt->Define_color(Blanco,Rojo9);
           Vt->Centra_texto(70,xcad);
           Vt->Define_color(Blanco,Negro);
           Vt->Visualiza_texto(75,90,TXT5);
           Vt->Define_color(Blanco,Rojo1);
           // calcula el inicio del dia primero
           n_md = f.Numero_max_dias_mes(mes,ano);
           f.Gregoriano_a_juliano(1,mes,ano,jul);
           ini = f.Dia_semana(jul);
           // llena la table de los dias
           for(i = 0; i < 50; i++) dias[i] = 0;
           for(i = 0; i < n_md; i++) dias[ini + i] = dias[(i + ini)- 1] + 1;
           for(i = 0; i < 6; i++) {
               for(xi = 1; xi < 8; xi++) {
                 xx = xi + (i * 7);
                 sprintf(xcad2,"%d",dias[xx]);
                 // Visualiza los dias del mes
                 if(xx <= n_md + ini && dias[xx] > 0) Vt->Visualiza_texto(75+(((xi-1) * 3)*8),(11+i)*10,xcad2);
              }
           }
           db =0;
        }
        NCO->Administrador();
        // Revisa el estado de los botones
        if(Vt->Itera() || !Programa_activo || Tecla == ESC) ct = 1;
        if(Bt1->Oprimido() || st == 1) {
            e_a = 1, mes--, db =1;
            if(!mes) mes = 12, ano--;
        }
        if(Bt2->Oprimido() || st == 2) {
            e_a = 2, mes++, db = 1;
            if(mes == 13) mes = 1, ano++;
        }
        // Actualiza el boton activo
        Bt1->Activar(e_a == 1);
        Bt2->Activar(e_a == 2);
        // Desactiva la salida por Esc
//        if(ct == -1) ct = 0;
   }
   delete Bt1;
   delete Bt2;
   delete Vt;
}

