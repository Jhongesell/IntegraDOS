// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifndef __GEN_REP_HPP__
#define __GEN_REP_HPP__

#include "man_arch.hpp"
#include "cadenas.hpp"


// Define el tipo de reporte a generar
#define CANCELADO            0
#define REPORTE_IMPRESO      1
#define REPORTE_GRABADO      2
#define REPORTE_VISUALIZADO  3
#define CONFIGURACION        4
// Definiciones para el puerto de impresion
#define LPT1                 0
#define LPT2                 1
#define LPT3                 2



// Clase manipuladora del generador de reportes
class Ctrl_reporte: public Cadenas
{

  private:

    unsigned int PUERTO:     2;    // Define el puerto de impresi¢n
    unsigned int Graba:      1;    // Variable de estatus de grabaci¢n activada en 1
    unsigned int T_REPORTE:  3;    // Variable que define el tipo de reporte REPORTE_IMPRESO, REPORTE_GRABADO, REPORTE_VISUALIZADO
    unsigned int MARGEN;           // Define el margen en el reporte
    unsigned int N_COLUMNAS;       // Numero maximo de caracteres por renglon
    unsigned int N_RENGLONES;      // Numero maximo de lineas por pagina
    unsigned int Columna;          // Numero actual de caracteres en el reporte
    unsigned int Renglon;          // Numero de linea actual en el reporte
    unsigned int Pagina;           // Numero de pagina dentro del reporte
    unsigned int N_Lineas_pagina;  // Numero de lines por pagina (Valor de referencia)
    unsigned int Error;            // Variable que indica el Error si es que existe
    unsigned int i,lg;             // Variables temporales
    char *N_archivo;               // Nombre del archivo para generar el reporte
    char *x_arr1;                  // Definici¢n del arreglo temporal de trabajo
    char *x_arr2;                  // Definici¢n del arreglo temporal de trabajo
    char *x_arr3;                  // Definici¢n del arreglo de trabajo
    char *xcad;                    // Cadena que indicara la pagina impresa actualmente
    Manipulador_archivos *Gr;      // Puntero al manipulador de archivos


                  // Graba o imprime una linea de texto
    void          Graba_arreglo(void) const;
                  // Seleciona el medio sobre el que se generara el reporte
    unsigned int  Menu(void);

  public:
                  Ctrl_reporte(void)
                  {
                     N_Lineas_pagina = 66;
                     Columna = Pagina = Renglon = 1;
                     Error = 0;  // (0) Normal, (1) Abortado por el usuario, (2) Falla general
                     N_archivo = NULL;
                     Gr = NULL;
                     T_REPORTE = REPORTE_VISUALIZADO;
                  }

                  // Destructor de la clase 
                 ~Ctrl_reporte();
                  // Selecciona los parametros del generador de reportes
    void          Parametros(const unsigned int margen,const unsigned int renglo,const unsigned int colum,const unsigned int t_rep,const unsigned int puerto, const char *archivo);
                  // Indica el estado del generador de reportes
    unsigned int  Estado(void) const
                  {return Error;}
                  // Asigna el contenido a la actual linea del reporte
    void          Formato_impresion(const unsigned int x, const unsigned int y, const char *text) const;
                  // Libera la actual pagina de impresion
    void          Libera_pagina(void) const;
                  // Asigna un caracter de control
    void          Caracter_ctrl(const char car) const;
                  // Indica el puerto de impresion activo
    void          Puerto(const unsigned int puerto) const
                  {((Ctrl_reporte*)this)->PUERTO = puerto;}
                  // Selecciona el margen dentro del reporte
    void          Margen(const unsigned int margen) const  
                  {((Ctrl_reporte*)this)->MARGEN = margen;}
                  // Selecciona la longitud de la pagina del reporte
    void          Longitud_pagina(const unsigned int n_renglones) const
                  {((Ctrl_reporte*)this)->N_RENGLONES = n_renglones;}
                  // Indica la actual columan dentro del reporte
    unsigned int  Columna_actual(void) const 
                  {return Columna;}
                  // Indica el actual renglon dentro del reporte
    unsigned int  Renglon_actual(void) const 
                  {return Renglon;}
                  // Fija el numero maximo de lineas por reporte
    void          Fija_max_lineas_pagina(const unsigned int lp)
                  {N_Lineas_pagina = lp;}
                  // Fija el tipo de reporte
    void          Fija_tipo_reporte(unsigned int tp)
                  {T_REPORTE = tp;}
};


#endif


