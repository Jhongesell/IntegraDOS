// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

/*



#include "alarma.hpp"
#include "v_iconos.hpp"
#include "r_esp.hpp"
#include "cad_car.hpp"
#include "hor_seg.hpp"
#include "fechas.hpp"
#include "r_var.hpp"
#include "man_arch.hpp"
#include "get.hpp"
#include "ctrl_tec.hpp"
#include "teclado.hpp"

extern "C" {
   #include <stdlib.h>
   #include <string.h>
}

// Definicion de objeto mouse grafico
extern GMouse *Gm;
// Definicion de objeto control de teclado
extern Ctrl_teclado *Ct;

#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "DEFINICION DE ALARMAS"
   #define TXT2      "TIPO    HORA                 MENSAJE"
   #define TXT3      "ALARMA ACTIVADA"
   #define IDIOMA    1  // Espa¤ol
#else
   #define TXT1      "DEFINICION DE ALARMAS"
   #define TXT2      "TIPO    HORA                 MENSAJE"
   #define TXT3      "ALARMA ACTIVADA"
   #define IDIOMA    0  // Ingles
#endif
#define ARCHIVO   "SYS\\ALARMA.DAT"
#define N_GETS    12



// Activa y define la hora en que sonara la alarma
void Alarma::Define_alarma(void)
{
   char hora[4][20], mensaje[4][45], tipo[4][20], car, cad[10];
   int i,x1,x2,y1,y2,tc,y=90, ct=0, get_act = 0, get_sw = 0;
   for(i = 0; i < 4; i++) {
       ltoa(TIPO[i],tipo[i],10);
       s_r_t(tipo[i],1,32);
       n_seg__horas(ALARMA[i],hora[i]);
       s_r_t(hora[i],5,32);
       s_r_t(M_ALARMA[i],40,32,mensaje[i]);
   }

   Ventana_iconos Vt;
   Vt.Activa_graba_ventana(1);
   Vt.Ventana_centrada(TXT1,480,320,0);
   Vt.Dibuja();
   Vt.Actual_pos_ventana(x1,y1,x2,y2);
   Vt.Define_color_texto(Negro);
   Vt.Visualiza_texto(30,60,TXT2);

   Get Gts[N_GETS];
   Gts[0].Activa_get(1);
   for(i = 0; i < 4; i++) {
      Gts[i*3].Parametros(tipo[i],x1+40,y1+y,0);
      Gts[i*3].Dibuja();
      Gts[1+(i*3)].Parametros(hora[i],x1+80,y1+y,0,"H");
      Gts[1+(i*3)].Dibuja();
      Gts[2+(i*3)].Parametros(mensaje[i],x1+160,y1+y,30);
      Gts[2+(i*3)].Dibuja();
      y+=40;
   }
   Gts[0].Activa_get(1);

   Icono Ic1;
   Ic1.Define_icono("OK.ICO",x1+222,y2-50);
   Ic1.Dibuja();

   while(!ct) {
      // Lee el mouse
      Gm->Mpos();
      // Lee el codigo del teclado
      Ct->Retorna_bufer_teclado(car,tc);
      if(Vt.Itera()) ct = 1;
      if(Ic1.Oprimido()) {
         for(i = 0; i < 4; i++) {
            Gts[i*3].Cadena_editada(tipo[i]);
            Gts[1+(i*3)].Cadena_editada(hora[i]);
            Gts[2+(i*3)].Cadena_editada(mensaje[i]);
         }

         for(i = 0; i < 4; i++) {
             TIPO[i] = atol(tipo[i]);
             strcpy(cad,hora[i]);
             strcat(cad,":00");
             ALARMA[i] = horas__n_seg(cad);
             s_trim(mensaje[i],M_ALARMA[i]);
         }
         Graba_archivo_alarma();
         ct = 1;
      }
      for(i = 0; i < N_GETS; i++) {
         if(Gts[i].Presionado(BR)) get_act = i, get_sw = 1;
         Gts[i].Edita(car,tc);
      }
      if((tc == ENTER || tc == FLECHA_ABAJO) && get_act < (N_GETS -1)) get_act ++, get_sw = 1;
      if(tc == FLECHA_ARRIBA && get_act > 0) get_act --, get_sw = 1;
      if(get_sw) {
         for(i = 0; i < N_GETS; i++) {
            Gts[i].Activa_get(i == get_act);
            get_sw = 0;
         }
      }
   }
   Revisa_activ_alarma();
}


// Graba las horas en la que se activara la alarma y los mensajes respectivos
void Alarma::Graba_archivo_alarma(void)
{
   Manipulador_archivos Ga;
   Ga.Parametros(ARCHIVO,GRABA_ARCHIVO,300,TXT1);
   char cad[45];
   for(int i = 0; i < 4; i++) {
      ltoa(TIPO[i],cad,10);
      Ga.Graba_linea(cad);
      ltoa(ALARMA[i],cad,10);
      Ga.Graba_linea(cad);
      Ga.Graba_linea(M_ALARMA[i]);
   }
}


// Lectura del archivo de alarma (s¡ es que existe)
void Alarma::Lee_archivo_alarma(void)
{
   char cad[101];
   int i;
   for(i = 0; i < 4; i++) {
       TIPO[i] = 0;
       ALARMA[i] = 0;
       M_ALARMA[i][0] = 0;
   }
   if(n_archivos_directorios(ARCHIVO,FA_ARCH)) {
      Manipulador_archivos La;
      La.Parametros(ARCHIVO,LEE_ARCHIVO,300,TXT1);
      for(i = 0; i < 4; i++) {
          La.Lee_linea(cad);
          if(!La.Estado()) TIPO[i] = atol(cad);
          La.Lee_linea(cad);
          if(!La.Estado()) ALARMA[i] = atol(cad);
          La.Lee_linea(cad);
          if(!La.Estado()) s_trim(cad,M_ALARMA[i]);
      }
   }
   Revisa_activ_alarma();
}


// Checa si la alarma a sido activada y visualiza el mensaje de su activaci¢n
void Alarma::Checa_alarma(void)
{
   long n_s = n_seg(), N_S_E = 600;
   if(n_s < 100) Activa1 = Activa2 = Activa3 = Activa4 = 1;

   if(Activa1) {
      if(n_s > ALARMA[0] && n_s < (ALARMA[0] + N_S_E)) {
         Mensaje_alarma(0);
         Activa1 = 0;
      }
   }
   if(Activa2) {
      if(n_s > ALARMA[1] && n_s < (ALARMA[1] + N_S_E)) {
         Mensaje_alarma(1);
         Activa2 = 0;
      }
   }
   if(Activa3) {
      if(n_s > ALARMA[2] && n_s < (ALARMA[2] + N_S_E)) {
         Mensaje_alarma(2);
         Activa3 = 0;
      }
   }
   if(Activa4) {
      if(n_s > ALARMA[3] && n_s < (ALARMA[3] + N_S_E)) {
         Mensaje_alarma(3);
         Activa4 = 0;
      }
   }
}



// Genera el mensaje de la fecha y hora en la que se activo la alarma
void Alarma::Mensaje_alarma(const int nms)
{
   char fecha_esc[40],fecha[15];
   f_f_hoy(fecha);
   f_fecha_escrita(fecha,fecha_esc,1,IDIOMA);
   Mensaje(TXT3,"",fecha_esc,"",M_ALARMA[nms]);
   if(TIPO[nms] == 0) {
      ALARMA[nms] = 0;
      M_ALARMA[nms][0] = 0;
      Graba_archivo_alarma();
   }
   Revisa_activ_alarma();
}


// Revisa si alguna de las alarmas esta activa
void Alarma::Revisa_activ_alarma(void)
{
   A_activa = 0;
   if(ALARMA[0] > 0) A_activa = Activa1 = 1;
   if(ALARMA[1] > 0) A_activa = Activa2 = 1;
   if(ALARMA[2] > 0) A_activa = Activa3 = 1;
   if(ALARMA[3] > 0) A_activa = Activa4 = 1;
}





*/

