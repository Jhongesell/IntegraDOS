// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



char s_comv_may_min(const char car, const unsigned int t_p);
  // Comvierte el caracter CAR segun la siguiente convicci¢n
  // Si T_P es   (1) comvierte de mayuscula a miniscula y vice versa
  //             (2) convierte a minuscula
  //             (3) convierte a mayuscula


char s_conv_may_min(const char car, const unsigned int t_p)
{
   char x_c = car;
   if(t_p == 2) {
      if(car >= 65 && car <= 90) x_c = car + 32;
      if(car == 165) x_c = 164;
   }
   if(t_p == 3) {
      if(car >= 97 && car <= 122) x_c = car -32;
      if(car == 164) x_c = 165;
   }
   if(t_p == 1) {
      if(car >= 65 && car <= 90) x_c = car + 32;
       else if(car >= 97 && car <= 122) x_c = car - 32;
      if(car == 164) x_c = 165;
       else if(car == 165) x_c = 164;
   }
   if(t_p == 1 || t_p == 3) {
      if(car == 160 || car == 133) x_c = 65;  // a con acento
      if(car == 138 || car == 130) x_c = 69;  // e con acento
      if(car == 161 || car == 141) x_c = 73;  // i con acento
      if(car == 162 || car == 149) x_c = 79;  // o con acento
      if(car == 163 || car == 151 || car == 129) x_c = 85;  // u con acento o dieresis
   }
   return x_c;
}

