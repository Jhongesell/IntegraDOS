 // Autor:     Antonio Carrillo Ledesma. 
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

extern "C" {
   #include <dir.h>
   #include <string.h>
}

void nombre_drive_logico(const unsigned int t_p, const unsigned int n_m_d, char * drive);
// Regresa el nombre del drive logico instalado en el sistema con la mascara [- -]
// si T_P es distinto de cero inicializa la busqueda, en caso contrario continua esta
// apartir del ultimo drive encontrado, si son todos retorna un puntero nulo


void nombre_drive_logico(const unsigned int t_p, const unsigned int n_m_d, char *drive)
{
   static int disk = 0;
   int save = getdisk(), done = 1;
   strcpy(drive,"[- -]");
   if(t_p) disk = 0;
   while(done) {
      if(disk < n_m_d) {
         setdisk(disk);
         if(disk == getdisk())  {
            drive[2] = 65 + disk;
            done = 0;
         }
       } else {
         done = 0;
         drive[0] = 0;
      }
      disk++;
   }
   setdisk(save);
}
