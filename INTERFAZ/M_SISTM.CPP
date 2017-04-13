// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#include "m_sistem.hpp"
#include "nucleo.hpp"
#include "r_var.hpp"
#include "cad_car.hpp"
#include "r_esp.hpp"
#include "teclado.hpp"

extern "C" {
	#include <string.h>
}

// Definicion externa del objeto Nucleo
extern Nucleo *NCO;
// Indica cual texto explicativo a visualizar
extern unsigned int N_texto_explicativo;


#ifdef _IDIOMA_ESPANOL_

	#define TXT1        "DESCRIPCION DEL SISTEMA"
	#define TXT2        "SISTEMAS"
	#define TXT3        "NUEVO SISTEMA"
	#define TXT4        "Nombre del sistema:"
	#define TXT5        "Excede el m ximo n£mero de sistemas"
	#define TXT6        "por proyecto"

	static const char *M_sistemas[] = {
		"Nuevo Sistema",
		"Borra Sistema",
		"Duplica Sistema",
		"Renombra Sistema",
		"Edita Ecuaciones",
		"Imprime Sistema",
		"0"
	};

	static const char *C_M_sistemas = "NBDREI";

	static const char *M_D_sistemas[] = {
		"Edita Descripci¢n",
		"Grabar Como ...",
		"Imprime Descripci¢n",
		"Copia al Portapapeles",
		"0"
	};

	static const char *C_D_sistemas = "EGIC";

#else

	#define TXT1        "SYSTEM DESCRIPTION"
	#define TXT2        "SYSTEMS"
	#define TXT3        "NEW SYSTEM"
	#define TXT4        "System name:"

	static const char *M_sistemas[] = {
      "New System",
      "Delete System",
      "Copy System",
      "Rename System",
      "Edit Ecuations",
      "Print System",
      "0"
   };

   static const char *C_M_sistemas = "NDCREP";

   static const char *M_D_sistemas[] = {
      "Edit Description",
      "Save as ...",
      "Print Description",
      "Copy to buffer",
      "0"
   };

   static const char *C_D_sistemas = "ESPC";

#endif




// Costructor del selecionador de sistemas
void Selecciona_sistema::Parametros(const int x1,const int y1,const int x2,const int y2)
{
	Ctrl_estructura::Libera_memoria();
   X1 = x1, Y1 = y1, X2 = x2, Y2 = y2;
   x_elem_menu = Ipresionado = 0;
   Sistemas = NULL;
   Arr_menu = NULL;
   // Define los iconos a mostrar
   Iconos = new Icono[12];
   Iconos[0].Define_icono("NUEVO.ICO",10,29);
   Iconos[1].Define_icono("BASURERO.ICO",60,29);
   Iconos[2].Define_icono("COPY.ICO",110,29);
   Iconos[3].Define_icono("RENAME.ICO",160,29);
   Iconos[4].Define_icono("EDITA.ICO",210,29);
   Iconos[5].Define_icono("EDITA.ICO",260,29);
   Iconos[6].Define_icono("MAKE.ICO",310,29);
   Iconos[7].Define_icono("PRINTER.ICO",360,29);
   Iconos[8].Define_icono("SAVE.ICO",410,29);
   Iconos[9].Define_icono("ARCHIV.ICO",460,29);
   Niconos = 10;
   // Apertura de la ventana descripcion
   VDC = new Ventana_titulo;
   VDC->Define_ventana(TXT1,X1+300, Y1+80, X2-40, Y2-70);

   // Ventana indicadora de la ventana de descripci¢n
   VDCID = new Traza_indicador;
   VDCID->Define_indicador(X2-86, Y1+110, X2-86, Y2-80);
   // Parametros para la visualizacion de la descripcion
   Va = new Visualiza_archivo;
   Va->Parametros(X1+311,Y1+111,X2-86,Y2-81,"");
   // Apertura de la ventana de sistemas
   VMA = new Ventana_titulo;
   VMA->Define_ventana(TXT2,X1+40, Y1+80, X1+280, Y2-70);
   // Ventana indicadora de la ventana de sistemas
   VMAID = new Traza_indicador;
   VMAID->Define_indicador(X1+226, Y1+110, X1+276, Y2-80);
   // Parametros del menu
   M1 = new Menu;
   M1->Define_ventana_menu(X1+50, Y1+110, X1+225, Y2-80,Blanco);
   // Carga los sistemas
   Carga_sistemas();
   // Visualiza los iconos y ventanas de trabajo
   Visualiza();
}



// Carga los sistemas
void Selecciona_sistema::Carga_sistemas(void)
{
   char sistema[80];
   unsigned int i, xi;
   // Revisa el numero de proyectoas a cargar
   if(Sistemas) delete [] Sistemas;
   if(Arr_menu) delete [] Arr_menu;
   // Inicializacion de arreglo de archivos
   Sistemas = new char [Informacion->Numero_sistemas*(LG_MAX_NOM_SISTEMA+1)];
   Arr_menu = new const char *[Informacion->Numero_sistemas+2];
   Opcion_actual_menu = 0;
   // Carga el nombre de los sistemas
   for(i = 0; i < Informacion->Numero_sistemas; i++) {
      // Obtiene el nombre del archivo
      strcpy(sistema,Informacion->Inf_sistema[i]->Nombre);
      // Copia el nombre de archivo al arreglo temporal
      for(xi = 0; xi < strlen(sistema); xi++) Sistemas[i*LG_MAX_NOM_SISTEMA+xi] = sistema[xi];
      Sistemas[i*LG_MAX_NOM_SISTEMA+xi] = 0;
      // Asigna el puntero del nombre de archivo al arreglo de punteros
      Arr_menu[i] = &Sistemas[i*LG_MAX_NOM_SISTEMA];
   }
   M1->Parametros_menu(Arr_menu,"","",Informacion->Numero_sistemas);
   Carga_descripcion_sistema();
}


// Carga la descripcion del sistema
void Selecciona_sistema::Carga_descripcion_sistema(void)
{
   unsigned int i;
   // Carga la descripcion del sistema
   Va->Borra_contenido_editor();
   if(Informacion->Numero_sistemas) {
      // Carga la descripcion al visualizador
      if(Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Descripcion) {
         for(i = 0; i < Informacion->Inf_sistema[Opcion_actual_menu]->N_e_Descripcion; i++) {
             Va->Adiciona_linea(1,Informacion->Inf_sistema[Opcion_actual_menu]->Descripcion[i],2);
         }
      } else Va->Adiciona_linea(0,"",2);
   } else Va->Adiciona_linea(0,"",2);
   Va->Posicionarse(1,1);
}


// Visualiza iconos asi como ventanas de trabajo
void Selecciona_sistema::Visualiza(void)
{
   // Dibuja a los iconos
   for(int i = 0; i < Niconos; i++) Iconos[i].Dibuja();
   // Dibuja el manu de seleccion del sistema
   VMA->Dibuja();
   VMAID->Dibuja();
   M1->Dibuja();
   // Dibuja la visualizacion de la descripcion
   VDC->Dibuja();
   VDCID->Dibuja();
   Va->Revisualiza_pantalla();
}


// Controla al selecionador de sistemas
void Selecciona_sistema::Controlador(char caracter, int tecla)
{
   int opc, i;
   unsigned int sw = 0;
   Ipresionado = 0;
   // Teclas de accion rapida
   switch(tecla) {
      case INS:
      case CTRL_N: // Nuevo sistema
        Ipresionado = 1, tecla = -1;
        break;
      case DEL:    // Borra el sistema
      case CTRL_Y:
        Ipresionado = 2, tecla = -1;
        break;
      case CTRL_C: // Duplica el sistema
        Ipresionado = 3, tecla = -1;
        break;
      case CTRL_R: // Renombra el sistema
        Ipresionado = 4, tecla = -1;
        break;
      case CTRL_E: // Edita ecuaciones
        Ipresionado = 5, tecla = -1;
        break;
      case CTRL_D: // Edita descripcion del sistema
        Ipresionado = 6, tecla = -1;
        break;
      case CTRL_M: // Crea el archivo exe
        Ipresionado = 7, tecla = -1;
        break;
      case CTRL_P: // Imprime el sistema
        Ipresionado = 8, tecla = -1;
        break;
      case CTRL_S: // Graba el proyecto
        Ipresionado = 9, tecla = -1;
        break;
      case CTRL_B: // Retorna al menu anterior
        Ipresionado = 10, tecla = -1;
        break;
   }
   // Controlador de la descripcion
   opc = VDCID->Controlador(Va->Linea_actual(),Va->Total_lineas(),Va->Lineas_desplegar());
   if(opc == 1) tecla = FLECHA_ARRIBA;
   if(opc == 2) tecla = FLECHA_ABAJO;

   // Activa el menu de opciones
   if(NCO->Retorna_estado_mouse().button == BL) {
      vent.y = NCO->Retorna_estado_mouse().y;
      vent.x = NCO->Retorna_estado_mouse().x;

      // Menu de sistemas
      if(VMA->Ventana::Mouse_dentro()) {
         if(Opcion_menu(M_sistemas,C_M_sistemas,selec,vent)) Ipresionado = selec == 6 ? 8 : selec;
      }
      // Menu de descripcion del proyecto
      if(VDC->Ventana::Mouse_dentro()) {
         if(Opcion_menu(M_D_sistemas,C_D_sistemas,selec,vent)) {
            if(selec == 1) Ipresionado = Informacion->Numero_sistemas ? 6 : 1;
            switch(selec) {
               case 1: Ipresionado = Informacion->Numero_sistemas ? 6 : 1; break;
               case 2: tecla = CTRL_F_W; break; // Grabar como ...
               case 3: tecla = CTRL_F_P; break; // Imprime el archivo
               case 4: tecla = CTRL_F_C; break; // Copia el archivo al portapapeles
            }
         }
      }
   }

   // Controla el movimiento de visualizacion del archivo
   if(VDC->Mouse_dentro()) Va->Controlador(tecla);
   // Controlador de menu
   opc = VMAID->Controlador(M1->Opcion_actual_menu(),M1->Retorna_total_elementos(),M1->Retorna_elementos_paneo());
   if(opc == 1) tecla = FLECHA_ARRIBA;
   if(opc == 2) tecla = FLECHA_ABAJO;
   // Controla el movimiento del menu
   if(VMA->Mouse_dentro()) M1->Controlador_menu(caracter,tecla,sw);
   if(sw) Ipresionado = Informacion->Numero_sistemas ? 5 : 1;
   // Revisa el comportamiento de los iconos
   for(i = 0; i < Niconos; i++) {
      // Revisa si algun icono fue presionado
      if(Iconos[i].Oprimido()) Ipresionado = i + 1;
      // Revisa sobre que icono esta el mouse
      if(Iconos[i].Mouse_dentro()) N_texto_explicativo = i + 15;
   }
   // Revisa si el mouse esta dentro de las ventanas
   if(VMA->Mouse_dentro()) N_texto_explicativo = 13;
   if(VDC->Mouse_dentro()) N_texto_explicativo = 14;
   // Carga la descripcion de cada sistema
   if(M1->Opcion_actual_menu()-1 != Opcion_actual_menu) {
      Opcion_actual_menu = (int) M1->Opcion_actual_menu()-1;
      Carga_descripcion_sistema();
      Va->Revisualiza_pantalla();
   }
   // Revisa que existan sistemas para poder operar sobre el
   if(!Informacion->Numero_sistemas && Ipresionado > 1 && Ipresionado < Niconos) {
      Ipresionado = 0;
   }
   // Indica que se edite la descripcion del sistema
   if(Va->Ventana::Mouse_dentro() && NCO->Retorna_estado_mouse().button == BR) Ipresionado = Informacion->Numero_sistemas ? 6 : 1;
}


// Adiciona un nuevo sistema al proyecto
void Selecciona_sistema::Nuevo(void)
{
	if(Informacion->Numero_sistemas >= NUMERO_SISTEMAS) {
		NCO->Mensaje(TXT5,TXT6);
		return;
	}
	char cad[80];
	strcpy(cad,"NONAME");
	if(Captura_cadena(TXT3,TXT4,cad,LG_MAX_NOM_SISTEMA,0) == 1) {
		s_trim(cad);
		if(Valida_nombre_sistema(cad)) {
			// Adiciona un nuevo sistema al proyecto
			Informacion->Inf_sistema[Informacion->Numero_sistemas] = new Informacion_sistema;
			// Obtiene la longitud del nombre del sistema
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->Nombre = new char[LG_MAX_NOM_SISTEMA+1];
			// Copia al nombre del nuevo sistema
			strcpy(Informacion->Inf_sistema[Informacion->Numero_sistemas]->Nombre,cad);
			// Inicializa los datos del sistema
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->N_e_Descripcion = 0;
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->N_e_Ecuaciones  = 0;
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->N_e_Parametros  = 0;
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->N_e_Func_aux    = 0;
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->Descripcion     = NULL;
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->Ecuaciones      = NULL;
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->Parametros      = NULL;
			Informacion->Inf_sistema[Informacion->Numero_sistemas]->Func_aux        = NULL;
			// Actualiza el numero de sistemas en el proyecto
			Informacion->Numero_sistemas++;
			Informacion->Modificada = 1;
			Selecciona_sistema::Carga_sistemas();
		}
	}
}


