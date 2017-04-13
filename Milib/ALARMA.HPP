// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


// Si Tipo es (0) la alarma es unica
//            (1) La alarma es diaria


class Alarma {
   private:
      int  TIPO[4];                   // Arreglo que indica el tipo de alarma
      long ALARMA[4];                 // Arreglo que contiene las horas en las cuales se activara la alarma
      char M_ALARMA[4][45];           // Arreglo que contiene los mensajes que seran visualizados al activarse la alarma
      unsigned int A_activa: 1;       // Indica si hay alguna alarma activa
      unsigned int Activa1:  1;       // Alarma 1 activa
      unsigned int Activa2:  1;       // Alarma 2 activa
      unsigned int Activa3:  1;       // Alarma 3 activa
      unsigned int Activa4:  1;       // Alarma 4 activa


         // Revisa si alguna de las alarmas esta activa
      void Revisa_activ_alarma(void);
         // Graba la alarma y los mensajes de esta
      void Graba_archivo_alarma(void);
         // Visualiza el mensaje de la fecha y hora en la que se activo la alarma
      void Mensaje_alarma(const int nms);

   public:
           // Lee el archivo de la alarma cargando las cuatro alarmas y sus mensajes respectivos
      void Lee_archivo_alarma(void);
           // Define 4 alarmas cada una con su respectivo mensaje de activaci¢n
      void Define_alarma(void);
           // Si hay alarmas activas, revisa y visualiza el mensaje de activaci¢n
      void Checa_alarma(void);
           // Indica si existe alguna alarma activa
      int  Alarma_activa(void)
           {return A_activa;}
};
