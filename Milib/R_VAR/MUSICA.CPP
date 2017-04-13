// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void musica(void);
  // Rutina que genera musica hasta que se pulse alguna tecla


#include <dos.h>
#include <stdlib.h>
#include <conio.h>

void musica(void)
{
   int frec;
   unsigned char p;
   union {
     long divisor;
     unsigned char c[2];
   } cuenta;

   do {
      frec = rand();
   } while(frec > 8000);
   cuenta.divisor = 1193280l / frec;
   outportb(67, 182);
   outportb(66, cuenta.c[0]);
   outportb(66, cuenta.c[1]);
   p = inportb(97);
   outportb(97,p | 3);
   delay(280);
   outportb(97, p);
}
