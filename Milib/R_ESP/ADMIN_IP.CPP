// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


//////////////////////////////////////////////////////////
// Clase Administradora de impresion                    //
//////////////////////////////////////////////////////////


#include "admin_ip.hpp"
#include "nucleo.hpp"
#include "r_esp.hpp"

extern "C" {
   #include <stdio.h>
   #include <bios.h>
}

// Objeto global al nuclo del sistema
extern Nucleo *NCO;


#ifdef _IDIOMA_ESPANOL_
   #define TXT1     "Imprimiendo p gina n£mero: %u"
   #define TXT2     "Error en el dispocitivo de impresi¢n"
   #define TXT3     "Por favor revise la impresora"
#else
   #define TXT1     "Printing page number: %u"
   #define TXT2     "Error in printing device"
   #define TXT3     "Check printing machine, please"
#endif


#define LONG_BUFF 300


// Inicializa la clase
void Administrador_impresion::Inicializa_Administrador_impresion(void)
{
   Cad_tmp = NULL;
   Linea = Pagina = 1;
   st = Activo = 0;
   Puerto = LPT1;
}


// Destructor de la clase
void Administrador_impresion::Borra_Administrador_impresion(void)
{
   delete []Cad_tmp;
   if(Activo) delete La;
   Linea = Pagina = 1;
   Activo = 0;
}


// Parametros de la clase
void Administrador_impresion::Parametros_Administrador_impresion(const char *arch, const unsigned int puerto)
{
   Cad_tmp = new char[LONG_BUFF+10];
   La = new Manipulador_archivos;
   La->Parametros(arch,LEE_ARCHIVO,LONG_BUFF,"");
   Activo = 1, Puerto = puerto;
}


// Imprime el archvo
void Administrador_impresion::Controlador_Administrador_impresion(void)
{
   st = 0;
   if(La->Retorna_estado() != 2) {
      La->Lee_linea(Cad_tmp);
      Imprime_cadena(Cad_tmp);
   } else st = 1;
   if(st) Borra_Administrador_impresion();
}


// Imprime la cadena CAD en la impresora
void Administrador_impresion::Imprime_cadena(const char *cad)
{
   // Controla el estado de la impresora
   if(!Ctrl_impresora()) {
      i = 0;
      while(cad[i]) {
         biosprint(0,cad[i],Puerto);
         i++;
      }
      biosprint(0,13,Puerto);
      biosprint(0,10,Puerto);
      if(Linea >= 66) {
         Linea = 1, Pagina ++;
//      if(sw != Pagina) {
//         sprintf(xcad,TXT1,Pagina);
//         NCO->Visualiza_texto(xcad);
//         sw = Pagina;
      } else Linea ++;
   } else st = 1;
}


// Revisa que el puerto de impresion este trabajando correctamente
int Administrador_impresion::Ctrl_impresora(void)
{
///////////////////////////////////////
// falta controlar errores           //
///////////////////////////////////////
   int status = 0;
   status = biosprint(2,0,Puerto);
   while(status != 144) {
      status = biosprint(2,0,Puerto);
      if(status == 136 || status == 40 || status == 8 || status == 48) {
         status = NCO->Opcion("",TXT2,TXT3);  // Se solicita intervencion del usuario
         if(status == 1) return 1;  // Usuario cancela la impresi¢n
      }
      if(!Programa_activo) return 1; // Aborta programa
   }
   return 0;  // Sin error
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/*
void inv_puerto_lpt(void);
   // Rutina que invierte los puestros de impresion LPT1 y LPT2
   // Nombre del archivo: INV_LPT.CPP



void inv_puerto_lpt(void){
   unsigned int direccion;
   direccion = peek(0x40,0x08);      // Obtiene direccion de lpt1
   poke(0x40,0x08,peek(0x40,0x0A));  // Copia direccion de lpt2 a lpt1
   poke(0x40,0x0A,direccion);        // Copia direccion de lpt1 a lpt2
}
*/

/*
  // Program to demonstrate checking the parallel ports to see if
  // it is ready to print.  Some other info about the printer is
  // shown in the process.   If you check the port in this way,
  // you won't have to wait for a timeout error when you try to
  // write to the printer and fail.

  #include <dos.h>
  #include <stdio.h>
  #include <bios.h>

  // these are various single bits in the status byte returned
  // from the printer.
  #define TIMEOUT 0x01
  // bits 1 and 2 are not used.
  #define PRERROR 0x08
  #define ONLINE  0x10
  #define NOPAPER 0x20
  #define ACKNOWL 0x40
  #define NOTBUSY 0x80

  void ShowStatus( unsigned printerID );

  main()
  {
      int i;
      for (i=0; i<3; ++i)
      ShowStatus(i);
      return 0;
  }

  void ShowStatus(unsigned printerID)
  {
      // pointer to BIOS data area which has port addresses of printers
      static unsigned far *biosdata =
      (unsigned far *) MK_FP(0x0040,0x0008);
      unsigned char status;

      printf("\nPrinter Port LPT%d:\n",printerID+1);
      // the port address is set to 0 if the printer is not installed
      if (*(biosdata+printerID)==0)
      {
          printf("Not Installed\n");
          return;
      }

      // we'll use interrupt 0x17, function 1 (Initialize Printer) to check the status of the printer.
      status = biosprint(1,0,printerID);

      // display some info about the printer (optional)
      if (status & TIMEOUT) printf("Timeout\t");
      else printf("No Timeout\t");
      if (status & PRERROR) printf("Error\t");
      else printf("No Error\t");
      if (status & ONLINE)  printf("Online\t");
      else printf("Not Online\t");
      if (status & NOPAPER) printf("\nOut of Paper\t");
      else printf("\nHas Paper\t");
      if (status & ACKNOWL) printf("No Acknowledge\t");
      else printf("Acknowledge\t");
      if (status & NOTBUSY) printf("Not Busy\n");
      else printf("Busy\n");

      // ONLINE and NOTBUSY must both be ON for the printer to be  OK to print to.
      if ((status & (ONLINE|NOTBUSY)) == (ONLINE|NOTBUSY))
      printf("READY to Print\n");
      else printf("NOT READY to Print\n");
  }

*/

