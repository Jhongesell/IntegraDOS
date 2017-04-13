// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

#ifndef __PANTGRAF_HPP__
#define __PANTGRAF_HPP__

extern "C" {
   #include <conio.h>
}

// Defina la clase de pantalla grafica
class Pantalla_Grafica {
   private:

      unsigned Grabar: 1;                // Indica si se grabara la pantalla de texto
      int      Modo_Grafico;             // Indica el modo grafico
      int      Manejador_Grafico;        // Indica el manejador grafico
      char    *ARCH_TEMP;                // Nombre del archivo para salvar la pantalla texto
      struct   text_info i_s;            // Estructura del modo texto

   public:

           // Inicializa la pantalla grafiaca
      void Inicializa(const char *path,const int graba, const int svga, const int ct, const int cf);
           // Destruye a la pantalla grafica
      void Destruye(void);
};

#endif

