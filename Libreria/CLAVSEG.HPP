// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#include "cadenas.hpp"

#ifndef __CLAVSEG_HPP__
#define __CLAVSEG_HPP__



class Clave_seguridad: virtual public Cadenas 
{
    private:

       char Clave[100];
       char Encriptada[100];
       char Desencriptada[100];
       
       void          Ctrl_proceso(const bool st);

    public:
                     // Constructores
                     Clave_seguridad(void);

       const char   *Encripta(const char *cv);
       const char   *Desencripta(const char *cv);
       
};


#endif 

