// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int s_es_mayusculas(const char *cad);
  // Rutina que retorna verdadero si no contiene caracteres alfabeticos en minusculas
  // en caso contrario retorna (0)
  // Soporta caracteres como  …‚Š¡¢•£—¤¥
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena




int s_es_mayusculas(const char *cad)
{
   while(*cad) {
      if(!((*cad > 64 && *cad < 91) || *cad == 165)) return 0; 
   }
   return 1;
}
