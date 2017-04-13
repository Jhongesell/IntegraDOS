// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void n_seg__horas(const long n_seg, char *horas);
  // Retorna una cadena de horas a partir de el numero de segundos,
  // la hora es con el formato HH:MM:SS

extern "C" {
   #include <stdio.h>
}

void n_seg__horas(const long n_seg, char *horas)
{
   long n_s = n_seg;
   int n_h = 0, n_m = 0;
   if(n_s < 0 || n_s > 86400l) n_s = 0;
   while(n_s >= 3600) n_s -= 3600, n_h++;
   while(n_s >= 60)   n_s -= 60,   n_m++;
   sprintf(horas,"%#02d:%#02d:%#02d", n_h, n_m, n_s);
}
