// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  2.1-A


#include "get.hpp"
#include "teclado.hpp"
#include "cad_car.hpp"
#include "nucleo.hpp"
#include "r_esp.hpp"
#include "fechas.hpp"

extern "C" {
   #include <string.h>
   #include <graphics.h>
   #include <stdlib.h> // solo para conversiones
   #include <stdio.h>
}

#ifdef _IDIOMA_ESPANOL_
   #define TXT1      "El campo no es valido"
#else
   #define TXT1      "Not valid field"
#endif


// Objeto global al nuclo del sistema
extern Nucleo *NCO;

extern char DELIMIT_FECHA;

// Constructor de la clase
Get::Get(void)
{
   Cad_vis = Cad_org = Cad_tmp = Car_exc = Car_adi = Cad_ult_mod = NULL;
   Mayusculas = Minusculas = 0;
   Todo = Fecha = Hora = Alfanumericos = Alfabeticos = 0;
   Digitos = Digitos_sig = Digitos_sig_pto = Not_cientifica = Not_exponencial = 0;
   Expresion_art = Expresion_art_fun = 0;
   Password = Caracteres_ad = Caracteres_ex = 0;
   Limpiar_cadena = sw_limpia_cad = Get_activo = 0;
   sw = Captura_activa = Captura_valida = 1;
}

// Destructor de la clase
Get::~Get()
{
   delete []Cad_org;
   delete []Cad_tmp;
   delete []Car_exc;
   delete []Car_adi;
   delete []Cad_vis;
   delete []Cad_ult_mod;
   delete Ctrl_mov;
}


// Get para numeros int
void Get::Parametros(const int num, const int x1, const int y1, const int n_c_v)
{
   char xcad[80];
   sprintf(xcad,"%d",num);
   s_r_t(xcad,10,' ');
   Captura_valida = 0;
   Parametros(xcad,x1,y1,n_c_v,"KUS");
}



// Get para numeros long
void Get::Parametros(const long num, const int x1, const int y1, const int n_c_v)
{
   char xcad[80];
   sprintf(xcad,"%ld",num);
   s_r_t(xcad,15,' ');
   Captura_valida = 0;
   Parametros(xcad,x1,y1,n_c_v,"KUS");
}


// Get para numeros doble
void Get::Parametros(const double num, const int x1, const int y1, const int n_c_v, const unsigned int tp)
{
   char xcad[80];
   if(tp) sprintf(xcad,"%1.9e",num);
    else sprintf(xcad,"%9.9f",num);
   Not_exponencial = tp;
   s_r_t(xcad,30,' ');
   Captura_valida = 0;
   Parametros(xcad,x1,y1,n_c_v,"KU#");
}


// Get para cadenas
void Get::Parametros(const char *cad, const int x1, const int y1, const int n_c_v, const char *temp, const char *car_ad, const char *car_ex)
{
   // Obtiene el largo de la cadena
   Lg_cad = strlen(cad);
   N_car_vis = n_c_v;
   if(!N_car_vis) N_car_vis = Lg_cad;
   // Solicita espacio para almacenar la cadena
   Cad_org = new char[Lg_cad+3];
   Cad_tmp = new char[Lg_cad+3];
   Cad_ult_mod = new char[Lg_cad+3];
   Car_adi = new char[strlen(car_ad)+3];
   Car_exc = new char[strlen(car_ex)+3];
   Cad_vis = new char[N_car_vis+3];
   // Copia la cadena a la de trabajo
   strcpy(Cad_org,cad);
   strcpy(Cad_tmp,cad);
   strcpy(Cad_ult_mod,cad);
   strcpy(Car_adi,car_ad);
   strcpy(Car_exc,car_ex);
   // Define la ventana
   Ventana_marco::Define_ventana(x1,y1,x1+((N_car_vis+1)*A_CAR),y1+25);
   Ctrl_mov = new Controlador_movimiento;
   Ctrl_mov->Parametros(Lg_cad,N_car_vis,"","");
   Ctrl_mov->Fija_elemento_actual(1);

   // Valida al templete
   if(strspn(temp,"KULNAD#9PCEFHST") != strlen(temp)) NCO->Visualiza_error(0,"Caracteres desconocidos","en el templete de captura");
/*
   for(int xi = 0; xi < strlen(temp); xi++) {
      if(!Busca_caracter("KULNAD#9PCEFHST",temp[xi],pos))
        NCO->Visualiza_error(0,"Caracteres desconocidos","en el templete de captura");
   }
*/ 

   // Revisa el tipo de templete
   unsigned int pos;
   if(Busca_caracter(temp,'T',pos)) Todo               = 1;
   if(Busca_caracter(temp,'K',pos)) Limpiar_cadena     = sw_limpia_cad = 1;
   if(Busca_caracter(temp,'U',pos)) Mayusculas         = 1;
   if(Busca_caracter(temp,'L',pos)) Minusculas         = 1;
   if(Busca_caracter(temp,'N',pos)) Alfanumericos      = 1;
   if(Busca_caracter(temp,'A',pos)) Alfabeticos        = 1;
   if(Busca_caracter(temp,'P',pos)) Password           = 1;
   if(Busca_caracter(temp,'C',pos)) Caracteres_ad      = 1;
   if(Busca_caracter(temp,'E',pos)) Caracteres_ex      = 1;
   if(Busca_caracter(temp,'F',pos)) Fecha              = 1, Captura_valida = 0;
   if(Busca_caracter(temp,'H',pos)) Hora               = 1, Captura_valida = 0;
   if(Busca_caracter(temp,'D',pos)) Digitos            = 1;
   if(Busca_caracter(temp,'S',pos)) Digitos_sig        = 1;
   if(Busca_caracter(temp,'9',pos)) Digitos_sig_pto    = 1;
   if(Busca_caracter(temp,'#',pos)) Not_cientifica     = 1;
   if(Busca_caracter(temp,'X',pos)) Expresion_art      = 1;
   if(Busca_caracter(temp,'Y',pos)) Expresion_art_fun  = 1;
}

// Dibuja la ventana del get asi como el texto de este
void Get::Dibuja(void)
{
   // Dibuja la ventana
   Ventana_marco::Dibuja();
   // Visualiza la cadena
   if(!Password) {
      Substr(Cad_tmp,Ctrl_mov->Elemento_inicial_paneo()-1,N_car_vis,Cad_vis);
      Ventana_marco::Visualiza_texto(5,8,Cad_vis);
   }
   // Visualizacion del cursor
   if(Get_activo) Ventana_marco::Visualiza_caracter(5+((Ctrl_mov->Actual_elemento_paneo() - 1)*A_CAR),13,'_', Rojo1,0);
}



// Permite la edicion de la cadena
void Get::Edita(const char car, const int tec)
{
   int xtec;
   char xcar;

   // Cambia el tipo de cursor del mouse
   if(Ventana_marco::Mouse_dentro()) NCO->Selecciona_tipo_mouse(3);

   // Revisa si el get esta activo
   if(!Get_activo) {
      if(sw) {
         // Oculta el cursor
         Ventana_marco::Visualiza_caracter(5+((Ctrl_mov->Actual_elemento_paneo() - 1)*A_CAR),13,'_',C_VENT,0);
         // Revisa si la captura es valida, encaso contrario visualiza un mensaje
         if(!Captura_completada()) NCO->Mensaje("",TXT1,Cad_tmp);
      }
      sw = 0;
      return;
   } else {
      if(!sw) Ventana_marco::Visualiza_caracter(5+((Ctrl_mov->Actual_elemento_paneo() - 1)*A_CAR),13,'_',Rojo1,0);
      sw = 1;
   }
   if(tec == -1) return;
   sw = 1;
   // Visualizacion del cursor
   Ventana_marco::Visualiza_caracter(5+((Ctrl_mov->Actual_elemento_paneo() - 1)*A_CAR),13,'_',C_VENT,0);
   if(Captura_activa) {
      if(sw_limpia_cad) {
         if(car) Borra_cadena(Cad_tmp);
         sw_limpia_cad = 0;
      }
      xcar = Valida_caracter_captura(car);
      xtec = Edita_cadena(xcar,tec);
      if(xtec == -1) xtec = tec;
   } else xtec = tec;
   Ctrl_mov->Horizontal(xtec,Cad_tmp);
   // Visualizacion de la cadena de edicion
   if(!Password) {
      Substr(Cad_tmp,Ctrl_mov->Elemento_inicial_paneo()-1,N_car_vis,Cad_vis);
      Ventana_marco::Visualiza_texto(5,8,Cad_vis,1);
   }
   // Visualizacion del cursor
   Ventana_marco::Visualiza_caracter(5+((Ctrl_mov->Actual_elemento_paneo() - 1)*A_CAR),13,'_',Rojo1,0);
}


// Edita la cadena
int Get::Edita_cadena(const char car, const int tc)
{
  int sw = -1;
  switch(tc) {
     case CTRL_C:
        // Copia la cadena actual al portapapeles
        NCO->Crea_porta_papeles(1);
        s_trim(Cad_tmp);
        NCO->Inserta_porta_papeles(Cad_tmp);
        s_r_t(Cad_tmp,Lg_cad,' ');
        break;
     case CTRL_V:
        // Copia el porta papeles a la cadena actual
        Substr(NCO->Retorna_cadena_porta_papeles(0),0,Lg_cad,Cad_tmp);
        s_r_t(Cad_tmp,Lg_cad,' ');
        break;
     case DEL:
        // Copia la cadena a la que contiene la anterior modificacion
        Salva_anterior_modificacion();
        // Borra el caracter sobre el que se esta posicionado recorriendo el texto que esta a la derecha
        Borra_caracter(Cad_tmp,Ctrl_mov->Elemento_actual()-1);
        break;
     case BACKSPACE:
        // Copia la cadena a la que contiene la anterior modificacion
        Salva_anterior_modificacion();
        // Borra el caracter m s a la izquierda
        Borra_caracter(Cad_tmp,Ctrl_mov->Elemento_actual()-2);
        sw = FLECHA_IZQUIERDA;
        break;
     case CTRL_U:
        // Copia la cadena a la que contiene la anterior modificacion
        Salva_anterior_modificacion();
        // Regresa a la cadena como estaba antes de empesar la edicion
        strcpy(Cad_tmp,Cad_org);
        sw = HOME;
        break;
     case CTRL_Y:
        // Copia la cadena a la que contiene la anterior modificacion
        Salva_anterior_modificacion();
        // Borra toda la cadena
        Borra_cadena(Cad_tmp);
        sw = HOME;
        break;
     case CTRL_T:
        // Copia la cadena a la que contiene la anterior modificacion
        Salva_anterior_modificacion();
        // Borra la palabra a la derecha
        s_borra_palabra_sig(Cad_tmp,Ctrl_mov->Elemento_actual()-1);
        break;
     case CTRL_F:
        // Copia la cadena a la que contiene la anterior modificacion
        Salva_anterior_modificacion();
        // Borra de la actual posici¢n hasta el final de la cadena
        Borra_cadena_pos_fin(Cad_tmp,Ctrl_mov->Elemento_actual()-1);
        break;
     case CTRL_I:
        // Copia la cadena a la que contiene la anterior modificacion
        Salva_anterior_modificacion();
        // Borra de la actual posici¢n hasta el inicio de la cadena
        Borra_cadena_ini_pos(Cad_tmp,Ctrl_mov->Elemento_actual()-1);
        break;
     case ESC:
        // Restaura la ultima modificacion
        strcpy(Cad_tmp,Cad_ult_mod);
        break;
  }
  if(car) {
     // Copia la cadena a la que contiene la anterior modificacion
     Salva_anterior_modificacion();
     if(NCO->Estado_Insert()) s_ins_car(Cad_tmp,Ctrl_mov->Elemento_actual()-1,car); // Inserta el caracter desplazando el resto del texto
      else Cad_tmp[Ctrl_mov->Elemento_actual()-1] = car;           // Remplaza el caracter actual
     sw = FLECHA_DERECHA;
  }
  return sw;
}



char Get::Valida_caracter_captura(const char car)
{
   unsigned int pos;
   char xcar = car;
   if(Mayusculas) xcar = s_conv_may_min(xcar,3);
   if(Minusculas) xcar = s_conv_may_min(xcar,2);
   if(Fecha || Hora) {
      if(Fecha && (Es_caracter_digito(xcar) || xcar == DELIMIT_FECHA)) return xcar;
      if(Hora && (Es_caracter_digito(xcar) || xcar == ':')) return xcar;
   } else {
      if(Todo) return xcar;
      if(Alfanumericos && Es_caracter_alfnumerico(xcar)) return xcar;
      if(Alfabeticos && s_es_car_alf_ace(xcar)) return xcar;
      if(Digitos || Digitos_sig || Digitos_sig_pto || Not_cientifica || Expresion_art || Expresion_art_fun) {
         if(Es_caracter_digito(xcar)) return xcar;
         if(Digitos_sig || Digitos_sig_pto || Not_cientifica || Expresion_art || Expresion_art_fun) {
            if((xcar == '+' || xcar == '-') && Ctrl_mov->Elemento_actual() == 1) return xcar;
            if(Digitos_sig_pto || Not_cientifica || Expresion_art || Expresion_art_fun) {
               if(xcar == '.') {
                  if(!Busca_caracter(Cad_tmp,'.',pos)) return xcar;
               }
               if(Not_cientifica || Expresion_art || Expresion_art_fun) {
                  if((xcar == 'e' || xcar == 'E') && (!Busca_caracter(Cad_tmp,'E',pos) && !Busca_caracter(Cad_tmp,'e',pos))) return xcar;
                  if(xcar == '+' || xcar == '-') {
                     if(Ctrl_mov->Elemento_actual() > 1 && (Cad_tmp[Ctrl_mov->Elemento_actual() -2] == 'E' || Cad_tmp[Ctrl_mov->Elemento_actual() -2] == 'e')) return xcar;
                  }
                  if(Expresion_art || Expresion_art_fun) {
                     if(xcar == 'e' || xcar == 'E') return xcar;
                     if(Busca_caracter("*-/+.()",xcar,pos)) return xcar;
                     if(Expresion_art_fun) {
                        if((xcar >= 'A' && xcar >= 'Z') || (xcar >= 'a' && xcar >= 'z')) return xcar;
                     }
                  }
               }
            }
         }
      }
      if(Caracteres_ad) {
         if(Busca_caracter(Car_adi,xcar,pos)) return car;
      }
      if(Caracteres_ex) {
         if(Busca_caracter(Car_exc,xcar,pos)) return 0;
      }
   }
   return 0;
}


int Get::Captura_completada(void)
{
   Captura_valida = 1;
   if(Fecha && Valida_fecha(Cad_tmp)) Captura_valida = 0;
   if(Hora && Valida_hora(Cad_tmp,2)) Captura_valida = 0;
   if((Digitos || Digitos_sig ||Digitos_sig_pto || Not_cientifica) && !Es_cadena_numero(Cad_tmp)) Captura_valida = 0;
   return Captura_valida;
}


// Retorna la cadena editada
void Get::Cadena_editada(char *cad)
{
    strcpy(cad,Cad_tmp);
}

// Retorna la cadena editada
void Get::Cadena_editada(int &num)
{
    num = atoi(Cad_tmp);
}


// Retorna la cadena editada
void Get::Cadena_editada(long &num)
{
    num = atol(Cad_tmp);
}

// Retorna la cadena editada
void Get::Cadena_editada(double &num)
{
    char *prt;
    num = strtod(Cad_tmp,&prt);
}

// Cambia el contenido de la cadena
void Get::Cambia_cadena(const char *cad)
{
   // Borra el contenido anterior de la cadena
   delete []Cad_org;
   delete []Cad_tmp;
   delete []Cad_ult_mod;
   // Obtiene el largo de la cadena
   Lg_cad = strlen(cad);
   // Solicita espacio para almacenar la cadena
   Cad_org = new char[Lg_cad+1];
   Cad_tmp = new char[Lg_cad+1];
   Cad_ult_mod = new char[Lg_cad+1];
   // Copia la cadena a la de trabajo
   strcpy(Cad_org,cad);
   strcpy(Cad_tmp,cad);
   strcpy(Cad_ult_mod,cad);
   Ctrl_mov->Fija_elemento_actual(1);
   Ctrl_mov->Fija_total_elementos(Lg_cad);
   if(Limpiar_cadena) sw_limpia_cad = 1;
}

// Cambia el contenido de la cadena
void Get::Cambia_cadena(const int num)
{
   char cad[80];
   sprintf(cad,"%d",num);
   s_r_t(cad,10,' ');
   strcpy(Cad_org,cad);
   strcpy(Cad_tmp,cad);
   Ctrl_mov->Fija_elemento_actual(1);
   if(Limpiar_cadena) sw_limpia_cad = 1;
}


// Cambia el contenido de la cadena
void Get::Cambia_cadena(const long num)
{
   char cad[80];
   sprintf(cad,"%ld",num);
   s_r_t(cad,15,' ');
   strcpy(Cad_tmp,cad);
   strcpy(Cad_org,cad);
   Ctrl_mov->Fija_elemento_actual(1);
   if(Limpiar_cadena) sw_limpia_cad = 1;
}

// Cambia el contenido de la cadena
void Get::Cambia_cadena(const double num)
{
   char cad[80];
   if(Not_exponencial) sprintf(cad,"%1.9e",num);
    else sprintf(cad,"%9.9f",num);
   s_r_t(cad,30,' ');
   strcpy(Cad_tmp,cad);
   strcpy(Cad_org,cad);
   Ctrl_mov->Fija_elemento_actual(1);
   if(Limpiar_cadena) sw_limpia_cad = 1;
}


// Copia la cadena a la que contiene la anterior modificacion
void Get::Salva_anterior_modificacion(void)
{
   strcpy(Cad_ult_mod,Cad_tmp);
}

