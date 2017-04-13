// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void Reinicializa_sistema(const int tp);
// Reinicializa el sistema
  // (1) Reinicializacion en frio
  // (0) Reinicializacion en caliente



void Reinicializa_sistema(const int tp)
{
   int MAGIC = 4660;
   long BOOT_SEG = 0xffffL, BOOT_OFF = 0x0000L, BOOT_ADR  = ((BOOT_SEG << 16) | BOOT_OFF);
   long DOS_SEG  = 0x0040L, RESET_FLAG = 0x0072L, RESET_ADR = ((DOS_SEG << 16) | RESET_FLAG);

   if(tp) MAGIC = 0;
   void ((far *fp)()) = (void (far *)()) BOOT_ADR;
   *(int far *)RESET_ADR = MAGIC;
   (*fp)();
}

