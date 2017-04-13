// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void hora_act(char *cad, const unsigned int t_h);
  // Retorna la hora actual del sistema modificando la cadena CAD, T_H indica si visualiza
  // en formato  (0) HH:MM:SS                  [HH rango 00 - 23]
  //	           (1) HH:MM:SS [A.M. o P.M.]    [HH rango 00 - 12]
  //             (2) HH:MM                     [HH rango 00 - 23]
  //             (3) HH:MM [A.M. o P.M.]       [HH rango 00 - 12]


extern "C" {
   #include <conio.h>
   #include <stdio.h>
   #include <dos.h>
   #include <string.h>
}

void hora_act(char *cad, const unsigned int t_h)
{
   char *x_cad = "P.M.", buff[40];
   struct time t;
   gettime(&t);
   if(t_h == 1 || t_h == 3) {
      if(t.ti_hour > 12) t.ti_hour -= 12;
       else {
         if(t.ti_hour != 12) x_cad = "A.M.";
      }
   }
   sprintf(buff,"%#02d:%#02d:%#02d %s",t.ti_hour,t.ti_min,t.ti_sec,x_cad);
   if(!t_h) buff[8] = 0;
   if(t_h == 2 || t_h == 3) {
      if(buff[0] == '0' && buff[1] != '0') buff[0] = 32;
      buff[5] = buff[8];
      buff[6] = buff[9];
      buff[7] = buff[10];
      buff[8] = buff[11];
      buff[9] = buff[12];
      buff[10] = buff[13];
   }
   strcpy(cad,buff);
}


