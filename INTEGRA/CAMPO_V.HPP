//////////////////////////////////////////////////////////////
//                                                          //
//              I N T E G R A    V E R. 2.01                //
//       (Analisador de sistemas din micos interativos)     //
//                                                          //
// Desarrollado para ambiente DOS en modo de video          //
// VGA y SVGA a 256 colores.                                //
//                                                          //
// Autores:                                                 //
//     Humberto Carrillo Calvet                             //
//     Antonio Carrillo Ledesma                             //
//     Luis Nava Fernandez                                  //
//                                                          //
// Desarrollado en:                                         //
//     Laboratorio de Dinamica no Lineal                    //
//     Facultad de Ciencias, UNAM, M‚xico D.F.              //
//     Tel‚fono 6 22-48-70                                  //
//     E-mail: dinamica@athena.fciencias.unam.mx            //
//                                                          //
// Propiedad intelectual, todos los derechos reservados     //
// conforme a la ley, registro en tr mite.                  //
// Revisi¢n  1.1-A                                          //
//                                                          //
//////////////////////////////////////////////////////////////



#ifndef __CAMPO_V__
#define __CAMPO_V__


#include "m_d_usr.hpp"


class Campo_vectorial: virtual public M_Definicion_usuario
{

   private:

             // Dibuja la cabeza de la flecha del campo vectorial
      void   Cabeza_flecha(const double ini[], const double fin[]);


   protected:

             // Configura el campo vectorial
      void   Configura_campo_vectorial(void);
             // Dibuja el campo vectorial
      void   Dibuja_campo_vectorial(const C_3D max, const C_3D min);
};



#endif

