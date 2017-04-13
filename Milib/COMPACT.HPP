// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __COMPACT_HPP__
#define __COMPACT_HPP__

// Clase que compacta y descompacta cadenas que posean imagenes graficas
// usa tres tipos de compactacion  
//    Deja la cadena tal cual estaba
//    Compacta usando un byte como indice
//    Compacta usando dos bytes como indice



class Compacta {

   private:
      unsigned int    T_compactacion;

   public:
      unsigned int Longitud_memoria_compactacion(const char *cad, const unsigned int lg);
      void         Compacta_cadena(const char *cad, const unsigned int lg, char *cad_comp, unsigned int &lg_comp);
      unsigned int Longitud_memoria_descompactacion(const char *cad_comp, const unsigned int lg_comp);
      void         Descompacta_cadena(const char *cad_comp, const unsigned int lg_comp, char *cad, unsigned int &lg);
};

#endif

