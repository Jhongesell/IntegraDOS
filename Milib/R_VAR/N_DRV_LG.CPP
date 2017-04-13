 // Autor:     Antonio Carrillo Ledesma. 
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A

extern "C" {
   #include <dir.h>
}


int numero_drives_logicos(const unsigned int n_m_d); 
// Retorna el numero de drives logicos habilitados en la computadora


int numero_drives_logicos(const unsigned int n_m_d) 
{ 
   int save = 0, disk = 0, n_drvs = 0; 
   save = getdisk(); 
   for(disk = 0;disk < n_m_d ;disk++) { 
       setdisk(disk);
       if(disk == getdisk()) n_drvs++;
   }
   setdisk(save);
   return n_drvs;
} 
