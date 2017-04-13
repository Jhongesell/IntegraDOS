// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.01-A


////////////////////////////////////////////////////////////////
//  Clase que controla al porta papeles                       //
//  Permite inicializar y controlar el porta papeles          //
////////////////////////////////////////////////////////////////


#ifndef __PORTA_PA_HPP__
#define __PORTA_PA_HPP__

class Porta_papeles {

   private:

       unsigned int   Activo:          1;  // Indica si esta activo el portapapeles
       unsigned int   Total_elementos;     // Total elementos del portapapeles
       unsigned int   Actual_elemento;     // Actual elemento del portapapeles
       unsigned int   i;                   // Variable temporal
       char         **Informacion;         // Puntero a la informacion 
       char          *Cad_temp;            // Cadena temporal

   protected:

                      // Inicializa el portapapeles
               void   Inicializa_porta_papeles(void);
                      // Borra el portapapeles
               void   Borra_porta_papeles(void);

   public:

                      // Crea el portapapeles con NUM_ELEM elementos
               void   Crea_porta_papeles(unsigned int num_elem);
                      // Inserta en el portapapeles la cadena CAD
               void   Inserta_porta_papeles(const char *cad);
                      // Retorna el contenido del portapapeles numero NUM_ELEM
      const    char  *Retorna_cadena_porta_papeles(unsigned int num_elem);
                      // Retorna el total de elementos del portapapeles
      unsigned int    Retorna_total_elementos(void) const
                      {return Total_elementos;}
                      // Retorna el actual elemento dentro del portapapeles
      unsigned int    Retorna_Actual_elemento(void) const
                      {return Actual_elemento;}
                      // Retorna el estado del portapapeles
      unsigned int    Retorna_Estado_porta_papeles(void) const
                      {return Activo;}
};


#endif


// El ejemplo esta en la clase nucleo 

