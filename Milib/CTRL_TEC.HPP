// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __CTRL_TEC_HPP__
#define __CTRL_TEC_HPP__


// Definicion de la clase que controla al teclado
class Ctrl_teclado {

   private:

      unsigned int  Insert:                 1; // Estado de Insert 
      unsigned int  LONGITUD_BUFER_TECLADO;    // Longitud del bufer de teclado
      unsigned int  i;                         // Variables de uso general
               int *BUFER_TECLADO;             // Bufer del teclado
               int  Press;                     // Variable utilizada para control de teclas conjuntas
               int  mod, tp;                   // Variable de uso general

   protected:

                    // Lee bufer del teclado
      void          Lee_bufer_teclado(void) const;
                    // Inicializa el bufer de teclado
      void          Inicializa_bufer_teclado(const unsigned int longitud_bufer);
                    // Borra el bufer de teclado
      void          Borra_bufer_teclado(void) const
                    {delete []BUFER_TECLADO;}


   public:

                    // Limpia el bufer de teclado
      void          Limpia_bufer_teclado(void) const;
                    // Almacena un caracter en el bufer de teclado
      void          Almacena_bufer_teclado(const int tec) const;
                    // Almacena una cadena en el bufer de teclado
      void          Almacena_cadena_bufer_teclado(const char *cad) const;
                    // Busca una tecla en el bufer de teclado
      int           Busca_bufer_teclado(const int tec) const;
                    // Retorna el ultimo caracter o tecla del bufer de teclado
      void          Retorna_bufer_teclado(char &caracter, int &tecla) const;
                    // Retorna el caracteractual del bufer (sin quitarlo del bufer)
      void          Actual_tecla_bufer(char &caracter, int &tecla) const;
                    // Retorna el estado de Insert
      unsigned int  Estado_Insert(void) const
                    {return Insert;}
};

#endif

