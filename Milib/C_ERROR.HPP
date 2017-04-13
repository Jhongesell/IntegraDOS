// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __C_ERROR_HPP__
#define __C_ERROR_HPP__

// Clase de control de errores
 
class Control_Errores
{
   public:
           // Visualiza una ventana con el texto indicado para el error
      void Visualiza_error(const unsigned int  t_p, const char *text1, const char *text2 = "", const char *text3 = "", const char *text4 = "", const char *text5 = "");
           // Controla y visualiza el error ocurrido
      void Ctrl_error(const int xn_e, const char *xt1 = "", const char *xt2 = "");

           // Presenta una ventana hasta con 5 lineas de texto
      void Mensaje(const char *txt1, const char *txt2 = "", const char *txt3 = "", const char *txt4 = "", const char *txt5 = "");
           // Presenta una ventana hasta con 5 lineas de texto aceptando la opcion aceptar y Cancelar, retornando
           // (1)   Aceptar
           // (2)   Cancelar
           // (3)   Cerrado de la ventana
      int  Opcion(const char *txt1, const char *txt2 = "", const char *txt3 = "", const char *txt4 = "", const char *txt5 = "");
};


#endif

