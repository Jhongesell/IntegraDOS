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



#include "bintegra.hpp"
#include "nucleo.hpp"
#include "\libreria\gen_rep.hpp"
#include "cad_car.hpp"
#include "combo_b.hpp"
#include "r_esp.hpp"
#include "get.hpp"
#include "vis_ayud.hpp"

extern "C" {
   #include <string.h>
}


// Definicion externa del objeto Nucleo
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Dimenciones de la pantalla
extern int X_MAX;
extern int Y_MAX;
// Indica cual texto explicativo a visualizar
extern unsigned int N_texto_explicativo;
// Indica cual texto explicativo a visualizar
extern unsigned int N_texto_explicativo;

#ifdef _IDIOMA_ESPANOL_


   // Nombre de los colores
   static const char *Nombre_colores[] = {
     "Negro",
     "Gris Claro",
     "Gris Oscuro",
     "Azul Claro",
     "Azul Oscuro",
     "Verde Claro",
     "Verde Oscuro",
     "C¡an Claro",
     "C¡an Oscuro",
     "Rojo Claro",
     "Rojo Oscuro",
     "Magenta Claro",
     "Magenta Oscuro",
     "CafŠ",
     "Amarillo",
     "Blanco",
     "0"
   };

   #define TXT01    "GRABAR ESPECIFICACIONES"
   #define TXT02    "LEER ESPECIFICACIONES"
   #define TXT03    "IMPRIMIR ESPECIFICACIONES"
   #define TXT04    "SISTEMA:"
   #define TXT05    "PARAMETROS:"
   #define TXT06    "METODO NUMERICO:"
   #define TXT07    "DIMENSIONES DE LA VENTANA"
   #define TXT07a   "DIMENSIONES DE LA VENTANA 1"
   #define TXT07b   "DIMENSIONES DE LA VENTANA 2"

   #define TXT08    "Horizontal   M ximo: %f,  M¡nimo: %f"
   #define TXT09    "Vertical     M ximo: %f,  M¡nimo: %f"
   #define TXT10    "Tercer Eje   M ximo: %f,  M¡nimo: %f"
   #define TXT12    "Eje              M¡nimo        M ximo"
   #define TXT13    "Horizontal"
   #define TXT14    "Vertical"
   #define TXT15    "Tercer eje"
   #define TXT16    "Todos"
   #define TXT17    "Factor"
   #define TXT18    "Error en los valores de la pantalla"
   #define TXT19    "No se tomar n en cuenta estos valores"
   #define TXT20    "COLORES DEL SISTEMA"
   #define TXT21    "Ejes Coordenados:"
   #define TXT22    "Color del Gr fico"
   #define TXT23    "ESCENARIO GRAFICO"
   #define TXT26    "Color de la funci¢n"
   #define TXT27    "a gr ficar"
   #define TXT28    "ECUACIONES:"
   #define TXT29    "FUNCIONES AUXILIARES A GRAFICAR:"
   #define TXT30    "INTEGRACION NUMERICA:"
   #define TXT31    "CONDICION INICIAL"
   #define TXT32    "CONDICION FINAL"
   #define TXT40    "EL archivo no es de este proyecto"
   #define TXT41    "Pertenece al proyecto:"
#else
#endif


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


extern const char *Parametros_metodos_integracion1[];
extern const char *Parametros_metodos_integracion2[];
extern const char *Metodos_integracion[];


////////////////////////////////////////////////////////////////////////
//               Inicializa las variables del sistema                 //
////////////////////////////////////////////////////////////////////////

void Base_integra::Inicializa_variables(void)
{
   // Inicializa la clase usuario
   Definicion_usuario::Inicializa();

   // Inicializa la configuracion del EXE
   Definicion_usuario::Configuracion();

   // Inicializa la clase metodos numericos
   Metodos_numericos::Inicializa();
   Cadena_graficar[0] = 0;
   unsigned int i, xi;
   for(i = 0; i < Numero_sistemas ; i++) {
       // Condiciones iniciales
       Ac_Tiempo[0][i] = At_Tiempo[0][i] = 0; // Inicializa la variable Tiempo
       for(xi = 0; xi < NUMERO_ECUACIONES; xi++) Ac_Condicion[0][i][xi] = At_Condicion[0][i][xi] = 0;
       Ac_Tiempo[1][i] = At_Tiempo[1][i] = 0; // Inicializa la variable Tiempo
       for(xi = 0; xi < NUMERO_ECUACIONES; xi++) Ac_Condicion[1][i][xi] = At_Condicion[1][i][xi] = 0;

       // Dimenciones de la ventana de trabajo numero 1
       Dim_minima[0][i].x = Config[0][i][0];
       Dim_minima[0][i].y = Config[0][i][1];
       Dim_minima[0][i].z = Config[0][i][2];
       Dim_maxima[0][i].x = Config[0][i][3];
       Dim_maxima[0][i].y = Config[0][i][4];
       Dim_maxima[0][i].z = Config[0][i][5];
       // Dimenciones de la ventana de trabajo numero 2
       Dim_minima[1][i].x = Config[1][i][0]; 
       Dim_minima[1][i].y = Config[1][i][1];
       Dim_minima[1][i].z = Config[1][i][2];
       Dim_maxima[1][i].x = Config[1][i][3]; 
       Dim_maxima[1][i].y = Config[1][i][4];
       Dim_maxima[1][i].z = Config[1][i][5];
       
       P_a = 0; // Indica el juego activo de parametros
       Metodo_N_actual[i] = 7; // Fija el actual metodo numerico de integraci¢n Runge-Kutta-Fehlberg

       Redimensiona(i);

       // Ejes, color de graficos y el tipo de grafico a generar
       Color_grafica[i] = 0; // Color del grafico al integrar
       Color_grafica_aux [i] = 1; // Color del grafico de las funciones auxiliares
       Color_ejes[i].x = 9, Color_ejes[i].y = 6, Color_ejes[i].z = 4; // Color de los ejes coordenados por cada sistema
       Grafica_puntos[i] = 1;  // Grafica en puntos
   }

   // Numero de puntos a graficar para funciones auxiliares
   NUMERO_PUNTOS = 2500.0;  

   // Fija el sistema actual
   Sistema_actual = 0;
   // Fija el guardar posici¢n
   Guarda_posicion = NO;
   // Fija la visualizacion de coordenadas
   Visualiza_coordenadas_sistema = SI;
   // Fija la visualizacion del sistema en coordenadas rectangulares
   Coord_rectangulares = SI; 
   // Genera la lista de los escenarios
	Genera_lista_escenarios();
}


void Base_integra::Redimensiona(unsigned int i)
{
   double temp1, temp2, temp3;
   // Control del cursor

   // Ventana uno
   Paso_cursor[0][i].x = (Dim_maxima[0][i].x - Dim_minima[0][i].x)/ 200.0;
   Paso_cursor[0][i].y = (Dim_maxima[0][i].y - Dim_minima[0][i].y)/ 200.0;
   Paso_cursor[0][i].z = (Dim_maxima[0][i].z - Dim_minima[0][i].z)/ 200.0;
   Incremento_paso[0][i].x = Paso_cursor[0][i].x;
   Incremento_paso[0][i].y = Paso_cursor[0][i].y;
   Incremento_paso[0][i].z = Paso_cursor[0][i].z;

   // Ventana dos
   Paso_cursor[1][i].x = (Dim_maxima[1][i].x - Dim_minima[1][i].x)/ 200.0;
   Paso_cursor[1][i].y = (Dim_maxima[1][i].y - Dim_minima[1][i].y)/ 200.0;
   Paso_cursor[1][i].z = (Dim_maxima[1][i].z - Dim_minima[1][i].z)/ 200.0;
   Incremento_paso[1][i].x = Paso_cursor[1][i].x;
   Incremento_paso[1][i].y = Paso_cursor[1][i].y;
   Incremento_paso[1][i].z = Paso_cursor[1][i].z;

   // Metodos numericos
   temp1 = (Dim_maxima[0][i].x - Dim_minima[0][i].x)* 50.0;
   temp2 = (Dim_maxima[0][i].y - Dim_minima[0][i].y)* 50.0;
   temp3 = temp1 > temp2 ? temp1 : temp2;
   temp1 = (Dim_maxima[0][i].z - Dim_minima[0][i].z)* 50.0;
   temp2 = temp1 > temp3 ? temp1 : temp3;
   MAX_VALOR[0][i] = temp2;
   temp1 = (Dim_maxima[1][i].x - Dim_minima[1][i].x)* 50.0;
   temp2 = (Dim_maxima[1][i].y - Dim_minima[1][i].y)* 50.0;
   temp3 = temp1 > temp2 ? temp1 : temp2;    
   temp1 = (Dim_maxima[1][i].z - Dim_minima[1][i].z)* 50.0;
   temp2 = temp1 > temp3 ? temp1 : temp3;
   MAX_VALOR[1][i] = temp2;

   // Campo vectorial
   Parametros_camp_vect[i][0] = 20.0; // Numero de vectores X
   Parametros_camp_vect[i][1] = 20.0; // Numero de vectores Y
   Parametros_camp_vect[i][2] = (Dim_maxima[0][i].x - Dim_minima[0][i].x) / Parametros_camp_vect[i][0]; // Longitud de X
   Parametros_camp_vect[i][3] = (Dim_maxima[0][i].y - Dim_minima[0][i].y) / Parametros_camp_vect[i][1]; // Longitud de Y
}



///////////////////////////////////////////////////////////////////
//                  Graba el estado del sistema                  //
///////////////////////////////////////////////////////////////////

void Base_integra::Graba_estado(const int tp, const char *xarch)
{
   unsigned int i1, i2, i3;
   char arch[100], *TXT = TXT01;

   if(tp) {
      Manipulador_archivos ma;
      ma.Cambia_ext_path(Proyecto,"CFG",arch);
      if(!Retorna_archivo_seleccionado(arch,0,TXT,"CFG",arch)) return;
   } else strcpy(arch,xarch);

   // Inicializa el objeto manipulador de archivos
   Manipulador_archivos *Grab_param = new Manipulador_archivos;
   Grab_param->Parametros(arch,GRABA_ARCHIVO,0,TXT);
   strcpy(arch,Proyecto);
   s_r_t(arch,20,32);
   Grab_param->Graba_caracteres(arch,15);
   for(i1 = 0; i1 < Numero_sistemas; i1++) {
       // Graba parametros de cada sistema
       Grab_param->Graba_entero(Numero_parametros[i1]);
       for(i2 = 0; i2 < Numero_parametros[i1]; i2 ++) Grab_param->Graba_double(_P[P_a][i1][i2]);
       // Graba la configuracion de los metodos numericos
       Grab_param->Graba_entero(Metodo_N_actual[i1]);
       for(i2 = 0; i2 < NUMERO_METODOS_NUMERICOS; i2 ++) {
           for(i3 = 0; i3 < Parametros_metodo[i1][i2][0]; i3++) Grab_param->Graba_double(Parametros_metodo[i1][i2][i3]);
       }
       // Graba la dimencion la ventana
       for(i3 = 0; i3 < 2; i3++) {
          Grab_param->Graba_double(Dim_maxima[i3][i1].x);
          Grab_param->Graba_double(Dim_minima[i3][i1].x);
          Grab_param->Graba_double(Dim_maxima[i3][i1].y);
          Grab_param->Graba_double(Dim_minima[i3][i1].y);
          Grab_param->Graba_double(Dim_maxima[i3][i1].z);
          Grab_param->Graba_double(Dim_minima[i3][i1].z);
       }
       // Graba la configuracion de la ventana
       Grab_param->Graba_entero(Color_ejes[i1].x);
       Grab_param->Graba_entero(Color_ejes[i1].y);
       Grab_param->Graba_entero(Color_ejes[i1].z);
       Grab_param->Graba_entero(Color_grafica[i1]);
       Grab_param->Graba_entero(Color_grafica_aux[i1]);

       for(i3 = 0; i3 < 2; i3++) {
          Grab_param->Graba_double(Escala[i3][i1].x);
          Grab_param->Graba_double(Escala[i3][i1].y);
          Grab_param->Graba_double(Escala[i3][i1].z);
          Grab_param->Graba_entero(Centro[i3][i1].x);
          Grab_param->Graba_entero(Centro[i3][i1].y);
          Grab_param->Graba_double(Angulos[i3][i1][0]);
          Grab_param->Graba_double(Angulos[i3][i1][1]);
          // Graba el esenario activo por sistema
          Grab_param->Graba_entero(Escenario[i3][i1].x);
          Grab_param->Graba_entero(Escenario[i3][i1].y);
          Grab_param->Graba_entero(Escenario[i3][i1].z);
       }
       // Graba configuracion del cursor
       for(i3 = 0; i3 < 2; i3++) {
          Grab_param->Graba_double(Paso_cursor[i3][i1].x);
          Grab_param->Graba_double(Incremento_paso[i3][i1].x);
          Grab_param->Graba_double(Paso_cursor[i3][i1].y);
          Grab_param->Graba_double(Incremento_paso[i3][i1].y);
          Grab_param->Graba_double(Paso_cursor[i3][i1].z);
          Grab_param->Graba_double(Incremento_paso[i3][i1].z);
       }
       // Graba la configuracion del campo vectorial
       for(i2 = 0; i2 < 4; i2++) Grab_param->Graba_double(Parametros_camp_vect[i1][i2]);
   }
   delete Grab_param;
}


///////////////////////////////////////////////////////////////////
//                    Lee el estado del sistema                  //
///////////////////////////////////////////////////////////////////

int Base_integra::Lee_estado(const char *archivo, const int tp)
{
   unsigned int i1, i2, i3;
   int st = 1;
   char arch[100], *TXT = TXT02;

   if(tp) {
      Manipulador_archivos ma;
      ma.Cambia_ext_path(Proyecto,"CFG",arch);
      if(!Retorna_archivo_seleccionado(arch,1,TXT,"CFG",arch)) return 0;
   } else strcpy(arch,archivo);

   // Inicializa el objeto manipulador de archivos
   Manipulador_archivos *Lee_param = new Manipulador_archivos;
   Lee_param->Parametros(arch,LEE_ARCHIVO,0,TXT);

   Lee_param->Lee_caracteres(arch,15);
   i1 = 0;
   while(Proyecto[i1]) {
      if(arch[i1] != Proyecto[i1]) st = 0;
      i1++;
   }
   if(st) {
      for(i1 = 0; i1 < Numero_sistemas; i1++) {
          // Lee parametros de cada sistema
          Lee_param->Lee_entero(Numero_parametros[i1]);
          for(i2 = 0; i2 < Numero_parametros[i1]; i2 ++) Lee_param->Lee_double(_P[P_a][i1][i2]);
          // Lee la configuracion de los metodos numericos
          Lee_param->Lee_entero(Metodo_N_actual[i1]);
          for(i2 = 0; i2 < NUMERO_METODOS_NUMERICOS; i2 ++) {
              Lee_param->Lee_double(Parametros_metodo[i1][i2][0]);
              for(i3 = 1; i3 < Parametros_metodo[i1][i2][0]; i3++) Lee_param->Lee_double(Parametros_metodo[i1][i2][i3]);
          }
          // Lee la dimencion la ventana
          for(i3 = 0; i3 < 2; i3++) {
             Lee_param->Lee_double(Dim_maxima[i3][i1].x);
             Lee_param->Lee_double(Dim_minima[i3][i1].x);
             Lee_param->Lee_double(Dim_maxima[i3][i1].y);
             Lee_param->Lee_double(Dim_minima[i3][i1].y);
             Lee_param->Lee_double(Dim_maxima[i3][i1].z);
             Lee_param->Lee_double(Dim_minima[i3][i1].z);
          }
          // Lee la configuracion de la ventana
          Lee_param->Lee_entero(Color_ejes[i1].x);
          Lee_param->Lee_entero(Color_ejes[i1].y);
          Lee_param->Lee_entero(Color_ejes[i1].z);
          Lee_param->Lee_entero(Color_grafica[i1]);
          Lee_param->Lee_entero(Color_grafica_aux[i1]);

          for(i3 = 0; i3 < 2; i3++) {
             Lee_param->Lee_double(Escala[i3][i1].x);
             Lee_param->Lee_double(Escala[i3][i1].y);
             Lee_param->Lee_double(Escala[i3][i1].z);
             Lee_param->Lee_entero(Centro[i3][i1].x);
             Lee_param->Lee_entero(Centro[i3][i1].y);
             Lee_param->Lee_double(Angulos[i3][i1][0]);
             Lee_param->Lee_double(Angulos[i3][i1][1]);
             // Lee el esenario activo por sistema
             Lee_param->Lee_entero(Escenario[i3][i1].x);
             Lee_param->Lee_entero(Escenario[i3][i1].y);
             Lee_param->Lee_entero(Escenario[i3][i1].z);
          }
          // Lee configuracion del cursor
          for(i3 = 0; i3 < 2; i3++) {
             Lee_param->Lee_double(Paso_cursor[i3][i1].x);
             Lee_param->Lee_double(Incremento_paso[i3][i1].x);
             Lee_param->Lee_double(Paso_cursor[i3][i1].y);
             Lee_param->Lee_double(Incremento_paso[i3][i1].y);
             Lee_param->Lee_double(Paso_cursor[i3][i1].z);
             Lee_param->Lee_double(Incremento_paso[i3][i1].z);
          }
          // Lee la configuracion del campo vectorial
          for(i2 = 0; i2 < 4; i2++) Lee_param->Lee_double(Parametros_camp_vect[i1][i2]);
      }
    } else {
      arch[12] = 0;
      Manipulador_archivos ma;
      ma.Cambia_ext_path(arch,"EXE");
      NCO->Mensaje(TXT40,TXT41,arch);
   }
   delete Lee_param;
   return st;
}


///////////////////////////////////////////////////////////////////
//                Imprime el estado del sistema                  //
///////////////////////////////////////////////////////////////////

void Base_integra::Imprime_estado(const int tr, const int dp)
{
   unsigned int i1, i2, xi = 0, xxi = Numero_sistemas;
   char xcad[100], xcad1[20], *TXT = TXT03;

	Ctrl_reporte *Ip = new Ctrl_reporte;
	// Solo el sistema actual
   if(tr) xi = Sistema_actual, xxi = Sistema_actual + 1;
   // Selecciona el dispositivo del reporte
   if(dp) Ip->Parametros(0,60,120,REPORTE_VISUALIZADO,LPT1,TXT);
    else Ip->Parametros(0,60,120,CONFIGURACION,LPT1,TXT);

   for(i1 = xi; i1 < xxi; i1++) {
       // Imprime el nombre del sistema
       sprintf(xcad,"%s %s",TXT04,Nombre_sistema[i1]);
       Ip->Formato_impresion(1,3,xcad);

       // Imprime las ecuaciones del sistema
       Ip->Formato_impresion(3,2,TXT28);
       for(i2 = 0; i2 < Numero_ecuaciones[i1]; i2++) {
           Ip->Formato_impresion(6,1,Nombre_ecuacion[i1][i2]);
       }

       // Imprime los parametros del sistema
       if(Numero_parametros[i1]) {
          Ip->Formato_impresion(3,2,TXT05);
          for(i2 = 0; i2 < Numero_parametros[i1]; i2++) {
              sprintf(xcad,"%s = %1.9e",Nombre_parametro[i1][i2],_P[P_a][i1][i2]);
              Ip->Formato_impresion(6,1,xcad);
          }
       }
       // Funciones auxliliares a graficar
       if(Numero_funciones_auxiliares[i1]) {
          Ip->Formato_impresion(3,2,TXT29);
          for(i2 = 0; i2 < Numero_funciones_auxiliares[i1]; i2 ++) {
             sprintf(xcad,"[CTRL-F%d]  %s",i2+1,Funciones_auxiliares[i1][i2]);
             Ip->Formato_impresion(6,1,xcad);
          }
       }


       // Imprime la dimension las ventanas
       Ip->Formato_impresion(3,2,TXT07a);
       sprintf(xcad,TXT08,Dim_maxima[0][i1].x,Dim_minima[0][i1].x);
       Ip->Formato_impresion(6,1,xcad);
       sprintf(xcad,TXT09,Dim_maxima[0][i1].y,Dim_minima[0][i1].y);
       Ip->Formato_impresion(6,1,xcad);
       sprintf(xcad,TXT10,Dim_maxima[0][i1].z,Dim_minima[0][i1].z);
       Ip->Formato_impresion(6,1,xcad);
       // Segunda ventana
       if(Doble_ventana) {
          Ip->Formato_impresion(3,2,TXT07b);
          sprintf(xcad,TXT08,Dim_maxima[1][i1].x,Dim_minima[1][i1].x);
          Ip->Formato_impresion(6,1,xcad);
          sprintf(xcad,TXT09,Dim_maxima[1][i1].y,Dim_minima[1][i1].y);
          Ip->Formato_impresion(6,1,xcad);
          sprintf(xcad,TXT10,Dim_maxima[1][i1].z,Dim_minima[1][i1].z);
          Ip->Formato_impresion(6,1,xcad);
       }

       // Imprime la configuracion de los metodos numericos
       Ip->Formato_impresion(3,2,TXT06);
       s_nocar(Metodos_integracion[Metodo_N_actual[i1]],127,xcad,1);
       Ip->Formato_impresion(6,1,xcad);
       for(i2 = 0; i2 < Parametros_metodo[i1][Metodo_N_actual[i1]][0]; i2++) {
          if(Parametros_metodo[i1][Metodo_N_actual[i1]][0] < 3.0) s_nocar(Parametros_metodos_integracion1[i2],127,xcad,1);
           else s_nocar(Parametros_metodos_integracion2[i2],127,xcad,1);
          Ip->Formato_impresion(6,1,xcad);
          sprintf(xcad,"= %1.9e",Parametros_metodo[i1][Metodo_N_actual[i1]][i2+1]);
          Ip->Formato_impresion(30,0,xcad);
       }

       // Condiciones iniciales y finales
       Ip->Formato_impresion(3,2,TXT30);
       Ip->Formato_impresion(6,1,TXT31);
       Ip->Formato_impresion(35,0,TXT32);
       for(i2 = 0; i2 < Numero_ecuaciones[i1]; i2++) {
           Nombre_ecuacion_sistema_actual(i1,i2,xcad1);
           strcat(xcad1,"(t)");
           s_r_t(xcad1,7,32);
           sprintf(xcad,"%s = %1.9e",xcad1,Ac_Condicion[Ventana_activa][i1][i2]);
           Ip->Formato_impresion(6,1,xcad);
           sprintf(xcad,"%s = %1.9e",xcad1,At_Condicion[Ventana_activa][i1][i2]);
           Ip->Formato_impresion(35,0,xcad);
       }
       sprintf(xcad,"Tiempo  = %1.9e",Ac_Tiempo[Ventana_activa][i1]);
       Ip->Formato_impresion(6,1,xcad);
       sprintf(xcad,"Tiempo  = %1.9e",At_Tiempo[Ventana_activa][i1]);
       Ip->Formato_impresion(35,0,xcad);

       Ip->Formato_impresion(6,2,"N£mero de Iteraciones");
       Ip->Formato_impresion(6,1,"al Integrar");
       sprintf(xcad,"= %ld",Numero_iteraciones);
       Ip->Formato_impresion(30,0,xcad);
       if(Parametros_metodo[i1][Metodo_N_actual[i1]][0] == 3.0) {
          Ip->Formato_impresion(6,1,"Paso m¡nimo");
          sprintf(xcad,"= %1.9e",Paso_minimo);
          Ip->Formato_impresion(30,0,xcad);
          Ip->Formato_impresion(6,1,"Paso m ximo");
          sprintf(xcad,"= %1.9e",Paso_maximo);
          Ip->Formato_impresion(30,0,xcad);
       }

       Ip->Formato_impresion(35,4," ");
   }
   delete Ip;
}

////////////////////////////////////////////////////////////////////////
//   Selecciona las ecuaciones a visualizar en los ejes coordenados   //
////////////////////////////////////////////////////////////////////////

int Base_integra::Escenarios(const int tp)
{
   unsigned int x1, x2, y1, y2, st = 0, i;
   unsigned opz;
   opz = Escenario[tp][Sistema_actual].z;

   C_2I vent;
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(TXT23,240,230,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Define_color_texto(Negro);
   Vt->Limpia_recuadro(15,50,170,200,Gris1,Blanco,Gris9);
   // visualiza el nombre de los escenarios graficos
   Vt->Visualiza_texto(20,40,TXT21);
   Vt->Visualiza_texto(20,60,TXT13);
   Vt->Visualiza_texto(20,105,TXT14);
   Vt->Visualiza_texto(20,150,TXT15);

   const char   **C_ec1 = new const char *[NUMERO_ECUACIONES+3];
   const char   **C_ec2 = new const char *[NUMERO_ECUACIONES+3];

   for(i = 0; i < (Numero_ecuaciones[Sistema_actual] + 3); i++) {
       C_ec1[i] = C_Ecuaciones1[i];
       C_ec2[i] = C_Ecuaciones2[i];
   }

   // Escenario no activo
   if(Escenario[tp][Sistema_actual].z == 0) opz = ind_no_avil + 1;

   vent.x = x1 + 20, vent.y = y1 + 75;
   Combo_box *Co1 = new Combo_box;
   Co1->Parametros(C_ec1,vent,Escenario[tp][Sistema_actual].x,"");
   Co1->Dibuja(C_ec1);

   vent.y = y1 + 120;
   Combo_box *Co2 = new Combo_box;
   Co2->Parametros(C_ec1, vent,Escenario[tp][Sistema_actual].y,"");
   Co2->Dibuja(C_ec1);

   vent.y = y1 + 165;
   Combo_box *Co3 = new Combo_box;
   Co3->Parametros(C_ec2, vent,opz,"");
   Co3->Dibuja(C_ec2);


   Icono *Ic1 = new Icono;
   Icono *Ic2 = new Icono;
   Ic1->Define_icono("OK.ICO",x2-50,y1+70);
   Ic1->Dibuja();
   Ic2->Define_icono("CANCEL.ICO",x2-50,y1+130);
   Ic2->Dibuja();

   while(!st) {
      NCO->Administrador();
      if(Vt->Itera() || !Programa_activo || Tecla == ESC) break;
      Co1->Controla(Tecla, C_ec1);
      Co2->Controla(Tecla, C_ec1);
      Co3->Controla(Tecla, C_ec2);
      if(Ic1->Oprimido()) {
         // Valida que la seleccion sea unica
         if(Co1->Retorna_opcion() == Co2->Retorna_opcion() || Co1->Retorna_opcion() == Co3->Retorna_opcion() || Co2->Retorna_opcion() == Co3->Retorna_opcion()) {
            st = 0;
            NCO->Mensaje("En los escenarios a visualizar","hay almenos uno repetido");
         } else st = 1;
      }
      if(Ic2->Oprimido()) st = 2;
   }
   if(st == 1) {
      Escenario[tp][Sistema_actual].x = Co1->Retorna_opcion();
      Escenario[tp][Sistema_actual].y = Co2->Retorna_opcion();
      Escenario[tp][Sistema_actual].z = Co3->Retorna_opcion();
      // Escenario no activo
      if(Escenario[tp][Sistema_actual].z == (ind_no_avil + 1)) Escenario[tp][Sistema_actual].z = 0;
      // Revisa si la configuracion sera para ambas ventanas
      if(Doble_ventana) {
         if(Opcion("¨Desea utilizar est  configuraci¢n","en ambas ventanas?") == 1)  {
            Escenario[!tp][Sistema_actual].x = Escenario[tp][Sistema_actual].x ;
            Escenario[!tp][Sistema_actual].y = Escenario[tp][Sistema_actual].y ;
            Escenario[!tp][Sistema_actual].z = Escenario[tp][Sistema_actual].z ;
         }
      }
   }
   delete Co1;
   delete Co2;
   delete Co3;
   delete Ic1;
   delete Ic2;
   delete Vt;
   delete []C_ec1;
   delete []C_ec2;
   return st;
}



///////////////////////////////////////////////////////////////////
//         Cambia la dimencion de la ventana de trabajo          //
///////////////////////////////////////////////////////////////////

int Base_integra::Cambia_dimenciones_pantalla(const int tp)
{
   int i, y = 90, get_act = 0, get_sw = 0, st = 0;
   double x[8];
   unsigned int x1,y1,x2,y2;
   x[0] = Dim_minima[tp][Sistema_actual].x;
   x[1] = Dim_maxima[tp][Sistema_actual].x;
   x[2] = Dim_minima[tp][Sistema_actual].y;
   x[3] = Dim_maxima[tp][Sistema_actual].y;
   x[4] = Dim_minima[tp][Sistema_actual].z;
   x[5] = Dim_maxima[tp][Sistema_actual].z;
   x[6] = 0, x[7] = 0;
   Ventana_iconos *Vt = new Ventana_iconos;
   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(TXT07,430,290,0);
   Vt->Dibuja();
   Vt->Define_color_texto(Negro);
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Visualiza_texto(20,65,TXT12);
   Vt->Limpia_recuadro(20,80,350,190,Gris1,Blanco,Gris9);
   Vt->Visualiza_texto(35,100,TXT13);
   Vt->Visualiza_texto(35,130,TXT14);
   Vt->Visualiza_texto(35,160,TXT15);
   Vt->Limpia_recuadro(20,200,350,270,Gris1,Blanco,Gris9);
   Vt->Visualiza_texto(35,220,TXT16);
   Vt->Visualiza_texto(35,250,TXT17);
   Get *Gts = new Get[8];
   Gts[0].Activa_get(1);
   for(i = 0; i < 3; i++) {
      Gts[i*2].Parametros(x[i*2],x1+130,y1+y,10,0);
      Gts[i*2].Dibuja();
      Gts[1+(i*2)].Parametros(x[i*2+1],x1+240,y1+y,10,0);
      Gts[1+(i*2)].Dibuja();
      y+=30;
   }
   Gts[6].Parametros(x[6],x1+130,y1+210,10,0);
   Gts[6].Dibuja();
   Gts[7].Parametros(x[7],x1+130,y1+240,10,0);
   Gts[7].Dibuja();

   Icono *Ic1 = new Icono;
   Icono *Ic2 = new Icono;
   Ic1->Define_icono("OK.ICO",x2-60,y1+110);
   Ic1->Dibuja();
   Ic2->Define_icono("CANCEL.ICO",x2-60,y1+200);
   Ic2->Dibuja();

   while(!st) {
      NCO->Administrador();
      if(Vt->Itera() || !Programa_activo || Tecla == ESC) break;
      for(i = 0; i < 8; i++) {
         if(Gts[i].Presionado(BR)) get_act = i, get_sw = 1;
         Gts[i].Edita(Caracter,Tecla);
      }
      if((Tecla == ENTER || Tecla == FLECHA_ABAJO) && get_act < 7) get_act ++, get_sw = 1;
      if((Tecla == FLECHA_ARRIBA) && get_act > 0) get_act --, get_sw = 1;
      if(get_sw) {
         for(i = 0; i < 8; i++) {
            Gts[i].Activa_get(i == get_act);
            get_sw = 0;
         }
      }
      if(Ic1->Oprimido()) st = 1;
      if(Ic2->Oprimido()) st = 2;
   }
   if(st == 1) {
      for(i = 0; i < 8; i++) Gts[i].Cadena_editada(x[i]);
   }
   delete []Gts;
   delete Ic1;
   delete Ic2;
   delete Vt;
   if(st == 1) {
///////////////////////////////////////////////////////////////
// Validar las dimenciones de la ventana pero siempre x > y
///////////////////////////////////////////////////////////////
      if(x[0] < x[1] && x[2] < x[3] && x[4] < x[5]) {
         // Actualiza la dimencion de la pantalla
         Dim_minima[tp][Sistema_actual].x = x[0], Dim_maxima[tp][Sistema_actual].x = x[1], Dim_minima[tp][Sistema_actual].y = x[2], Dim_maxima[tp][Sistema_actual].y = x[3], Dim_minima[tp][Sistema_actual].z = x[4],Dim_maxima[tp][Sistema_actual].z = x[5];
         // Actualiza la dimencion de la ventana (Todos)
         if(x[6]) Dim_minima[tp][Sistema_actual].x = -x[6], Dim_maxima[tp][Sistema_actual].x = x[6], Dim_minima[tp][Sistema_actual].y = -x[6], Dim_maxima[tp][Sistema_actual].y = x[6], Dim_minima[tp][Sistema_actual].z = -x[6], Dim_maxima[tp][Sistema_actual].z = x[6];
         // Actualiza la dimencion de la ventana (Factor)
         if(x[7]) Dim_minima[tp][Sistema_actual].x *= x[7], Dim_maxima[tp][Sistema_actual].x *= x[7], Dim_minima[tp][Sistema_actual].y *= x[7], Dim_maxima[tp][Sistema_actual].y *= x[7], Dim_minima[tp][Sistema_actual].z *= x[7], Dim_maxima[tp][Sistema_actual].z *= x[7];
         // Valida la dimencion de la ventana
         if(Dim_maxima[tp][Sistema_actual].x < 0) Dim_maxima[tp][Sistema_actual].x = 0;
         if(Dim_maxima[tp][Sistema_actual].y < 0) Dim_maxima[tp][Sistema_actual].y = 0;
         if(Dim_maxima[tp][Sistema_actual].z < 0) Dim_maxima[tp][Sistema_actual].z = 0;
         if(Dim_minima[tp][Sistema_actual].x > 0) Dim_minima[tp][Sistema_actual].x = 0;
         if(Dim_minima[tp][Sistema_actual].y > 0) Dim_minima[tp][Sistema_actual].y = 0;
         if(Dim_minima[tp][Sistema_actual].z > 0) Dim_minima[tp][Sistema_actual].z = 0;

         // Revisa si la configuracion sera para ambas ventanas
         if(Doble_ventana) {
            if(Opcion("¨Desea utilizar est  configuraci¢n","en ambas ventanas?") == 1)  {
               Dim_maxima[!tp][Sistema_actual].x = Dim_maxima[tp][Sistema_actual].x;
               Dim_maxima[!tp][Sistema_actual].y = Dim_maxima[tp][Sistema_actual].y;
               Dim_maxima[!tp][Sistema_actual].z = Dim_maxima[tp][Sistema_actual].z;
               Dim_minima[!tp][Sistema_actual].x = Dim_minima[tp][Sistema_actual].x;
               Dim_minima[!tp][Sistema_actual].y = Dim_minima[tp][Sistema_actual].y;
               Dim_minima[!tp][Sistema_actual].z = Dim_minima[tp][Sistema_actual].z;
            }
         }

         // Redimensiona todo aquello que esta relacionado con las dimenciones de la ventana
         Redimensiona(Sistema_actual);
      } else NCO->Mensaje("",TXT18,TXT19);
   }
   return st;
}



///////////////////////////////////////////////////////////////////
//                        CONTROLA EL CURSOR                     //
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
//                Controla el movimiento del cursor              //
///////////////////////////////////////////////////////////////////

void Base_integra::Controla_cursor(const int tecla)
{
   int i;
   aux[0] = 0;
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]+1; i++) aux[i] = 0.0;
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = Ac_Condicion[Ventana_activa][Sistema_actual][i];
   aux[i+1] = Ac_Tiempo[Ventana_activa][Sistema_actual];
   aux1.x = aux[Escenario[Ventana_activa][Sistema_actual].x];
   aux1.y = aux[Escenario[Ventana_activa][Sistema_actual].y];
   aux1.z = aux[Escenario[Ventana_activa][Sistema_actual].z];

   switch(tecla) {
      case 1:
         Paso_cursor[Ventana_activa][Sistema_actual].x += Incremento_paso[Ventana_activa][Sistema_actual].x; 
         Paso_cursor[Ventana_activa][Sistema_actual].y += Incremento_paso[Ventana_activa][Sistema_actual].y; 
         Paso_cursor[Ventana_activa][Sistema_actual].z += Incremento_paso[Ventana_activa][Sistema_actual].z;
         break;
      case 2:
         Paso_cursor[Ventana_activa][Sistema_actual].x -= Incremento_paso[Ventana_activa][Sistema_actual].x;
         Paso_cursor[Ventana_activa][Sistema_actual].y -= Incremento_paso[Ventana_activa][Sistema_actual].y;
         Paso_cursor[Ventana_activa][Sistema_actual].z -= Incremento_paso[Ventana_activa][Sistema_actual].z;
         if(Paso_cursor[Ventana_activa][Sistema_actual].x <= 0.0) Paso_cursor[Ventana_activa][Sistema_actual].x = Incremento_paso[Ventana_activa][Sistema_actual].x;
         if(Paso_cursor[Ventana_activa][Sistema_actual].y <= 0.0) Paso_cursor[Ventana_activa][Sistema_actual].y = Incremento_paso[Ventana_activa][Sistema_actual].y;
         if(Paso_cursor[Ventana_activa][Sistema_actual].z <= 0.0) Paso_cursor[Ventana_activa][Sistema_actual].z = Incremento_paso[Ventana_activa][Sistema_actual].z;
         break;
      case FLECHA_DERECHA: // Mueve el cursor positivamente en el eje horizontal
         aux1.x += Paso_cursor[Ventana_activa][Sistema_actual].x;
         aux2.x += Paso_cursor[Ventana_activa][Sistema_actual].x;
         break;
      case FLECHA_IZQUIERDA: // Mueve el cursor negativamente en el eje horizontal
         aux1.x -= Paso_cursor[Ventana_activa][Sistema_actual].x;
         aux2.x -= Paso_cursor[Ventana_activa][Sistema_actual].x;
         break;
      case FLECHA_ARRIBA: // Mueve el cursor positivamente en el eje vertical
         aux1.y += Paso_cursor[Ventana_activa][Sistema_actual].y;
         aux2.y += Paso_cursor[Ventana_activa][Sistema_actual].y;
         break;
      case FLECHA_ABAJO: // Mueve el cursor negativamente en el eje vertical
         aux1.y -= Paso_cursor[Ventana_activa][Sistema_actual].y;
         aux2.y -= Paso_cursor[Ventana_activa][Sistema_actual].y;
         break;
      case PGDN: // Mueve el cursor positivamente en el tercer eje
         aux1.z += Paso_cursor[Ventana_activa][Sistema_actual].z;
         aux2.z += Paso_cursor[Ventana_activa][Sistema_actual].z;
         break;
      case PGUP: // Mueve el cursor negativamente en el tercer eje 
         aux1.z -= Paso_cursor[Ventana_activa][Sistema_actual].z;
         aux2.z -= Paso_cursor[Ventana_activa][Sistema_actual].z;
   }
   if(Ventana_activa) {
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]+1; i++)  aux[i] = 0.0;
      aux[Escenario[Ventana_activa][Sistema_actual].x] = aux1.x;
      aux[Escenario[Ventana_activa][Sistema_actual].y] = aux1.y;
      aux[Escenario[Ventana_activa][Sistema_actual].z] = aux1.z;
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++)  Ac_Condicion[Ventana_activa][Sistema_actual][i] = aux[i+1];
      Ac_Tiempo[Ventana_activa][Sistema_actual] = aux[i+1];
   }

   if(!Ventana_activa || Graficar_dos_ventanas == 4) {
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]+1; i++)  aux[i] = 0.0;
      aux[Escenario[Ventana_activa][Sistema_actual].x] = aux1.x;
      aux[Escenario[Ventana_activa][Sistema_actual].y] = aux1.y;
      aux[Escenario[Ventana_activa][Sistema_actual].z] = aux1.z;
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++)  Ac_Condicion[Ventana_activa][Sistema_actual][i] = aux[i+1];
      Ac_Tiempo[Ventana_activa][Sistema_actual] = aux[i+1];

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++)  Ac_Condicion[!Ventana_activa][Sistema_actual][i] = Ac_Condicion[Ventana_activa][Sistema_actual][i];
      Ac_Tiempo[!Ventana_activa][Sistema_actual] = Ac_Tiempo[Ventana_activa][Sistema_actual];
   }
   Actualiza_coordenadas();
}



///////////////////////////////////////////////////////////////////
//                       EJES COOREDENADOS                       //
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//     Selecciona el color de los ejes coordenados la ventana    //
///////////////////////////////////////////////////////////////////

void Base_integra::Selecciona_colores(void)
{
   Ventana_iconos *Vt = new Ventana_iconos;
   unsigned int x1,x2,y1,y2,st = 0;
   C_2I vent;
   Vt->Activa_graba_ventana(1);
   Vt->Ventana_centrada(TXT20,460,230,0);
   Vt->Dibuja();
   Vt->Actual_pos_ventana(x1,y1,x2,y2);
   Vt->Define_color_texto(Negro);
   Vt->Limpia_recuadro(15,50,200,200,Gris1,Blanco,Gris9);
   Vt->Limpia_recuadro(210,50,390,200,Gris1,Blanco,Gris9);
   // visualiza el nombre de los metodos numericos
   Vt->Visualiza_texto(20,40,TXT21);
   Vt->Visualiza_texto(20,60,TXT13);
   Vt->Visualiza_texto(20,105,TXT14);
   Vt->Visualiza_texto(20,150,TXT15);
   Vt->Visualiza_texto(220,60,TXT22);
   Vt->Visualiza_texto(220,105,TXT26);
   Vt->Visualiza_texto(220,115,TXT27);

   vent.x = x1 + 20, vent.y = y1 + 75;
   Combo_box *Co1 = new Combo_box;
   Co1->Parametros(Nombre_colores,vent,Color_ejes[Sistema_actual].x+1,"");
   Co1->Dibuja(Nombre_colores);

   vent.y = y1 + 120;
   Combo_box *Co2 = new Combo_box;
   Co2->Parametros(Nombre_colores,vent,Color_ejes[Sistema_actual].y+1,"");
   Co2->Dibuja(Nombre_colores);

   vent.y = y1 + 165;
   Combo_box *Co3 = new Combo_box;
   Co3->Parametros(Nombre_colores,vent,Color_ejes[Sistema_actual].z+1,"");
   Co3->Dibuja(Nombre_colores);

   vent.y = y1 + 75, vent.x = x1 + 220;
   Combo_box *Co4 = new Combo_box;
   Co4->Parametros(Nombre_colores,vent,Color_grafica[Sistema_actual]+1,""); 
   Co4->Dibuja(Nombre_colores);

   vent.y = y1 + 130, vent.x = x1 + 220;
   Combo_box *Co5 = new Combo_box;
   Co5->Parametros(Nombre_colores,vent,Color_grafica_aux[Sistema_actual]+1,""); 
   Co5->Dibuja(Nombre_colores);

   Icono *Ic1 = new Icono;
   Icono *Ic2 = new Icono;
   Ic1->Define_icono("OK.ICO",x2-50,y1+70);
   Ic1->Dibuja();
   Ic2->Define_icono("CANCEL.ICO",x2-50,y1+130);
   Ic2->Dibuja();

   while(!st) {
      NCO->Administrador();
      if(Vt->Itera() || !Programa_activo || Tecla == ESC) break;
      Co1->Controla(Tecla,Nombre_colores);
      Co2->Controla(Tecla,Nombre_colores);
      Co3->Controla(Tecla,Nombre_colores);
      Co4->Controla(Tecla,Nombre_colores);
      Co5->Controla(Tecla,Nombre_colores);
      if(Ic1->Oprimido()) st = 1;
      if(Ic2->Oprimido()) st = 2;
   }                    
   if(st == 1) {
      Color_ejes[Sistema_actual].x = Co1->Retorna_opcion()-1;
      Color_ejes[Sistema_actual].y = Co2->Retorna_opcion()-1;
      Color_ejes[Sistema_actual].z = Co3->Retorna_opcion()-1;
      Color_grafica[Sistema_actual] = Co4->Retorna_opcion()-1;
      Color_grafica_aux[Sistema_actual] = Co5->Retorna_opcion()-1;
      C_3I colores;
      colores.x = Colores[Color_ejes[Sistema_actual].x];
      colores.y = Colores[Color_ejes[Sistema_actual].y];
      colores.z = Colores[Color_ejes[Sistema_actual].z];
      VT1->VT1->Fija_color_ejes(colores);
   }
   delete Co1;
   delete Co2;
   delete Co3;
   delete Co4;
   delete Co5;
   delete Ic1;
   delete Ic2;
   delete Vt;
}



///////////////////////////////////////////////////////////////////
//    RUTINAS DE VISUALIZACION DENTRO DE LA VENTANA DE TRABAJO   //
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//                   Dibuja la ventana de trabajo                //
///////////////////////////////////////////////////////////////////

void Base_integra::Dibuja_ventana_trabajo(void) 
{
   int sw = 0;
   // Define titilo de la ventana de trabajo
   VT1->VT1->Define_titulo(Nombre_sistema[Sistema_actual]);
   if(VT1->Estado_ventana_doble()) VT1->VT2->Define_titulo(Nombre_sistema[Sistema_actual]);

   // Redibuja el titulo de las ventanas
   VT1->Fija_ventana_activa(Ventana_activa);

   // Fija proyeccion actual
   if(Escenario[0][Sistema_actual].z == 0) sw = 1;  // XY
   VT1->VT1->Fija_proyeccion_actual(sw);
   sw = 0;
   if(Escenario[1][Sistema_actual].z == 0) sw = 1;  // XY
   if(VT1->Estado_ventana_doble()) VT1->VT2->Fija_proyeccion_actual(sw);

   // Nombre de ejes coordenados
   char xejex[100];
   char xejey[100];
   char xejez[100];
   Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[0][Sistema_actual].x -1,xejex);
   Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[0][Sistema_actual].y -1,xejey);
   Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[0][Sistema_actual].z -1,xejez);
   VT1->VT1->Fija_nombre_ejes(xejex,xejey,xejez);
   if(VT1->Estado_ventana_doble()) {
      Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[1][Sistema_actual].x -1,xejex);
      Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[1][Sistema_actual].y -1,xejey);
      Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[1][Sistema_actual].z -1,xejez);
      VT1->VT2->Fija_nombre_ejes(xejex,xejey,xejez);
   }
   // Colores de ejes coordenados
   C_3I colores;
   colores.x = Colores[Color_ejes[Sistema_actual].x];
   colores.y = Colores[Color_ejes[Sistema_actual].y];
   colores.z = Colores[Color_ejes[Sistema_actual].z];
   VT1->VT1->Fija_color_ejes(colores);
   if(VT1->Estado_ventana_doble()) VT1->VT2->Fija_color_ejes(colores);
   // Dibuja la ventana de trabajo
   VT1->VT1->Dibuja();
   if(VT1->Estado_ventana_doble()) VT1->VT2->Dibuja();
   v_coord = SI;
   Guarda_posicion = NO;
   Actualiza_coordenadas();
}


#include <math.h>

///////////////////////////////////////////////////////////////////
//         Dibuja un punto dentro de la ventana de trabajo       //
///////////////////////////////////////////////////////////////////

void Base_integra::Dibuja_punto(const double ini[], const double t, const int vtn, const char col, const int tp)
{ 
   int i, sw = 1;
   aux[0] = 0;
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = ini[i];
   aux[i+1] = t;
   char col1 = col == 255 ? Colores[Color_grafica[Sistema_actual]] : col;

   if(vtn == 0 || vtn == 3 || vtn == 4) {
      if(vtn == 3 && Ventana_activa) sw = 0;
      if(sw) {
         aux1.x = aux[Escenario[0][Sistema_actual].x];
         aux1.y = aux[Escenario[0][Sistema_actual].y];
         aux1.z = aux[Escenario[0][Sistema_actual].z];
         // Revisa si grafica en coordenadas polares
         if(!Coord_rectangulares && !Escenario[0][Sistema_actual].z) VT1->VT1->Fija_tipo_coordenadas(0);
         VT1->VT1->Dibuja_punto(aux1,col1,tp);
         VT1->VT1->Fija_tipo_coordenadas(1);
      }
   } 

   if(vtn == 1 || vtn == 3 || vtn == 4) {
      if(Doble_ventana) sw = 1;
       else sw = 0;
      if(vtn == 3 && !Ventana_activa) sw = 0;
      if(sw) {
         aux1.x = aux[Escenario[1][Sistema_actual].x];
         aux1.y = aux[Escenario[1][Sistema_actual].y];
         aux1.z = aux[Escenario[1][Sistema_actual].z];
         if(!Coord_rectangulares && !Escenario[1][Sistema_actual].z) VT1->VT2->Fija_tipo_coordenadas(0);
         VT1->VT2->Dibuja_punto(aux1,col1,tp);
         VT1->VT2->Fija_tipo_coordenadas(1);
      }
   }
}


///////////////////////////////////////////////////////////////////
//        Dibuja una linea dentro de la ventana de trabajo       //
///////////////////////////////////////////////////////////////////

void Base_integra::Dibuja_linea(const double ini[], const double fin[], const double t_ini, const double t_fin, const int vtn, const char col, const int tp)
{
   int i, sw = 1;
   aux[0] = 0;
   char col1 = col == 255 ? Colores[Color_grafica[Sistema_actual]] : col;
   if(vtn == 0 || vtn == 3 || vtn == 4) {
      if(vtn == 3 && Ventana_activa) sw = 0;
      if(sw) {
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = ini[i];
         aux[i+1] = t_ini;
         aux1.x = aux[Escenario[0][Sistema_actual].x];
         aux1.y = aux[Escenario[0][Sistema_actual].y];
         aux1.z = aux[Escenario[0][Sistema_actual].z];
         aux[0] = 0;
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = fin[i];
         aux[i+1] = t_fin;
         aux2.x = aux[Escenario[0][Sistema_actual].x];
         aux2.y = aux[Escenario[0][Sistema_actual].y];
         aux2.z = aux[Escenario[0][Sistema_actual].z];
         // Revisa si grafica en coordenadas polares
         if(!Coord_rectangulares && !Escenario[0][Sistema_actual].z) VT1->VT1->Fija_tipo_coordenadas(0);
         VT1->VT1->Dibuja_linea(aux1,aux2,col1,tp);
         VT1->VT1->Fija_tipo_coordenadas(1);
      }
   }

   if(vtn == 1 || vtn == 3 || vtn == 4) {
      if(Doble_ventana) sw = 1;
       else sw = 0;
      if(vtn == 3 && !Ventana_activa) sw = 0;
      if(sw) {
         aux[0] = 0;
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = ini[i];
         aux[i+1] = t_ini;
         aux1.x = aux[Escenario[1][Sistema_actual].x];
         aux1.y = aux[Escenario[1][Sistema_actual].y];
         aux1.z = aux[Escenario[1][Sistema_actual].z];
         aux[0] = 0;
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = fin[i];
         aux[i+1] = t_fin;
         aux2.x = aux[Escenario[1][Sistema_actual].x];
         aux2.y = aux[Escenario[1][Sistema_actual].y];
         aux2.z = aux[Escenario[1][Sistema_actual].z];
         // Revisa si grafica en coordenadas polares
         if(!Coord_rectangulares && !Escenario[1][Sistema_actual].z) VT1->VT2->Fija_tipo_coordenadas(0);
         VT1->VT2->Dibuja_linea(aux1,aux2,col1,tp);
         VT1->VT2->Fija_tipo_coordenadas(1);
      }
   }
}


void Base_integra::Dibuja_punto_ventana(const C_3D ini, const int vtn, const char col, const int tp)
{ 
   int sw = 1;
   char col1 = col == 255 ? Colores[Color_grafica[Sistema_actual]] : col;

   if(vtn == 0 || vtn == 3 || vtn == 4) {
      if(vtn == 3 && Ventana_activa) sw = 0;
      if(sw) {
         // Revisa si grafica en coordenadas polares
         if(!Coord_rectangulares && !Escenario[0][Sistema_actual].z) VT1->VT1->Fija_tipo_coordenadas(0);
         VT1->VT1->Dibuja_punto(ini,col1,tp);
         VT1->VT1->Fija_tipo_coordenadas(1);
      }
   } 

   if(vtn == 1 || vtn == 3 || vtn == 4) {
      if(Doble_ventana) sw = 1;
       else sw = 0;
      if(vtn == 3 && !Ventana_activa) sw = 0;
      if(sw) {
         if(!Coord_rectangulares && !Escenario[1][Sistema_actual].z) VT1->VT2->Fija_tipo_coordenadas(0);
         VT1->VT2->Dibuja_punto(ini,col1,tp);
         VT1->VT2->Fija_tipo_coordenadas(1);
      }
   }
}


///////////////////////////////////////////////////////////////////
//        Dibuja una linea dentro de la ventana de trabajo       //
///////////////////////////////////////////////////////////////////

void Base_integra::Dibuja_linea_ventana(const C_3D ini, const C_3D fin, const int vtn, const char col, const int tp)
{
   int sw = 1;
   char col1 = col == 255 ? Colores[Color_grafica[Sistema_actual]] : col;
   if(vtn == 0 || vtn == 3 || vtn == 4) {
      if(vtn == 3 && Ventana_activa) sw = 0;
      if(sw) {
         // Revisa si grafica en coordenadas polares
         if(!Coord_rectangulares && !Escenario[0][Sistema_actual].z) VT1->VT1->Fija_tipo_coordenadas(0);
         VT1->VT1->Dibuja_linea(ini,fin,col1,tp);
         VT1->VT1->Fija_tipo_coordenadas(1);
      }
   }

   if(vtn == 1 || vtn == 3 || vtn == 4) {
      if(Doble_ventana) sw = 1;
       else sw = 0;
      if(vtn == 3 && !Ventana_activa) sw = 0;
      if(sw) {
         // Revisa si grafica en coordenadas polares
         if(!Coord_rectangulares && !Escenario[1][Sistema_actual].z) VT1->VT2->Fija_tipo_coordenadas(0);
         VT1->VT2->Dibuja_linea(ini,fin,col1,tp);
         VT1->VT2->Fija_tipo_coordenadas(1);
      }
   }
}




///////////////////////////////////////////////////////////////////
//       Actualiza coordenadas dentro del area de trabajo        //
///////////////////////////////////////////////////////////////////

void Base_integra::Actualiza_coordenadas(void)
{
   // Actualiza las coordenadas en la ventana de trabajo
   unsigned int i;
   if(VT1->VT2->Retorna_ventana_activa() || (Doble_ventana && Graficar_dos_ventanas == 4)) {
      aux[0] = 0;
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = Ac_Condicion[1][Sistema_actual][i];
      aux[i+1] = Ac_Tiempo[1][Sistema_actual];
      aux1.x = aux[Escenario[1][Sistema_actual].x];
      aux1.y = aux[Escenario[1][Sistema_actual].y];
      aux1.z = aux[Escenario[1][Sistema_actual].z];
      VT1->VT2->Fija_coordenadas(aux1);
   } 
   if(VT1->VT1->Retorna_ventana_activa() || (Doble_ventana && Graficar_dos_ventanas == 4)) {
      aux[0] = 0;
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = Ac_Condicion[0][Sistema_actual][i];
      aux[i+1] = Ac_Tiempo[0][Sistema_actual];
      aux1.x = aux[Escenario[0][Sistema_actual].x];
      aux1.y = aux[Escenario[0][Sistema_actual].y];
      aux1.z = aux[Escenario[0][Sistema_actual].z];
      VT1->VT1->Fija_coordenadas(aux1);
   }
}

