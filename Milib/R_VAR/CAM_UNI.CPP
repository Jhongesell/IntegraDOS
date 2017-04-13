// Autor:     Antonio Carrillo Ledesma. 
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


#include "r_esp.hpp"
#include "nucleo.hpp"

// Objeto global al nuclo del sistema
extern Nucleo *NCO;

extern "C" {
   #include <direct.h>
   #include <dir.h>
   #include <bios.h>
}

extern char *PATH_ACTUAL;



int cambia_unidad(const char *path);
// Cambia la actual unida por la especificada en el PATH, valida la existencia de
// diskettes en la unidad A y B


int cambia_unidad(const char *path)
{
   getcwd(PATH_ACTUAL,MAXPATH);
   if(PATH_ACTUAL[0] == path[0]) return 0;
   int err = 1, xdisk = path[0] - 64, st = 1;
   if(xdisk == 1 || xdisk == 2) {
      struct diskinfo_t dinfo;
      int result;
      static char dbuf[512];
      dinfo.drive =  xdisk -1;    /* drive number for A: */
      dinfo.head  =  0;           /* disk head number */
      dinfo.track =  0;           /* track number */
      dinfo.sector  =  1;         /* sector number */
      dinfo.nsectors =  1;        /* sector count */
      dinfo.buffer = dbuf;        /* data buffer */
      result = _bios_disk(_DISK_VERIFY, &dinfo);
      if((result & 0xff00) != 0) {
          NCO->Ctrl_error(12,"",path);
          st = 0;
      }
   }
   if(st){
      if(_chdrive(xdisk)) err = 0;
      getcwd(PATH_ACTUAL,MAXPATH);
   }
   return err;
}


