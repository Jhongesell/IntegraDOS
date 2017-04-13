// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


int cambia_directorio(const char *path);
// Cambia el actual directorio al especificado por PATH


#include "cad_car.hpp"
#include "r_var.hpp"

extern "C" {
   #include <string.h>
   #include <dir.h>
   #include <dos.h>
}


extern char *PATH_ACTUAL;


int cambia_directorio(const char *path)
{
    int err = 0;
    char xpath[MAXPATH];
    strcpy(xpath,path);
  inicio_reconocimiento:
    s_trim(xpath);
    if(!xpath[0]) return 0;
    // Cambia de drive si es necesario
    if(xpath[1] == ':') {
       char unid[2];
       unid[0] = xpath[0], unid[1] = 0;
       xpath[0] = xpath[1] = 32;
       cambia_unidad(unid);
       goto inicio_reconocimiento;
    }
    // Sube un nivel en el arbol
    if(xpath[0] == 46 && xpath[1] == 46) {
       chdir("..");
       xpath[0] =  xpath[1] = 32;
       if(xpath[2] == 92) xpath[2] = 32;
       goto inicio_reconocimiento;
    }
    // Permanece en el mismo nivel
    if(xpath[0] == 46 && xpath[1] == 92) {
       xpath[0] =  xpath[1] =  32;
       goto inicio_reconocimiento;
    }
    // Hace el cambio de directorio
    if(n_archivos_directorios(xpath,FA_DIREC)) chdir(xpath);
    getcwd(PATH_ACTUAL,80);
    return err;
}

