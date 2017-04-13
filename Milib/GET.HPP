// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  2.1-A

#ifndef __GET_H__
#define __GET_H__


#include "ctrl_mov.hpp"
#include "cadenas.hpp"
#include "fechas.hpp"
#include "tiempo.hpp"
#include "v_marco.hpp"


class Get: public Ventana_marco, public Fechas, public Tiempo
{
   private:

      int   Lg_cad;                            // Guarda la longitud de la cadena a capturar
      char *Cad_org;                           // Guarda la cadena original
      char *Cad_tmp;                           // Guarda la cadena de trabajo
      char *Car_adi;                           // Caracteres adicionales para la captura
      char *Car_exc;                           // Caracteres excluidos para la captura
      char *Cad_vis;                           // Contiene la cadena a visualizar
      char *Cad_ult_mod;
      Controlador_movimiento *Ctrl_mov;        // Controlador de movimiento
      unsigned int Limpiar_cadena:     1;      // Indica que si el primer caracter tecleado es distinto de uno de movimiento borrara el contenido de la cadena
      unsigned int sw_limpia_cad:      1;      // Controla la accion de Limpiar cadena
      unsigned int Todo:               1;      // Permite la captura de cualquier caracter
      unsigned int Mayusculas:         1;      // Indica que los caracteres alfabeticos seran combertidos en mayuscula
      unsigned int Minusculas:         1;      // Indica que los caracteres alfabeticos seran combertidos en minusculas
      unsigned int Alfanumericos:      1;      // Indica que solo se capturaran caracteres alfanumericos
      unsigned int Alfabeticos:        1;      // Indica que solo se capturaran caracteres alfabeticos
      unsigned int Password:           1;      // Indica que la cadena a capturar es de pasword
      unsigned int Caracteres_ad:      1;      // Indica que se tomaran en cuenta los caracteres especificados en la cadena CAR_ADICIONALES
      unsigned int Caracteres_ex:      1;      // Indica que se excluiran los caracteres especificados en la cadena CAR_EXCLUIDOS
      unsigned int Fecha:              1;      // Indica que se capturara una fecha
      unsigned int Hora:               1;      // Indica que se capturara una hora
      unsigned int Digitos:            1;      // Indica que solo se capturaran caracteres numericos
      unsigned int Digitos_sig:        1;      // Indica que solo se capturaran caracteres numericos y el signo menos o mas
      unsigned int Digitos_sig_pto:    1;      // Indica que solo se capturaran caracteres numericos , el signo menos o mas y punto decimal
      unsigned int Not_cientifica:     1;      // Indica que solo se capturaran caracteres numericos , el signo menos o mas, punto decimal asi como notacion cientifica
      unsigned int Expresion_art:      1;      // Indica que se capturara una expresion numerica (soporta operandos aritmeticos)
      unsigned int Expresion_art_fun:  1;      // Indica que se capturara una expresion numerica (soporta como operandos a funciones)
      unsigned int Captura_activa:     1;      // Indica si esta activa la captura o solo es permitido la visualizaci¢n
      unsigned int Get_activo:         1;      // Indica si el Get esta activo
      unsigned int Captura_valida:     1;      // Indica que la captura es valida o completa
      unsigned int sw:                 1;      // Variable de control de cursor
      unsigned int Not_exponencial:    1;      // Indica que convierta el numero a notacion exponencial
      int          N_car_vis;                  // Indica el numero de caracteres a visualizar

            // Edita la cadena
      int   Edita_cadena(const char car, const int tc);
            // Valida si el caracter corresponde al tipo de captura
      char  Valida_caracter_captura(const char car);
            // Copia la cadena a la que contiene la anterior modificacion
      void  Salva_anterior_modificacion(void);


   public:
            // Costructor y destructor de la clase
            Get(void);
           ~Get();
            // Asigna los parametros de la edicion
      void  Parametros(const char *cad, const int x1, const int y1, const int n_c_v, const char *temp="TKU", const char *car_ad="", const char *car_ex="");
      void  Parametros(const int num  , const int x1, const int y1, const int n_c_v);
      void  Parametros(const long num , const int x1, const int y1, const int n_c_v);
      void  Parametros(const double num, const int x1, const int y1, const int n_c_v, const unsigned int tp);
            // Dibuja la ventana del get
      void  Dibuja(void);
            // Realiza la edicion de la cadena
      void  Edita(const char car, const int tec);
            // Retorna la cadena editada
      void  Cadena_editada(char *cad);
      void  Cadena_editada(int &num);
      void  Cadena_editada(long &num);
      void  Cadena_editada(double &num);
            // Cambia el contenido de la cadena
      void  Cambia_cadena(const char *cad);
      void  Cambia_cadena(const int num);
      void  Cambia_cadena(const long num);
      void  Cambia_cadena(const double num);
            // Retorna la longitud de la cadena
      int   Longitud_cadena(void) const
            {return Lg_cad;}
            // Indica si la captura esta terminada (Valida la captura)
      int   Captura_completada(void);
            // Activa Get
      void  Activa_get(const unsigned int tp) const
            {Get_activo = tp;}
            // Activa captura en el Get
      void  Activa_captura_get(const int act) const
            {((Get*)this)->Captura_activa = act;}
};

#endif

