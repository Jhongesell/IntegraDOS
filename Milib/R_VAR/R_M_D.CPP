// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned long esp_memoria(const int t_p);
  // Retorna la memoria libre, si T_P es (0) Memoria del monton
  //                                     (1) Memoria del monton lejano

extern "C" {
   #include <alloc.h>
}

unsigned long esp_memoria(const int t_p)
{
  if(t_p) return farcoreleft();   // 1 Memoria monton lejano.
  return coreleft();              // 0 Memoria del monton.
}

