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
//     Luis A. Nava Fernandez                               //
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


// Todas las rutinas son independientes de la dimension del sistema


#include "m_d_usr.hpp"
#include "v_iconos.hpp"
#include "nucleo.hpp"
#include "barr_ind.hpp"
#include "menu.hpp"
#include "get.hpp"
#include "teclado.hpp"
#include "r_esp.hpp"
#include "vis_arch.hpp"

#include "cad_car.hpp"
#include "\libreria\ana_exp.hpp"
#include "\libreria\calnpol.hpp"
#include "vis_ayud.hpp"

extern "C" {
   #include <string.h>
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
// Indica el tipo de error en rutinas matematicas
extern int ERROR_MATEMATICO;            
// Indica si se visualiza el error matematico
extern int VISUALIZA_ERROR_MATEMATICO;  



// Valor para los colores
const char Colores[] = {
  Negro, // 0
  Gris1, // 1
  Gris9, // 2
  Azul1, // 3
  Azul9, // 4
  Verde1,// 5
  Verde9,// 6
  Cian1, // 7
  Cian9, // 8
  Rojo1, // 9
  Rojo9, // 10
  Magenta1, // 11
  Magenta9, // 12
  Cafe,     // 13
  Amarillo, // 14
  Blanco    // 15
};


#ifdef _IDIOMA_ESPANOL_

   #define TXT01   "BIBLIOTECA DE SISTEMAS" 
   #define TXT02   "Sistemas:"
   #define TXT03   "Seleccione el sistema a trabajar"
   #define TXT04   "El sistema actual no tiene par metros"
   #define TXT05   "PARAMETROS"
   #define TXT06   "Seleccione el par metro a cambiar"
   #define TXT07   "Presione ENTER para actualizar el valor del par metro"
   #define TXT08   "Tiempo"
   #define TXT09   "No Activo"
   #define TXT10   "Edite el valor del par metro"
   #define TXT11   "Par metro: %s"
   #define TXT12   "Edita Par metro"
   #define TXT13   "CONDICION INICIAL"
   #define TXT14   "Tiempo"
   #define TXT15   "Seleccione la condici¢n inicial a cambiar" 
   #define TXT16   "Edite el valor de la condici¢n inicial"
   #define TXT17   "EDICION DE CONDICION INICIAL"
   #define TXT18   "Tiempo"
   #define TXT19   "SISTEMA ACTUAL:"
   #define TXT20   "ECUACIONES:"
   #define TXT22   "FUNCIONES AUXILIARES A GRAFICAR:"
   #define TXT22a   "FUNCIONES AUXILIARES"
   #define TXT23   "Escriba la expresi¢n a gr ficar"
   #define TXT24   "Gr ficando la expresi¢n"
   #define TXT25   "Solo est n definidas funciones auxiliares"
   #define TXT26   "a gr ficar para los ejes x e y"
   #define TXT27   "AVISO"

   #define TXT30   "Las funciones auxiliares se definen como:"
   #define TXT31   "F(x)=-a*sin(x)"
   #define TXT32   "¢"
   #define TXT33   "F(y)=-a*sin(y)"
   #define TXT34   "CAPTURA VALOR PARA ISOCLINA"
   #define TXT35   "PENDIENTE"

#else

#endif



//////////////////////////////////////////////////////////////////////////
//        Selecciona el sistema mostrando las ecuaciones de este        //
//////////////////////////////////////////////////////////////////////////

int M_Definicion_usuario::Selecciona_sistema(void)
{
   unsigned int X1,Y1,X2,Y2,sw = 0;
   int opc , i, xi;
   int st = 0, s_actual = Sistema_actual;
   char xcad[300];
   // Apertura de la ventana de sistemas
   Ventana_iconos *VMA = new Ventana_iconos;
   VMA->Activa_graba_ventana(1);
   VMA->Ventana_centrada(TXT01,360,300,0); 
   VMA->Actual_pos_ventana(X1,Y1,X2,Y2);
   VMA->Dibuja();
   // Ventana indicadora de la ventana de sistemas
   Traza_indicador *VMAID = new Traza_indicador;
   VMAID->Define_indicador(X2-99,Y1+60,X2-99,Y2-160);
   VMAID->Dibuja();
   // Parametros del menu
   Menu *M1 = new Menu;
   M1->Define_ventana_menu(X1+30,Y1+60,X2-100,Y2-160,Blanco);
   // Inicializacion de arreglo de sistemas
   char *Cad_trab = new char [(NUMERO_SISTEMAS+1)*(LG_MAX_NOM_SISTEMA+1)];
   const char **Arr_menu = new const char *[NUMERO_SISTEMAS+1];
   int num_sistemas = 0;
   // Carga el nombre de los sistemas
   for(i = 0; i < Numero_sistemas; i++) {
       // Copia la cadena al arreglo temporal
       if(Nombre_sistema[i][0]) {
          for(xi = 0; xi < strlen(Nombre_sistema[i]); xi++) Cad_trab[i*LG_MAX_NOM_SISTEMA+xi] = Nombre_sistema[i][xi];
          Cad_trab[i*LG_MAX_NOM_SISTEMA+xi] = 0;
          num_sistemas ++;
        } else {
          Cad_trab[i*LG_MAX_NOM_SISTEMA]= 32;
          Cad_trab[i*LG_MAX_NOM_SISTEMA+1] = 0;
       }
       // Asigna el puntero del nombre de archivo al arreglo de punteros
       Arr_menu[i] = &Cad_trab[i*LG_MAX_NOM_SISTEMA];
   }
   // Asigna los parametros al menu
   M1->Parametros_menu(Arr_menu,"","",num_sistemas);
   // Dibuja el menu
   M1->Dibuja();
   // Declara e inicializa al visualizador
   Visualiza_archivo *Ea  = new Visualiza_archivo;
   Ea->Parametros(X1+30,Y2-150,X2-75,Y2-20,"");
   // Carga el sistema y las ecuaciones al visualizador
   Ea->Adiciona_linea(1,TXT19,2);
   sprintf(xcad,"   %s",Nombre_sistema[s_actual]);
   Ea->Adiciona_linea(3,xcad,2);
   Ea->Adiciona_linea(1," ",2);
   Ea->Adiciona_linea(1,TXT20,2);
   // Ecuaciones del sistema
   for(i = 0; i < Numero_ecuaciones[s_actual]; i++) {
       sprintf(xcad,"   %s",Nombre_ecuacion[s_actual][i]);
       Ea->Adiciona_linea(1,xcad,2);
   }
   // Parametros del sistema
   if(Numero_parametros[s_actual]) {
      Ea->Adiciona_linea(1," ",2);
      Ea->Adiciona_linea(1,TXT05,2);
      for(i = 0; i < Numero_parametros[s_actual]; i++) {
          sprintf(xcad,"   %s = %f",Nombre_parametro[s_actual][i],_P[P_a][s_actual][i]);
          Ea->Adiciona_linea(1,xcad,2);
      }
   }
   // Funciones auxliliares a graficar
   if(Numero_funciones_auxiliares[s_actual]) {
      Ea->Adiciona_linea(1," ",2);
      Ea->Adiciona_linea(1,TXT22,2);
      for(i = 0; i < Numero_funciones_auxiliares[s_actual]; i ++) {
         sprintf(xcad,"   [CTRL-F%d]  %s",i+1,Funciones_auxiliares[s_actual][i]);
         Ea->Adiciona_linea(1,xcad,2);
      }
   }
   // Revisualiza el visualizador
   Ea->Revisualiza_pantalla();

   Icono *Ic1 = new Icono;
   Icono *Ic2 = new Icono;
   Ic1->Define_icono("OK.ICO",X2-50,Y1+60);
   Ic1->Dibuja();
   Ic2->Define_icono("CANCEL.ICO",X2-50,Y1+110);
   Ic2->Dibuja();
   VMA->Define_color_texto(Negro);
   VMA->Visualiza_texto(30,50,TXT02);
   // Texto explicativo
   NCO->Visualiza_texto(TXT03);
   while(!st) {
      NCO->Administrador();
      if(VMA->Itera() || !Programa_activo || Tecla == ESC) break;
      // Controlador de menu
      opc = VMAID->Controlador(M1->Opcion_actual_menu(),M1->Retorna_total_elementos(),M1->Retorna_elementos_paneo());
      // Controla el movimiento del menu
      if(M1->Mouse_dentro() || opc) {
         if(opc == 1) Tecla = FLECHA_ARRIBA;
         if(opc == 2) Tecla = FLECHA_ABAJO;
         M1->Controlador_menu(Caracter,Tecla,sw);
         if(sw) {
            s_actual = sw -1;
            // Borra el contenido del visualizador
            Ea->Borra_contenido_editor();
            // Carga el sistema y las ecuaciones al visualizador
            Ea->Adiciona_linea(1,TXT19,2);
            sprintf(xcad,"   %s",Nombre_sistema[s_actual]);
            Ea->Adiciona_linea(3,xcad,2);
            Ea->Adiciona_linea(1," ",2);
            Ea->Adiciona_linea(1,TXT20,2);
            // Ecuaciones del sistema
            for(i = 0; i < Numero_ecuaciones[s_actual]; i++) {
                sprintf(xcad,"   %s",Nombre_ecuacion[s_actual][i]);
                Ea->Adiciona_linea(1,xcad,2);
            }
            // Parametros del sistema
            if(Numero_parametros[s_actual]) {
               Ea->Adiciona_linea(1," ",2);
               Ea->Adiciona_linea(1,TXT05,2);
               for(i = 0; i < Numero_parametros[s_actual]; i++) {
                   sprintf(xcad,"   %s = %f",Nombre_parametro[s_actual][i],_P[P_a][s_actual][i]);
                   Ea->Adiciona_linea(1,xcad,2);
               }
            }
            // Funciones auxliliares a graficar
            if(Numero_funciones_auxiliares[s_actual]) {
               Ea->Adiciona_linea(1," ",2);
               Ea->Adiciona_linea(1,TXT22,2);
               for(i = 0; i < Numero_funciones_auxiliares[s_actual]; i ++) {
                  sprintf(xcad,"   [CTRL-F%d]  %s",i+1,Funciones_auxiliares[s_actual][i]);
                  Ea->Adiciona_linea(1,xcad,2);
               }
            }
            Ea->Posicionarse(1,1);
            // Revisualiza el visualizador
            Ea->Revisualiza_pantalla();
         }
      }
      // Controla el movimiento dentro del visualizador 
      if(Ea->Mouse_dentro()) Ea->Controlador(Tecla);
      // boton de aceptar
      if(Ic1->Oprimido()) st = 1;
      // Boton de cancelar
      if(Ic2->Oprimido()) st = 2;
   }
   delete Ea;
   delete Ic1;
   delete Ic2;
   delete M1;
   delete VMAID;
   delete VMA;
   delete []Cad_trab;
   delete []Arr_menu;
   if(st == 1) {
      Sistema_actual = s_actual;
      Genera_lista_escenarios();
   }
   return st;
}


//////////////////////////////////////////////////////////////////////////
//                   Genera la lista de los escenarios                  //
//////////////////////////////////////////////////////////////////////////
void M_Definicion_usuario::Genera_lista_escenarios(void)
{
   unsigned int i;
   char xcad[200], xcad1[20];
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
       Nombre_ecuacion_sistema_actual(Sistema_actual,i,xcad1);
       sprintf(xcad,"%s(t)",xcad1);
       strcpy(C_Ecuaciones1[i],xcad);
       sprintf(xcad,"%s(t)",xcad1);
       strcpy(C_Ecuaciones2[i],xcad);
   }
   strcpy(C_Ecuaciones1[i],TXT08);
   strcpy(C_Ecuaciones1[i+1],"0");
   strcpy(C_Ecuaciones1[i+2],"0");
   strcpy(C_Ecuaciones2[i],TXT08);
   strcpy(C_Ecuaciones2[i+1],TXT09);
   strcpy(C_Ecuaciones2[i+2],"0");
   ind_no_avil = i + 1;
}


//////////////////////////////////////////////////////////////////////////
//          Nombre de la ecuacion indicada en el sistema actual         //
//////////////////////////////////////////////////////////////////////////
void M_Definicion_usuario::Nombre_ecuacion_sistema_actual(const unsigned int sis, const unsigned int n_e, char *cad)
{
   unsigned int pos;
   if(n_e < Numero_ecuaciones[sis]) {
      if(Busca_caracter(Nombre_ecuacion[sis][n_e],'\'',pos)) s_substr(Nombre_ecuacion[sis][n_e],0,pos,cad);
   } else strcpy(cad,TXT08);
}


//////////////////////////////////////////////////////////////////////////
// Muestra los par metros del sistema actual y permite su modificaci¢n  //
//////////////////////////////////////////////////////////////////////////

void M_Definicion_usuario::Cambia_parametros_sistema(void)
{
   if(!Numero_parametros[Sistema_actual]) {
      NCO->Mensaje("",TXT04);
      return;
   }
   unsigned int x1,y1,x2,y2, sw = 0, mod_parm = 0;
   int opc,i,xi;
   // Apertura de la ventana de parametros
   Ventana_iconos *VMA = new Ventana_iconos;
   VMA->Activa_graba_ventana(1);
   VMA->Ventana_centrada(TXT05,300,250,0);
   VMA->Dibuja();
   VMA->Actual_pos_ventana(x1,y1,x2,y2);
   // Ventana indicadora de la ventana de parametros
   Traza_indicador *VMAID = new Traza_indicador;
   VMAID->Define_indicador(x2-49,y1+60,x2-49,y2-20);
   VMAID->Dibuja();
   // Parametros del menu
   Menu *M1 = new Menu;
   M1->Define_ventana_menu(x1+30,y1+60,x2-50,y2-20,Blanco);
   // Inicializacion de arreglo de parametros
   char Cad_temp[100];
   char *Cad_trab = new char [(Numero_parametros[Sistema_actual]+1)*(LG_MAX_NOM_PARAMETRO+21)];
   const char **Arr_menu = new const char *[Numero_parametros[Sistema_actual]+1];
   // Carga el nombre y valor de los parametros
   for(i = 0; i < Numero_parametros[Sistema_actual]; i++) {
      // Copia la cadena al arreglo temporal
      sprintf(Cad_temp,"%s = %1.9e",Nombre_parametro[Sistema_actual][i],_P[P_a][Sistema_actual][i]);
      for(xi = 0; xi < strlen(Cad_temp); xi++) Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = Cad_temp[xi];
      Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = 0;
      // Asigna el puntero del nombre de archivo al arreglo de punteros
      Arr_menu[i] = &Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)];
   }
   // Asigna los parametros al menu
   M1->Parametros_menu(Arr_menu,"","",Numero_parametros[Sistema_actual]);
   // Dibuja el menu
   M1->Dibuja();
   // Texto explicativo
   NCO->Visualiza_texto(TXT06);
   // Loop de trabajo
   while(1) {
      NCO->Administrador();
      if(VMA->Itera() || !Programa_activo || Tecla == ESC) break;
      // Controlador de menu
      opc = VMAID->Controlador(M1->Opcion_actual_menu(),M1->Retorna_total_elementos(),M1->Retorna_elementos_paneo());
      if(opc == 1) Tecla = FLECHA_ARRIBA;
      if(opc == 2) Tecla = FLECHA_ABAJO;
      // Controla el movimiento del menu
      M1->Controlador_menu(Caracter,Tecla,sw);
      // Desavilita la salida presionando ESC o el mouse afuera en el menu
      if(sw) {
         NCO->Visualiza_texto(TXT10);
         sprintf(Cad_temp,TXT11,Nombre_parametro[Sistema_actual][sw-1]);
         if(Captura_cadena(TXT12,Cad_temp,_P[P_a][Sistema_actual][sw-1],1) == 1) {
            mod_parm = 1;
            // Carga el nombre y valor de los parametros
            for(i = 0; i < Numero_parametros[Sistema_actual]; i++) {
                // Copia la cadena al arreglo temporal
                sprintf(Cad_temp,"%s = %1.9e",Nombre_parametro[Sistema_actual][i],_P[P_a][Sistema_actual][i]);
                for(xi = 0; xi < strlen(Cad_temp); xi++) Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = Cad_temp[xi];
                Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = 0;
                // Asigna el puntero al arreglo de punteros
                Arr_menu[i] = &Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)];
            }
            M1->Dibuja();
         }
         // Texto explicativo
         NCO->Visualiza_texto(TXT06);
      }
   }
   Guarda_posicion = NO;

   // Revisa si la configuracion sera para ambas ventanas
   if(mod_parm) {
      if(Doble_ventana) {
         if(Opcion("¨Desea utilizar est  configuraci¢n","en ambas ventanas?") == 1)  {
            for(i = 0; i < Numero_parametros[Sistema_actual]; i++) {
                _P[!P_a][Sistema_actual][i] = _P[P_a][Sistema_actual][i];
            }
         }
      }
   }

   delete M1;
   delete VMAID;
   delete VMA;
   delete []Cad_trab;
   delete []Arr_menu;
}


//////////////////////////////////////////////////////////////////////////
// Muestra las condiciones iniciales del sistema actual y permite su    //
// modificaci¢n                                                         //
//////////////////////////////////////////////////////////////////////////

void M_Definicion_usuario::Cambia_Condicion_inicial(void)
{
   unsigned int x1,y1,x2,y2, sw = 0;
   int opc,i,xi;
   char xcad[20];
   char Cad_temp[200];
   char *Cad_trab = new char [(Numero_ecuaciones[Sistema_actual]+2)*(LG_MAX_NOM_PARAMETRO+21)];
   const char **Arr_menu = new const char *[Numero_ecuaciones[Sistema_actual]+2];
   // Apertura de la ventana de parametros
   Ventana_iconos *VMA = new Ventana_iconos;
   VMA->Activa_graba_ventana(1);
   VMA->Ventana_centrada(TXT13,300,250,0);
   VMA->Dibuja();
   VMA->Actual_pos_ventana(x1,y1,x2,y2);
   // Ventana indicadora de la ventana de parametros
   Traza_indicador *VMAID = new Traza_indicador;
   VMAID->Define_indicador(x2-49,y1+60,x2-49,y2-20);
   VMAID->Dibuja();
   // Parametros del menu
   Menu *M1 = new Menu;
   M1->Define_ventana_menu(x1+30,y1+60,x2-50,y2-20,Blanco);
   // Inicializacion de arreglo de parametros
   // Carga el nombre y valor de las condiciones iniciales
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
      // Copia la cadena al arreglo temporal
      Nombre_ecuacion_sistema_actual(Sistema_actual,i,xcad);
      s_rellena(xcad,32,6);
      sprintf(Cad_temp,"%s = %1.9e",xcad,Ac_Condicion[Ventana_activa][Sistema_actual][i]);
      for(xi = 0; xi < strlen(Cad_temp); xi++) Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = Cad_temp[xi];
      Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = 0;
      Arr_menu[i] = &Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)];
   }
   strcpy(xcad,TXT14);
   s_rellena(xcad,32,6);
   sprintf(Cad_temp,"%s = %1.9e",xcad,Ac_Tiempo[Ventana_activa][Sistema_actual]);
   for(xi = 0; xi < strlen(Cad_temp); xi++) Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = Cad_temp[xi];
   Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = 0;
   Arr_menu[i] = &Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)];


   // Asigna los parametros al menu
   M1->Parametros_menu(Arr_menu,"","",(Numero_ecuaciones[Sistema_actual]+1));
   // Dibuja el menu
   M1->Dibuja();
   // Texto explicativo
   NCO->Visualiza_texto(TXT15);
   // Loop de trabajo
   while(1) {
      NCO->Administrador();
      if(VMA->Itera() || !Programa_activo || Tecla == ESC) break;
      // Controlador de menu
      opc = VMAID->Controlador(M1->Opcion_actual_menu(),M1->Retorna_total_elementos(),M1->Retorna_elementos_paneo());
      if(opc == 1) Tecla = FLECHA_ARRIBA;
      if(opc == 2) Tecla = FLECHA_ABAJO;
      // Controla el movimiento del menu
      M1->Controlador_menu(Caracter,Tecla,sw);
      // Desavilita la salida presionando ESC o el mouse afuera en el menu
      if(sw) {
         NCO->Visualiza_texto(TXT16);
         unsigned int xi;
         if(sw -1 < Numero_ecuaciones[Sistema_actual]) {
            Nombre_ecuacion_sistema_actual(Sistema_actual,sw-1,Cad_temp);
            xi = Captura_cadena(TXT17,Cad_temp,Ac_Condicion[Ventana_activa][Sistema_actual][sw-1],1);
          }  else {
            strcpy(Cad_temp,TXT18);
            xi = Captura_cadena(TXT17,Cad_temp,Ac_Tiempo[Ventana_activa][Sistema_actual],1);
         }
         if(xi == 1) {
            // Carga el nombre y valor de las condiciones iniciales
            for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
                Nombre_ecuacion_sistema_actual(Sistema_actual,i,xcad);
                // Copia la cadena al arreglo temporal
                s_rellena(xcad,32,6);
                sprintf(Cad_temp,"%s = %1.9e",xcad,Ac_Condicion[Ventana_activa][Sistema_actual][i]);
                for(xi = 0; xi < strlen(Cad_temp); xi++) Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = Cad_temp[xi];
                Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = 0;
                Arr_menu[i] = &Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)];
            }
            strcpy(xcad,TXT14);
            s_rellena(xcad,32,6);
            sprintf(Cad_temp,"%s = %1.9e",xcad,Ac_Tiempo[Ventana_activa][Sistema_actual]);
            for(xi = 0; xi < strlen(Cad_temp); xi++) Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = Cad_temp[xi];
            Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)+xi] = 0;
            Arr_menu[i] = &Cad_trab[i*(LG_MAX_NOM_PARAMETRO+20)];

            M1->Dibuja();
         }
         // Texto explicativo
         NCO->Visualiza_texto(TXT15);
      }
   }
   Guarda_posicion = NO;
   delete M1;
   delete VMAID;
   delete VMA;
   delete []Arr_menu;
   delete []Cad_trab;
}




//////////////////////////////////////////////////////////////////////////
// Permite capturar y visualizar las funciones auxiliares               //
// indicadas por el usuario                                             //
//////////////////////////////////////////////////////////////////////////

void M_Definicion_usuario::Captura_funcion_auxiliar(void)
{
   char xcad[200],cad[200];
   unsigned int x1, y1, x2, y2, st = 0;
   Ventana_iconos *Vt = new Ventana_iconos;
   Get *G1 = new Get;

   Vt->Activa_graba_ventana(1);
   Vt->Define_colores(Gris1,Cian9,Gris9,Blanco);
   Vt->Ventana_centrada(TXT22a,250,80,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);

   // Copia la cadena que contiene la ultima expresion graficada
   strcpy(xcad,Cadena_graficar);
   s_r_t(xcad,50,32);
   G1->Parametros(xcad,x1+10,y1+40,27,"TK");
   G1->Activa_get(1);
   G1->Dibuja();
   NCO->Visualiza_texto(TXT23);
   while(1) {
      NCO->Administrador();
      // Revisa el estado del icono
      if(Vt->Itera() || !Programa_activo || Tecla == ESC) break;
      // Edicion
      G1->Edita(Caracter,Tecla);
      G1->Presionado(BR);
      if(Tecla == ENTER) {
         G1->Cadena_editada(cad);
         if(!(cad[0] == 'F' && cad[1] == '(' && (cad[2] == 'x' || cad[2] == 'y') && cad[3] == ')' && cad[4] == '=')) {
            NCO->Mensaje(TXT30,TXT31,TXT32,TXT33);
          } else {
            st = 1;
            break;
         }
      }
   }

   delete G1;
   delete Vt;
   if(st) {
      // Copia la expresion editada a la cadena que contiene la ultima expresion graficada
      strcpy(Cadena_graficar,cad);
      // Manda a graficar la expresion
   }
}



//////////////////////////////////////////////////////////////////////////
//   Grafica la funcion pasada como parametro en forma de texto ASCII   //
//////////////////////////////////////////////////////////////////////////
void M_Definicion_usuario::Grafica_funcion(const char *exp, const C_3D max, const C_3D min) 
{
   C_3D c_ini, a_coord; 
   double inc;

   unsigned int i, pos;
   char xcad[200],cad[200], *xcad1, *xcad2, *xcad3;
                                               
   // Realiza el analisis sintactico de la expresion
   Analizador_sintactico *As = new Analizador_sintactico;
   // Valida la expresion
   As->Crea_arbol(exp);
   if(!As->Numero_errores()) {
      // Obtiene la expresion en notacion polaca
      As->Retorna_expresion(xcad,POSTORDEN);
      NCO->Visualiza_texto(TXT24);

      xcad1 = new char[200];
      xcad2 = new char[200];
      xcad3 = new char[200];
      // Remplaza el valor de los parametros del actual sistema
      if(Numero_parametros[Sistema_actual]) {
         for(i = 0; i < Numero_parametros[Sistema_actual]; i++) {
            sprintf(xcad3,"|%s|",Nombre_parametro[Sistema_actual][i]);
            while(Busca_cadena(xcad,xcad3,pos)) {
               Substr(xcad,0,pos,xcad1);
               Substr(xcad,pos+3,200,xcad2);
               sprintf(xcad,"%s|%f|%s",xcad1,_P[Sistema_actual][i],xcad2);
            }
         }
      }

      // Inicializa variables
      c_ini.x = 0.0;
      c_ini.y = 0.0;
      c_ini.z = 0.0;

      // Inidca que no se visualize el error matematico
      VISUALIZA_ERROR_MATEMATICO = 0;

      Calculadora_notacion_polaca  cnp;

      // Revisa sobre que eje se hara la graficacion
      switch(xcad[0]) {
         case 'y': // Graficacion sobre el eje X
            // Remplaza la variable por su valor
            strcpy(cad,xcad);
            while(Busca_cadena(cad,"|y|",pos)) {
               s_substr(cad,0,pos,xcad1);
               s_substr(cad,pos+3,200,xcad2);
               sprintf(cad,"%s|M01|%s",xcad1,xcad2);
            }
            inc = (max.y - min.y) / NUMERO_PUNTOS;
            for(c_ini.y = min.y; c_ini.y <= max.y; c_ini.y += inc) {
               a_coord = c_ini;

               // Actualiza el valor a calcular
               cnp.Asigna_valor(0,c_ini.y);
               // Evalua la expresion en notacion polaca
               c_ini.x = cnp.Evalua(cad);
               if(ERROR_MATEMATICO) {
                  ERROR_MATEMATICO = 0;
                  continue;
               }

               if(!Grafica_puntos[Sistema_actual]) Dibuja_linea_ventana(a_coord,c_ini,3,Colores[Color_grafica_aux[Sistema_actual]],1);
                else Dibuja_punto_ventana(c_ini,3,Colores[Color_grafica_aux[Sistema_actual]],1);
            }
            break;
         case 'x': // Graficacion sobre el eje Y
            // Remplaza la variable por su valor
            strcpy(cad,xcad);
            while(Busca_cadena(cad,"|x|",pos)) {
               s_substr(cad,0,pos,xcad1);
               s_substr(cad,pos+3,200,xcad2);
               sprintf(cad,"%s|M01|%s",xcad1,xcad2);
            }
            inc = (max.x - min.x) / NUMERO_PUNTOS;
            for(c_ini.x = min.x; c_ini.x <= max.x; c_ini.x += inc) {
               a_coord = c_ini;
               // Actualiza el valor a calcular
               cnp.Asigna_valor(0,c_ini.x);
               // Evalua la expresion en notacion polaca
               c_ini.y = cnp.Evalua(cad);
               if(ERROR_MATEMATICO) {
                  ERROR_MATEMATICO = 0;
                  continue;
               }

               if(!Grafica_puntos[Sistema_actual]) Dibuja_linea_ventana(a_coord,c_ini,3,Colores[Color_grafica_aux[Sistema_actual]],1);
                else Dibuja_punto_ventana(c_ini,3,Colores[Color_grafica_aux[Sistema_actual]],1);
            }
            break;
         default:
            NCO->Mensaje(TXT25,TXT26);
      }
      delete []xcad1;
      delete []xcad2;
      delete []xcad3;
   } else {
      // visualiza los errores encontrados dentro de la expresion
      Vis_archivo *Ve = new Vis_archivo;
      Ve->Parametros(" ",TXT27,350,300,VISUALIZA);
      // Carga el sistema y las ecuaciones al visualizador
      for(int i = 0; i < As->Numero_errores(); i++) Ve->Adiciona_linea(As->Retorna_error(i));
      Ve->Loop();
      delete Ve;
   }
   delete As;
   // Inidca que se visualize el error matematico
   VISUALIZA_ERROR_MATEMATICO = 1;
}





//////////////////////////////////////////////////////////////////////////
//                Grafica isoclinas del sistema actual                  //
//////////////////////////////////////////////////////////////////////////
void M_Definicion_usuario::Grafica_ceroclinas(const unsigned int tp, const unsigned int eje, const C_3D max, const C_3D min)
{
   double c_ini[2],c[NUMERO_ECUACIONES],incx, incy, Tolerancia = 1e-10; 
   unsigned int xi,i, Max_iter = 20; 
   double p0,p1,q0,q1,p, Valor_isoclina = 0.0;

   // Inidca que no se visualize el error matematico
   VISUALIZA_ERROR_MATEMATICO = 0;

   // Inicializa variables
   for(i = 0; i < NUMERO_ECUACIONES; i++) _X[i] = 0.0, c[i] = 0,0;
   At_Tiempo[Ventana_activa][Sistema_actual] = t = Ac_Tiempo[Ventana_activa][Sistema_actual]; // Fija valor para el tiempo

   if(tp) {
      if(Captura_cadena(TXT34,TXT35,Valor_isoclina,0) != 1) return;
   }
/////////////////////////////////////////////////////
// Tratar de controlar desbordamiento numerico
// acotando con el maximo valor por eje coordenado
/////////////////////////////////////////////////////

   /////////////////////////////////////////////////////////////
   // Ceroclinas tomando como fijo el eje Y
   incx = (max.x - min.x) / (NUMERO_PUNTOS*0.01);
   incy = (max.y - min.y) / (NUMERO_PUNTOS*0.5);


   for(c_ini[0] = min.y; c_ini[0] <= max.y; c_ini[0] += incy) {
      _X[1] = c_ini[0];
      for(c_ini[1] = min.x; c_ini[1] <= max.x; c_ini[1] += incx) {
         for(xi = 0; xi < 2; xi++) {
            p0 = c_ini[1];
            p1 = c_ini[1] + incx;
            _X[0] = p0;
            q0 = ( this->*funcion[Sistema_actual][xi] )() - Valor_isoclina;
            if(ERROR_MATEMATICO) {
               ERROR_MATEMATICO = 0;
               continue;
            }
            // Controla el desbordamiento numerico
            if(fabs(q0) > MAX_VALOR[Ventana_activa][Sistema_actual]) continue;
            _X[0] = p1;
            q1 = ( this->*funcion[Sistema_actual][xi] )() - Valor_isoclina;
            if(ERROR_MATEMATICO) {
               ERROR_MATEMATICO = 0;
               continue;
            }
            // Controla el desbordamiento numerico
            if(fabs(q1) > MAX_VALOR[Ventana_activa][Sistema_actual]) continue;
            for(i = 2; i < Max_iter; i++) {
               if((q1-q0) == 0.0) break;
               p = p1 - (q1*((p1-p0)/(q1-q0)));
               // Controla divergencia
               if(fabs(p-p1) > (max.x - min.x)) break;
               if(fabs(p-p1) < Tolerancia) {
                  c[0] = p, c[1] = c_ini[0];
                  Dibuja_punto(c,At_Tiempo[Ventana_activa][Sistema_actual],3,Colores[Color_grafica_aux[Sistema_actual]],1);
                  break;
               }
               p0 = p1, q0 = q1, p1 = p;
               _X[0] = p;
               q1 = ( this->*funcion[Sistema_actual][xi] )() - Valor_isoclina;
               // Controla el desbordamiento numerico
               if(ERROR_MATEMATICO) {
                  ERROR_MATEMATICO = 0;
                  continue;
               }
               if(fabs(q1) > MAX_VALOR[Ventana_activa][Sistema_actual]) break;
            }
         }
      }
      // Cancela la graficacion de ceroclinas
      if(NCO->Busca_bufer_teclado(ESC)) break;
   }
   /////////////////////////////////////////////////////////////


   /////////////////////////////////////////////////////////////
   // Ceroclinas tomando como fijo el eje X
   incy = (max.y - min.y) / (NUMERO_PUNTOS*0.01);
   incx = (max.x - min.x) / (NUMERO_PUNTOS*0.5);


   for(c_ini[1] = min.x; c_ini[1] <= max.x; c_ini[1] += incx) {
      _X[0] = c_ini[1];
      for(c_ini[0] = min.y; c_ini[0] <= max.y; c_ini[0] += incy) {
         for(xi = 0; xi < 2; xi++) {
            p0 = c_ini[0];
            p1 = c_ini[0] + incy;
            _X[1] = p0;
            q0 = ( this->*funcion[Sistema_actual][xi] )() - Valor_isoclina;
            if(ERROR_MATEMATICO) {
               ERROR_MATEMATICO = 0;
               continue;
            }
            // Controla el desbordamiento numerico
            if(fabs(q0) > MAX_VALOR[Ventana_activa][Sistema_actual]) continue;
            _X[1] = p1;
            q1 = ( this->*funcion[Sistema_actual][xi] )() - Valor_isoclina;
            if(ERROR_MATEMATICO) {
               ERROR_MATEMATICO = 0;
               continue;
            }
            // Controla el desbordamiento numerico
            if(fabs(q1) > MAX_VALOR[Ventana_activa][Sistema_actual]) continue;
            for(i = 2; i < Max_iter; i++) {
               if((q1-q0) == 0.0) break;
               p = p1 -(q1*((p1-p0)/(q1-q0)));
               // Controla divergencia
               if(fabs(p-p1) > (max.y - min.y)) break;
               // Controla el desbordamiento numerico
               if(fabs(p-p1) < Tolerancia) {
                  c[1] = p, c[0] = c_ini[1];
                  Dibuja_punto(c,At_Tiempo[Ventana_activa][Sistema_actual],3,Colores[Color_grafica_aux[Sistema_actual]],1);
                  break;
               }
               p0 = p1, q0 = q1, p1 = p;
               _X[1] = p;
               q1 = ( this->*funcion[Sistema_actual][xi] )() - Valor_isoclina;
               if(ERROR_MATEMATICO) {
                  ERROR_MATEMATICO = 0;
                  continue;
               }
               // Controla el desbordamiento numerico
               if(fabs(q1) > MAX_VALOR[Ventana_activa][Sistema_actual]) break;
            }
         }
      }
      // Cancela la graficacion de ceroclinas
      if(NCO->Busca_bufer_teclado(ESC)) break;
   }
   /////////////////////////////////////////////////////////////


   // Inidca que se visualize el error matematico
   VISUALIZA_ERROR_MATEMATICO = 1;
}


// Cambia la definicion de la ventana uno a la ventana dos y viceversa
void M_Definicion_usuario::Invierte_ventanas(void)
{
   int i, i1; 
   C_2I   v1;
   double v2;
   C_3D   v3;
   C_3I   v4;
   for(i = 0; i < Numero_sistemas; i++) {
       v4 = Escenario[0][i];
       Escenario[0][i] = Escenario[1][i];
       Escenario[1][i] = v4;

       v3 = Dim_maxima[0][i];
       Dim_maxima[0][i] = Dim_maxima[1][i];
       Dim_maxima[1][i] = v3;
             
       v3 = Dim_minima[0][i];   
       Dim_minima[0][i] = Dim_minima[1][i];
       Dim_minima[1][i] = v3;   

       v3 = Escala[0][i];       
       Escala[0][i] = Escala[1][i];
       Escala[1][i] = v3;
              
       v2 = Angulos[0][i][0];   
       Angulos[0][i][0] = Angulos[1][i][0];
       Angulos[1][i][0] = v2;   

       v2 = Angulos[0][i][1];
       Angulos[0][i][1] = Angulos[1][i][1];   
       Angulos[1][i][1] = v2;   

       v1 = Centro[0][i];
       Centro[0][i] = Centro[1][i];       
       Centro[1][i] = v1;       

       for(i1 = 0; i1 < Numero_ecuaciones[i]; i1++) {
          v2 = Ac_Condicion[0][i][i1];
          Ac_Condicion[0][i][i1] = Ac_Condicion[1][i][i1];
          Ac_Condicion[1][i][i1] = v2;
       }

       for(i1 = 0; i1 < Numero_ecuaciones[i]; i1++) {
          v2 = At_Condicion[0][i][i1];
          At_Condicion[0][i][i1] = At_Condicion[1][i][i1];
          At_Condicion[1][i][i1] = v2;
       }

       v2 = Ac_Tiempo[0][i];
       Ac_Tiempo[0][i] = Ac_Tiempo[1][i];
       Ac_Tiempo[1][i] = v2;

       v2 = At_Tiempo[0][i];
       At_Tiempo[0][i] = At_Tiempo[1][i];
       At_Tiempo[1][i] = v2;

       v2 = MAX_VALOR[0][i];
       MAX_VALOR[0][i] = MAX_VALOR[1][i];
       MAX_VALOR[1][i] = v2;

       v3 = Paso_cursor[0][i]; 
       Paso_cursor[0][i] = Paso_cursor[1][i];
       Paso_cursor[1][i] = v3;

       v3 = Incremento_paso[0][i]; 
       Incremento_paso[0][i] = Incremento_paso[1][i];
       Incremento_paso[1][i] = v3;
   }
}

