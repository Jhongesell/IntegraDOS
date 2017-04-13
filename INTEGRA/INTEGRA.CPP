//////////////////////////////////////////////////////////////
//                                                          //
//              I N T E G R A    V E R. 1.0                 //
//       (Analisador de sistemas din micos interativos)     //
//                                                          //
// Desarrollado para ambiente DOS en modo de video          //
// VGA y SVGA a 256 colores.                                //
//                                                          //
// Autores:                                                 //
//                                                          //
//     Humberto Carrillo Calvet                             //
//     Antonio Carrillo Ledesma                             //
//     Luis A. Nava Fernandez                               //
//                                                          //
// Desarrollado en:                                         //
//                                                          //
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



#include "integra.hpp"
#include "nucleo.hpp"
#include "selecc.hpp"
#include "combo_b.hpp"
#include "version.hpp"
#include "\libreria\gen_rep.hpp"

extern "C" {
   #include <graphics.h>
   #include <stdio.h>
   #include <math.h>
   #include <dir.h>
}



// Definicion externa del objeto Nucleo
extern Nucleo *NCO;
// Indica si el programa esta o no activo
extern int Programa_activo;
// Tecla actual del bufer
extern int Tecla;
// Caracter actual del bufer
extern char Caracter;
// Dimenciones de la pantalla
extern int X_MAX;
extern int Y_MAX;
// Indica el archivo que se mostrara de ayuda
extern char *ARCHIVO_AYUDA;       
// Indica cual texto explicativo a visualizar
extern unsigned int N_texto_explicativo;
// Metodos de Intebgracion numerica
extern const char *Metodos_integracion[];
// Nombre de los parametros de los metodos numericos
extern const char *Parametros_metodos_integracion[];


// Graba la imagen en formato GIF
void GIF_DumpEga10(char *filename);



// Nombre del archivo temporal para la ultima orbita
static const char *ARCH_TEMPORAL = "TMPVENT.$$$";
static const char *ARCH_CONFIG_TEMP = "TEMP.CFG";
// Nombre del directorio que contiene la descripcion del proyecto y sistemas
static const char *ARCH_DESCRIP = "C:\\INTEGRA\\DESCRIP\\";



#ifdef _IDIOMA_ESPANOL_

   // Menu de ayuda
   const char *M_AYUDA_MENU[] = {
      "Que es INTEGRA",
      "Manejo de INTEGRA",
      "Funciones Gen‚ricas",
      "Funciones Validas",
      "Acerca de...",
      "0"
   };
   const char *M_AYUDA_MENU_TEC = "QMFUA";
   // Archivos de ayuda
   const char *M_AYUDA_ARCHIVO[] = {
      "C:\\INTEGRA\\SYS\\HELP\\INTEGRA.HLP",
      "C:\\INTEGRA\\SYS\\HELP\\FINTEGRA.HLP",
      "C:\\INTEGRA\\SYS\\HELP\\AYUDAG.HLP",
      "C:\\INTEGRA\\SYS\\HELP\\FUNCIONE.HLP",
      "C:\\INTEGRA\\SYS\\HELP\\ACERCADE.HLP",
   };



   // Menu de archivos
   static const char *M_archivos[] = {
      "Reporte Especificaciones",
      "Graba Especificaciones",
      "Lee Especificaciones",
      "Restaura Especificaciones",
      "Descripci¢n Proyecto",
      "Graba Pantalla (GIF)",
      "Visualiza Archivo",
      "Crea / Edita Archivo",
      "Acerca de ...",
      "Termina Programa",
      "0"
   };
   #define OPC03     "GLORDPEVAT"


   // Menu de sistemas
   static const char *M_sistemas[] = {
      "Biblioteca de Sistemas",
      "Descripci¢n del Sistema",
      "Reporte Especificaciones",
      "0"
   };
   #define OPC04     "SDVR"

   // Menu de parametros
   static const char *M_parametros[] = {
      "Modifica Par metros",
      "Par metros Originales",
      "0"
   };
   #define OPC05     "PO"

   // Menu del cursor (condiciones iniciales)
   static const char *M_cursor[] = {
      "Mover a ...",
      "Mover al Origen",
      "Paso del Cursor",
      "Incremento del Paso",
      "",
      "0"
   };
   #define OPC06     "MOPIC"

   // Menu de la pantalla de trabajo
   static const char *M_ventana[] = {
      "Dimensiones",
      "Limpiar Sin Consv. Posc.",
      "Limpiar Consv. Posc.",
      "Grabar (IMG)",
      "Leer   (IMG)",
      "",              // Una o dos ventanas
      "",              // Grafica en una o en ambas ventanas simultaneamente
      "",              // Invierte ventanas
      "0"
   };
   #define OPC07     "DSCGLVTI"

   // Menu del metodo grafico
   static const char *M_grafico[] = {
      "Escenario Gr fico",
      "Mover Origen",
      "Graduar Ejes",
      "Acercar / Alejar",
      "Rotaciones",
      "",
      "Ceroclinas",            
      "Isoclinas",            
      "Funciones Auxiliares",  
      "Colores del Sistema",
      "0"
   };
   #define OPC08     "EMHARNCIFO"

   // Menu del campo vectorial
   static const char *M_campo_vectorial[] = {
      "Configurar",
      "Dibujar",
      "0"
   };
   #define OPC09     "CD"

   // Metodos numericos
   static const char *M_Numerico[] = {
      "Configurar",
      "0"
   };
   #define OPC10     "C"

///////////////////////////////////////////
// Revisar los elementos del menu
///////////////////////////////////////////
   // Menu de la ventana de visualizacion
   static const char *M_ventana_mouse[] = {
      "Maximizar",           
      "Minimizar",           
      "Grabar (IMG)",
      "Leer   (IMG)",
      "Limpiar",
      "0"
   };
   #define OPC01    "XNGLI"  // Opciones



   #define MSG01    "Terminar el programa y retornar al DOS"
   #define MSG02    "Visualiza la ayuda sobre el sistema"
   #define MSG03    "Selecciona un sistema de la biblioteca de sistemas"
   #define MSG04    "Cambia el valor de los par metros del sistema actual"
   #define MSG05    "Activa la proyecci¢n XY"
   #define MSG06    "Cambia dimensi¢n de la ventana de trabajo"
   #define MSG07    "Configura el m‚todo num‚rico de integraci¢n"
   #define MSG08    "Selecciona el escenario gr fico"
   #define MSG09    "Dibuja el campo vectorial"
   #define MSG10    "Selecciona las condiciones iniciales (Cursor)"
   #define MSG11    "Integra en tiempo positivo"
   #define MSG12    "Integra en tiempo negativo"
   #define MSG13    "Limpia la ventana de trabajo"
   #define MSG14    "Cambia la gr fica de puntos a l¡neas y viceversa"

   #define TXT2     "Escriba:"
   #define TXT3     "Donde N es (3) SVGA 1024x768 256 colores (*)"
   #define TXT4     "           (2) SVGA  800x600 256 colores (*)"
   #define TXT5     "           (1) SVGA  640x480 256 colores (*)"
   #define TXT6     "           (0) VGA   640x480  16 colores"
   #define TXT7     " (*) Corra primero el programa HGXMOUSE.EXE"


   #define TXT01    "Acciones sobre la ventana de trabajo"
   #define TXT04    "Men£ de archivos"
   #define TXT05    "Men£ de Sistemas"
   #define TXT06    "Men£ de Par metros"
   #define TXT07    "Menu de Cursor"
   #define TXT08    "Ver Coordenadas"
   #define TXT09    "No ver Coordenadas"
   #define TXT10    "Men£ de la ventana de trabajo"
   #define TXT11    "Men£ de escenarios gr ficos"
   #define TXT12    "Ejes con Nombre"
   #define TXT13    "Ejes sin Nombre"
   #define TXT14    "Men£ del campo vectorial"
   #define TXT15    "Men£ de los m‚todos n£mericos"
   #define TXT16    "Archivo  Sistemas  Param.  Cursor  Ventana   Escenarios  C. Vect.  M. Numer."
   #define TXT17    "%s = %3.5f  Paso = %3.5f  %s = %3.5f  Paso = %3.5f  %s = %3.5f  Paso = %3.5f"
   #define TXT17A   "%s = %3.5f  Paso = %3.5f  %s = %3.5f  Paso = %3.5f"
   #define TXT18    "Visualizando la descripci¢n del proyecto"
   #define TXT19    "DESCRIPCION DEL PROYECTO"
   #define TXT20    "Nombre del archivo para grabar la pantalla en formato GIF"
   #define TXT21    "GRABA PANTALLA"
   #define TXT22    "Seleccione el archivo a editar"
   #define TXT23    "EDITOR"
   #define TXT24    "Seleccione el archivo a visualizar"
   #define TXT25    "VISUALIZADOR"
   #define TXT26    "Visualizando la descripci¢n del sistema"
   #define TXT27    "Nombre del archivo para grabar las especificaciones"
   #define TXT28    "Nombre del archivo para leer las especificaciones"
   #define TXT29    "Reporte del estado del sistema"
   #define TXT30    "Capture la condici¢n inicial"
   #define TXT31    "MOVER A ..."
   #define TXT32    "Eje Horizontal"
   #define TXT33    "Eje Vertical"
   #define TXT34    "Tercer Eje"
   #define TXT35    "Capture el paso del cursor"
   #define TXT36    "PASO DEL CURSOR"
   #define TXT37    "Capture el incremento del paso del cursor"
   #define TXT38    "INCREMENTO DEL PASO"
   #define TXT39    "Capture las dimensiones de la ventana de trabajo"
   #define TXT40    "Seleccione el escenario a gr ficar"
   #define TXT41    "Utilice las flechas para mover el origen, Esc para terminar"
   #define TXT42    "Utilice [+,-] para acercar / alejar, Esc para terminar"
   #define TXT43    "Utilice las flechas para rotar, Esc para terminar"
   #define TXT44    "Seleccione los colores a usar en el sistema"
   #define TXT45    "Configure el campo vectorial"
   #define TXT46    "Seleccione y configure el m‚todo n£merico de integraci¢n"
   #define TXT48    "GRABA IMAGEN"
   #define TXT49    "LEE IMAGEN"
   #define TXT50    "Escriba la expresi¢n a gr ficar"
   #define TXT51    "Grabando la ventana de trabajo ..."
   #define TXT52    "Leyendo la ventana de trabajo ..."
   #define TXT60    "Se encontro el archivo de estado del proyecto"
   #define TXT61    "¨Desea trabajar con esa configuraci¢n?"
   #define TXT70    "Capture el n£mero de puntos a graficar por la funci¢n auxiliar"
   #define TXT71    "CAPTURA PUNTOS A GRAFICAR"
   #define TXT72    "NUMERO DE PUNTOS"
   #define TXT73    "Gr ficando isoclinas presione [ESC] para cancelar"
   #define TXT80    "Editando el archivo de notas del proyecto"
   #define TXT81    "ARCHIVO DE NOTAS"
   #define TXT90    "CURSO TEMPORAL"
   #define TXT100   "¨Desea activar graba calculo n£merico?"
   #define TXT101   "¨Desea desactivar graba calculo n£merico?"
   
#else

#endif






////////////////////////////////////////////////////////////////////////
//                  Proceso inicial del sistena integra               //
////////////////////////////////////////////////////////////////////////

void Integra::Proceso_inicial(void)
{
   // Indica que las ventanas no han sido invertidas
   Inversion_ventanas = 0;
   // Define los iconos de Integra
   Niconos = 12;
   Iconos = new Icono [Niconos];
   Iconos[0].Define_icono("SISTEMA.ICO",10,43);
   Iconos[1].Define_icono("PARAMET.ICO",55,43);
   Iconos[2].Define_icono("EJE_XY.ICO",100,43);
   Iconos[3].Define_icono("PANTALL.ICO",145,43);
   Iconos[4].Define_icono("MET_INT.ICO",190,43);
   Iconos[5].Define_icono("EJE_GRF.ICO",235,43);
   Iconos[6].Define_icono("CAM_VEC.ICO",280,43);
   Iconos[7].Define_icono("CURSOR.ICO",325,43);
   Iconos[8].Define_icono("INT_POS.ICO",400,43);
   Iconos[9].Define_icono("INT_NEG.ICO",445,43);
   Iconos[10].Define_icono("LIMPIA.ICO",520,43);
   Iconos[11].Define_icono("PUN_LIN.ICO",565,43);


   Coordenadas = new char[200];
   Texto_explicativo = new char *[16];
   Texto_explicativo[0] = "";
   Texto_explicativo[1] = MSG01;
   Texto_explicativo[2] = MSG02;
   Texto_explicativo[3] = MSG03;
   Texto_explicativo[4] = MSG04;
   Texto_explicativo[5] = MSG05;
   Texto_explicativo[6] = MSG06;
   Texto_explicativo[7] = MSG07;
   Texto_explicativo[8] = MSG08;
   Texto_explicativo[9] = MSG09;
   Texto_explicativo[10] = MSG10;
   Texto_explicativo[11] = MSG11;
   Texto_explicativo[12] = MSG12;
   Texto_explicativo[13] = MSG13;
   Texto_explicativo[14] = MSG14;

   Ipresionado = 0;
   v_coord = 1;

   // Inicializa las variables del sistema
   Inicializa_variables();

   Doble_ventana  = 1;           // Dos ventanas visibles
   Estado_ventana = 2;           // Ventana minimizadas
   Ventana_activa = 0;           // Primera ventana
   Graficar_dos_ventanas = 4;    // Grafica simultaneamente en las dos ventanas
                                 // (1) Ventana uno solamente
                                 // (2) Ventana dos solamente
                                 // (3) Solo en la ventana activa
                                 // (4) Simultaneamente en ambas ventanas
                                 

   // Inicializa la clase de la ventana de graficacion
   VT1 = new Ctrl_ventana_grafica;
   VT1->Inicializa(Doble_ventana,Nombre_sistema[Sistema_actual],TXT90);

   // Indica que la ventana principal no esta activa
   Vt->Ventana_activa(NO);
   // Dibuja la pantalla de trabajo
   Dibuja_aplicacion();

   // Carga la configuracion de la ventana de trabajo para cada sistema
   for(unsigned int i = 0; i < Numero_sistemas; i++) {
       VT1->VT1->Fija_dimenciones(Dim_maxima[0][i],Dim_minima[0][i]);
       VT1->VT1->Retorna_configuracion(Escala[0][i],Centro[0][i],Angulos[0][i][0],Angulos[0][i][1]);
       VT1->VT2->Fija_dimenciones(Dim_maxima[1][i],Dim_minima[1][i]);
       VT1->VT2->Retorna_configuracion(Escala[1][i],Centro[1][i],Angulos[1][i][0],Angulos[1][i][1]);
   }
   // Graba la configuracion original del proyecto
   Graba_estado(0,ARCH_CONFIG_TEMP);

   // Lee el archivo de configuracion si existe
   char arch[MAXPATH];
   Manipulador_archivos ma;
   ma.Cambia_ext_path(Proyecto,"CFG",arch);
   if(n_archivos_directorios(arch,FA_ARCH)) {
      if(NCO->Opcion(TXT60,TXT61) == 1) Lee_estado(arch,0);
   }
   // Actualiza los valores del la ventana al sistema actual
   VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
   VT1->VT1->Fija_configuracion(Escala[0][Sistema_actual],Centro[0][Sistema_actual],Angulos[0][Sistema_actual][0],Angulos[0][Sistema_actual][1]);
   VT1->VT2->Fija_dimenciones(Dim_maxima[1][Sistema_actual],Dim_minima[1][Sistema_actual]);
   VT1->VT2->Fija_configuracion(Escala[1][Sistema_actual],Centro[1][Sistema_actual],Angulos[1][Sistema_actual][0],Angulos[1][Sistema_actual][1]);
	Dibuja_ventana_trabajo();

   // Inicializa el reporte de integracion numerica
   Rep_num = new Ctrl_reporte;
   Rep_num->Parametros(0,66,300,REPORTE_GRABADO,LPT1,"REPINT.REP");
}


////////////////////////////////////////////////////////////////////////
//                     Proceso final de integra                       //
////////////////////////////////////////////////////////////////////////

void Integra::Proceso_final(void)
{
   delete Rep_num;
	unlink(ARCH_CONFIG_TEMP);
	VT1->Destruye();
	delete VT1;
	delete []Iconos;
	delete []Coordenadas;
}


////////////////////////////////////////////////////////////////////////
//                  Controlador del sistema integra                   //
////////////////////////////////////////////////////////////////////////

void Integra::Controlador()
{
   // Revisa las acciones sobre la ventana de trabajo
   selec = VT1->VT1->Itera();
   // Cambia el tamano de la ventana de trabajo
   if(selec == 1) Control_acciones(2000);
   if(selec == 2) Control_acciones(2001);
   // Checa procesos si esta activa la segunda ventana
   if(VT1->Estado_ventana_doble()) {
         // Activa a la segunda ventana y desactiva la primera (pero siguen visibles las dos)
      if(VT1->VT2->Presionado(BR)) {
         Ventana_activa = 1;
         P_a = 1;
         Guarda_posicion = NO;
         v_coord = SI;
         VT1->Fija_ventana_activa(1);
         Actualiza_coordenadas();
      }
      selec = VT1->VT2->Itera();
      // Cambia el tamano de la ventana de trabajo
      if(selec == 1) {
         Invierte_ventanas();
         Inversion_ventanas = 1;
         Control_acciones(2000);
      }
     
      // Activa el menu de la ventana numero 2
      if(VT1->VT2->Presionado(BL)) {
         if(Ventana_activa == 1) {
            Visualiza_texto(TXT01);
            vent.y = NCO->Retorna_estado_mouse().y;
            vent.x = NCO->Retorna_estado_mouse().x;
            if(Opcion_menu(M_ventana_mouse,OPC01,selec,vent)) Control_acciones(1999+selec);
         }
      }
   }

   // Activa el menu de la ventana numero 1
   if(VT1->VT1->Presionado(BL)) {
      if(Ventana_activa == 0) {
         Visualiza_texto(TXT01);
         vent.y = NCO->Retorna_estado_mouse().y;
         vent.x = NCO->Retorna_estado_mouse().x;
         if(Opcion_menu(M_ventana_mouse,OPC01,selec,vent)) Control_acciones(1999+selec);
      }
   }
   // Activa a la primera ventana y desactiva la segunda (pero siguen visibles las dos)
   if(VT1->VT1->Presionado(BR)) {
      Ventana_activa = 0;
      P_a = 0;
      Guarda_posicion = NO;
      v_coord = SI;
      VT1->Fija_ventana_activa(0);
      Actualiza_coordenadas();
   }


   // Revisa el comportamiento de los iconos del sistema
   for(int i = 0; i < Niconos; i++) {
       // Revisa si algun icono fue presionado
       if(Iconos[i].Oprimido()) Ipresionado = i + 1;
       // Revisa sobre que icono esta el mouse
       if(Iconos[i].Mouse_dentro()) N_texto_explicativo = i + 3;
   }

   // Activacion del menu
   if(Vt->Presionado(BR)) {
      vent.y = NCO->Retorna_estado_mouse().y;
      vent.x = NCO->Retorna_estado_mouse().x;
      if(vent.y > 26 && vent.y < 40) {
         delay(200);
         if(vent.x > 10  && vent.x < 65 ) Tecla = ALT_A;
         if(vent.x > 80  && vent.x < 145) Tecla = ALT_S;
         if(vent.x > 160 && vent.x < 205) Tecla = ALT_P;
         if(vent.x > 225 && vent.x < 273) Tecla = ALT_C;
         if(vent.x > 288 && vent.x < 355) Tecla = ALT_T;
         if(vent.x > 369 && vent.x < 450) Tecla = ALT_E;
         if(vent.x > 465 && vent.x < 530) Tecla = ALT_V;
         if(vent.x > 545 && vent.x < 615) Tecla = ALT_N;
      }
   }

   // Controla los procesos seleccionador por los iconos
   switch(Ipresionado) {
      case 1: // Selecciona el sistema actual
         Control_acciones(200);
         break;
      case 2: // Parametros del sistema
         Control_acciones(300);
         break;
      case 3: // Selecciona proyeccion XY
//         if(Ventana_activa) {
//            Escenario[1][Sistema_actual].z = 0;
//            VT1->VT2->Fija_dimenciones(Dim_maxima[1][Sistema_actual],Dim_minima[1][Sistema_actual]);
//          } else {
//            Escenario[0][Sistema_actual].z = 0;
//            VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
//          }
         Escenario[Ventana_activa][Sistema_actual].z = 0;
         Dibuja_ventana_trabajo();
         VT1->Borra_archivo_ventana();
         break;
      case 4: // Cambia los parametros de la pantalla
         Control_acciones(600);
         break;
      case 5: // Menu de metodos numericos
         Control_acciones(1000);
         break;
      case 6: // Menu de graficos
         Control_acciones(700);
         break;
      case 7: // Campo vectorial
         Control_acciones(801);
         break;
      case 8: // Cursor
         Control_acciones(500);
         break;
      case 9: // Integra en tiempo positivo
         Caracter = 'F';
         break;
      case 10: // Integra en tiempo negativo
         Caracter = 'B';
         break;
      case 11: // Limpia la ventana de trabajo
         Aplicacion::Dibuja();
         Redibuja_pantalla();
         break;
      case 12: // Cambia la grafica de puntos a lineas y viseversa
         Caracter = 'P';
         break;
   }
   Ipresionado = 0;
   // Controla las tareas que utilizan solo un caracter para activar
   switch(Caracter) {
///////////////////////////////////////////////////
// Estas teclas son de uso temporal
//         if(Ventana_activa) VT1->VT2->Fija_tipo_ventana(!VT1->VT2->Retorna_tipo_ventana());
//          else VT1->VT1->Fija_tipo_ventana(!VT1->VT1->Retorna_tipo_ventana());
//         break;
//      case 'o': // Visualiza en coordenadas polares
//      case 'O':
//         Coord_rectangulares = Coord_rectangulares ? 0 : 1;
///////////////////////////////////////////////////
      case 'a':
      case 'A':
         if(NCO->Opcion(TXT100) == 1) Reporte_activo = 1;  // Acvtiva la grabacion del reporte de integracion numerica
         break;
      case 'd':
      case 'D':
         if(NCO->Opcion(TXT101) == 1) Reporte_activo = 0;  // Desactiva la grabacion del reporte de integracion numerica
         break;
      case 'c':  // Limpia la ventana de trabajo
      case 'C':
         Control_acciones(2004);
         break;
      case 'p':  // Grafica con punto o lineas
      case 'P':
         if(Grafica_puntos[Sistema_actual]) Grafica_puntos[Sistema_actual] = 0;
          else Grafica_puntos[Sistema_actual] = 1;
         break;
      case 'f': // Integra en tiempo positivo
      case 'F':
         VT1->Visualiza_cursor(NO);
         // Graba untima orbita de la ventana 
         if(!Guarda_posicion) VT1->Graba_archivo_ventana();
         Visualiza_texto("");
         Integra_numericamente(1);
         Guarda_posicion = SI;
         // Visualiza el cursor
         VT1->Visualiza_cursor(SI);
         break;
      case 'b': // Integra en tiempo negativo
      case 'B':
         VT1->Visualiza_cursor(NO);
         // Graba untima orbita de la ventana 
         if(!Guarda_posicion) VT1->Graba_archivo_ventana();
         Visualiza_texto("");
         Integra_numericamente(0);
         Guarda_posicion = SI;
         // Visualiza el cursor
         VT1->Visualiza_cursor(SI);
         break;
      case 'l':
      case 'L':
         VT1->Visualiza_cursor(NO);
         VT1->Lee_archivo_ventana();
         Guarda_posicion = NO;
         break;
      case '+': // Incrementa el paso en el cursor
         Controla_cursor(1);
         v_coord = SI;
         break;
      case '-': // Decrementa el paso del cursor
         Controla_cursor(2);
         v_coord = SI;
         break;
   }

   Numero_funcion = 0;
   // Controla las tareas usando mas de una tecla para activar
   switch(Tecla) {
      case FLECHA_DERECHA:   // Mueve el cursor positivamente en el eje X
      case FLECHA_IZQUIERDA: // Mueve el cursor negativamente en el eje X
      case FLECHA_ARRIBA:    // Mueve el cursor positivamente en el eje Y
      case FLECHA_ABAJO:     // Mueve el cursor negativamente en el eje Y
      case PGDN:             // Mueve el cursor positivamente en el eje Z
      case PGUP:             // Mueve el cursor negativamente en el eje Z
         Controla_cursor(Tecla);
         v_coord = SI;
         Guarda_posicion = NO;
         break;
      case CTRL_F1:  // Graficacion de funciones definidas por la interfaz
         Numero_funcion = 1;
         break;
      case CTRL_F2:
         Numero_funcion = 2;
         break;
      case CTRL_F3:
         Numero_funcion = 3;
         break;
      case CTRL_F4:
         Numero_funcion = 4;
         break;
      case CTRL_F5:
         Numero_funcion = 5;
         break;
      case CTRL_F6:
         Numero_funcion = 6;
         break;
      case CTRL_F7:
         Numero_funcion = 7;
         break;
      case CTRL_F8:
         Numero_funcion = 8;
         break;
      case CTRL_F9:
         Numero_funcion = 9;
         break;
      case CTRL_F10: // Funciones a graficar
         Control_acciones(708);
         break;
      case ALT_F10: // Fija el numero de puntos para graficar las funciones auxiliares
         Control_acciones(710);
         break;
      case ALT_F9:  // Grafica ceroclinas
         Control_acciones(706);
         break;
      case CTRL_N_O: // Editor del archivo de notas
         {
            char xarch[MAXPATH];
            Manipulador_archivos ma;
            sprintf(xarch,"C:\\INTEGRA\\EXES\\%s",Proyecto);
            ma.Cambia_ext_path(xarch,"NOT");
            Visualiza_texto(TXT80);
            V_E_archivo(xarch,TXT81,EDITA,0);
         }
         break;
      case ALT_A: // Menu de archivos
         vent.x = 10,vent.y = 40;
         Visualiza_texto(TXT04);
         if(Opcion_menu(M_archivos,OPC03,selec,vent)) Control_acciones(99+selec);
         break;
      case ALT_S: // Menu de sistemas
         Visualiza_texto(TXT05);
         vent.x = 80,vent.y = 40;
         if(Opcion_menu(M_sistemas,OPC04,selec,vent)) Control_acciones(199+selec);
         break;
      case ALT_P: // Menu de parametros
         Visualiza_texto(TXT06);
         vent.x = 160,vent.y = 40;
         if(Opcion_menu(M_parametros,OPC05,selec,vent)) Control_acciones(299+selec);
         break;
      case ALT_C: // Menu del cursor
         Visualiza_texto(TXT07);
         vent.x = 225,vent.y = 40;
         if(!Visualiza_coordenadas_sistema) M_cursor[4] = TXT08;
          else M_cursor[4] = TXT09;
         if(Opcion_menu(M_cursor,OPC06,selec,vent)) Control_acciones(499+selec);
         break;
      case ALT_T: // Menu de la ventana de trabajo
         Visualiza_texto(TXT10);
         vent.x = 288,vent.y = 40;
         // Cambia los elementos del menu
         M_ventana[6] = "0";
         if(Doble_ventana) {
            M_ventana[5] = "Una Ventana";
            if(Graficar_dos_ventanas == 4) M_ventana[6] = "Gr fica en Una Ventana";
             else M_ventana[6] = "Gr fica en Ambas Ventanas";
            M_ventana[7] = "Invierte Ventanas";
         } else M_ventana[5] = "Dos Ventanas";
         if(Opcion_menu(M_ventana,OPC07,selec,vent)) Control_acciones(599+selec);
         break;
      case ALT_E: // Menu del escenario grafico
         Visualiza_texto(TXT11);
         vent.x = 369,vent.y = 40;
         if(!VT1->Retorna_Visualiza_nombre_eje()) M_grafico[5] = TXT12;
          else M_grafico[5] = TXT13;
         if(Opcion_menu(M_grafico,OPC08,selec,vent,"G")) Control_acciones(699+selec);
         break;
      case ALT_V: // Menu del Campo vectorial
         Visualiza_texto(TXT14);
         vent.x = 465,vent.y = 40;
         if(Opcion_menu(M_campo_vectorial,OPC09,selec,vent)) Control_acciones(799+selec);
         break;
      case ALT_N: // Menu de los metodos numericos
         Visualiza_texto(TXT15);
         vent.x = 545,vent.y = 40;
         if(Opcion_menu(M_Numerico,OPC10,selec,vent)) Control_acciones(999+selec);
         break;
      case CTRL_C: // Limpia pantalla conservando la actual posici¢n
         Control_acciones(602);
         break;
      case CTRL_G: // Graba el contenido de la ventana de graficacion
         Control_acciones(2100);
         break;
      case CTRL_L: // Lee el contenido de una ventana de graficacion
         Control_acciones(2101);
         break;
      case CTRL_W: // Graba estado del proyecto
         Control_acciones(100);
         break;
      case CTRL_R: // Lee estado del proyecto
         Control_acciones(101);
         break;
   }

   // Visualiza las coordenadas del sistema
   if(v_coord || !N_texto_explicativo) Visualiza_coordenadas();
   // Controla la visualizacion de las funciones auxiliares a graficar
   if(Numero_funcion) {
      if(Numero_funcion <= Numero_funciones_auxiliares[Sistema_actual]) {
         // Oculta el cursor
         VT1->Visualiza_cursor(NO);
         Visualiza_texto("");
         C_3D max, min;
         // valores maximos y minimos de los ejes coordenados
         if(Ventana_activa) VT1->VT2->Retorna_dimenciones_maximas(max,min);
          else VT1->VT1->Retorna_dimenciones_maximas(max,min);
         Grafica_funcion(Funciones_auxiliares[Sistema_actual][Numero_funcion-1], max, min);
         // Muestra el cursor
         VT1->Visualiza_cursor(SI);
      }
   }
/*
   ///////////////////////////////////
   // Usada para seber posicion de raton.
   ///////////////////////////////////
   if(Vt->Presionado(BR)) {
      vent.y = NCO->Retorna_estado_mouse().y;
      vent.x = NCO->Retorna_estado_mouse().x;
      sprintf(Coordenadas,"X = %d  Y = %d",vent.x,vent.y);
      Visualiza_texto(Coordenadas);
   }
*/
}


////////////////////////////////////////////////////////////////////////
//                       Pantalla de presentacion                     //
////////////////////////////////////////////////////////////////////////

void Integra::Pantalla_presentacion(void)
{
   Visualiza_texto("Acerca de ...");
   Ventana_iconos *Vpresent = new Ventana_iconos;
   Vpresent->Activa_graba_ventana(1);
   Vpresent->Ventana_centrada(VER_INTEGRA,480,300,0);
   Vpresent->Dibuja();
   Vpresent->Define_color_texto(Negro);
   Vpresent->Limpia_recuadro(20,50,460,130,Gris1,Blanco,Gris9);
   Vpresent->Define_tipo_letra(1,0,4,4,4,4);
   Vpresent->Centra_texto(60,"SISTEMA INTEGRA");
   Vpresent->Define_color_texto(Azul9);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   Vpresent->Centra_texto(110,"ANALIZADOR DE SISTEMAS DINAMICOS");
   Vpresent->Define_color_texto(Negro);
   Vpresent->Centra_texto(140,VERSION);

   Vpresent->Define_tipo_letra(2,0,-2,0,0,0);
   Vpresent->Define_color_texto(Rojo1);
   Vpresent->Centra_texto(155,FECHA_VERSION);
   Vpresent->Define_tipo_letra(0,0,-1,0,0,0);
   Vpresent->Define_color_texto(Negro);
   Vpresent->Centra_texto(190,"Autores:");
   Vpresent->Centra_texto(206,"Humberto Carrillo, Antonio Carrillo y Luis Nava");
   Vpresent->Centra_texto(240,"Laboratorio de Din mica no Lineal");
   Vpresent->Centra_texto(250,"Facultad de Ciencias");
   Vpresent->Centra_texto(260,"Universidad Nacional Aut¢noma de M‚xico");

   Vpresent->Define_color_texto(Rojo1);
   Vpresent->Centra_texto(280,REGISTRO);
   while(!Vpresent->Itera() && Programa_activo && Tecla != ENTER && Tecla != ESC) NCO->Administrador();
   delete Vpresent;
   // Mensaje del sistema
   NCO->Mensaje("ANALIZADOR DE SISTEMAS DINAMICOS",VERSION,"","Esta es una versi¢n de prueba");
}

////////////////////////////////////////////////////////////////////////
//                          Pantalla de terminacion                   //
////////////////////////////////////////////////////////////////////////

void Integra::Pantalla_terminacion(void)
{
   // Mensaje del sistema
   NCO->Mensaje("Cualquier error o comentario","inicarlo por favor a:","","acl@hp.fciencias.unam.mx");
}

////////////////////////////////////////////////////////////////////////
//                   Dibuja la pantalla de trabajo                    //
////////////////////////////////////////////////////////////////////////

void Integra::Dibuja_aplicacion(void)
{
   // Dibuja el area del menu vertical
   Vt->Limpia_recuadro(1,25,X_MAX-1,39,Gris1,Gris9,Blanco);
   Vt->Define_color_texto(Blanco);
   Vt->Visualiza_texto(11,30,TXT16);
   Vt->Define_color_texto(Negro);
   Vt->Visualiza_texto(10,29,TXT16);
   // Dibuja la area de iconos
   Vt->Limpia_recuadro(1,40,X_MAX-1,82,Gris1,Gris9,Blanco);
   // Dibuja a los iconos
   for(i = 0; i < Niconos; i++) Iconos[i].Dibuja();
   // Dibuja la ventana de graficacion
   Dibuja_ventana_trabajo();
}


////////////////////////////////////////////////////////////////////////
//         Visualiza coordenadas dentro de la ventana de trabajo      //
////////////////////////////////////////////////////////////////////////

void Integra::Visualiza_coordenadas(void)
{
   if(v_coord) {
      if(Visualiza_coordenadas_sistema) {
         aux[0] = 0;
         if(Ventana_activa) {
            aux[0] = 0;
            for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = Ac_Condicion[1][Sistema_actual][i];
            aux[i+1] = Ac_Tiempo[1][Sistema_actual];
            aux1.x = aux[Escenario[1][Sistema_actual].x];
            aux1.y = aux[Escenario[1][Sistema_actual].y];
            aux1.z = aux[Escenario[1][Sistema_actual].z];
          } else {
            aux[0] = 0;
            for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) aux[i+1] = Ac_Condicion[0][Sistema_actual][i];
            aux[i+1] = Ac_Tiempo[0][Sistema_actual];
            aux1.x = aux[Escenario[0][Sistema_actual].x];
            aux1.y = aux[Escenario[0][Sistema_actual].y];
            aux1.z = aux[Escenario[0][Sistema_actual].z];
         }
         // Cambia dependiendo del escenario ver que se visualizara
         char xcad1[20], xcad2[20], xcad3[20];
         Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[Ventana_activa][Sistema_actual].x-1,xcad1);
         Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[Ventana_activa][Sistema_actual].y-1,xcad2);
         Nombre_ecuacion_sistema_actual(Sistema_actual,Escenario[Ventana_activa][Sistema_actual].z-1,xcad3);

         if(Escenario[Ventana_activa][Sistema_actual].z) sprintf(Coordenadas,TXT17,xcad1,aux1.x,Paso_cursor[Ventana_activa][Sistema_actual].x,xcad2,aux1.y,Paso_cursor[Ventana_activa][Sistema_actual].x,xcad3,aux1.z,Paso_cursor[Ventana_activa][Sistema_actual].z);
          else sprintf(Coordenadas,TXT17A,xcad1,aux1.x,Paso_cursor[Ventana_activa][Sistema_actual].x,xcad2,aux1.y,Paso_cursor[Ventana_activa][Sistema_actual].y);
         Texto_explicativo[15] = Coordenadas;
      } else Texto_explicativo[15] = " ";
      NCO->Visualiza_texto(Texto_explicativo[15]);
   }
   N_texto_explicativo = 15;
   v_coord = 0;
}


///////////////////////////////////////////////////////////////////
//             Control de acciones dentro del integra            //
///////////////////////////////////////////////////////////////////

void Integra::Control_acciones(const unsigned int tp)
{
   t_ceroclina = 1;  // Indica que grafique isoclina
   switch(tp) {
      case 100: // Imprime estado del proyecto
         Visualiza_texto(TXT29);
         Imprime_estado(0,0);
         break;
      case 101: // Graba estado del proyecto
         Visualiza_texto(TXT27);
         VT1->VT1->Retorna_configuracion(Escala[0][Sistema_actual],Centro[0][Sistema_actual],Angulos[0][Sistema_actual][0],Angulos[0][Sistema_actual][1]);
         VT1->VT2->Retorna_configuracion(Escala[1][Sistema_actual],Centro[1][Sistema_actual],Angulos[1][Sistema_actual][0],Angulos[1][Sistema_actual][1]);
         Graba_estado(1,"");
         break;
      case 102: // Lee estado del proyecto
         Visualiza_texto(TXT28);
         if(Lee_estado("",1)) {
            VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
            VT1->VT1->Fija_configuracion(Escala[0][Sistema_actual],Centro[0][Sistema_actual],Angulos[0][Sistema_actual][0],Angulos[0][Sistema_actual][1]);
            VT1->VT2->Fija_dimenciones(Dim_maxima[1][Sistema_actual],Dim_minima[1][Sistema_actual]);
            VT1->VT2->Fija_configuracion(Escala[1][Sistema_actual],Centro[1][Sistema_actual],Angulos[1][Sistema_actual][0],Angulos[1][Sistema_actual][1]);
            Dibuja_ventana_trabajo();
         }
         break;
      case 103: // Restaura estado del proyecto
         Visualiza_texto(TXT28);
         if(Lee_estado(ARCH_CONFIG_TEMP,0)) {
            VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
            VT1->VT1->Fija_configuracion(Escala[0][Sistema_actual],Centro[0][Sistema_actual],Angulos[0][Sistema_actual][0],Angulos[0][Sistema_actual][1]);
            VT1->VT2->Fija_dimenciones(Dim_maxima[1][Sistema_actual],Dim_minima[1][Sistema_actual]);
            VT1->VT2->Fija_configuracion(Escala[1][Sistema_actual],Centro[1][Sistema_actual],Angulos[1][Sistema_actual][0],Angulos[1][Sistema_actual][1]);
            Dibuja_ventana_trabajo();
         }
         break;
      case 104: // Visualiza la descripcion del proyecto
         Visualiza_texto(TXT18);
         sprintf(cad,"%s%s0",ARCH_DESCRIP,Proyecto);
         V_E_archivo(cad,TXT19,VISUALIZA,0);
         break;
      case 105: // Graba pantalla en formato GIF
         Visualiza_texto(TXT20);
         if(Retorna_archivo_seleccionado(cad,0,TXT21,"GIF","*.GIF")) GIF_DumpEga10(cad);
         break;
      case 106: // Visualiza Archivo
         Visualiza_texto(TXT24);
         V_E_archivo("*.TXT",TXT25,VISUALIZA,1);
         break;
      case 107: // Edita Archivo
         Visualiza_texto(TXT21);
         V_E_archivo("*.TXT",TXT23,EDITA,1);
         break;
      case 108: // Pantalla de presentacion
         Pantalla_presentacion();
         break;
      case 109: // Termina el programa
         NCO->Almacena_bufer_teclado(ALT_X);
         break;
      case 200: // Selecciona sistema
         Control_acciones(2001);
         VT1->VT1->Retorna_configuracion(Escala[0][Sistema_actual],Centro[0][Sistema_actual],Angulos[0][Sistema_actual][0],Angulos[0][Sistema_actual][1]);
         VT1->VT2->Retorna_configuracion(Escala[1][Sistema_actual],Centro[1][Sistema_actual],Angulos[1][Sistema_actual][0],Angulos[1][Sistema_actual][1]);
         if(Selecciona_sistema() == 1) {
            VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
            VT1->VT1->Fija_configuracion(Escala[0][Sistema_actual],Centro[0][Sistema_actual],Angulos[0][Sistema_actual][0],Angulos[0][Sistema_actual][1]);
            VT1->VT2->Fija_dimenciones(Dim_maxima[1][Sistema_actual],Dim_minima[1][Sistema_actual]);
            VT1->VT2->Fija_configuracion(Escala[1][Sistema_actual],Centro[1][Sistema_actual],Angulos[1][Sistema_actual][0],Angulos[1][Sistema_actual][1]);
            Dibuja_ventana_trabajo();
            // Borra archivo de ultima orbita
            VT1->Borra_archivo_ventana();
         }
         break;
      case 201: // Descripcion del sistema
         {
         Visualiza_texto(TXT26);
         sprintf(cad,"%s%s%u",ARCH_DESCRIP,Proyecto,Sistema_actual+1);
         V_E_archivo(cad,Nombre_sistema[Sistema_actual],VISUALIZA,0);
         }
         break;
      case 202: // Reporte del estado del sistema
         Visualiza_texto(TXT29);
         Imprime_estado(1,0);
         break;
      case 300: // Parametros del sistema
         Cambia_parametros_sistema();
         break;
      case 301: // Parametros originales del sistema actual
         M_Definicion_usuario::Parametros(Sistema_actual,Ventana_activa);
         break;
      case 500: // Mover a ...
         Visualiza_texto(TXT30);
         Cambia_Condicion_inicial();
         Actualiza_coordenadas();
         Guarda_posicion = NO;
         v_coord = SI;
         break;
      case 501: // Al Origen
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) Ac_Condicion[Ventana_activa][Sistema_actual][i] = 0.0;
         Ac_Tiempo[Ventana_activa][Sistema_actual] = 0.0;
         Actualiza_coordenadas();
         Guarda_posicion = NO;
         v_coord = SI;
         break;
      case 502: // Paso del cursor
         Visualiza_texto(TXT35);
         if(Captura_cadenas(TXT36,TXT32,Paso_cursor[Ventana_activa][Sistema_actual].x,TXT33,Paso_cursor[Ventana_activa][Sistema_actual].y,TXT34,Paso_cursor[Ventana_activa][Sistema_actual].z,1) == 1) {
            if(Paso_cursor[Ventana_activa][Sistema_actual].x <= 0.0) Paso_cursor[Ventana_activa][Sistema_actual].x = 0.01;
            if(Paso_cursor[Ventana_activa][Sistema_actual].y <= 0.0) Paso_cursor[Ventana_activa][Sistema_actual].y = 0.01;
            if(Paso_cursor[Ventana_activa][Sistema_actual].z <= 0.0) Paso_cursor[Ventana_activa][Sistema_actual].z = 0.01;
            v_coord = SI;
         }
         break;
      case 503: // Incremento del paso del cursor
         Visualiza_texto(TXT37);
         if(Captura_cadenas(TXT38,TXT32,Incremento_paso[Ventana_activa][Sistema_actual].x,TXT33,Incremento_paso[Ventana_activa][Sistema_actual].y,TXT34,Incremento_paso[Ventana_activa][Sistema_actual].z,1) == 1) {
            if(Incremento_paso[Ventana_activa][Sistema_actual].x <= 0.0) Incremento_paso[Ventana_activa][Sistema_actual].x = 0.01;
            if(Incremento_paso[Ventana_activa][Sistema_actual].y <= 0.0) Incremento_paso[Ventana_activa][Sistema_actual].y = 0.01;
            if(Incremento_paso[Ventana_activa][Sistema_actual].z <= 0.0) Incremento_paso[Ventana_activa][Sistema_actual].z = 0.01;
            v_coord = SI;
         }
         break;
      case 504: // Visualiza coordenadas
         if(Visualiza_coordenadas_sistema) Visualiza_coordenadas_sistema = NO;
          else Visualiza_coordenadas_sistema = SI;
         v_coord = SI;
         break;
      case 600: // Dimenciones de los ejes coordenados de la ventana de trabajo
         Visualiza_texto(TXT39);
         if(Cambia_dimenciones_pantalla(Ventana_activa) == 1) {
            VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
            VT1->VT1->Retorna_configuracion(Escala[0][Sistema_actual],Centro[0][Sistema_actual],Angulos[0][Sistema_actual][0],Angulos[0][Sistema_actual][1]);
            VT1->VT2->Fija_dimenciones(Dim_maxima[1][Sistema_actual],Dim_minima[1][Sistema_actual]);
            VT1->VT2->Retorna_configuracion(Escala[1][Sistema_actual],Centro[1][Sistema_actual],Angulos[1][Sistema_actual][0],Angulos[1][Sistema_actual][1]);
            Dibuja_ventana_trabajo();
         }
         break;
      case 601: // Limpia pantalla sin conservar la actual posici¢n
         Control_acciones(2004);
         break;
      case 602: // Limpia pantalla conservando la actual posici¢n
         Control_acciones(2004);
         Guarda_posicion = SI;
         break;
      case 603: // Graba imagen formato propio
         Control_acciones(2002);
         break;
      case 604: // Lee imagen formato propio
         Control_acciones(2003);
         break;
      case 605: // Activa una o dos ventanas
         Doble_ventana = !Doble_ventana;
         if(Doble_ventana) Control_acciones(2001);
          else Control_acciones(2000);
         break;
      case 606: // Indica que grafique las orbitas en una o ambas ventanas de graficacion
         VT1->Visualiza_cursor(NO);
         Graficar_dos_ventanas = Graficar_dos_ventanas == 3 ? 4: 3;
         if(Graficar_dos_ventanas == 4) {
            for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) Ac_Condicion[!Ventana_activa][Sistema_actual][i] = Ac_Condicion[Ventana_activa][Sistema_actual][i];
            Ac_Tiempo[!Ventana_activa][Sistema_actual] = Ac_Tiempo[Ventana_activa][Sistema_actual];
            v_coord = SI;
            Guarda_posicion = NO;
         }
         Actualiza_coordenadas();
         VT1->Visualiza_cursor(SI);
         break;
      case 607: // Invierte ventanas
         if(Doble_ventana) {
            Invierte_ventanas();
            Estado_ventana = 3;
            Control_acciones(2001);
         }
         break;
      case 700: // Escenarios y curvas
         Visualiza_texto(TXT40);
         if(Escenarios(Ventana_activa) == 1) {
            Dibuja_ventana_trabajo();
            VT1->Borra_archivo_ventana();
         }
         break;
      case 701: // Mover el origen
         if(VT1->Retorna_tipo_ventana()) {
            Visualiza_texto(TXT41);
            Control_acciones(2004);
            while(Tecla != ESC && Programa_activo) {
               NCO->Administrador();
               if(Tecla == -1) continue;
               VT1->Mover_origen(Tecla);
            }
         }
         break;
      case 702: // Graduar ejes coordenados
//         VT1->VT1->Gradua_ejes();
         break;
      case 703: // Acercar / alejar ejes
         Visualiza_texto(TXT42);
         Control_acciones(2004);
         while(Tecla != ESC && Programa_activo) {
            NCO->Administrador();
            if(Tecla == -1) continue;
            VT1->Acercar_alejar(Caracter);
         }
         break;
      case 704: // Rotaciones
         if(!VT1->Retorna_proyeccion_actual()) {
            Visualiza_texto(TXT43);
            Control_acciones(2004);
            while(Tecla != ESC && Programa_activo) {
               NCO->Administrador();
               if(Tecla == -1) continue;
               VT1->Rota_ejes(Tecla);
            }
         }
         break;
      case 705: // Visualiza nombre de ejes
         VT1->Fija_Visualiza_nombre_eje();
         Control_acciones(2004);
         break;
      case 706: // Ceroclinas
         t_ceroclina = 0;
      case 707: // Isoclinas
         Visualiza_texto(TXT73);
         VT1->Graba_archivo_ventana();
         {
            C_3D max, min;
            // valores maximos y minimos de los ejes coordenados
            if(Ventana_activa) VT1->VT2->Retorna_dimenciones_maximas(max,min);
             else VT1->VT1->Retorna_dimenciones_maximas(max,min);
            // Llama al controlador de funciones auxiliares
            M_Definicion_usuario::Grafica_ceroclinas(t_ceroclina,0,max,min);
         }
         // Muestra el cursor
         VT1->Visualiza_cursor(SI);
         break;
      case 708: // Funciones a graficar
         // Oculta el cursor
         VT1->Visualiza_cursor(NO);
         Visualiza_texto(TXT50);
         // Llama al controlador de funciones auxiliares
         M_Definicion_usuario::Captura_funcion_auxiliar();
         {
            C_3D max, min;
            // valores maximos y minimos de los ejes coordenados
            if(Ventana_activa) VT1->VT2->Retorna_dimenciones_maximas(max,min);
             else VT1->VT1->Retorna_dimenciones_maximas(max,min);
            Grafica_funcion(Cadena_graficar, max, min); 
         }
         // Muestra el cursor
         VT1->Visualiza_cursor(SI);
         break;
      case 709: // Cambia los colores de ejes coordenados y graficos generados
         Visualiza_texto(TXT44);
         Selecciona_colores();
         VT1->VT1->Revisualiza_ejes();
         if(Ventana_activa) VT1->VT2->Revisualiza_ejes();
         break;
      case 710: // Fija el numero de puntos para graficar las funciones auxiliares
         Visualiza_texto(TXT70);
         if(Captura_cadena(TXT71,TXT72,NUMERO_PUNTOS,0) == 1) {
            if(NUMERO_PUNTOS <= 10.0) NUMERO_PUNTOS = 10.0;
         }
         break;
      case 800: // Configura el campo vectoria
         Visualiza_texto(TXT45);
         Configura_campo_vectorial();
         break;
      case 801: // Dibuja el campo vectorial
         VT1->Visualiza_cursor(NO);
         {
            C_3D max,min;
            // valores maximos y minimos de los ejes coordenados
            if(Ventana_activa) VT1->VT2->Retorna_dimenciones_maximas(max,min);
             else VT1->VT1->Retorna_dimenciones_maximas(max,min);
            Dibuja_campo_vectorial(max,min);
         }
         VT1->Visualiza_cursor(SI);
         break;
      case 1000: // Configura a los Metodos numericos
         Visualiza_texto(TXT46);
         Selecciona_metodo_integracion();
         break;
      case 2000: // Maximiza la ventana
         if(Estado_ventana != 3) {
            VT1->Fija_ventana_activa(0);
            VT1->Borra_archivo_ventana();
            Guarda_posicion = NO;
            v_coord = SI;
            Doble_ventana = 0;
            Estado_ventana = 3;
            Ventana_activa = 0;
            VT1->Fija_ventana_doble(0);
            VT1->VT1->Cambiar_tamano(3,83,X_MAX-3,Y_MAX-31);
            VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
            Redibuja_pantalla();
         }
         break;
      case 2001: // Minimiza la ventana
         if(Estado_ventana != 2) {
            // Si las ventanas fueron invertidas por maximizar la segunda ventana esta son de nuevo invertidas para estar como al principio
            if (Inversion_ventanas) {
               Inversion_ventanas = 0;
               Invierte_ventanas();
            }
            Guarda_posicion = NO;
            v_coord = SI;
            Doble_ventana = 1;
            Estado_ventana = 2;
            VT1->Fija_ventana_doble(1);
            VT1->VT1->Cambiar_tamano(3,83,X_MAX/2,Y_MAX-31);
            VT1->VT1->Fija_dimenciones(Dim_maxima[0][Sistema_actual],Dim_minima[0][Sistema_actual]);
            VT1->VT2->Fija_dimenciones(Dim_maxima[1][Sistema_actual],Dim_minima[1][Sistema_actual]);
            VT1->Borra_archivo_ventana();
            // Redibuja la ventana dos
            VT1->Fija_ventana_activa(1);
            Ventana_activa = 1;
            Actualiza_coordenadas();
            VT1->VT2->Dibuja();
            // Redibuja la ventana uno
            VT1->Fija_ventana_activa(0);
            Ventana_activa = 0;
            Actualiza_coordenadas();
            VT1->VT1->Dibuja();
            Redibuja_pantalla();
         }
         break;
      case 2002: // Graba imagen formato propio
         VT1->Visualiza_cursor(NO);
         Visualiza_texto(TXT51);
         if(Retorna_archivo_seleccionado(cad,0,TXT48,"IMG","*.IMG")) {
            if(Ventana_activa) VT1->VT2->Graba_ventana(cad,SIN_FORMATO);
             else VT1->VT1->Graba_ventana(cad,SIN_FORMATO);
         }
         VT1->Visualiza_cursor(SI);
         break;
      case 2003:  // Lee imagen formato propio
         VT1->Visualiza_cursor(NO);
         Visualiza_texto(TXT52);
         if(Retorna_archivo_seleccionado(cad,1,TXT49,"IMG","*.IMG")) {
            if(Ventana_activa) VT1->VT2->Lee_ventana(cad,SIN_FORMATO);
             else VT1->VT1->Lee_ventana(cad,SIN_FORMATO);         
         }
         VT1->Visualiza_cursor(SI);
         break;
      case 2004:  // Limpia la ventana activa sin conservar la posicion 
         VT1->Graba_archivo_ventana();
         if(VT1->VT1->Retorna_ventana_activa()) VT1->VT1->Dibuja();
         if(VT1->VT2->Retorna_ventana_activa()) VT1->VT2->Dibuja();
         VT1->Visualiza_cursor(SI);
         v_coord = SI;
         Guarda_posicion = NO;
         break;
      case 2100: // Graba el contenido de la ventana de graficacion
         VT1->Visualiza_cursor(NO);
         if(Ventana_activa) {
            unsigned int x1, x2, y1, y2;
            VT1->VT2->Actual_pos_ventana(x1,y1,x2,y2);
            graba_pantalla_grafica(x1,y1+21,x2,y2,ARCH_TEMPORAL);
          } else {
            unsigned int x1, x2, y1, y2;
            VT1->VT1->Actual_pos_ventana(x1,y1,x2,y2);
            graba_pantalla_grafica(x1,y1+21,x2,y2,ARCH_TEMPORAL);
         }
         VT1->Visualiza_cursor(SI);
         break;
      case 2101: // Lee el contenido de una ventana de graficacion
         VT1->Visualiza_cursor(NO);
         if(Ventana_activa) {
            unsigned int x1, x2, y1, y2;
            VT1->VT2->Actual_pos_ventana(x1,y1,x2,y2);
            if(n_archivos_directorios(ARCH_TEMPORAL,FA_ARCH)) lee_pantalla_grafica(x1,y1+21,ARCH_TEMPORAL);
          } else {
            unsigned int x1, x2, y1, y2;
            VT1->VT1->Actual_pos_ventana(x1,y1,x2,y2);
            if(n_archivos_directorios(ARCH_TEMPORAL,FA_ARCH)) lee_pantalla_grafica(x1,y1+21,ARCH_TEMPORAL);
         }
         VT1->Visualiza_cursor(SI);
         break;
   }
}


//////////////////////////////////////////////////////////////////
//                Pantalla de estado del sistema                //
//////////////////////////////////////////////////////////////////

void Integra::Pantalla_estado_sistema(void)
{
   // Visualiza estado del sistema
   Imprime_estado(1,1);
}

// Redibuja pantalla
void Integra::Redibuja_pantalla(void)
{
   Dibuja_aplicacion();
}


//////////////////////////////////////////////////////////////////
//                        RUTINA PRINCIPAL                      //
//////////////////////////////////////////////////////////////////

void main(int argc, char *argv[])
{
   int svga = 0, st = 0;

   // Revisa a que modo grafico iniciara la pantalla Grafica
   if(argc == 2) {
      svga = argv[1][0] - '0';
      if(svga < 0 || svga > 3) st = 1;
   } else st = 1;
   if(st) printf("\n\n%s\n\n%s\n   %s N\n\n%s\n%s\n%s\n%s\n%s\n",VER_INTEGRA,TXT2,argv[0],TXT3,TXT4,TXT5,TXT6,TXT7);
    else {
      Integra *If;
      If = new Integra;
      If->Ejecuta(VER_INTEGRA,svga);
      delete If;
   }
}








