// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A



extern "C" {
   #include <dir.h>
   #include <dos.h>
   #include <stdlib.h>
   #include <string.h>
}
#include "cad_car.hpp"


// Retorna el nombre del archivo y/o directorio que se encuentren en el PATH,
// segun los atributos de busqueda indicados en ATRIB_BUS, si T_P es distinto de cero
// inicializa la busqueda en caso contrario la ontinua apartir del ultimo archivo
// encontrado
// Atributos (FA_ARCH)   Archivo
//           (FA_DIREC) Subdirectorios
void nombre_archivo_directorio(const char *path, const unsigned int atrib_bus, const unsigned int t_p, char *arch);


void nombre_archivo_directorio(const char *path, const unsigned int atrib_bus, const unsigned int t_p, char *archivo)
{
   static int done = 0;
   static struct ffblk est_arch;
   archivo[0] = 0;
   if(t_p) done = findfirst(path,&est_arch,FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_DIREC|FA_ARCH);
   if(!done) {
      // Descarta el subdirectorio actual
      if(est_arch.ff_name[0] == 46 && est_arch.ff_name[1] != 46) done = findnext(&est_arch);
      // Solo considera a los Subdirectorios
      if(atrib_bus == FA_DIREC) {
         if(!(est_arch.ff_attrib & FA_DIREC)) {
            while(!done) {
              done = findnext(&est_arch);
              if(est_arch.ff_attrib & FA_DIREC) break;
            }
            if(done) return;
         }
      }
      // Solo considera a los archivos
      if(atrib_bus == FA_ARCH) {
         if(est_arch.ff_attrib & FA_DIREC) {
            while(!done) {
              done = findnext(&est_arch);
              if(!(est_arch.ff_attrib & FA_DIREC)) break;
            }
            if(done) return;
         }
      }
      
      s_r_t(est_arch.ff_name,14,32,archivo);
//      // Retorna la longitud del archivo
//      if(atrib_bus != FA_DIREC && est_arch.ff_attrib != FA_DIREC) {
//         ltoa(est_arch.size,bytes,10);
//         strcat(archivo,s_formato(bytes,12,0));
//      }
      done = findnext(&est_arch);
   }
}
