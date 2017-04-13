// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

///////////////////////////////////////////////////////
// Administrador de impresion                        //
///////////////////////////////////////////////////////


#ifndef __ADMIN_IP_HPP__
#define __ADMIN_IP_HPP__

// Definiciones para el puerto de impresion
#define LPT1                 0
#define LPT2                 1
#define LPT3                 2


#include "..\libreria\man_arch.hpp"


class Administrador_impresion {

   private:

      unsigned int          Activo:   1;      // Indica si esta activa la impresion
      unsigned int          Puerto:   3;      // Define el puerto de impresi¢n
      unsigned int          st:       1;      // Indica el estado en la impresion
      unsigned int          Pagina;           // Indica la pagina actual
      unsigned int          Linea;            // Indica la linea actual
      unsigned int          i;                // Indice de uso general
      Manipulador_archivos *La;               // Lee archivo a imprimir
      char                 *Cad_tmp;          // Cadena temporal


                   // Revisa que el puerto de impresion este trabajando correctamente
      int          Ctrl_impresora(void);


   protected:

                   // Inicializa la clase
      void         Inicializa_Administrador_impresion(void);
                   // Destructor de la calse
      void         Borra_Administrador_impresion(void);
                   // Lee e imprime el archvo
      void         Controlador_Administrador_impresion(void);


   public:

                   // Manbda a imprimir la cadena CAD 
      void         Imprime_cadena(const char *cad);
                   // Parametros de la clase
      void         Parametros_Administrador_impresion(const char *arch, const unsigned int puerto);
                   // Retorna estado del administrador de impresion
      unsigned int Retona_estado_Administrador_impresion(void) const
                   { return Activo;}
};

#endif


