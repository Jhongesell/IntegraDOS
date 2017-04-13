// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_greg__fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano, char *fecha);
  // De una fecha con formato gregoriano, retorna una fecha con formato cadena
  // En caso de error en la fecha retornar  una cadena en blanco
  // Nombre del archivo: F_GREFEC.CPP




// Parametros definidos en el archivo VAR_GLOB.HPP
extern int  TIPO_FECHA;       // Indica el tipo de fecha (0) DDYYMM, (1) MMDDYY, (2) YYMMDD
extern int  CENTURIA;         // Indica si se activa la centuria
extern char DELIMIT_FECHA;    // Es el delimitador en la fecha '/', '-', '.'
extern char ACT_CENTURIA[];   // Contiene la centuria actual


#include "fechas.hpp"
extern  "C" {
   #include <stdlib.h>
   #include <string.h>
}



void f_greg__fecha(const unsigned int dia, const unsigned int mes, const unsigned int ano, char *fecha)
{
    unsigned int n_m_d, i = 0, xa = 0;
    n_m_d = f_n_max_dias_mes(mes,ano);
    if(dia == 0 || dia > n_m_d || mes == 0 || mes > 12 || ano == 0 || ano > 2100) {
       strcpy(fecha,"        ");
       return;
    }

    char xf[30], ldia[30], lmes[30], x_cad[100];
    itoa(dia,x_cad,10);
    if(dia > 9) {
       ldia[0] = x_cad[0];
       ldia[1] = x_cad[1];
     } else {
       ldia[0] = 48;
       ldia[1] = x_cad[0];
    }

    itoa(mes,x_cad,10);
    if(mes > 9) {
       lmes[0] = x_cad[0];
       lmes[1] = x_cad[1];
     } else {
       lmes[0] = 48;
       lmes[1] = x_cad[0];
    }

    xa = ano;
    if(ano < 100) xa +=  (atoi(ACT_CENTURIA) * 100);
    itoa(xa,x_cad,10);

    if(TIPO_FECHA != 1 && TIPO_FECHA != 2) {
       xf[0] = ldia[0];
       xf[1] = ldia[1];
       xf[2] = DELIMIT_FECHA;
       xf[3] = lmes[0];
       xf[4] = lmes[1];
       xf[5] = DELIMIT_FECHA;
       if(CENTURIA) {
          xf[6] = x_cad[0];
          xf[7] = x_cad[1];
          i = 2;
       }
       xf[6 + i] = x_cad[2];
       xf[7 + i] = x_cad[3];
       xf[8 + i] = 0;
    }


    if(TIPO_FECHA == 1) {
       xf[0] = lmes[0];
       xf[1] = lmes[1];
       xf[2] = DELIMIT_FECHA;
       xf[3] = ldia[0];
       xf[4] = ldia[1];
       xf[5] = DELIMIT_FECHA;
       if(CENTURIA) {
	  xf[6] = x_cad[0];
	  xf[7] = x_cad[1];
	  i = 2;
       }
       xf[6 + i] = x_cad[2];
       xf[7 + i] = x_cad[3];
       xf[8 + i] = 0;
    }

    if(TIPO_FECHA == 2) {
       if(CENTURIA) {
	  xf[0] = x_cad[0];
	  xf[1] = x_cad[1];
	  i = 2;
       }
       xf[0 + i] = x_cad[2];
       xf[1 + i] = x_cad[3];
       xf[2 + i] = DELIMIT_FECHA;
       xf[3 + i] = lmes[0];
       xf[4 + i] = lmes[1];
       xf[5 + i] = DELIMIT_FECHA;
       xf[6 + i] = ldia[0];
       xf[7 + i] = ldia[1];
       xf[8 + i] = 0;
    }
    strcpy(fecha,xf);
}

