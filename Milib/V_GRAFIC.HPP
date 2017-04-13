// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#ifndef __V_GRAFIC_HPP__
#define __V_GRAFIC_HPP__


#include "v_iconos.hpp"
#include "definic.hpp"

extern "C" {
   #include <graphics.h>
   #include <string.h>
   #include <math.h>
}

// Indica el maximo numero de pixes horizontales hay en ambiente grafico
extern int X_MAX;
// Indica el maximo numero de pixes verticales hay en ambiente grafico
extern int Y_MAX;



/////////////////////////////////////////////////////////////
//                                                         //
//            Base de la ventana de graficacion            //
//                                                         //
/////////////////////////////////////////////////////////////

class Ventana_graficacion: public Ventana_iconos
{
   public:

      C_3D          Minimo;                      // Contiene los valores de X, Y, Z minimos de los ejes
      C_3D          Maximo;                      // Contiene los valores de X, Y, Z maximos de los ejes
      C_3D          Orig_maximo;                 // Contiene los valores originales de los ejes coordenados
      C_3D          Orig_minimo;                 // Contiene los valores originales de los ejes coordenados

      C_3D          C_reales;                    // Posicion dentro de los ejes coordenados (reales)

      C_2I          V_pos_ini;                   // Indica la posicion donde iniciara la visualizacion de la ventana
//      C_2I          V_pos_fin;                   // Indica la posicion donde finaliza la visualizacion de la ventana

      unsigned int  Tipo_ventana:        1;      // Indica el tipo de ventan a visualizar


                    // Inicializa la clase
      virtual void  Inicializa(void) const;
                    // Dibuja al ventana
      virtual void  Dibuja(void) const
                    { Ventana_iconos::Dibuja(); }
                    // Retorna la dimencion de los ejes coordenados
      void          Retorna_dimenciones_maximas(C_3D &v_max,C_3D &v_min) const
                    { v_max = Maximo, v_min =  Minimo; }
      void          Retorna_dimenciones_originales(C_3D &v_max,C_3D &v_min) const
                    { v_max = Orig_maximo, v_min =  Orig_minimo; }
                    // Modifica coordenadas reales
      virtual void  Fija_coordenadas(const C_3D n_cda) const
                    { ((Ventana_graficacion*)this)->C_reales = n_cda; }
                    // Retorna el valor de coordenadas reales
      C_3D          Retorna_coordenadas(void) const
                    { return C_reales; }

              void  Fija_tipo_ventana(const int tv) const
                    { ((Ventana_graficacion*)this)->Tipo_ventana = tv;}

      unsigned int  Retorna_tipo_ventana(void) const
                    { return Tipo_ventana; }

};




/////////////////////////////////////////////////////////////
//                                                         //
//                Base de Ejes coordenados                 //
//                                                         //
/////////////////////////////////////////////////////////////



class B_Ventana_ejes_coordenados: public Ventana_graficacion
{
   private:

      double        Factor;                      // Factor de acercamiento dentro de la ventana
      double        xTeta;                       // Variables temporales unsadas en conversion a coordenadas polares
      double        xNorma;
      C_3D          xaux1;
      C_3D          xaux2;
      C_2I          taux1,taux2;                  

      double        ST, SF, CT, CF;              // Valores usados para la proyecci¢n
      double        pt_x, pt_y;                  // Valores temporales para proyeccion
      double        x1,x2,x3,x4;                 // Valores temporales
      C_ptl         aux0;                        // Valores temporales
      C_2I          aux1,aux2;                   // Valores temporales
      C_3D          aux3,aux4;

                    // Revisa que la zona de graficacion este dentro de la ventana
      virtual int   Revisa_graficacion(void) const  = 0;

   protected:

      unsigned int  Proyeccion:          1;      // Indica la proyeccion actual
      unsigned int  Rectangulares:       1;      // Indica si valor de las coordenadas son del tipo rectangulares o polares
      double        TETA, FI;                    // Valores usados para la proyecci¢n
      C_3D          Escala;                      // Escala en X, Y, Z
      C_2I          Centro;                      // Centro de la pantalla
      C_3D          Maximos_ejes;                // Valor maximo de Ejes coordenados


                    // Fija escalas
      void          Fija_escalas(void) const;
                    // Calcula valores para proyecciones
      void          Calcula_valores(void) const;

   public:

                    // Inicializa la clase
      virtual void  Inicializa(const int t_proy, const double factor) const;
                    // Modifica la dimencion de los ejes coordenados
      virtual void  Fija_dimenciones(const C_3D v_max, const C_3D v_min) const
                    {
                       // Valores originales 
                       ((Ventana_graficacion*)this)->Orig_maximo = v_max;
                       ((Ventana_graficacion*)this)->Orig_minimo = v_min;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                       
// Se quito esto (homegeniza las dimensiones de la ventana de trabajo)
/*                       
                       // Valor maximo de los ejes
                       ((B_Ventana_ejes_coordenados*)this)->Maximos_ejes.x = fabs(v_max.x) > fabs(v_min.x) ? fabs(v_max.x): fabs(v_min.x);
                       ((B_Ventana_ejes_coordenados*)this)->Maximos_ejes.y = fabs(v_max.y) > fabs(v_min.y) ? fabs(v_max.y): fabs(v_min.y);
                       ((B_Ventana_ejes_coordenados*)this)->Maximos_ejes.z = fabs(v_max.z) > fabs(v_min.z) ? fabs(v_max.z): fabs(v_min.z);
                       // Valor maximo y minimo de los ejes coordenados (ya homogenizados)
                       ((Ventana_graficacion*)this)->Maximo = Maximos_ejes;
                       ((Ventana_graficacion*)this)->Minimo.x = -Maximos_ejes.x;
                       ((Ventana_graficacion*)this)->Minimo.y = -Maximos_ejes.y;
                       ((Ventana_graficacion*)this)->Minimo.z = -Maximos_ejes.z;
*/                       
// Se agrego esto (respeta las dimensiones de la ventana de trabajo)
                       ((B_Ventana_ejes_coordenados*)this)->Maximos_ejes.x = v_max.x;
                       ((B_Ventana_ejes_coordenados*)this)->Maximos_ejes.y = v_max.y;
                       ((B_Ventana_ejes_coordenados*)this)->Maximos_ejes.z = v_max.z;
                       ((Ventana_graficacion*)this)->Maximo = Maximos_ejes;
                       ((Ventana_graficacion*)this)->Minimo.x = v_min.x;
                       ((Ventana_graficacion*)this)->Minimo.y = v_min.y;
                       ((Ventana_graficacion*)this)->Minimo.z = v_min.z;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                       
                       Fija_escalas();
                    }
                    // Fija la proyeccion actual
      void          Fija_proyeccion_actual(const int proy) const
                    { ((B_Ventana_ejes_coordenados*)this)->Proyeccion = proy; }
      void          Fija_tipo_coordenadas(const unsigned int tc)
                    { ((B_Ventana_ejes_coordenados*)this)->Rectangulares = tc; }
                    // Retorna la proyeccion actual
      int           Retorna_proyeccion_actual(void) const
                    { return Proyeccion; }
                    // Mueve el origen coordenado
      virtual void  Mover_origen(const int tp) const;
                    // Acercar alejar el origen
      virtual void  Acercar_alejar(const char tp) const;
                    // Retorna la posicion del mouse dentro de la ventana
      virtual int   Posicion_mouse(C_2D &pos) const;
                    // Dibuja un punto dentro de la ventana
      virtual void  Dibuja_punto(const C_3D aux, const char col, const int tp) const;
                    // Dibuja una linea dentro de la ventana
      virtual void  Dibuja_linea(const C_3D aux1, const C_3D aux2, const char col, const int tp) const;

// usarlo en ventana de caja
//                    // Acercamiento
//      void          Acercamiento(void) const;


           // Calcula la proyeccion de un punto de 2D a la ventana
     int   Proyecciones_2D(const C_3D pt_r, C_2I &pt_p) const;
           // Calcula la proyeccion de un punto de 3D a la ventana
     int   Proyecciones_3D(const C_3D pt_r, C_2I &pt_p) const;
           // Rota los los ejes coordenados
     void  Rota_ejes(const int tp) const;
};




/////////////////////////////////////////////////////////////
//                                                         //
// Ventana con cursor                                      //
//                                                         //
/////////////////////////////////////////////////////////////

class V_cursor: virtual public B_Ventana_ejes_coordenados
{
   private:

      unsigned int  sw_cursor:   1;    // Muestra u oculta el cursor
      char         *Cursor;            // Puntero que contiene la imagen abajo del cursor
      C_2I          Pos_ant;           // Ultimo punto donde se visualizo el cursor
      C_2I          Pos_tmp;           // Posicion temporal


   protected:
                    // Inicializa la clase
      virtual void  Inicializa(void) const;
                    // Destruye la clase
      virtual void  Destruye(void) const
                    { delete []Cursor; }

   public:

                    // Visualiza el cursor tridimencional
      void          Visualiza_cursor(const int tp) const;

};




/////////////////////////////////////////////////////////////
//                                                         //
//         Ventana con ejes coordenados en 2D y 3D         //
//                                                         //
/////////////////////////////////////////////////////////////

class V_ejes_coordenados: virtual public B_Ventana_ejes_coordenados
{
   private:

      char          *Nombre_eje_X;               // Nombre del eje X
      char          *Nombre_eje_Y;               // Nombre del eje Y
      char          *Nombre_eje_Z;               // Nombre del eje Z

      C_3I           Color_eje;                  // Contiene los colores para los ejes XYZ
      unsigned int   Visualiza_nombre_eje:    1; // Indica si visualiza el nombre del eje coordenado

   protected:
                      // Dibuja ejes coordenados en 2D
      void            Dibuja_ejes_2D(const int tp) const;
                      // Dibuja ejes coordenados en 3D
      void            Dibuja_ejes_3D(const int tp) const;


   public:
                      // Inicializa la clase
      virtual void    Inicializa(const int t_proy, const double factor) const;
                      // Destruye la clase
      virtual void    Destruye(void) const
                      { 
                         delete [] Nombre_eje_X;
                         delete [] Nombre_eje_Y;
                         delete [] Nombre_eje_Z;
                      }
                      // Dibuja los ejes coordenados
              void    Dibuja_ejes_coordenados(const int tp) const
                      {
                         if(Proyeccion) Dibuja_ejes_2D(tp);
                          else Dibuja_ejes_3D(tp);
                      }
                      // Gradua los ejes coordenados
      void            Gradua_ejes(void)
                      {
          // No esta completo el procedimiento
//          C_3D ini;
//          ini.x = 0, ini.y = 0, ini.z = 0;
//          for(ini.x = Dim_minima[Sistema_actual].x; ini.x < Dim_maxima[Sistema_actual].x; ini.x +=(Dim_maxima[Sistema_actual].x-Dim_minima[Sistema_actual].x)/20) Dibuja_punto(ini,0,Magenta9);
//          ini.x = 0;
//          for(ini.y = Dim_minima[Sistema_actual].y; ini.y < Dim_maxima[Sistema_actual].y; ini.y +=(Dim_maxima[Sistema_actual].y-Dim_minima[Sistema_actual].y)/20) Dibuja_punto(ini,0,Magenta9);
//          ini.y = 0;
//          for(ini.z = Dim_minima[Sistema_actual].z; ini.z < Dim_maxima[Sistema_actual].z; ini.z +=(Dim_maxima[Sistema_actual].z-Dim_minima[Sistema_actual].z)/20) Dibuja_punto(ini,0,Magenta9);
                      }
                      // Retorna el estado de visualiza nombre de eje coordenado
      int             Retorna_Visualiza_nombre_eje(void) const
                      {
                         return Visualiza_nombre_eje;
                      }
                      // Fija el estado de visualiza nombre de eje coordenado
      void            Fija_Visualiza_nombre_eje(const int tp) const
                      {
                         ((V_ejes_coordenados*)this)->Visualiza_nombre_eje = tp;
                      }
                      // Fija el nombre de los ejes coordenado
      void            Fija_nombre_ejes(const char *eje_x, const char *eje_y, const char *eje_z)
                      {
                         if(Nombre_eje_X) delete [] Nombre_eje_X;
                         if(Nombre_eje_Y) delete [] Nombre_eje_Y;
                         if(Nombre_eje_Z) delete [] Nombre_eje_Z;
                         Nombre_eje_X = new char[strlen(eje_x)+1];
                         Nombre_eje_Y = new char[strlen(eje_y)+1];
                         Nombre_eje_Z = new char[strlen(eje_z)+1];
                         strcpy(Nombre_eje_X,eje_x);
                         strcpy(Nombre_eje_Y,eje_y);
                         strcpy(Nombre_eje_Z,eje_z);
                         // Se corta el nombre del eje coordenado a una letra
                         Nombre_eje_X[1]=0;
                         Nombre_eje_Y[1]=0;
                         Nombre_eje_Z[1]=0;
                      }
                      // Fija el color de los ejes coordenado
      void            Fija_color_ejes(const C_3I color) const
                      {
                         ((V_ejes_coordenados*)this)->Color_eje = color;
                      }
                      // Retorna el color de los ejes coordenado
      void            Retorna_color_ejes(C_3I &color) const
                      {
                         color = Color_eje;
                      }

};


///////////////////////////////////////////////////////////////
//                                                           //
// Ventana de graficacion en 2D y 3D con cursor y ejes       //
// cordenados                                                //
//                                                           //
///////////////////////////////////////////////////////////////

class V_graficacion: virtual public V_ejes_coordenados, virtual public V_cursor
{

   private:

                  // Revisa que la zona de graficacion este dentro de la ventana
      int         Revisa_graficacion(void) const;
      C_2I        taux1,taux2;                   // Valores temporales

   public:
                     // Inicializa la clase
      void        Inicializa(const int t_proy, const double factor) const
                  {
                     V_ejes_coordenados::Inicializa(t_proy,factor);
                     V_cursor::Inicializa();
                  }
                      // Destruye la clase
      void        Destruye(void) const
                  {
                   V_cursor::Destruye();
                   V_ejes_coordenados::Destruye();
                  }


                  // Rota los ejes coordenados
      void        Rota_ejes(const int tp) const
                  {
                      Visualiza_cursor(0);
                      Dibuja_ejes_coordenados(0);
                      B_Ventana_ejes_coordenados::Rota_ejes(tp);
                      Dibuja_ejes_coordenados(1);
                      Visualiza_cursor(1);
                  }
                  // Mueve el origen de los ejes coordenados
      void        Mover_origen(const int tp) const
                  {
                   Visualiza_cursor(0);
                   Dibuja_ejes_coordenados(0);
                   B_Ventana_ejes_coordenados::Mover_origen(tp);
                   Dibuja_ejes_coordenados(1);
                   Visualiza_cursor(1);
                  }
                  // Acerca o aleja al origen de los ejes coordenados
      void        Acercar_alejar(const int tp) const
                  {
                   Visualiza_cursor(0);
                   Dibuja_ejes_coordenados(0);
                   B_Ventana_ejes_coordenados::Acercar_alejar(tp);
                   Dibuja_ejes_coordenados(1);
                   Visualiza_cursor(1);
                  }
                  // Dibuja los ejes coordenados
      void        Dibuja(void) const
                  {
                   Visualiza_cursor(0);
                   B_Ventana_ejes_coordenados::Dibuja();
                   Dibuja_ejes_coordenados(1);
                   Visualiza_cursor(1);
                  }
      void        Fija_coordenadas(const C_3D n_cda) const
                  {
                   Visualiza_cursor(0);
                   V_ejes_coordenados::Fija_coordenadas(n_cda);
                   Visualiza_cursor(1);
                  }
                  // Revisualiza los ejes coordenados (Sobrepone los ejes)
      void        Revisualiza_ejes(void) const
                  {
                   Visualiza_cursor(0);
                   Dibuja_ejes_coordenados(1);
                   Visualiza_cursor(1);
                  }
/*
      void        Cambiar_tamano(const int x1, const int y1, const int x2, const int y2) const
                  {
                   V_ejes_coordenados::Cambiar_tamano(x1,y1,x2,y2);
                   Fija_escalas();
                   Dibuja();
                  }

*/
///////////////////////////////
// Optimizar la configuracion
                  // Retorna la actual configuracion de la ventana
      void        Retorna_configuracion(C_3D &esc, C_2I &cen, double &teta, double &fi) const
                  {
                     esc = Escala, cen = Centro;
                     teta = TETA,
                     fi = FI;
                  }

                  // Fija nueva configuracion de la ventana
      void        Fija_configuracion(const C_3D esc,const C_2I cen, const double teta, const double fi) const
                  {
                     ((V_graficacion*)this)->Escala = esc,
                     ((V_graficacion*)this)->Centro = cen;
                     ((V_graficacion*)this)->TETA = teta,
                     ((V_graficacion*)this)->FI = fi;
                     Calcula_valores();
                  }

              void  Fija_tipo_ventana(const int tv) const
                    { 
                      Ventana_graficacion::Fija_tipo_ventana(tv);
                      V_graficacion::Dibuja();
                    }



};


//////////////////////////////////////////////////////////////////
// Clase de control de la ventana grafica 
// (uno o dos ventanas graficas)
//////////////////////////////////////////////////////////////////

class Ctrl_ventana_grafica 
{

   public:

      V_graficacion   *VT1;
      V_graficacion   *VT2;
      unsigned int     Ventana_doble:    1;  // Una o dos ventanas
      
      struct Estado_ventana {                // Estado de la ventana
         unsigned int     Activa:                   1;  // (0) no, (1) si
         unsigned int     Tipo:                     3;  // (0) oculta, (1) minimizada, (2) mitad ventana, (3) maximizada
         unsigned int     Grafica:                  1;  // (0) No grafica, (1) Si grafica
         const char      *Archivo;                      // Nombre del archivo para la ventana
      } EVT1, EVT2;

      void Inicializa(const unsigned int tipo, const char *tit1, const char *tit2)
           {
              VT2 = new V_graficacion;
              VT2->Activa_color_refresco(Blanco);
              VT2->Nombre_iconos("MAXIM.ICO","MINIM.ICO");
              VT2->Define_colores(Blanco,Cian9,Gris9,Blanco);
              VT2->Define_ventana(tit2,(X_MAX/2)+1,83,X_MAX-3,Y_MAX-31,1);
              VT2->Inicializa(0,0.80);

              VT1 = new V_graficacion;
              VT1->Activa_color_refresco(Blanco);
              VT1->Nombre_iconos("MAXIM.ICO","MINIM.ICO");
              VT1->Define_colores(Blanco,Cian9,Gris9,Blanco);
              // Inicia como dible ventana o una sola ventana visible
              if(tipo) VT1->Define_ventana(tit1,3,83,X_MAX/2,Y_MAX-31,1);
              else VT1->Define_ventana(tit1,3,83,X_MAX-3,Y_MAX-31,1);
              VT1->Inicializa(0,0.80);
              // indica el numero de ventanas visibles simultaneamente
              Ventana_doble = tipo;
              // Primera ventana estara activa
              if(Ventana_doble) {
                 EVT1.Activa = 1; // Activa
                 EVT2.Activa = 0; // No activa
                 EVT1.Tipo   = 2; // Mitad de ventana
                 EVT2.Tipo   = 2; // Mitad de ventana
                 EVT1.Grafica= 1; // Si Grafica 
                 EVT2.Grafica= 1; // Si Grafica 
               } else {
                 EVT1.Activa = 1; // Activa
                 EVT2.Activa = 0; // No activa
                 EVT1.Tipo   = 3; // Toda la ventana
                 EVT2.Tipo   = 0; // Oculta
                 EVT1.Grafica= 1; // Si Grafica 
                 EVT2.Grafica= 0; // No Grafica 
              }
              EVT1.Archivo = "ORB_ANT1.$$$";
              EVT2.Archivo = "ORB_ANT2.$$$";
           }

   
      void           Destruye(void)
                     {
                        VT1->Destruye();
                        VT2->Destruye();
                     }

                     // Fija la ventana activa
      void           Fija_ventana_activa(const unsigned int est)
                     {
                        EVT1.Activa = !est;
                        EVT2.Activa = est;
                        Visualiza_cursor(NO);
                        VT1->Ventana_activa(EVT1.Activa);
                        if(Ventana_doble) VT2->Ventana_activa(EVT2.Activa);
                        Visualiza_cursor(SI);
                     }
                     // Retorna el estado de doble ventana
      unsigned int   Estado_ventana_doble(void)
                     {return Ventana_doble;}
                     // Fija si estaran activas las dos ventanas o solo una
      void           Fija_ventana_doble(const unsigned int est)
                     {
                      Ventana_doble = est;
                      if(Ventana_doble) {
                         EVT1.Tipo   = 2; // Mitad de ventana
                         EVT2.Tipo   = 2; // Mitad de ventana
                       } else {
                         EVT1.Tipo   = 3; // Toda la ventana
                         EVT2.Tipo   = 0; // Oculta
                      }
                      Fija_ventana_activa(0);
                     }
                     // Visualiza u oculta el cursor
      void           Visualiza_cursor(const int tp)
                     {
                        VT1->Visualiza_cursor(tp);
                        if(Ventana_doble) VT2->Visualiza_cursor(tp);
                     }  
                     // Borra los archivos de ventana      
      void           Borra_archivo_ventana(void)
                     {
                        unlink(EVT1.Archivo);
                        unlink(EVT2.Archivo);
                     }
                     // Graba los archivo de ventana      
      void           Graba_archivo_ventana(void)
                     {
                        Visualiza_cursor(NO);
                        VT1->Graba_ventana(EVT1.Archivo,CON_FORMATO);                        
                        if(Ventana_doble) VT2->Graba_ventana(EVT2.Archivo,CON_FORMATO);
                     }
                     // Lee los archivos de ventana
      void           Lee_archivo_ventana(void)
                     {
                        if(n_archivos_directorios(EVT1.Archivo,_A_ARCH)) {
                           VT1->Lee_ventana(EVT1.Archivo,CON_FORMATO);
                           if(Ventana_doble) VT2->Lee_ventana(EVT2.Archivo,CON_FORMATO);
                           Visualiza_cursor(SI);
                        }
                     }

                     // Retorna la proyeccion del la ventana activa
      int            Retorna_proyeccion_actual(void)
                     {
                        if(VT1->Retorna_ventana_activa()) return(VT1->Retorna_proyeccion_actual());
                        if(VT2->Retorna_ventana_activa()) return(VT2->Retorna_proyeccion_actual());
                        return(0);
                     }
                     // Rota los ejes de la ventana activa      
      void           Rota_ejes(const int tp)
                     {
                        if(VT1->Retorna_ventana_activa()) VT1->V_graficacion::Rota_ejes(tp);
                        if(VT2->Retorna_ventana_activa()) VT2->V_graficacion::Rota_ejes(tp);
                     }
                     // Acerca/aleja los ejes de la ventana activa      
      void           Acercar_alejar(const char car)
                     {
                        if(VT1->Retorna_ventana_activa()) VT1->V_graficacion::Acercar_alejar(car);
                        if(VT2->Retorna_ventana_activa()) VT2->V_graficacion::Acercar_alejar(car);
                     }
                     // retorna el tipo de ventana de la ventana activa
      unsigned int   Retorna_tipo_ventana(void)
                     {
                        if(VT1->Retorna_ventana_activa()) return(VT1->Retorna_tipo_ventana());
                        if(VT2->Retorna_ventana_activa()) return(VT2->Retorna_tipo_ventana());
                        return(0);
                     }
                     // Mueve los ejes de la ventana activa      
      void           Mover_origen(const int tp)
                     {
                        if(VT1->Retorna_ventana_activa()) VT1->V_graficacion::Mover_origen(tp);
                        if(VT2->Retorna_ventana_activa()) VT2->V_graficacion::Mover_origen(tp);
                     }
                     // Fija si visualiza el nombre de los ejes coordenados
      void           Fija_Visualiza_nombre_eje(void) 
                     {
                        if(VT1->Retorna_ventana_activa()) VT1->Fija_Visualiza_nombre_eje(!VT1->Retorna_Visualiza_nombre_eje());
                        if(VT2->Retorna_ventana_activa()) VT2->Fija_Visualiza_nombre_eje(!VT2->Retorna_Visualiza_nombre_eje());
                     }
                     // Retorna si visualiza el nombre de los ejes coordenados
      int            Retorna_Visualiza_nombre_eje(void)
                     {
                        if(VT1->Retorna_ventana_activa()) return(VT1->Retorna_Visualiza_nombre_eje());
                        if(VT2->Retorna_ventana_activa()) return(VT2->Retorna_Visualiza_nombre_eje());
                        return (0);
                     }

};    


#endif

