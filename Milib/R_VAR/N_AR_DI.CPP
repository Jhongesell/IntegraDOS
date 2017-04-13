// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

extern "C" {
   #include <dos.h>
   #include <dir.h>
}


// Retorna el numero de archivos y/o directorios encontrados en la trayectoria
// PATH segun las indicaciones de busqueda especificada en ATRIB_BUS
// Atributos (FA_ARCH)   Archivo
//           (FA_DIREC) Subdirectorios
int n_archivos_directorios(const char *path, const unsigned int atrib_bus);



int n_archivos_directorios(const char *path, const unsigned int atrib_bus)
{
   int done = 0, i = 0;
   struct ffblk est_arch;
   done = findfirst(path,&est_arch,FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_DIREC|FA_ARCH);
   while(!done) {
      if(!(est_arch.ff_name[0]== 46 && est_arch.ff_name[1] != 46)) {
         // Solo cuenta subdirectorios salvo el propio
         if(atrib_bus == FA_DIREC) {
            if(est_arch.ff_attrib & FA_DIREC) i++;
         }
         if(atrib_bus == FA_ARCH) {
            if(!(est_arch.ff_attrib & FA_DIREC)) i++;
         }
      }
      done = findnext(&est_arch);
   }
   return i;
}

