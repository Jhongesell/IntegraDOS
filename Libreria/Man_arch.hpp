// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A


#ifndef __man_arch_hpp__
#define __man_arch_hpp__

#ifdef __PROG_DOS__
extern "C" {
#endif
   #include <stdio.h>
#ifdef __PROG_DOS__
}
#endif

#include "cadenas.hpp"


#define LEE_ARCHIVO       0
#define GRABA_ARCHIVO     1
#define ADICIONA_ARCHIVO  2


// Clase manipuladora del manipulador de archivos
class Manipulador_archivos: virtual public Cadenas   
{
  private:

    unsigned int     MODO:              2;        // Guarda el modo de apertura del archivo
    unsigned int     INICIALIZADO:      1;        // Indica si fue inicializado el archivo
    int              Estado;                      // Variable que indica el Estado del objeto
    char             car;                         // Variable de caracter temporal
    unsigned int     LG_MAX;                      // Indica la longitud maxima de la cadena a trabajar
    int              inttmp;                      // Variables temporal
    long             longtmp;
    unsigned int     uinttmp;
    unsigned char    cartmp;
    
    FILE            *ARCHIVO_TRABAJO;             // Puntero al archivo generado como reporte
    char            *ARCHIVO;                     // Nombre del archivo a trabajar
    char            *RUTINA;                      // Nombre de la rutina que llama al manipulador de archivos
    char            *DELIMITADOR;                 // Contiene el delimitador entre Lineas

  public:

         // Costructor y destructor de la clase
         Manipulador_archivos(void);
        ~Manipulador_archivos();

         // Parametros para el manipulador de archivos
    void Parametros(const char *archivo, const int modo, const unsigned int lg_max, const char *rutina = "");
         // Cuenta el numero de caracteres y lineas dentro del archivo si T_P es (1) si T_P es (0) cuenta la cantidad de caracteres reales
    void Longitud_archivo(long &lg_archivo, long &nm_lineas, const int t_p);
         // Lee la siguiente linea del archivo, retornando la longitud de la linea
    unsigned int Lee_linea(char *cadena);
         // Graba una cadena como una linea del archivo, retornando la longitud de esta
    unsigned int Graba_linea(const char *cadena);
         // Se posiciona en la linea indicada por LINEA
    unsigned int Busca_linea(const unsigned int linea);
         // Lee el numero de caracteres indicados en N_C y los deja en la cadena CADENA, retornando  el numero de caracteres leidos
    unsigned int Lee_caracteres(char *cadena, const unsigned int n_c);
         // Graba una una cadena de caracteres, retornando el numero de caracteres grabados
    unsigned int Graba_caracteres(const char *cadena, const unsigned int n_c);
         // Lee un entero
    void Lee_entero(int &num);
         // Lee un entero sin signo
    void Lee_entero(unsigned int &num);
         // Lee un carcater sin signo
    void Lee_caracter(char &num);
         // Lee un entero largo
    void Lee_entero_largo(long &num);
         // Lee un flotante double
    void Lee_double(double &num);
         // Graba un entero
    void Graba_entero(const int ent);
         // Graba un entero sin signo
    void Graba_entero(const unsigned int ent);
         // Graba un caracter sin signo
    void Graba_caracter(const char car);
         // Graba un entero largo
    void Graba_entero_largo(const long ent);
         // Graba un flotante double
    void Graba_double(const double num);
         // Debuelve el Estado del manipulador de archivos
    int  Retorna_estado(void) const
         {return Estado;}
         // Posiciona al indicador del archivo en el inicio de este
    void Inicio_archivo(void)
         {Estado = 1; rewind(ARCHIVO_TRABAJO);}


         // Copia el archivo indicado como fuente al archivo objeto
    int  Copia_archivo(const char *archivo_fuente, const char *archivo_objeto);
         // Remueve el archivo especificado en el path, en caso de tener permisos de solo lectura y/o oculto son cambiados a archivo normal. Despues de borra el archivo, si tp es (1) reescribe este para evitar su recuperación.
    int  Remueve_archivo(const char *arch, const int tp);
         // Retorna un nombre de archivo con extención EXT
    void Cambia_ext_path(const char *path, const char *ext, char *pathmod);
         // Retorna un nombre de archivo con extención EXT
    void Cambia_ext_path(char *path, const char *ext);
         // Construye una trayectoria
    void Construye_trayectoria(const char *tray, const char *arch, const char *ext, char *xpath);
         // Adiciona a una cadena con terminador nulo los caracteres LF y LN
    void Construye_trayectoria(const char *tray, const char *arch, char *xpath);
         // Adiciona a una cadena con terminador nulo los caracteres LF y LN
    void Convierte_linea(char *xcad);
         // Ajusta el nombre de archivo para visualizacion
    void AjustaNombreArchivo(const char *cad, char *xcad);
};

#endif


