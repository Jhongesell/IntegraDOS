// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


unsigned int dia_sem(const unsigned long dia_juliano);
  // De una fecha con formato juliano, retorna el numero de d¡a de la semana con rango [0 - 7]
  // (1) Domingo, (2) Lunes, (3) Martes, (4) Miercoles, (5) Jueves,
  // (6) Viernes, (7) Sabado, (0) Error en la fecha juliana
  // Nombre del archivo: F_D_SEMJ.CPP




unsigned int f_dia_sem(const unsigned long dia_juliano)
{
   unsigned int d_s;
   if(dia_juliano > 0) d_s = (unsigned int)  ((( dia_juliano + 1) % 7) + 1);
   return d_s;
}
