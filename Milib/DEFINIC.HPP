// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


                                                                           
#ifndef __DEFINIC_HPP__
#define __DEFINIC_HPP__


// Coordenadas en 2 dimenciones
struct C_2D {
   double x;
   double y;
};


// Coordenadas en 3 dimenciones
struct C_3D {
   double x;
   double y;
   double z;
};


// Cordenadas dobles en formato entero
struct C_2I {
   int x;
   int y;
};

#ifndef C_3Idefinicion
#define C_3Idefinicion
// Esta declaracion esta en el archivo C:\integra\sys\include\def_user.hpp
// Cordenadas triples en formato entero
struct C_3I {
   int x;
   int y;
   int z;
};
#endif

struct C_ptl {
   long x;
   long y;
};


// Coordenadas en ventana
struct C_dbl {
   int x1;
   int y1;
   int x2;
   int y2;
};



#endif

