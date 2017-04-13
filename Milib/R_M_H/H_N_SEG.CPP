// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


long n_seg(void);
  // Regresa el numero de segundos desde el comienzo del dia, rango de [0 - 86400]

extern "C" {
   #include <dos.h>
}

long n_seg(void)
{
  long n_s = 0;
  struct time t;
  gettime(&t);
  n_s = long (t.ti_hour * 3600L) + long (t.ti_min * 60L) + long (t.ti_sec);
  return n_s;
}