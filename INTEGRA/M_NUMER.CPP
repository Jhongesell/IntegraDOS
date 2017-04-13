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




//////////////////////////////////////////////////////////////
//   CLASE DE METODOS NUMERICOS DE INTEGRACION  NUMERICA    //
//////////////////////////////////////////////////////////////




#include "m_numer.hpp"
#include "nucleo.hpp"
#include "v_iconos.hpp"
#include "combo_b.hpp"
#include "r_esp.hpp"
#include "get.hpp"
#include "teclado.hpp"
#include "\libreria\gen_rep.hpp"
#include "cad_car.hpp"

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



// Activa la suspension de integraci¢n numerica
#define _SUSPENDE_INTEGRACION_
// Activa la validacion del calculo para evitar overflow
#define _VALIDA_CALCULO_
// Indica en numro de integraciones a realizar antes de revisar su se suspende la integraci¢n 
#define NUMERO_INTEGRACIONES_REVISAR  2500


#ifdef _IDIOMA_ESPANOL_

	#define TXT01   "Se ha calculado un valor muy grande"
   #define TXT02   "posible error por desbordamiento"
   #define TXT03   "¨Desea continuar con el c lculo?"
   #define TXT04   "METODOS DE INTEGRACION"
   #define TXT05   "M‚todo de integraci¢n:"
   #define TXT06   "Par metros:"
   #define TXT07   "Nombre:"
   #define TXT08   "Valor:"
   #define TXT10   "Error en el c lculo n£merico"
   #define TXT11   "Se suspende la integraci¢n"
   #define TXT12   "Integrando en tiempo positivo %s iteraciones, presione [Esc] para cancelar"
   #define TXT13   "Integrando en tiempo negativo %s iteraciones, presione [Esc] para cancelar"


   // Nombre de los metodos numericos
   const char *Metodos_integracion[] = {
      "Euler",
      "Euler Modificado",
      "Punto Medio",
      "Heun",
      "Runge-Kutta 4",
      "Euler P.V.",
      "Euler Modificado P. V.",
      "Runge-Kutta 4 P.V.",
		"Runge-Kutta-Fehlberg",
      "0",
      "0",
      "0"
   };
   const char *C_Metodos_integracion = "EMPHRUONF";

   // Parametros de los metodos de integracion 1 a 5
   const char *Parametros_metodos_integracion1[] = {
      "Tiempo a Integrar",
      "Paso de Integraci¢n",
      "0"
   };
   const char *C_Parametros_metodos_integracion1 = "TP";
   // Parametros de los metodos de integracion 6 a 8
   const char *Parametros_metodos_integracion2[] = {
      "Tiempo a Integrar",
      "Tolerancia al Integrar",
      "Paso Maximo al Integrar",
      "0"
   };
   const char *C_Parametros_metodos_integracion2 = "TOP";

#else
#endif




//////////////////////////////////////////////////////////////////////
//           Inicializa la clase de metodos numericos               //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Inicializa(void)
{
   // Asignacion de los metodos numericos
   Metodos[0]  = &Metodos_numericos::Euler;
   Metodos[1]  = &Metodos_numericos::Euler_modificado;
   Metodos[2]  = &Metodos_numericos::Punto_medio;
   Metodos[3]  = &Metodos_numericos::Heun;
   Metodos[4]  = &Metodos_numericos::Runge_Kutta4;
   Metodos[5]  = &Metodos_numericos::Euler_p_v;
   Metodos[6]  = &Metodos_numericos::Euler_modificado_p_v;
   Metodos[7]  = &Metodos_numericos::Runge_Kutta4_p_v;
   Metodos[8]  = &Metodos_numericos::Runge_Kutta_fehlberg;
///////////////////////////////////////
// Cambiar por el metodo adecuado
   Metodos[9]  = &Metodos_numericos::Runge_Kutta4;
	Metodos[10] = &Metodos_numericos::Runge_Kutta4;
///////////////////////////////////////
   // Asignacion de los parametros de los metodos numericos
   for(int i = 0; i < Numero_sistemas; i++) {
      Parametros_metodo[i][0][0] = 2.0;
      Parametros_metodo[i][0][1] = 100.0;
      Parametros_metodo[i][0][2] = 0.001;
      Parametros_metodo[i][0][3] = 0.0;
      Parametros_metodo[i][1][0] = 2.0;
      Parametros_metodo[i][1][1] = 100.0;
      Parametros_metodo[i][1][2] = 0.002;
      Parametros_metodo[i][1][3] = 0.0;
      Parametros_metodo[i][2][0] = 2.0;
      Parametros_metodo[i][2][1] = 100.0;
      Parametros_metodo[i][2][2] = 0.002;
      Parametros_metodo[i][2][3] = 0.0;
      Parametros_metodo[i][3][0] = 2.0;
      Parametros_metodo[i][3][1] = 100.0;
      Parametros_metodo[i][3][2] = 0.002;
      Parametros_metodo[i][3][3] = 0.0;
      Parametros_metodo[i][4][0] = 2.0;
      Parametros_metodo[i][4][1] = 100.0;
      Parametros_metodo[i][4][2] = 0.004;
      Parametros_metodo[i][4][3] = 0.0;
		Parametros_metodo[i][5][0] = 3.0;
      Parametros_metodo[i][5][1] = 100.0;
		Parametros_metodo[i][5][2] = 1.0e-6;
		Parametros_metodo[i][5][3] = 0.5;
		Parametros_metodo[i][6][0] = 3.0;
		Parametros_metodo[i][6][1] = 100.0;
		Parametros_metodo[i][6][2] = 1.0e-8;
		Parametros_metodo[i][6][3] = 0.5;
		Parametros_metodo[i][7][0] = 3.0;
		Parametros_metodo[i][7][1] = 100.0;
      Parametros_metodo[i][7][2] = 1.0e-9;
      Parametros_metodo[i][7][3] = 0.5;
      Parametros_metodo[i][8][0] = 3.0;
      Parametros_metodo[i][8][1] = 100.0;
      Parametros_metodo[i][8][2] = 1.0e-8;
      Parametros_metodo[i][8][3] = 0.5;
//////////////////////////////////////////////////
// Cambiar por los valores adecuados
      Parametros_metodo[i][9][0] = 3.0;
      Parametros_metodo[i][9][1] = 100.0;
      Parametros_metodo[i][9][2] = 0.002;
      Parametros_metodo[i][9][3] = 0.1;
      Parametros_metodo[i][10][0] = 3.0;
      Parametros_metodo[i][10][1] = 100.0;
		Parametros_metodo[i][10][2] = 0.004;
      Parametros_metodo[i][10][3] = 0.2;
//////////////////////////////////////////////////
      Numero_iteraciones = 0;
      Paso_maximo = Paso_minimo = 0.0;
   }
   Reporte_activo = 0;
}

//////////////////////////////////////////////////////////////////////
//        Controla la integraci¢n n£merica del sistema              //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Integra_numericamente(const int ti)
{
	Integracion_positiva = ti;
	for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = Ac_Condicion[Ventana_activa][Sistema_actual][i];
	Tiempo_actual  = Ac_Tiempo[Ventana_activa][Sistema_actual];
	Tiempo_integracion = 0.0;
	Numero_iteraciones = 0;
	Paso_integracion  = Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][2];
	if(!Integracion_positiva) Paso_integracion *= -1.0;
	if(Guarda_posicion) {
		for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
		Tiempo_actual  = At_Tiempo[Ventana_activa][Sistema_actual];
	}
	// Graba los valores del actual metodo numerico
	if(Reporte_activo) {
		 int i2;
		 // Nommbre del sistema
       Rep_num->Formato_impresion(1,3,Nombre_sistema[Sistema_actual]);
		 // Ecuaciones
		 for(i2 = 0; i2 < Numero_ecuaciones[Sistema_actual]; i2++) {
			  Rep_num->Formato_impresion(3,1,Nombre_ecuacion[Sistema_actual][i2]);
		 }
		 // Parametros
		 if(Numero_parametros[Sistema_actual]) {
			 for(i2 = 0; i2 < Numero_parametros[Sistema_actual]; i2++) {
				  sprintf(C_imp,"%s = %1.9e",Nombre_parametro[Sistema_actual][i2],_P[P_a][Sistema_actual][i2]);
				  Rep_num->Formato_impresion(3,1,C_imp);
			 }
		 }
       // Imprime la configuracion de los metodos numericos
       s_nocar(Metodos_integracion[Metodo_N_actual[Sistema_actual]],127,C_imp,1);
       Rep_num->Formato_impresion(3,1,C_imp);
       for(i2 = 0; i2 < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][0]; i2++) {
          if(Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][0] < 3.0) s_nocar(Parametros_metodos_integracion1[i2],127,C_imp,1);
           else s_nocar(Parametros_metodos_integracion2[i2],127,C_imp,1);
          Rep_num->Formato_impresion(3,1,C_imp);
          sprintf(C_imp,"= %1.9e",Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][i2+1]);
          Rep_num->Formato_impresion(27,0,C_imp);
       }
       Rep_num->Formato_impresion(1,2,"=================================================================================");
   }

   // Manda a ejecutar el metodo activo con las condiciones iniciales
   (this->*Metodos[Metodo_N_actual[Sistema_actual]])();
   NCO->Limpia_bufer_teclado();
}  


//////////////////////////////////////////////////////////////////////
// Muestra y permite cambiar el metodo de integracion actual  y sus //
// par metros                                                       //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Selecciona_metodo_integracion(void)
{
	unsigned int x1,x2,y1,y2;
 	C_2I vent;
	unsigned int st = 0, actual_metodo, actual_parm = 0, i = 0;
	double param[4];
	Ventana_iconos *Vt = new Ventana_iconos;
	Vt->Activa_graba_ventana(1);
	Vt->Ventana_centrada(TXT04,330,235,0);
	Vt->Dibuja();
	Vt->Actual_pos_ventana(x1,y1,x2,y2);
	Vt->Define_color_texto(Negro);
	Vt->Limpia_recuadro(15,50,265,100,Gris1,Blanco,Gris9);
	Vt->Limpia_recuadro(15,120,265,225,Gris1,Blanco,Gris9);
	// visualiza el nombre de los metodos numericos
	Vt->Visualiza_texto(20,40,TXT05);
	Vt->Visualiza_texto(20,110,TXT06);
	Vt->Visualiza_texto(20,130,TXT07);
	Vt->Visualiza_texto(20,175,TXT08);
	actual_metodo = Metodo_N_actual[Sistema_actual];

	vent.x = x1 + 20, vent.y = y1 + 65;

	// Combo box de del metodo numerico de integracion
	Combo_box *Co1 = new Combo_box;
	Co1->Parametros(Metodos_integracion, vent, actual_metodo + 1,C_Metodos_integracion);
	Co1->Dibuja(Metodos_integracion);
	for(i = 0; i < Parametros_metodo[Sistema_actual][actual_metodo][0]; i++) param[i] = Parametros_metodo[Sistema_actual][actual_metodo][i+1];

	vent.y = y1 + 145;

	// Get de captura del parametro
	Get *Gt = new Get;
	Gt->Parametros(param[actual_parm],vent.x,vent.y+45,20,1);
	Gt->Activa_get(1);
	Gt->Dibuja();

	// Iconos de la ventana de trabajo
	Icono *Ic1 = new Icono;
	Icono *Ic2 = new Icono;
	Ic1->Define_icono("OK.ICO",x2-50,y1+70);
	Ic1->Dibuja();
	Ic2->Define_icono("CANCEL.ICO",x2-50,y1+130);
	Ic2->Dibuja();

	// Combo box de parametros del metodo
	Combo_box *Co2;
	Co2 = new Combo_box;
	if(Parametros_metodo[Sistema_actual][actual_metodo][0] < 3.0) {
		Co2->Parametros(Parametros_metodos_integracion1,vent,1,C_Parametros_metodos_integracion1);
		Co2->Dibuja(Parametros_metodos_integracion1);
	 } else {
		Co2->Parametros(Parametros_metodos_integracion2,vent,1,C_Parametros_metodos_integracion2);
		Co2->Dibuja(Parametros_metodos_integracion2);
	}

	while(!st) {
		NCO->Administrador();
		if(Vt->Itera() || !Programa_activo || Tecla == ESC) break;

		// Revisa el comportamiento del combo box del metodo numerico de integracion
		Co1->Controla(Tecla, Metodos_integracion);
		// Se cambia el metodo numerico de integracion
		if(Co1->Retorna_opcion() != actual_metodo+1) {
			// Obtiene el nuevo numero del sistema y fija el numero de parametro
			actual_metodo = Co1->Retorna_opcion() -1;
			actual_parm = 0;
			// Actualiza el valor del parametro para ese metodo
			for(i = 0; i < Parametros_metodo[Sistema_actual][actual_metodo][0]; i++) param[i] = Parametros_metodo[Sistema_actual][actual_metodo][i+1];
			// Limpia el recuadro de parametros del metodo
			Vt->Limpia_recuadro(15,120,265,225,Gris1,Blanco,Gris9);
			// Destruye el combo box de parametros
			delete Co2;
			// Inicializa el combo box de parametros, segun el tipo de metodo numerico
			Co2 = new Combo_box;
			if(Parametros_metodo[Sistema_actual][actual_metodo][0] < 3.0) {
				Co2->Parametros(Parametros_metodos_integracion1,vent,1,C_Parametros_metodos_integracion1);
				Co2->Dibuja(Parametros_metodos_integracion1);
			 } else {
				Co2->Parametros(Parametros_metodos_integracion2,vent,1,C_Parametros_metodos_integracion2);
				Co2->Dibuja(Parametros_metodos_integracion2);
			}
			// Cambie el valor del parametro
			Gt->Cambia_cadena(param[actual_parm]);
			// Dibuja el parametro
			Gt->Dibuja();
		}

		// Revisa el comportamiento del combo box de parametros del metodo
		if(Parametros_metodo[Sistema_actual][actual_metodo][0] < 3.0) Co2->Controla(Tecla,Parametros_metodos_integracion1);
		 else Co2->Controla(Tecla,Parametros_metodos_integracion2);
		// Se cambia el parametro del metodo numerico
		if(Co2->Retorna_opcion() != actual_parm+1) {
			// Actualiza el valor anterior
			Gt->Cadena_editada(param[actual_parm]);
			// Obtiene el nuevo numero del parametro
			actual_parm = Co2->Retorna_opcion()-1;
			// Cambia el valor del parametro
			Gt->Cambia_cadena(param[actual_parm]);
			// Dibuja el parametro
			Gt->Dibuja();
		}
		// Revisa si se edita el valor del parametro
		Gt->Edita(Caracter,Tecla);
		// Revisa el comportamiento de los iconos de la ventana
		if(Ic1->Oprimido()) st = 1;
		if(Ic2->Oprimido()) st = 2;
	}
	// Se aceptan los cambios
	if(st == 1) {
		// Actualiza el valor anterior
		Gt->Cadena_editada(param[actual_parm]);
		// Actualiza el valores de los parametros del metodo numerico activo
		for(i = 0; i < Parametros_metodo[Sistema_actual][actual_metodo][0]; i++) {
			if(param[i] > 0.0) Parametros_metodo[Sistema_actual][actual_metodo][i+1] = param[i];
		}
		Metodo_N_actual[Sistema_actual] = actual_metodo;
		Guarda_posicion = NO;
	}
	delete Gt;
	delete Co1;
	delete Co2;
	delete Ic1;
	delete Ic2;
	delete Vt;
}





/////////////////////////////////////////////////////////////////////
//                                                                //
//                 Metodos de integraci¢n numerica                 //
//                                                                 //
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//                          Metodo Euler                            //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Euler(void)
{
	// Inicializa las variables
	for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
	At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

	// Loop del metodo numerico
	while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }

		// Dibuja la grafica con puntos
		if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

		// Actualiza valores
		t = At_Tiempo[Ventana_activa][Sistema_actual];
		for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i];

		// Calcula el nuevo valor inicial
		for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
			 // Calculo con la ecuacion diferencial
			 aux1[i] = ( this->*funcion[Sistema_actual][i] )();

			 // Controla los errores en el calculo matematico
			 if(ERROR_MATEMATICO) {
				 Error_calculo();
				 return;
			 }

			 // Actualiza la actual condicion inicial
			 At_Condicion[Ventana_activa][Sistema_actual][i] += (aux1[i] * Paso_integracion);

			 #ifdef _VALIDA_CALCULO_
			 // Controla el desbordamiento numerico
			 if(fabs(At_Condicion[Ventana_activa][Sistema_actual][i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
				 if(Desbordamiento_numerico()) return;
			 }
			 #endif
		}
		// Actualiza el tiempo
		At_Tiempo[Ventana_activa][Sistema_actual] += Paso_integracion;
		// Actualiza el numero de iteraciones
		Tiempo_integracion += fabs(Paso_integracion);
		Numero_iteraciones ++;

		// Dibuja la grafica con lineas
		if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini, At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

		#ifdef _SUSPENDE_INTEGRACION_
		// Detiene la iteracion con la tecla [ESC]
		if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
			if(Suspende_integracion()) break;
		}
		#endif

		// Guarda el ultimo valor calculado
		for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
		Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
	}
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}


//////////////////////////////////////////////////////////////////////
//                     Metodo Euler modificado                      //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Euler_modificado(void)
{
   // Inicializa las variables
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   // Loop del metodo numerico
   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }

      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      // Actualiza valores 
      t = At_Tiempo[Ventana_activa][Sistema_actual];
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i]; 
     
      // Calcula valores temporales
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          // Calcula con la ecucion diferencial
          aux1[i]  = ( this->*funcion[Sistema_actual][i] )();
 
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
      }

      // Actualiza valores 
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] += (aux1[i] * Paso_integracion);
      t += Paso_integracion;

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          aux2[i]   = ( this->*funcion[Sistema_actual][i] )();
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }

          At_Condicion[Ventana_activa][Sistema_actual][i] += (((aux1[i] + aux2[i]) * 0.5) * Paso_integracion);

          #ifdef _VALIDA_CALCULO_
          // Controla el desbordamiento numerico
          if(fabs(At_Condicion[Ventana_activa][Sistema_actual][i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
             if(Desbordamiento_numerico()) return;
          }
          #endif 
      }
      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso_integracion;
      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso_integracion);
      Numero_iteraciones++;

      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini,At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}


//////////////////////////////////////////////////////////////////////
//                        Metodo Punto medio                        //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Punto_medio(void)
{
   // Inicializa las variables
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   // Loop del metodo numerico
   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }
      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      // Actualiza valores
      t = At_Tiempo[Ventana_activa][Sistema_actual];
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i]; 
     
      // Calcula valores temporales
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          // Calcula con la ecucion diferencial
          aux1[i]  = ( this->*funcion[Sistema_actual][i] )();
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
      }

      // Actualiza valores 
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] += (aux1[i] * Paso_integracion * 0.5);
      t += (Paso_integracion * 0.5);

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          aux2[i]   = ( this->*funcion[Sistema_actual][i] )();
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
          At_Condicion[Ventana_activa][Sistema_actual][i] += (aux2[i] * Paso_integracion);
          #ifdef _VALIDA_CALCULO_
          // Controla el desbordamiento numerico
          if(fabs(At_Condicion[Ventana_activa][Sistema_actual][i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
             if(Desbordamiento_numerico()) return;
          }
          #endif
      }

      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso_integracion;
      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso_integracion);
      Numero_iteraciones++;

      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini,At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}

//////////////////////////////////////////////////////////////////////
//                        Metodo Heun                               //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Heun(void)
{
   // Inicializa las variables
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   // Loop del metodo numerico
   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }

      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      // Actualiza valores 
      t = At_Tiempo[Ventana_activa][Sistema_actual];
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i]; 
     
      // Calcula valores temporales
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          // Calcula con la ecucion diferencial
          aux1[i]  = ( this->*funcion[Sistema_actual][i] )();

          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
      }

      // Actualiza valores 
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] += (((aux1[i] * Paso_integracion) * 2) / 3);
      t += ((Paso_integracion * 2) / 3);

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          aux2[i]   = ( this->*funcion[Sistema_actual][i] )();
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }

          At_Condicion[Ventana_activa][Sistema_actual][i] += ((aux1[i] + (3 * aux2[i])) * Paso_integracion) / 4;

          #ifdef _VALIDA_CALCULO_
          // Controla el desbordamiento numerico
          if(fabs(At_Condicion[Ventana_activa][Sistema_actual][i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
             if(Desbordamiento_numerico()) return;
          }
          #endif 
      }
      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso_integracion;
      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso_integracion);
      Numero_iteraciones++;

      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini,At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);
      
      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}







//////////////////////////////////////////////////////////////////////
//                      Metodo Runge-Kutta orden 4                  //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Runge_Kutta4(void)
{
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }

      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      // Actualiza valores
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i]; 
      t = At_Tiempo[Ventana_activa][Sistema_actual];

      // Primer calculo temporal
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          aux1[i] = ( this->*funcion[Sistema_actual][i] )() * Paso_integracion;
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
      }

      // Actualiza valores 
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux1[i] * 0.5); 
      t += Paso_integracion * 0.5;

      // Segundo calculo temporal
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          aux2[i] = ( this->*funcion[Sistema_actual][i] )() * Paso_integracion;
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
      }

      // Actualiza valores 
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux2[i] * 0.5); 
      
      // Tercer calculo temporal
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          aux3[i] = ( this->*funcion[Sistema_actual][i] )() * Paso_integracion;
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
      }

      // Actualiza valores
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + aux3[i];
      t = At_Tiempo[Ventana_activa][Sistema_actual] + Paso_integracion;

      // Cuarto paso temporal
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
          aux4[i]  = ( this->*funcion[Sistema_actual][i] )() * Paso_integracion;
          // Controla los errores en el calculo matematico
          if(ERROR_MATEMATICO) {
             Error_calculo();
             return;
          }
          At_Condicion[Ventana_activa][Sistema_actual][i] += (aux1[i] + (2.0 * aux2[i]) + (2.0 * aux3[i]) + aux4[i]) / 6.0;
          #ifdef _VALIDA_CALCULO_
          // Controla el desbordamiento numerico
          if(fabs(At_Condicion[Ventana_activa][Sistema_actual][i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
             if(Desbordamiento_numerico()) return;
          }
          #endif
      }

      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso_integracion;

      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso_integracion);
      Numero_iteraciones++;

      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini,At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso_integracion,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}


//////////////////////////////////////////////////////////////////////
//                      Metodo Euler paso variable                  //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Euler_p_v(void)
{
   unsigned int sw;
   double eps  = Parametros_metodo[Sistema_actual][5][2] * 0.7;
   Paso        = Parametros_metodo[Sistema_actual][5][3] / 1000.0;
   Paso_maximo = Paso_minimo = Paso;
   if(!Integracion_positiva) Paso *= -1.0;

   // Inicializa las variables
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   // Loop del metodo numerico
   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }
      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      sw = 1;
      // Loop para el control de avance del metodo numerico
      while (sw) {
         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
         t = At_Tiempo[Ventana_activa][Sistema_actual];

         // Calcula la nueva condicion inicial
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Calculo con la ecuacion diferencial
             aux1[i] = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux2[i] = (aux1[i] * (Paso * 0.5)) + At_Condicion[Ventana_activa][Sistema_actual][i];
             #ifdef _VALIDA_CALCULO_
             // Controla el desbordamiento numerico
             if(fabs(aux2[i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
                if(Desbordamiento_numerico()) return;
             }
             #endif
         }

         // Actualiza valores con la nueva condicion calculada
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux2[i];
         t += (Paso*0.5);

         // Calcula la nueva condicion inicial
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Calculo con la ecuacion diferencial
             aux1[i] = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux3[i] = (aux1[i] * (Paso * 0.5)) + aux2[i];
             #ifdef _VALIDA_CALCULO_
             // Controla el desbordamiento numerico
             if(fabs(aux3[i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
                if(Desbordamiento_numerico()) return;
             }
             #endif
         }

         // Actualiza valores con la nueva condicion calculada
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux3[i];
         t += (Paso*0.5);

         xNorma = 0.0;
         // Calcula la anterior condicion inicial
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Calculo con la ecuacion diferencial
             aux1[i] = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux4[i] = (aux1[i] * (-Paso)) + aux3[i];
             // Calcula la distancia entre la condicion inicial calculada y la actual
             xNorma += pow((aux4[i] - At_Condicion[Ventana_activa][Sistema_actual][i]), 2.0); 
         }


         // Revisa si avanza en la integracion o cambia el tama¤o del Paso
         Norma = sqrt(xNorma);
         if(Norma < Parametros_metodo[Sistema_actual][5][2]) sw = 0;
          else Paso *= 0.9; // Disminuye el tama¤o del Paso de integracion
      }

      // Almacena el paso maximo y el minimo
      if(fabs(Paso) > Paso_maximo) Paso_maximo = fabs(Paso);
      if(fabs(Paso) < Paso_minimo) Paso_minimo = fabs(Paso);


      // Actualiza la actual condicion inicial
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = aux3[i];
      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso;
      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso);
      Numero_iteraciones ++;

      // Aumenta el tama¤o del Paso de integracion
      if(Norma < eps) {
         Paso *= 1.1;
         if(fabs(Paso) > Parametros_metodo[Sistema_actual][5][3]) {
            Paso = Parametros_metodo[Sistema_actual][5][3];
            if(!Integracion_positiva) Paso *= -1.0;
         }
      }

      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini, At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      // Guarda el ultimo valor calculado
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}






//////////////////////////////////////////////////////////////////////
//             Metodo Euler Modificado Paso variable                //
//////////////////////////////////////////////////////////////////////


void Metodos_numericos::Euler_modificado_p_v(void)
{
   unsigned int sw;
   double eps  = Parametros_metodo[Sistema_actual][6][2] * 0.7;
   Paso        = Parametros_metodo[Sistema_actual][6][3] / 1000.0;
   Paso_maximo = Paso_minimo = Paso;
   if(!Integracion_positiva) Paso *= -1.0;

   // Inicializa las variables
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   // Loop del metodo numerico
   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }

      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      // Loop para el control de avance del metodo numerico
      sw = 1;
      while (sw) {
         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
         t = At_Tiempo[Ventana_activa][Sistema_actual];

         // Calcula la nueva condicion inicial
///////////////////////////////////////////////////////////
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Calcula con la ecucion diferencial
             aux1[i] = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }
         // Actualiza valores temporales
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = (aux1[i] * (Paso * 0.5)) + At_Condicion[Ventana_activa][Sistema_actual][i];
         t += (Paso*0.5);

         // Calcula la siguiente condicion inicial
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Clacula con la ecuacion diferencial
             aux2[i]   = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux3[i] = (((aux1[i] + aux2[i]) * 0.5) * (Paso * 0.5)) + At_Condicion[Ventana_activa][Sistema_actual][i];
             #ifdef _VALIDA_CALCULO_
             // Controla el desbordamiento numerico
             if(fabs(aux3[i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
                if(Desbordamiento_numerico()) return;
             }
             #endif 
         }

         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux3[i];
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * 0.5);

         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Calcula con la ecucion diferencial
             aux1[i] = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }
         // Actualiza valores temporales
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = (aux1[i] * (Paso * 0.5)) + aux3[i];
         t += (Paso * 0.5);

         // Calcula la siguiente condicion inicial
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Clacula con la ecuacion diferencial
             aux2[i]   = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux4[i] = (((aux1[i] + aux2[i]) * 0.5) * (Paso*0.5)) + aux3[i];
             #ifdef _VALIDA_CALCULO_
             // Controla el desbordamiento numerico
             if(fabs(aux4[i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
                if(Desbordamiento_numerico()) return;
             }
             #endif 
         }

//////////////////////////////////////////////////////////
         // Actualiza valores con la nueva condicion calculada
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux4[i];
         t = At_Tiempo[Ventana_activa][Sistema_actual] + Paso;
         // Calcula la anterior condicion inicial
//////////////////////////////////////////////////////////
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Calcula con la ecucion diferencial
             aux1[i]  = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores temporales
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = (aux1[i] * (-Paso)) + aux4[i];
         t += (-Paso);

         xNorma = 0.0;
         // Calcula la siguiente condicion inicial
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             // Clacula con la ecuacion diferencial
             aux2[i]   = ( this->*funcion[Sistema_actual][i] )();
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux5[i] = (((aux1[i] + aux2[i]) * (-0.5)) * Paso) + aux4[i];
             // Calcula la distancia entre la condicion inicial calculada y la actual
             xNorma += pow((aux5[i] - At_Condicion[Ventana_activa][Sistema_actual][i]), 2.0);
         }
//////////////////////////////////////////////////////////


         // Revisa si avanza en la integracion o cambia el tama¤o del Paso
			Norma = sqrt(xNorma);
         if(Norma < Parametros_metodo[Sistema_actual][6][2]) sw = 0;
          else Paso *= 0.9; // Disminuye el tama¤o del Paso de integracion
      }

      // Almacena el paso maximo y el minimo
      if(fabs(Paso) > Paso_maximo) Paso_maximo = fabs(Paso);
      if(fabs(Paso) < Paso_minimo) Paso_minimo = fabs(Paso);


      // Actualiza la actual condicion inicial
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = aux4[i];
      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso;
      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso);
      Numero_iteraciones ++;

      // Aumenta el tama¤o del Paso de integracion
      if(Norma < eps) {
         Paso *= 1.1;
         if(fabs(Paso) > Parametros_metodo[Sistema_actual][6][3]) {
            Paso = Parametros_metodo[Sistema_actual][6][3];
            if(!Integracion_positiva) Paso *= -1.0;
         }
      }


      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini, At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      // Guarda el ultimo valor calculado
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}



//////////////////////////////////////////////////////////////////////
//                Metodo Runge-Kutta Paso variable                  //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Runge_Kutta4_p_v(void)
{
   unsigned int sw;
   double eps  = Parametros_metodo[Sistema_actual][7][2] * 0.7;
   Paso        = Parametros_metodo[Sistema_actual][7][3] / 1000.0;
   Paso_maximo = Paso_minimo = Paso;
   if(!Integracion_positiva) Paso *= -1.0;

   // Inicializa las variables
   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   // Loop del metodo numerico
   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }

      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      // Loop para el control de avance del metodo numerico
      sw = 1;
      while (sw) {
         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
         t = At_Tiempo[Ventana_activa][Sistema_actual];
////////////////////////////////////////////////////////////////////////
         // Calcula la nueva condicion inicial
         // Primer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux1[i] = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux1[i] * 0.5); 
         t += Paso * 0.25;

         // Segundo calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux2[i] = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux2[i] * 0.5); 
      
         // Tercer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux3[i] = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + aux3[i];
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * 0.5);

         // Cuarto Paso temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux4[i]  = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5); 
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux5[i] = (( aux1[i] + (2.0 * aux2[i]) + (2.0 * aux3[i]) + aux4[i] ) / 6.0) + At_Condicion[Ventana_activa][Sistema_actual][i];
             #ifdef _VALIDA_CALCULO_
             // Controla el desbordamiento numerico
             if(fabs(aux5[i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
                if(Desbordamiento_numerico()) return;
             }
             #endif
         }

         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux5[i];
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * 0.5);

         // Calcula la nueva condicion inicial
         // Primer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux1[i] = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux5[i] + (aux1[i] * 0.5); 
         t += Paso * 0.25;

         // Segundo calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux2[i] = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux5[i] + (aux2[i] * 0.5); 
      
         // Tercer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux3[i] = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux5[i] + aux3[i];
         t = At_Tiempo[Ventana_activa][Sistema_actual] + Paso;

         // Cuarto Paso temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux4[i]  = ( this->*funcion[Sistema_actual][i] )() * (Paso * 0.5); 
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux6[i] = (( aux1[i] + (2.0 * aux2[i]) + (2.0 * aux3[i]) + aux4[i] ) / 6.0) + aux5[i];
             #ifdef _VALIDA_CALCULO_
             // Controla el desbordamiento numerico
             if(fabs(aux6[i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
                if(Desbordamiento_numerico()) return;
             }
             #endif
         }


//////////////////////////////////////////////////////////////////////////
         // Actualiza valores con la nueva condicion calculada
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux6[i];
         t = At_Tiempo[Ventana_activa][Sistema_actual] + Paso;

/////////////////////////////////////////////////////////////////////////
         // Calcula la anterior condicion inicial
         // Primer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux1[i] = ( this->*funcion[Sistema_actual][i] )() * (-Paso);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux6[i] + (aux1[i] * (0.5)); 
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * 0.5);

         // Segundo calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux2[i] = ( this->*funcion[Sistema_actual][i] )() * (-Paso);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux6[i] + (aux2[i] * (0.5)); 
      
         // Tercer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux3[i] = ( this->*funcion[Sistema_actual][i] )() * (-Paso);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = aux6[i] + aux3[i];
         t = At_Tiempo[Ventana_activa][Sistema_actual];

         // Cuarto Paso temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux4[i]  = ( this->*funcion[Sistema_actual][i] )() * (-Paso);
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux7[i] = (( aux1[i] + (2.0 * aux2[i]) + (2.0 * aux3[i]) + aux4[i]) / 6.0) + aux6[i];
         }
/////////////////////////////////////////////////////////////////////////

         // Revisa si avanza en la integracion o cambia el tama¤o del Paso
         xNorma = 0.0;
         // Calcula la distancia entre la condicion inicial calculada y la actual
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) xNorma += pow((aux7[i] - At_Condicion[Ventana_activa][Sistema_actual][i]), 2.0); 
         Norma = sqrt(xNorma);
         // Controla el Paso de integracion
         if(Norma < Parametros_metodo[Sistema_actual][7][2]) sw = 0;
          else Paso *= 0.9; // Disminuye el tama¤o del Paso de integracion
      }

      // Almacena el paso maximo y el minimo
      if(fabs(Paso) > Paso_maximo) Paso_maximo = fabs(Paso);
      if(fabs(Paso) < Paso_minimo) Paso_minimo = fabs(Paso);

      // Actualiza la actual condicion inicial
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = aux6[i];
      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso;
      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso);
      Numero_iteraciones ++;

      // Aumenta el tama¤o del Paso de integracion
      if(Norma < eps) {
         Paso *= 1.1;
         if(fabs(Paso) > Parametros_metodo[Sistema_actual][7][3]) {
            Paso = Parametros_metodo[Sistema_actual][7][3];
            if(!Integracion_positiva) Paso *= -1.0;
         }
      }


      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini, At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      // Guarda el ultimo valor calculado
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}




//////////////////////////////////////////////////////////////////////
//                      Metodo Runge-Kutta-Fehlberg                  //
//////////////////////////////////////////////////////////////////////

void Metodos_numericos::Runge_Kutta_fehlberg(void)
{
   int sw;
   double delta, xdelta;
   Paso        = Parametros_metodo[Sistema_actual][8][3] / 1000.0 ;
   Paso_maximo = Paso_minimo = Paso;
   if(!Integracion_positiva) Paso *= -1.0;


   for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) At_Condicion[Ventana_activa][Sistema_actual][i] = c_ini[i];
   At_Tiempo[Ventana_activa][Sistema_actual] = Tiempo_actual;

   while(Tiempo_integracion < Parametros_metodo[Sistema_actual][Metodo_N_actual[Sistema_actual]][1]) {
      if(Reporte_activo) {
         sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
         Rep_num->Formato_impresion(3,1,C_imp);
      }

      // Dibuja la grafica con puntos
      if(Grafica_puntos[Sistema_actual]) Dibuja_punto(At_Condicion[Ventana_activa][Sistema_actual],At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      sw = 1;
      while(sw) {
         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i]; 
         t = At_Tiempo[Ventana_activa][Sistema_actual];

         // Primer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux1[i] = ( this->*funcion[Sistema_actual][i] )() * Paso;
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux1[i] * (1.0/4.0)); 
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * (1.0/4.0));

         // Segundo calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux2[i] = ( this->*funcion[Sistema_actual][i] )() * Paso;
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores 
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux1[i] * (3.0/32.0)) + (aux2[i] * (9.0/32.0)); 
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * (3.0/8.0));

      
         // Tercer calculo temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux3[i] = ( this->*funcion[Sistema_actual][i] )() * Paso;
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux1[i] * (1932.0/2197.0)) - (aux2[i] * (7200.0/2197.0)) + (aux3[i] * (7296.0/2197.0)) ; 
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * (12.0/13.0));

         // Cuarto paso temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux4[i]  = ( this->*funcion[Sistema_actual][i] )() * Paso;
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }

         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] + (aux1[i] * (439.0/216.0)) - (aux2[i] * 8.0) + (aux3[i] * (3680.0/513.0)) - (aux4[i] * (845.0/4104.0)) ; 
         t = At_Tiempo[Ventana_activa][Sistema_actual] + Paso;

         // Quinto paso temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux5[i]  = ( this->*funcion[Sistema_actual][i] )() * Paso;
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
         }
         // Actualiza valores
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) _X[i] = At_Condicion[Ventana_activa][Sistema_actual][i] - (aux1[i] * (8.0/27.0)) + (aux2[i] * 2.0) - (aux3[i] * (3544.0/2565.0)) + (aux4[i] * (1859.0/4104.0)) - (aux5[i] * (11.0/40.0)); 
         t = At_Tiempo[Ventana_activa][Sistema_actual] + (Paso * 0.5);
         delta = -1.0;
         sw = 0;

         // Sexto paso temporal
         for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) {
             aux6[i]  = ( this->*funcion[Sistema_actual][i] )() * Paso;
             // Controla los errores en el calculo matematico
             if(ERROR_MATEMATICO) {
                Error_calculo();
                return;
             }
             aux7[i] = fabs( (((1.0/360.0) * aux1[i]) - ((128.0/4275.0) * aux3[i]) - ((2197.0/75240.0) * aux4[i]) + ((1.0/50.0) * aux5[i])+ ((2.0/55.0) * aux6[i])) / Paso );
             // Se evita que sea cero
             if(aux7[i] < Parametros_metodo[Sistema_actual][8][2]) {
                // Se revisa que sea mayor que cero
                if(aux7[i] > 1.7e-308) {
                   // Calcula la delta
                   xdelta = 0.84 * pow(Parametros_metodo[Sistema_actual][8][2] / aux7[i] , (1.0/4.0));
                   if(xdelta > delta) delta = xdelta;
                } else delta = 1.7e+308;
             } else sw = 1;
         }

         // Revisa el cambio del paso de integracion
         if(sw) Paso *= 0.1;
      }
      // Revisa si es necesario incrementar el paso de integracion
      if(delta >= 10.0) Paso *= 1.5;
      if(fabs(Paso) > Parametros_metodo[Sistema_actual][8][3]) {
         Paso = Parametros_metodo[Sistema_actual][8][3];
         if(!Integracion_positiva) Paso *= -1.0;
      }

      // Almacena el paso maximo y el minimo
      if(fabs(Paso) > Paso_maximo) Paso_maximo = fabs(Paso);
      if(fabs(Paso) < Paso_minimo) Paso_minimo = fabs(Paso);

      // Actualiza las condiciones iniciales
      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++)  {
          At_Condicion[Ventana_activa][Sistema_actual][i] += ((25.0/216.0) * aux1[i]) + ((1408.0/2565.0) * aux3[i]) + ((2197.0/4104.0) * aux4[i]) - ((1.0/5.0) * aux5[i]);
          #ifdef _VALIDA_CALCULO_
          // Controla el desbordamiento numerico
          if(fabs(At_Condicion[Ventana_activa][Sistema_actual][i]) > MAX_VALOR[Ventana_activa][Sistema_actual]) {
             if(Desbordamiento_numerico()) return;
          }
          #endif
      }


      // Actualiza el tiempo
      At_Tiempo[Ventana_activa][Sistema_actual] += Paso;

      // Actualiza el numero de iteraciones
      Tiempo_integracion += fabs(Paso);
      Numero_iteraciones++;

      // Dibuja la grafica con lineas
      if(!Grafica_puntos[Sistema_actual]) Dibuja_linea(c_ini,At_Condicion[Ventana_activa][Sistema_actual],Tiempo_actual,At_Tiempo[Ventana_activa][Sistema_actual],Graficar_dos_ventanas);

      #ifdef _SUSPENDE_INTEGRACION_
      // Detiene la iteracion con la tecla [ESC]
      if(!(Numero_iteraciones % NUMERO_INTEGRACIONES_REVISAR) || Numero_iteraciones == 1) {
         if(Suspende_integracion()) break;
      }
      #endif

      for(i = 0; i < Numero_ecuaciones[Sistema_actual]; i++) c_ini[i] = At_Condicion[Ventana_activa][Sistema_actual][i];
      Tiempo_actual = At_Tiempo[Ventana_activa][Sistema_actual];
   }
   if(Reporte_activo) {
      sprintf(C_imp,"%1.6e %1.6e %1.6e %1.6e %1.6e %1.6e" ,Paso,At_Tiempo[Ventana_activa][Sistema_actual],At_Condicion[Ventana_activa][Sistema_actual][0],At_Condicion[Ventana_activa][Sistema_actual][1],At_Condicion[Ventana_activa][Sistema_actual][2],At_Condicion[Ventana_activa][Sistema_actual][3]);
      Rep_num->Formato_impresion(3,1,C_imp);
   }
}


void Metodos_numericos::Error_calculo(void)
{
   NCO->Mensaje(TXT10,TXT11);
   ERROR_MATEMATICO = 0;
}


int Metodos_numericos::Desbordamiento_numerico(void)
{
   if(NCO->Opcion(TXT01,TXT02,TXT03) != 1) return 1;
   return 0;
}

int Metodos_numericos::Suspende_integracion(void)
{
   char xnum[100], xcad[100], *xtext; 
   if(Integracion_positiva) xtext = TXT12;
    else xtext = TXT13;
   sprintf(xnum,"%lu",Numero_iteraciones);
   s_formato(xnum,6,0,1,xcad);
   sprintf(xnum,xtext,xcad);
   NCO->Visualiza_texto(xnum);
   if(NCO->Busca_bufer_teclado(ESC)) return 1;
   return 0;
}




