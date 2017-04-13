// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



long esp_disco(const int unidad, const int t_p);
  // Retorna si T_P es (1) Espacio libre   en la unidad, si UNIDAD es (0) actual
  //                   (2) Espacio total                              (1) A
  //                   (3) Espacio usado                              (2) B
  //                                                                  (3) C
  //                                                                  (4) D
  // Regresara -1 si hay algun error

extern "C" {
   #include <dos.h>
}


long esp_disco(const int unidad, const int t_p)
{
  long t_b = 0;
  struct dfree p;
  getdfree(unidad,&p);
  if(p.df_sclus == 0xffff) t_b = -1;  // Error por unidad
   else {
     if(t_p == 1) t_b = (long) p.df_avail * (long) p.df_sclus * (long) p.df_bsec;
     if(t_p == 2) t_b = (long) p.df_total * (long) p.df_sclus * (long) p.df_bsec;
     if(t_p == 3) t_b = (((long) p.df_total * (long) p.df_sclus * (long) p.df_bsec) - ((long) p.df_avail * (long) p.df_sclus * (long) p.df_bsec));
  }
  return t_b;
}
