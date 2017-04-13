// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



void laser(void);
// Rutina que genera un sonido de laser


#include <dos.h>

void laser(void)
{
  unsigned int frec;
  union {
    unsigned char c[2];
    long divisor;
  } cuenta;
  unsigned char p;
  p = inportb(97);
  outportb(97,p | 3);
  for(frec = 5000; frec >= 1000; frec -= 100) {
     cuenta.divisor = 1193280l / frec;
     outportb(67, 182);
     outportb(66, cuenta.c[0]);
     outportb(66, cuenta.c[1]);
     delay(30);
  }
  outportb(97, p);
}
