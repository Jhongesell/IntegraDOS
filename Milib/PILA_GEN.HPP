
// Clase generica para instanciar Pilas

template <class T>
class PILA
{
   private:
           // Tope de la Pila
      T   *Ptr_tope;
           // Fondo de la Pila
      T   *Ptr_fondo;
           // Longitud de la Pila
      int  Longitud;
 
  public:
            // Constructor de la clase Pila con valor por omicion de longitud de la Pila
            PILA(const unsigned int lg = 100)
            {Ptr_tope = Ptr_fondo = new T[Longitud = lg];}
            // Destructor de la clase Pila
           ~PILA(void)
            {delete Ptr_fondo;}
            // Pone en el tope de la Pila un elemento
      void  Poner(T x)
            {if(Ptr_tope - Ptr_fondo) < Longitud) *Ptr_tope++ = x;}
            // Quita del tope de la Pila un elemento
      void  Quitar(void)
            {if(Ptr_tope > Ptr_fondo) Ptr_tope --;}
            // Retorna el actual elemento en el tope de la Pila
      T     Actual(void) const
            {if(Ptr_tope > Ptr_fondo) return *(Ptr_tope-1);}
            // Retorna la maxima longitud de la Pila
      int   Maxima_longitud(void) const
            {return Longitud;}
            // Retorna el total de elementos actuales dentro de la Pila
      int   Total_elementos(void) const
            {return (Ptr_tope - Ptr_fondo);}
            // Retorna verdadero si esta vacia la Pila
      int   Esta_vacia(void) const
            {return (Ptr_fondo == Ptr_tope);}
            // Retorna verdadero si esta llena la Pila
      int   Esta_llena(void) const
            {((Ptr_tope - Ptr_fondo) == Longitud);}
};


// Nota esta definicion de Pila no trabaja con cadenas de caracteres

 
 
