// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Pasar a win 32
///////////////////////////////////////////////////////////////////////////////////////////////////////

void remueve_archivos(const char *files, const int tp);
// Remueve los archivos especificados en el path, en caso de tener permisos de solo
// lectura y/o oculto son cambiados a archivo normal. Despues de borra el archivo
// Si tp es (1) reescribe este para evitar su recuperaci¢n.

#include "r_esp.hpp"
#include "r_var.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;


extern "C" {
   #include <string.h>
   #include <stdio.h>
   #include <dos.h>
   #include <io.h>
   #include <dir.h>
   #include <sys\stat.h>
}


#ifdef _IDIOMA_ESPANOL_
   static const char *RUTINA = "REMUEVE ARCHIVO";
#else
   static const char *RUTINA = "DELETE FILE";
#endif


void remueve_archivos(const char *files, const int tp)
{
   unsigned int attr, handle, na = 0, i = 0;
   char arch[MAXPATH];
   na = n_archivos_directorios(files,FA_ARCH);
   for(i = 0; i < na; i++) {
      nombre_archivo_directorio(files,FA_ARCH,!i,arch);
      arch[14] = 0;
      // Revisa los atributos del archivos
      if(_dos_getfileattr(arch,&attr) != 0) {
         // Quita atributo HIDEN
         if(attr & FA_HIDDEN) {
            attr &= ~FA_HIDDEN;
           _dos_setfileattr(arch,attr);
         }
         // Quita atributo de READ ONLY
         if(attr & FA_RDONLY) {
            attr &= ~FA_RDONLY;
           _dos_setfileattr(arch,attr);
         }
         // Reescribe el archivo
         if(tp) {
            handle = creat(arch,S_IREAD | S_IWRITE);
            write(handle,RUTINA,8);
            close(handle);
         }
      }
      // Borra el archivo
      if(unlink(arch)) NCO->Ctrl_error(7,arch,RUTINA);
   }
}


   /*
   char *msg = "Error no se puede borrar el archivo:";
   int done, handle;
   unsigned int attr;
   struct ffblk ffblk;
   done = findfirst(files,&ffblk,FA_ARCH | FA_HIDDEN | FA_SYSTEM | FA_RDONLY);
   while(!done) {
      // Quita atributo HIDEN
      _dos_getfileattr(ffblk.ff_name,&attr);
      if(attr & FA_HIDDEN) {
         attr &= ~FA_HIDDEN;
        _dos_setfileattr(ffblk.ff_name,attr);
      }
      // Quita atributo de READ ONLY
      _dos_getfileattr(ffblk.ff_name,&attr);
      if(attr & FA_RDONLY) {
         attr &= ~FA_RDONLY;
        _dos_setfileattr(ffblk.ff_name,attr);
      }
      if(tp) {
         // Reescribe el archivo
         handle = creat(ffblk.ff_name,S_IREAD | S_IWRITE);
         write(handle,msg,8);
         close(handle);
      }
      // Borra el archivo
      if(unlink(ffblk.ff_name)) ctrl_err(7,ffblk.ff_name,"REMUEVE ARCHIVO");
      done = findnext(&ffblk);
   }
   */

