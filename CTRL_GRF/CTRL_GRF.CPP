////////////////////////////////////////////////////////////
//                                                          //
//              I N T E G R A    V E R. 2.01                //
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
//     e-mail: dinamica@athena.fciencias.unam.mx            //
//                                                          //
// Propiedad intelectual, todos los derechos reservados     //
// conforme a la ley, registro en tr mite.                  //
// Revisi¢n  1.1-A                                          //
//                                                          //
//////////////////////////////////////////////////////////////



#include "ctrl_grf.hpp"
#include "nucleo.hpp"
#include "teclado.hpp"
#include "vis_ayud.hpp"
#include "version.hpp"


extern Nucleo *NCO;                      // Definicion externa del objeto Nucleo
extern int Programa_activo;              // Indica si el programa esta o no activo
extern int Tecla;                        // Tecla actual del bufer
extern char Caracter;                    // Caracter actual del bufer
extern int X_MAX;                        // Dimenciones de la pantalla
extern int Y_MAX;                        // Dimenciones de la pantalla
extern unsigned int N_texto_explicativo; // Indica cual texto explicativo a visualizar



void GIF_DumpEga10(char *filename);



//////////////////////////////////////////////////////////////////
#include "prtgraph.h"
extern "C" {
   #include <dos.h>
   #include <math.h>
   #include <conio.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <stdarg.h>
   #include <string.h>
   #include <graphics.h>
   #include <assert.h>
   #include <alloc.h>
   #include <bios.h>
}



static unsigned  picwidth=8500,picheight=6375;
static int       leftmargin=500,topmargin=500, // 1000,1500
                 PicRotate=1, PicInverse=0;
static int  PRTno = 0;
static int  mode = 0;
//static char OutName[MAXPATH]="PRN";
//struct palettetype palette;      /* Used to read palette info  */
int static  printing=0;
int PRT_drv;
int PCXmode=0;
int ScreenPreview=0;


//int _PRT__pascal BuildingMsg( void far* UserPtr );
int _PRT__pascal StdUserPrtFunc( void far* UserPtr, PRT__handleT   *handlePPtr, const char far* BGIpath );
int harderr_handler(int errval,int ax,int bp,int si);
void interrupt CtrlBreak_handler(...);
// Imprime una imagen grafica
int Imprime_imagen(void far * UnusedPointer);



/*
int _scanf( const char * format, ... )
{  va_list ap;
   int     c,r;

   c=getchar();
   if ( c=='\n' )  return 0;
   va_start(ap,format);
   ungetc(c,stdin);
   r = vscanf (format,ap);  (void)getchar();
   va_end(ap);
   return r;
}
*/

//void AskOfParameters(void)
//{ 
//   int   c,MAXmode;
//   const char  *modename;
//   char  s[MAXPATH];

//   clrscr();
//   cprintf ( "\n\r  Nombre del dispocitivo de impresi¢n[%s]", OutName ); // Output device name
//   gets ( s ); if (*s!=0) strcpy(OutName,s);
//   cputs("\r\n");

/*
   PRT_SetOutName ( OutName );

   cputs ( "\r\n Seleccione el modo de impresi¢n\r\n" ); // Choose printer mode operation
   PRT_MaxMode ( PRTno, &MAXmode );
   for ( mode=0; mode<=MAXmode; mode++ )
   {
      PRT_ModeName(PRTno,mode,&modename );
      cprintf ( "        %2d - %s\r\n", mode, modename );
   }
   mode=MAXmode+1;
   do {
     c=getch();
     if (c==0) getch();
     else if ((unsigned)c-'0'<=(unsigned)MAXmode) mode=c-'0';
   }  while ( mode>MAXmode);
*/

//   cprintf ( "\n\r Picture width in 1/1000 inch [%d] ", picwidth );
//   _scanf ( "%d", &picwidth );
//   cprintf ( " Picture height in 1/1000 inch [%d] ", picheight );
//   _scanf ( "%d", &picheight );

//   cprintf ( " Top margin in 1/1000 inch [%d] ", topmargin );
//   _scanf ( "%d", &topmargin );
//   cprintf ( " Left margin in 1/1000 inch [%d] ", leftmargin );
//   _scanf ( "%d", &leftmargin );
//   cprintf ( " Rotate picture [%d] ", PicRotate );
//   _scanf ( "%d", &PicRotate );
//   cprintf ( " Inverse picture [%d] ", PicInverse );
//   _scanf ( "%d", &PicInverse );
//   cprintf ( " Screen Preview [%d] ", ScreenPreview );
//   _scanf ( "%d", &ScreenPreview );
//   cprintf ( " PCX mode [%d] ", PCXmode );
//   _scanf ( "%d", &PCXmode );
//}


void DrawAndPrint (void)
{  
   int   rc;
   int   PRT_mode=0;

   do {
      Imprime_imagen(NULL);
      if( printing ) {
         PRT_SetPrinterDrv ( PRTno, mode );
         PRT_SetPictureInch ( picwidth,picheight,( PicRotate ? PRT_ROTATE:0 ) | ( PicInverse ? PRT_INVERSE:0 ) );
         PRT_SetMargins ( leftmargin, topmargin );
         PRT_HaltPrinting = 0;
         rc=PRT_PrintBGI ( &PRT_drv, &PRT_mode,"C:\\INTEGRA\\SYS\\", Imprime_imagen,NULL);
         if( rc ) {  
            int  mode;
            mode=BGI_getgraphmode(Scrn_BGIgroup);
            restorecrtmode();
            cprintf ("\r\n error code %d (%s) from PRT_PrintBGI \r\n", rc,
                                    PRT_grapherrormsg(rc) );
            getch();
            BGI_setgraphmode(mode);
         }
         printing = 0;
      }
   }
   while(printing);
}

void  InitPBGI(void)
{
//   const char  *PRTname;
//   int   MaxPrinterNo;
   int      rc;

   rc=PRT_ReadDrivers(getenv("BGIPATH"),"C:\\INTEGRA\\SYS\\PRINTERS.DEF");
   if( rc!=0 ) {
      cputs ("\r\nNo se ecuentra el driver de definiciones\r\n" );
      exit(8);
   }
//   MaxPrinterNo = PRT_MaxDriver();
/*
   cputs ( "\r\n    Seleccione el tipo de impresora\r\n" );  // Choose printer type
   for ( PRTno=1; PRTno<=MaxPrinterNo; PRTno++ )
   {
      PRT_DriverName(PRTno,&PRTname );
      cprintf ( "        %2d - %s \r\n", PRTno, PRTname );
   }
   do {
      scanf( "%d",&PRTno);
      (void)getchar();
   } while ( PRTno>MaxPrinterNo || PRTno<=0 );
*/

   PRT_drv = DETECT; /* <-- needed if you don't want link BitImage BGI driver */
   PRT_drv = PRT_installuserdriver ( "BitImage", NULL );
   rc = PRT_registerfarbgidriver ( BitImage );

// AskOfParameters();

   PRT_SetUserPrintFunc(StdUserPrtFunc);
}




#ifdef __cplusplus
   void interrupt (*old_0x1b)(...);
#else
   void interrupt (*old_0x1b)(void);
#endif

void restore_0x1b(void)
{
   setvect(0x1b,old_0x1b);
}


// Imprime una imagen grafica
int Imprime_imagen(void far * UnusedPointer)
{
   unsigned int x = 0, y = 0, ren, car, numero,i;
   char color;
   // Limpia el area de impresion
   setfillstyle(1,Blanco);
   bar(0,0,X_MAX,Y_MAX);
   setbkcolor(Blanco);
   // Lee la imagen a imprimir
   Manipulador_archivos *Lee_pant  = new Manipulador_archivos;
   Lee_pant->Parametros("PRINT.$$$",LEE_ARCHIVO,0,"IMPRIME");
   Lee_pant->Lee_entero(ren);
   Lee_pant->Lee_entero(car);
   // Visualiza la imagen
   while(y <= ren) {
      Lee_pant->Lee_entero(numero);
      Lee_pant->Lee_caracter(color);
      for(i = 0; i < numero; i ++) {
          if(color != 15) putpixel(3 + x, 3 + y, Negro);
          x ++;
          if(x >= car) x = 0, y++;
      }
   }
   delete Lee_pant;
   if(biosprint(2,0,0) != 144) printing = 0;
    else printing = 1;
   return 0;
}



//////////////////////////////////////////////////////////////////

static const char *M_impresoras[] = {
   "9  Pins: STAR SG/SR",
   "9  Pins: IBM, EPSON",
   "9  Pins: EPSON",
   "9  Pins: PANASONIC KX-P1124",
   "9  Pins (Color): IBM, EPSON",
   "9  Pins (Color): EPSON",
   "24 Pins: Epson LQ1000",
   "24 Pins: IBM Pro X24",
   "24 Pins (Color): EPSON LQ860",
   "24 Pins (Color): IBM Pro X24",
   "PaintJet",
   "HP PaintJet",
   "HP LaserJet",
   "HP LaserJet II",
   "0"
};


// Archivos de ayuda
const char *M_AYUDA_ARCHIVO[] = {
   "C:\\INTEGRA\\SYS\\HELP\\CTRL_GRF.HLP",
   "C:\\INTEGRA\\SYS\\HELP\\FCTRL_GR.HLP",
   "C:\\INTEGRA\\SYS\\HELP\\AYUDAG.HLP",
   "C:\\INTEGRA\\SYS\\HELP\\ACERCADE.HLP",
};



#ifdef _IDIOMA_ESPANOL_
   // Menu de ayuda
   const char *M_AYUDA_MENU[] = {
      "Que es CTRL_GRF",
      "Manejo de CTRL_GRF",
      "Funciones Gen‚ricas",
      "Acerca de...",
      "0"
   };
   const char *M_AYUDA_MENU_TEC = "QMFA";

   static const char *M_ventana[] = {
      "Grabar (GIF)",
      "Grabar (IMG)",
      "Leer   (IMG)",
      "Imprimir",
      "Impresora",
      "Limpiar",
      "0"
   };

   char *P_menu = "RGLIMP";


   #define TXT7    "Acerca de ..."
   #define TXT8    "LEER IMAGEN"
   #define TXT9    "Leyendo imagen..."
   #define TXT10   "GRABAR IMAGEN"
   #define TXT11   "Grabando imagen..."
   #define TXT12   "Imprimiendo imagen..."
   #define TXT13   "Seleccione el dispositivo de impresi¢n"
   #define TXT21   "DUPLICAR IMAGEN"
   #define TXT22   "Nombre actual:"
   #define TXT23   "Nuevo nombre:"
   #define TXT24   "El archivo ya existe"
   #define TXT25   "RENOMBRAR IMAGEN"
   #define TXT26   "BORRAR IMAGEN"


   #define MS01    "Terminar el programa y retornar al DOS"
   #define MS02    "Visualiza la ayuda sobre el sistema"
   #define MS03    "Lee imagen generada por el sistema INTEGRA"
   #define MS04    "Graba la imagen actual"
   #define MS05    "Borra la imagen seleccionada"
   #define MS06    "Duplica la imagen"
   #define MS07    "Renombra la imagen"
   #define MS08    "Edita la imagen permitiendo inserci¢n de texto"
   #define MS09    "Imprime la imagen"
#else
   // Menu de ayuda
   const char *M_AYUDA_MENU[] = {
      "Que es CTRL_GRF?",
      "Manejo de CTRL_GRF",
      "Funciones Gen‚ricas",
      "About ...",
      "0"
   };
   const char *M_AYUDA_MENU_TEC = "QMFA";

   static const char *M_ventana[] = {
      "Save (GIF)",
      "Save (IMG)",
      "Read   (IMG)",
      "Print",
      "Printer",
      "Clean",
      "0"
   };

   char *P_menu = "ASRPRC";


   #define TXT7    "About ..."
   #define TXT8    "READ IMAGE"
   #define TXT9    "Reading image..."
   #define TXT10   "SAVE IMAGE"
   #define TXT11   "Saving image..."
   #define TXT12   "Printing image..."
   #define TXT13   "Select printing device"
   #define TXT21   "COPY IMAGE"
   #define TXT22   "Current name:"
   #define TXT23   "New name:"
   #define TXT24   "Existing file"
   #define TXT25   "RENAME IMAGE"
   #define TXT26   "DELETE IMAGE"


   #define MS01    "End program and return to DOS"
   #define MS02    "Visualize system help"
   #define MS03    "Read image generated by INTEGRA system"
   #define MS04    "Record current image"
   #define MS05    "Delete chosen image"
   #define MS06    "Copy image"
   #define MS07    "Rename image"
   #define MS08    "Edit image allowing text insertion"
   #define MS09    "Print image"
#endif



void main(void) //int argc, char *argv[])
{
//   harderr(Ctrl_error_harware);
//////////////////////////////////////////////////
   assert ( heapfillfree(0x12f7) > 0 );
   assert ( heapcheckfree(0x12f7) > 0 );
   assert ( heapcheck() > 0 );
   harderr(harderr_handler);
   old_0x1b =  getvect(0x1b);
   atexit(restore_0x1b);
   setvect(0x1b,CtrlBreak_handler);
//////////////////////////////////////////////
   int svga = 0; //, st = 0;

   // Revisa a que modo grafico iniciara la pantalla Grafica
//   if(argc == 2) {
//      svga = argv[1][0] - '0';
//      if(svga < 0 || svga > 4 || svga == 1) st = 1;
//   } else st = 1;
//   if(st) printf("\n\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",TXT1,TXT2,TXT3,TXT4,TXT5,TXT6,TXT7);
//    else {
       // Definicion del objeto interfaz
      Ctrl_grafico Cg;
      // Ejecuta la aplicacion interfaz
      Cg.Ejecuta(VER_CTRL_GRF,svga);
//   }
}


// Ejecuta la Aplicacion
void Ctrl_grafico::Proceso_inicial(void)
{
   // Define los iconos a mostrar
   Niconos = 7;
   Iconos = new Icono [Niconos];
   Iconos[0].Define_icono("LOAD.ICO",10,29);
   Iconos[1].Define_icono("SAVE.ICO",60,29);
   Iconos[2].Define_icono("BASURERO.ICO",110,29);
   Iconos[3].Define_icono("COPY.ICO",160,29);
   Iconos[4].Define_icono("RENAME.ICO",210,29);
   Iconos[5].Define_icono("EDITA.ICO",260,29);
   Iconos[6].Define_icono("PRINTER.ICO",310,29);

   Texto_explicativo = new char *[10];
   Texto_explicativo[0] = "";
   Texto_explicativo[1] = MS01;
   Texto_explicativo[2] = MS02;
   Texto_explicativo[3] = MS03;
   Texto_explicativo[4] = MS04;
   Texto_explicativo[5] = MS05;
   Texto_explicativo[6] = MS06;
   Texto_explicativo[7] = MS07;
   Texto_explicativo[8] = MS08;
   Texto_explicativo[9] = MS09;

   Archivo = new char[MAXPATH];
   archivo = new char[MAXPATH];
   Archivo[0] = archivo[0] =  0;
   // Dibuja la pantalla
   Dibuja_aplicacion();
   Imagen_cargada = 0;
}


// Controla a la aplicacion
void Ctrl_grafico::Controlador(void)
{
      Ipresionado = 0;
      for(int i = 0; i < Niconos; i++) {
          // Revisa si algun icono fue presionado
          if(Iconos[i].Oprimido()) Ipresionado = i + 1;
          // Revisa sobre que icono esta el mouse
          if(Iconos[i].Mouse_dentro()) N_texto_explicativo = i + 3;
      }

      // Activa el menu de la ventana
      if(Vt->Presionado(BL)) {
         vent.y = NCO->Retorna_estado_mouse().y;
         vent.x = NCO->Retorna_estado_mouse().x;
         if(Opcion_menu(M_ventana,P_menu,selec,vent,"")) {
            switch(selec) {
               case 1: // Grabar GIF
                  Tecla = CTRL_G;
                  break;
               case 2: // Grabar IMG
                  Ipresionado = 2;
                  break;
               case 3: // Leer IMG
                  Ipresionado = 1;
                  break;
               case 4: // Imprime imagen
                  Ipresionado = 7;
                  break;
               case 5: // Selecciona Impresora
                  Tecla = CTRL_I;
                  break;
               case 6: // Limpiar ventana
                  Caracter = 'C';
                  break;
            }
         }
      }


      // Control de procesos segun tecla presionada
      switch(Tecla) {
         case INSERT:
         case CTRL_L: // Cargar imagen
           Ipresionado = 1, Tecla = -1;
           break;
         case CTRL_S: // Graba imagen (IMG)
           Ipresionado = 2, Tecla = -1;
           break;
         case CTRL_G: // Graba imagen (GIF)
           if(Imagen_cargada) {
              if(Retorna_archivo_seleccionado(Archivo,0,TXT10
              ,".GIF","*.GIF")) {
                 // Gaba la imagen actual
                 GIF_DumpEga10(Archivo);
              }
           }
           break;
         case DEL:    // Borra imagen
         case CTRL_Y:
           Ipresionado = 3, Tecla = -1;
           break;
         case CTRL_C: // Duplica imagen
           Ipresionado = 4, Tecla = -1;
           break;
         case CTRL_R: // Renombra imagen
           Ipresionado = 5, Tecla = -1;
           break;
         case CTRL_E: // Edita imagen
           Ipresionado = 6, Tecla = -1;
           break;
         case CTRL_I: // Selecciona la impresora
           Visualiza_texto(TXT13);
           vent.x = (X_MAX -200) / 2, vent.y = (Y_MAX -180) / 2;
           if(Opcion_menu(M_impresoras,"SIEPCOLXQRJTAH",selec,vent,"")) PRTno = selec;
           break;
         case CTRL_P: // Imprime proyecto
           Ipresionado = 7, Tecla = -1;
           break;
      }
      // Controla los procesos seleccionador por los iconos
      switch(Ipresionado) {
         case 1: // Lee imagen
            Visualiza_texto(MS03);
            if(Retorna_archivo_seleccionado(Archivo,1,TXT8,".IMG","*.IMG")) {
                  Visualiza_texto(TXT9);
                  // Limpia la pantalla de trabajo
                  setfillstyle(1,Blanco);
                  bar(3,87,X_MAX-3,Y_MAX -31);
                  // Lee y visualiza la imagen
                  Lee_imagen_sin_formato(Archivo,3,80,X_MAX-3,Y_MAX -31);
                  Imagen_cargada = 1;
            }
            break;
         case 2: // Graba imagen
            if(Imagen_cargada) {
               Visualiza_texto(MS04);
               if(Retorna_archivo_seleccionado(Archivo,0,TXT10,".IMG",Archivo)) {
                     Visualiza_texto(TXT11);
                     // Gaba la imagen actual
                     Graba_imagen_sin_formato(Archivo,3,80,X_MAX-3,Y_MAX -31);
               }
            }
            break;
         case 3: // Borra imagen
           Visualiza_texto(MS05);
           if(Retorna_archivo_seleccionado(archivo,1,TXT26,".IMG","*.IMG")) {
              // Remueve el archivo especificado
              remueve_archivos(archivo,0);
           }
           break;
         case 4: // Duplica imagen
           if(Imagen_cargada) {
              Visualiza_texto(MS06);
              Duplica();
           }
           break;
         case 5: // Renombra imagen
           if(Imagen_cargada) {
              Visualiza_texto(MS07);
              Renombra();
           }
           break;
         case 6: // Edita imagen
           break;
         case 7: // Imprime imagen
           // Revisa si ya fue seleccionado algun tipo de impresora
           if(!PRTno) {
              Visualiza_texto(TXT13);
              vent.x = (X_MAX -200) / 2, vent.y = (Y_MAX -180) / 2;
              if(Opcion_menu(M_impresoras,"SIEPCOLXQRJTAH",selec,vent,"")) PRTno = selec;
           }
           // Revisa si hay imagen que imprimir
           if(Imagen_cargada && PRTno) {
              graba_pantalla_grafica(0,0,X_MAX,Y_MAX,"SAVEIMG.$$$");
              Visualiza_texto(TXT12);
              Graba_imagen_sin_formato("PRINT.$$$",3,80,X_MAX-3,Y_MAX -32);
              DrawAndPrint();
              lee_pantalla_grafica(0,0,"SAVEIMG.$$$");
           }
           break;
      }
      switch(Caracter) {
         case 'C': // Redibuja la pantalla
         case 'c':
            Dibuja_aplicacion();
            Imagen_cargada = 0;
            break;
      }

}


// Poceso final de la aplicacion
void Ctrl_grafico::Proceso_final(void)
{
   delete []Archivo;
   delete []archivo;
}


// Dibuja la ventana de la aplicacion
void Ctrl_grafico::Dibuja_aplicacion(void)
{
   // Dibuja la ventana con iconos
   Aplicacion::Dibuja();
   // Dibuja la area de iconos
   Vt->Limpia_recuadro(1,26,X_MAX-1,67,Gris1,Gris9,Blanco);
   // Dibuja a los iconos
   for(int i = 0; i < Niconos; i++) Iconos[i].Dibuja();
}


// Ventana de presentacion inicial
void Ctrl_grafico::Pantalla_presentacion(void)
{
   Visualiza_texto("Acerca de ...");
   Ventana_iconos *Vpresent = new Ventana_iconos;
   Vpresent->Activa_graba_ventana(1);
   Vpresent->Ventana_centrada(VER_CTRL_GRF,480,300,0);
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


// Pantalla de estado del sistema
void Ctrl_grafico::Pantalla_estado_sistema(void)
{

}



// Pantalla de terminacion
void Ctrl_grafico::Pantalla_terminacion(void)
{
}

// Redibuja pantalla
void Ctrl_grafico::Redibuja_pantalla(void)
{
   Dibuja_aplicacion();
}


// Duplica la actual imagen
void Ctrl_grafico::Duplica(void)
{
   char cad1[MAXPATH+10], cad2[MAXPATH+10];
   strcpy(cad1,Archivo);
   strcpy(cad2,Archivo);
   if(Captura_cadenas(TXT21,TXT22,cad1,TXT23,cad2,MAXPATH,0) == 1) {
      Manipulador_archivos ma;
      ma.Cambia_ext_path(cad2,"IMG",archivo);
      if(n_archivos_directorios(archivo,FA_ARCH)) NCO->Mensaje(TXT24,archivo);
       else ma.Copia_archivo(cad1,archivo); // Duplica el proyecto actual
   }
}      

// Renombra la actual imagen
void Ctrl_grafico::Renombra(void)
{
   char cad1[MAXPATH+10], cad2[MAXPATH+10];
   strcpy(cad1,Archivo);
   strcpy(cad2,Archivo);
   if(Captura_cadenas(TXT25,TXT22,cad1,TXT23,cad2,MAXPATH,0) == 1) {
      Manipulador_archivos ma;
      ma.Cambia_ext_path(cad2,"IMG",archivo);
      rename(cad1,archivo); // Renombra el actual proyecto
   }
}





