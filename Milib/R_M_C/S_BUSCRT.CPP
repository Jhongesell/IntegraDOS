// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


////////////////////////
// No se a revisado
////////////////////////


int s_bus_todos_car(const char *cad, const char car, const unsigned int t_p);
  // Indica la posici¢n en que fue encontrado el caracter CAR en la cadena CAD,
  // inicia en 0, retorna -1 si no lo encuentra
  // Si T_P es (0) Encuentra la primera ocurrencia
  //           (1) Encuentra la siguente ocurrencia (Utiliza variable estatica
  //               para guardar la posici¢n de la ultima ocurrencia)
  // Nombre del archivo: S_BUSCAU.CPP



int s_bus_todos_car(const char *cad, const char car, const unsigned int t_p)
{
   static unsigned int pos = 0;
   if(!t_p) pos = 0;
    else pos++;
   while(cad[pos]) {
      if(cad[pos] == car) return pos;
      pos++;
   }
   pos = 0;
   return -1;
}
