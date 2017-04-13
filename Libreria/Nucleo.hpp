// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CAAN-691229-TV5
// Dirección: Amsterdam 312 col. Hipódromo Condesa
// Teléfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisión  1.0-A

#ifndef __NUCLEO__ 
#define __NUCLEO__

#define ALARMA                1
#define INICIO_PROGRAMA       2
#define TERMINO_PROGRAMA      3
#define CAMPANADA             4
#define NUMERO_MAXIMO_FORMAS  30

class Nucleo {
   private:
      char sep[4];
      void *Arreglo_formas[NUMERO_MAXIMO_FORMAS];
      int   Arreglo_tipo_formas[NUMERO_MAXIMO_FORMAS];
      
   public:
      bool  Sonidos_activos;
      bool  Sonidos_hora;
      bool  Sonidos_alarma;

            // Constructor de la clase
            Nucleo(void)
            {
               Sonidos_activos = true;
               Sonidos_hora = true;
               Sonidos_alarma = true;
               sep[0] = 13;
               sep[1] = 10;
               sep[2] = 0;
               for (int i = 0; i < NUMERO_MAXIMO_FORMAS; i++) {
                   Arreglo_formas[i] = NULL;
                   Arreglo_tipo_formas[i] = 0;
               }
            }
            // Actuva el sonido indicado
      void  Activa_Sonido(const int sn);
            // Revisa y en su caso reinicializa la bitacora si esta saturada
      void  Revisa_llenado_bitacora(void);
            // Graba la bitacora del sistema
      void  Bitacora(const char *rut, const char *txt1, const char *txt2);
            // Adiciona forma 
      void  Adiciona_forma(void *forma, int &ptr, const int tipo);
            // Borra las formas no utilizadas
      void  Borrar_forma(void);
            // Activa la forma a ser borrada
      void  Activa_Borrado(const int i);
    
};

#endif





