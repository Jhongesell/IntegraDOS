// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


void f_f_hoy(unsigned int &dia, unsigned int &mes, unsigned int &ano);
  // Regresa la fecha actual del sistema con formato gregoriano en las variables DIA, MES, ANO
  // Nota: Esta rutina modifica las variables DIA, MES, ANO en su valor
  // Nombre del archivo: F_F_HOYG.CPP




extern  "C" {
   #include <dos.h>
}

void f_f_hoy(unsigned int &dia, unsigned int &mes, unsigned int &ano)
{
   struct date e_f;
   getdate(&e_f);
   dia = (unsigned int) e_f.da_day;
   mes = (unsigned int) e_f.da_mon;
   ano = (unsigned int) e_f.da_year;
}
