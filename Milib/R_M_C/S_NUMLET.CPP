// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

/*
#include "cad_car.hpp"
extern "C" {
   #include <stdlib.h>
   #include <string.h>
}

static char *UNID[] = {"UNO","DOS","TRES","CUATRO","CINCO","SEIS","SIETE","OCHO","NUEVE","CERO","UN"};
static char *ESPE[] = {"DIEZ","ONCE","DOCE","TRECE","CATORCE","QUINCE"};
static char *DECE[] = {"DIEC","VEINT","TREINTA","CUARENTA","CINCUENTA","SESENTA","SETENTA","OCHENTA","NOVENTA"};
static char *CENT[] = {"CIENTO ","DOCIENTOS ","TRECIENTOS ","CUATROCIENTOS ","QUINIENTOS ","SEICIENTOS ","SETECIENTOS ","OCHOCIENTOS ","NOVECIENTOS "};


char *S_CONV_NUMERO_A_LETRA(char cat_num[]);
void s_rellena_car_d(char *cadena, const char car, const unsigned int lg);


void s_conv_numero_a_letra(const char *cad, char *cad_ent, char *cad_dec)
{
   char xcad[40], texte[40], textd[5];
   *(cad_ent) = 0;
   *(cad_dec) = 0;
   s_for_num(cad,18,2,xcad);
   s_rellena_car_d(xcad,32,21);
   // Conversi¢n de decimales
   textd[0] = 32, textd[1] = xcad[19], textd[2] = xcad[20], textd[3] = 0;
   strcpy(cad_dec,S_CONV_NUMERO_A_LETRA(textd));
   // Conversi¢n de enteros
   s_lsubstr(xcad,18,texte);
   // Si es cero
   if(atoi(texte) == 0) {
      strcpy(cad_ent,UNID[9]);
      return;
   }
   // Miles de billones
   texte[0] = xcad[0], texte[1] = xcad[1], texte[2] = xcad[2], texte[3] = 0;
   if(atoi(texte) > 0) {
      strcpy(cad_ent,S_CONV_NUMERO_A_LETRA(texte));
      if(atoi(texte) > 1) strcat(cad_ent," MIL BILLONES ");
       else strcat(cad_ent," MIL BILLON ");
   }
   // Billones
   texte[0] = xcad[3], texte[1] = xcad[4], texte[2] = xcad[5], texte[3] = 0;
   if(atoi(texte) > 0) {
      strcat(cad_ent,S_CONV_NUMERO_A_LETRA(texte));
      if(atoi(texte) > 1) strcat(cad_ent," BILLONES ");
       else strcat(cad_ent," BILLON ");
   }
   // Miles de millones
   texte[0] = xcad[6], texte[1] = xcad[7], texte[2] = xcad[8], texte[3] = 0;
   if(atoi(texte) > 0) {
      strcat(cad_ent,S_CONV_NUMERO_A_LETRA(texte));
      if(atoi(texte) > 1) strcat(cad_ent," MIL MILLONES ");
       else strcat(cad_ent," MIL MILLON ");
   }
   // Millones
   texte[0] = xcad[9], texte[1] = xcad[10], texte[2] = xcad[11], texte[3] = 0;
   if(atoi(texte) > 0) {
      strcat(cad_ent,S_CONV_NUMERO_A_LETRA(texte));
      if(atoi(texte) > 1) strcat(cad_ent," MILLONES ");
       else strcat(cad_ent," MILLON ");
   }
   // Miles
   texte[0] = xcad[12], texte[1] = xcad[13], texte[2] = xcad[14], texte[3] = 0;
   if(atoi(texte) > 0) {
      strcat(cad_ent,S_CONV_NUMERO_A_LETRA(texte));
      strcat(cad_ent," MIL ");
   }
   // Cientos
   texte[0] = xcad[15], texte[1] = xcad[16], texte[2] = xcad[17], texte[3] = 0;
   if(atoi(texte) > 0) {
      strcat(cad_ent,S_CONV_NUMERO_A_LETRA(texte));
   }
}

void S_CONV_NUMERO_A_LETRA(char can_num[], char *can_let)
{
   can_let[0] = 0;
   int val = 0, i = 0;
   val = atoi(can_num);
   if(val == 0 || val == 1 || val == 100 || val == 200 || val == 300 || val == 400 || val == 500 || val == 600 || val == 700 || val == 800 || val == 900) {
      // Cero y uno
      if(val == 0 || val == 1) strcpy(can_let,UNID[9 + val]);
      // Cien
      if(val == 100) strcpy(can_let,"CIEN");
      // Trecientos a novecientos
      i = can_num[0] - 49;
      if(val >= 200 && val <= 900) strcat(can_let,CENT[i]);
    } else {
      // Centenas
      if(val > 99 && val < 1000) {
         i = can_num[0] - 49;
         strcpy(can_let,CENT[i]);
      }
      // Decenas
      can_num[0] = 32;
      val = atoi(can_num);
      if((val > 0 && val <= 15) || val == 20 || val == 30 || val == 40 || val == 50 || val == 60 || val == 70 || val == 80 || val == 90) {
         // Unidades del dos al nueve
         if(val >= 1 && val <= 9) {
            i = can_num[2] - 49;
            strcat(can_let,UNID[i]);
         }
         // Diez al quince
         if(val >= 10 && val <= 15) {
            i = can_num[2] - 48;
            strcat(can_let,ESPE[i]);
         }
         // Veinte
         if(val == 20) strcat(can_let,"VEINTE");
         // Treinta al noventa
         i = can_num[1] - 49;
         if(val >= 30 && val <= 90) strcat(can_let,DECE[i]);
       } else {
         if(val > 9 && val < 100) {
            i = can_num[1] - 49;
            strcat(can_let,DECE[i]);
         }
         if(val > 15 && val < 30) strcat(can_let,"I");
          else strcat(can_let," Y ");
         can_num[1] = 32;
         val = atoi(can_num);
         if(val > 0 && val < 10) {
            i = can_num[2] - 49;
            strcat(can_let,UNID[i]);
          }
      }


   }
}


void s_rellena_car_d(char *cadena, const char car, const unsigned int lg)
{
   char xcad[300], cadr[300];
   int xlg = 0, i = 0, n_esp = 0;
   s_trim(cadena,xcad);
   n_esp = lg - strlen(xcad);
   for(i = 0; i < n_esp;  i++)       cadr[i] = car;
   for(i = n_esp; i < lg; i++,xlg++) cadr[i] = xcad[xlg];
   cadr[lg] = 0;
   strcpy(cadena,cadr);
}


*/
