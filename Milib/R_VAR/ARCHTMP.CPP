// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// Tel‚fono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr mite
// Revisi¢n  1.1-A


// Path del sistema
extern char *PATH_ORIGINAL;

extern "C" {
   #include <stdio.h>
   #include <dir.h>
}

void Nombre_archivo_temporal(char *archivo);
  // Genera un nombre de archivo unico


void Nombre_archivo_temporal(char *archivo)
{
   char nomtmp[MAXPATH];
   tmpnam(nomtmp);
   sprintf(archivo,"%s\\%s",PATH_ORIGINAL,nomtmp);
}



