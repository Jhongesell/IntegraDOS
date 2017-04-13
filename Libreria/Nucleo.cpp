// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A

#include <vcl.h>
#include <fstream.h>
#include <mmsystem.h>
#include "d:\libreria\man_arch.hpp"
#include "d:\libreria\Fechora.hpp"
#include "d:\libreria\Formas\Editor.h"
#include "d:\libreria\formas\Notas.h"
#include "D:\Libreria\Formas\ArbMemo\ArbMemo.h"
#include "\libreria\pila.hpp"
#include "d:\libreria\nucleo.hpp"
#pragma hdrstop

PILA pila(20,ENTEROS);

extern char *ARCH_BITACORA;
extern char *TRAYECTORIA_ARCHIVOS;



void Nucleo::Activa_Sonido(const int sn)
{
   char arch[MAXPATH];
   char *Xarch = "";
   Manipulador_archivos ma;
   
   if (Sonidos_activos) { 
      switch (sn) {
         case ALARMA:
            Xarch = "CUCU";
            break;
         case INICIO_PROGRAMA:
            Xarch = "PUERTABR";
            break;
         case TERMINO_PROGRAMA:
            Xarch = "PUERTCER";
            break;
         case CAMPANADA:   
            Xarch = "CAMPANA";
            break;
      }
      ma.Construye_trayectoria(TRAYECTORIA_ARCHIVOS,Xarch,"WAV",arch);
      PlaySound(arch,NULL,SND_ASYNC);
   }
}


void Nucleo::Revisa_llenado_bitacora(void)
{
   ifstream rev(ARCH_BITACORA,ios::binary);
   rev.seekg(0,ifstream::end);
   int lg = rev.tellg();
   rev.close();
   if (lg > 64000) {
      Manipulador_archivos ma;
      char xarch[MAXPATH];
      
      ma.Cambia_ext_path(ARCH_BITACORA, "OLD", xarch);
      ma.Copia_archivo(ARCH_BITACORA, xarch);
      
      ofstream vac(ARCH_BITACORA,ios::binary|ios::trunc);
      vac.write("Bitacora reinicializada",24);
      vac.write(sep,2);
      vac.close();
   }
}


// Adiciona una entrada mas a la bitacora
void Nucleo::Bitacora(const char *rut, const char *txt1, const char *txt2)
{
   char xcad[30], cad[300];
   Fechas_Horas   fh;

   fh.Fecha_y_hora(xcad, 2, 0);
   ofstream grab(ARCH_BITACORA,ios::binary|ios::app);
   if (grab) {
      sprintf(cad,"%s %s %s %s %s",xcad,rut,txt1,txt2,sep);
      grab.write(cad,strlen(cad));
      grab.close();
   }
}

// Adiciona la forma indicada
void Nucleo::Adiciona_forma(void *forma, int &ptr, const int tipo)
{
   int ind;
   for (ind = 0; ind < NUMERO_MAXIMO_FORMAS; ind ++) {
      if (!Arreglo_formas[ind]) break;
   }
   Arreglo_formas[ind]      = forma;
   Arreglo_tipo_formas[ind] = tipo;
   ptr = ind;
}

// Borra las formas no usadas
void Nucleo::Borrar_forma(void)
{
   int ind = 0;
   while (!pila.Pila_vacia()) {
      pila.Sacar(ind);
      if (Arreglo_tipo_formas[ind] == 1) {
         TEditorForm *tmp = (TEditorForm *) Arreglo_formas[ind];
         if (tmp) delete tmp;
      }
      if (Arreglo_tipo_formas[ind] == 2) {
         TNotasForm *tmp = (TNotasForm*) Arreglo_formas[ind];
         if (tmp) delete tmp;
      }
      if (Arreglo_tipo_formas[ind] == 3) {
         TArbolMemoForm *tmp = (TArbolMemoForm*) Arreglo_formas[ind];
         if (tmp) delete tmp;
      }
      Arreglo_formas[ind] = NULL;
   }
}

// Activa el borrado de la forma
void Nucleo::Activa_Borrado(const int i)
{
   pila.Meter(i);
}

