// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


extern "C" {
   #include <string.h>
}


int s_pos_ant_car_dist(const char *cad,const char car, const unsigned int posi, const unsigned int posf);
  // Retorna la posicion del anterior caracter distinto de CAR en la cadena CAD
  // la busqueda inicia el POSI y termina en POSF. Si POSF es cero asume hasta el final de la cadena
  // Nombre del archivo: S_PACARD.CPP



int s_pos_ant_car_dist(const char *cad,const char car, const unsigned int posi, const unsigned int posf)
{
   int lg = strlen(cad) - 1;
   if(posf && posf < lg) lg = posf + 1;
   for(;lg > posi; lg --) {
        if(cad[lg] != car) return lg;
   }
   return 0;
}

