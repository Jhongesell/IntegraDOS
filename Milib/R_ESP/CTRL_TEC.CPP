// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



#include "r_esp.hpp"
#include "ctrl_tec.hpp"
#include "teclado.hpp"

extern "C" {
   #include <bios.h>
//////////////
//   #include <stdio.h>   // Solo para ver codigo
//////////////
}

// Inicializa el control de teclado
void Ctrl_teclado::Inicializa_bufer_teclado(const unsigned int longitud_bufer)
{
   LONGITUD_BUFER_TECLADO = longitud_bufer;
   BUFER_TECLADO = new int[LONGITUD_BUFER_TECLADO+1];
   Limpia_bufer_teclado();
}


// Lee los caracteres o codigos de teclado y los almacena el el bufer del teclado
void Ctrl_teclado::Lee_bufer_teclado(void) const
{
   while(bioskey(1)) {
      ((Ctrl_teclado*)this)->tp = bioskey(0);
      if((tp & 0xFF) > 31 && (tp & 0xFF) < 255) ((Ctrl_teclado*)this)->tp = (tp & 0xFF);
      ((Ctrl_teclado*)this)->mod = bioskey(2);
      ((Ctrl_teclado*)this)->Insert = 1;
      if(mod & 0x80) ((Ctrl_teclado*)this)->Insert = 0;
      // Teclas con modificador SHIFT
      if(mod & 0x01 || mod & 0x02) {
         switch(tp) {
            case FLECHA_ARRIBA:
              ((Ctrl_teclado*)this)->tp = SHIFT_FLECHA_ARRIBA;    break;   // Shift flecha arriba
            case FLECHA_ABAJO:
              ((Ctrl_teclado*)this)->tp = SHIFT_FLECHA_ABAJO;     break;   // Shift flecha arriba
            case FLECHA_DERECHA:
              ((Ctrl_teclado*)this)->tp = SHIFT_FLECHA_DERECHA;   break;   // Shift flecha derecha
            case FLECHA_IZQUIERDA:
              ((Ctrl_teclado*)this)->tp = SHIFT_FLECHA_IZQUIERDA; break;   // Shift flecha izquierda
            case TAB:
              ((Ctrl_teclado*)this)->tp = SHIFT_TAB;              break;   // Shift Tabulador
         }
      }
      if(Press) {
         if(Press == CTRL_F) {
            switch(tp) {
              case 'c':
              case 'C':
              case CTRL_C:
                 ((Ctrl_teclado*)this)->tp = CTRL_F_C;
                 break;
              case 'n':
              case 'N':
              case CTRL_N:
                 ((Ctrl_teclado*)this)->tp = CTRL_F_N;
                 break;
              case 'p':
              case 'P':
              case CTRL_P:
                 ((Ctrl_teclado*)this)->tp = CTRL_F_P;
                 break;
              case 'w':
              case 'W':
              case CTRL_W:
                 ((Ctrl_teclado*)this)->tp = CTRL_F_W;
                 break;
            }
         }
         if(Press == CTRL_J) {
            switch(tp) {
              case 'd':
              case 'D':
                ((Ctrl_teclado*)this)->tp = CTRL_J_D;
                break;
              case 'p':
              case 'P':
                ((Ctrl_teclado*)this)->tp = CTRL_J_P;
                break;
            }
         }
         if(Press == CTRL_K) {
            switch(tp) {
              case 'c':
              case 'C':
              case CTRL_C:
                 ((Ctrl_teclado*)this)->tp = CTRL_K_C;
                 break;
              case 'p':
              case 'P':
              case CTRL_P:
                 ((Ctrl_teclado*)this)->tp = CTRL_K_P;
                 break;
              case 'r':
              case 'R':
              case CTRL_R:
                 ((Ctrl_teclado*)this)->tp = CTRL_K_R;
                 break;
              case 'v':
              case 'V':
              case CTRL_V:
                 ((Ctrl_teclado*)this)->tp = CTRL_K_V;
                 break;
              case 'w':
              case 'W':
              case CTRL_W:
                 ((Ctrl_teclado*)this)->tp = CTRL_K_W;
                 break;
              case 'y':
              case 'Y':
              case CTRL_Y:
                 ((Ctrl_teclado*)this)->tp = CTRL_K_Y;
                 break;
              case '1':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_1;
                 break;
              case '2':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_2;
                 break;
              case '3':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_3;
                 break;
              case '4':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_4;
                 break;
              case '5':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_5;
                 break;
              case '6':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_6;
                 break;
              case '7':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_7;
                 break;
              case '8':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_8;
                 break;
              case '9':
                 ((Ctrl_teclado*)this)->tp = CTRL_K_9;
                 break;
            }
         }
         if(Press == CTRL_M) {
            switch(tp) {
              case 'd':
              case 'D':
                ((Ctrl_teclado*)this)->tp = CTRL_M_D;
                break;
              case 'p':
              case 'P':
                ((Ctrl_teclado*)this)->tp = CTRL_M_P;
                break;
            }
         }
         if(Press == CTRL_N) {
            switch(tp) {
              case 'a':
              case 'A':
                 ((Ctrl_teclado*)this)->tp = CTRL_N_A;
                 break;
              case 'c':
              case 'C':
              case CTRL_C:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_C;
                 break;
              case 'd':
              case 'D':
              case CTRL_D:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_D;
                 break;
              case 'e':
              case 'E':
              case CTRL_E:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_E;
                 break;
              case 'l':
              case 'L':
              case CTRL_L:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_L;
                 break;
              case 'n':
              case 'N':
              case CTRL_N:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_N;
                 break;
              case 'o':
              case 'O':
              case CTRL_O:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_O;
                 break;
              case 'p':
              case 'P':
              case CTRL_P:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_P;
                 break;
              case 'r':
              case 'R':
              case CTRL_R:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_R;
                 break;
              case 's':
              case 'S':
              case CTRL_S:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_S;
                 break;
              case 't':
              case 'T':
              case CTRL_T:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_T;
                 break;
              case 'v':
              case 'V':
              case CTRL_V:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_V;
                 break;
              case 'w':
              case 'W':
              case CTRL_W:
                 ((Ctrl_teclado*)this)->tp = CTRL_N_W;
                 break;
            }
         }
         if(Press == CTRL_O) {
            switch(tp) {
              case 'n':
              case 'N':
              case CTRL_N:
                 ((Ctrl_teclado*)this)->tp = CTRL_O_N;
                 break;
            }
         }
         if(Press == CTRL_Q) {
            switch(tp) {
              case 'a':
              case 'A':
              case CTRL_A:
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_A;
                 break;
              case 'b':
              case 'B':
              case CTRL_B:
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_B;
                 break;
              case 'c':
              case 'C':
              case CTRL_C:
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_C;
                 break;
              case 'f':
              case 'F':
              case CTRL_F:
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_F;
                 break;
              case 'k':
              case 'K':
              case CTRL_K:
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_K;
                 break;
              case 'r':
              case 'R':
              case CTRL_R:
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_R;
                 break;
              case '1':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_1;
                 break;
              case '2':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_2;
                 break;
              case '3':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_3;
                 break;
              case '4':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_4;
                 break;
              case '5':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_5;
                 break;
              case '6':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_6;
                 break;
              case '7':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_7;
                 break;
              case '8':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_8;
                 break;
              case '9':
                 ((Ctrl_teclado*)this)->tp = CTRL_Q_9;
                 break;
            }
         }
         ((Ctrl_teclado*)this)->Press = 0;
         Almacena_bufer_teclado(tp);
       } else {
         switch(tp) {
            case CTRL_M:
               ((Ctrl_teclado*)this)->Press = CTRL_M;
               break;
            case CTRL_J:
               ((Ctrl_teclado*)this)->Press = CTRL_J;
               break;
            case CTRL_F:
               ((Ctrl_teclado*)this)->Press = CTRL_F;
               break;
            case CTRL_K:
               ((Ctrl_teclado*)this)->Press = CTRL_K;
               break;
            case CTRL_N:
               ((Ctrl_teclado*)this)->Press = CTRL_N;
               break;
            case CTRL_O:
               ((Ctrl_teclado*)this)->Press = CTRL_O;
               break;
            case CTRL_Q:
               ((Ctrl_teclado*)this)->Press = CTRL_Q;
               break;
            default:
               Almacena_bufer_teclado(tp);
         }
      }
   }
}



// Limpia el contenido del bufer del teclado
void Ctrl_teclado::Limpia_bufer_teclado(void) const
{
   for(((Ctrl_teclado*)this)->i = 0; i < LONGITUD_BUFER_TECLADO; ((Ctrl_teclado*)this)->i++) ((Ctrl_teclado*)this)->BUFER_TECLADO[i] = 0;
   ((Ctrl_teclado*)this)->i = 1;
   while(i) {
      if(!bioskey(1)) ((Ctrl_teclado*)this)->i = 0;
      if(i) bioskey(0);
   }
   ((Ctrl_teclado*)this)->Press = 0;
}



// Almacena un codigo del teclado o el carcater indicado en TEC en el bufer
// del teclado, en caso de exeder su longitud no es almacenado
void Ctrl_teclado::Almacena_bufer_teclado(const int tec) const
{
////////////////
//   printf("%d\n",tec);  // Solo para ver codigo
////////////////
   ((Ctrl_teclado*)this)->i = 0;
   while(BUFER_TECLADO[i] && i < LONGITUD_BUFER_TECLADO) ((Ctrl_teclado*)this)->i++;
   if(i < LONGITUD_BUFER_TECLADO) BUFER_TECLADO[i] = tec;
}



// Almacena una cadena en el bufer de teclado, los careacteres que saturen
// al bufer seran descartados
void Ctrl_teclado::Almacena_cadena_bufer_teclado(const char *cad) const
{
   while(*cad) {
     Almacena_bufer_teclado((int) *cad);
     cad++;
   }
}


// Busca un caracter o codigo de teclado en el bufer, retornando su posicion
// mas uno si esta, o cero si no fue encontrado
int Ctrl_teclado::Busca_bufer_teclado(const int tec) const
{
   Lee_bufer_teclado();
   ((Ctrl_teclado*)this)->i = 0;
   while(BUFER_TECLADO[i] && i < LONGITUD_BUFER_TECLADO) {
      if(BUFER_TECLADO[i] == tec) return i+1;
      ((Ctrl_teclado*)this)->i++;
   }
   return 0;
}


// Retorna el primer caracter o codigo de tecla pulsado, Si retorna el caracter
// el codigo es cero, en caso contrario el caracter es nulo
void Ctrl_teclado::Retorna_bufer_teclado(char &caracter, int &tecla) const
{
   Lee_bufer_teclado();
   tecla = BUFER_TECLADO[0];
   caracter = 0;
   if(tecla) {
      for(((Ctrl_teclado*)this)->i = 0; i < LONGITUD_BUFER_TECLADO - 1; ((Ctrl_teclado*)this)->i++) BUFER_TECLADO[i] = BUFER_TECLADO[i+1];
      BUFER_TECLADO[LONGITUD_BUFER_TECLADO - 1] = 0;
      if(tecla > 0 && tecla < 255)  caracter = (char) tecla, tecla = 0;
   } else tecla = -1;
}


// Retorna el actual tecla y/o caracter en el bufer sin borrar este del bufer
void Ctrl_teclado::Actual_tecla_bufer(char &caracter, int &tecla) const
{
   Lee_bufer_teclado();
   tecla = BUFER_TECLADO[0];
   caracter = 0;
   if(!tecla) {
      if(tecla > 0 && tecla < 255) tecla = 0, caracter = (char) tecla;
   } else tecla = -1;
}


/*
void velocidad_teclado(const unsigned int velocidad);
  // Permite modificar la velocidad del teclado, el rango es de (0 - 15)
  // Si VELOCIDAD es (0)  Velocidad baja
  //                 (7)  Velocidad media
  //                 (15) Velocidad alta


#include <dos.h>



void velocidad_teclado(const unsigned int velocidad)
{
   if(velocidad < 16) {
      unsigned int coun = 0, stat = 0, vel = 0;
      vel = 15 - velocidad;
      outportb(0x60,0xf3);
      do {
         stat = inportb(0x60);
         coun++;
      } while ((coun < 1000) && (stat != 0xfa));
      if(coun < 1000) outportb(0x60,vel);
   }
}






*/
