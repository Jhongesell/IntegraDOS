// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



unsigned int f_n_max_dias_mes(const unsigned int mes, const unsigned int ano);
  // Retorna el numero maximo de dias por el mes (MES) y ano (ANO) pasados como parametros
  // Nombre del archivo: F_NMAXDG.CPP



unsigned int f_n_max_dias_mes(const unsigned int mes, const unsigned int ano)
{
   unsigned int n_m_d = 0,n_md[15] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
   if(mes == 0 || mes > 12) n_m_d = 0;
    else {
      if(mes == 2 && (ano % 4  == 0 && (ano % 100 != 0 || ano % 400 == 0))) n_m_d = 29;
       else n_m_d = n_md[mes];
   }
   return n_m_d;
}
