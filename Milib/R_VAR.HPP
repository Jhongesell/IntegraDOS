// Autor:     Antonio Carrillo Ledesma
// Direcci¢n: Amsterdam 312, colonia Hipodromo Condesa
// Telefono:  574-43-53
// Propiedad intelectual, derechos reservados conforme a la ley (Registro en tramite)


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                            Rutinas de uso general.                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#ifndef __R_VAR_HPP__
#define __R_VAR_HPP__


void sirena(void);
  // Genera el sonido de una sirena
  // Nombre del archivo: SIRENA.CPP


void laser(void);
  // Genera el sonido de un laser
  // Nombre del archivo: LASER.CPP


void musica(void);
  // Rutina que genera musica hasta que se pulse alguna tecla
  // Nombre del archivo: MUSICA.CPP


long esp_disco(const int unidad, const int t_p);
  // Retorna si T_P es (1) Espacio libre   en la unidad, si UNIDAD es (0) actual
  //                   (2) Espacio total                              (1) A
  //                   (3) Espacio usado                              (2) B
  //                                                                  (3) C
  //                                                                  (4) D
  // Regresara -1 si hay algun error
  // Nombre del archivo: R_E_D.CPP


unsigned long esp_memoria(const int t_p);
  // Retorna la memoria libre, si T_P es (0) Memoria del monton
  //                                     (1) Memoria del monton lejano
  // Nombre del archivo: R_E_M.CPP


void inv_puerto_lpt(void);
  // Rutina que invierte los puestros de impresion LPT1 y LPT2
  // Nombre del archivo: INV_LPT.CPP


long double Evalua_expresion(const char *exp, char *res);
  // Dada una cadena con expresiones aritmeticas las evalua entregando
  // una cadena con el resultado de esta
  // Nombre: VAL_EXP.CPP

//double Calculadora_not_polaca(const char *cadena);
  // Rutina que calcula el valor numerico de una expresion
  // en notacion polaca con separador de operandos el pipe
  // Nombre: VAL_EXP.CPP


int Valida_nombre_archivo(const char *nom_arch);
  // Valida que el nombre de archivo sea valido en caso contrario indica el
  // tipo de error en el nombre
  // Nombre del archivo: VAL_ARCH.CPP


int Valida_graba_sobreescribe(const char *arch);
  // Revisa si existe el archivo preguntando si se sobreescribe este
  // Retorna 1 si se puede grabar y 0 en caso contrario
  // Nombre del archivo: V_GRASOB.CPP

void mod_fecha_hora_sys(const char *n_fecha, const char *n_hora);
  // Modifica la fecha y hora del sistema, segun los parametros pasados a la rutina
  // Nombre del archivo: SET_FEHO.CPP

void remueve_archivos(const char *arc_pth, const int tp = 0);
  // Remueve los archivos especificados en el path, en caso de tener permisos de solo
  // lectura y/o oculto son cambiados a archivo normal. Despues de borra el archivo
  // Si tp es (1) reescribe este para evitar su recuperaci¢n.
  // Nombre del archivo: DELETE.CPP


//void Copia_archivo(const char *archivo_fuente, const char *archivo_objeto);
//  // Copia el archivo archivo_fuente en el archivo archivo_objeto
//  // Nombre del archivo: COPYFILE.CPP


int n_archivos_directorios(const char *path, const unsigned int atrib_bus);
  // Retorna el numero de archivos y/o directorios encontrados en la trayectoria
  // PATH segun las indicaciones de busqueda especificada en ATRIB_BUS
  // Atributos (_A_ARCH)   Archivo
  //           (_A_SUBDIR) Subdirectorios
  // Nombre del archivo: N_AR_DI.CPP


void nombre_archivo_directorio(const char *path, const unsigned int atrib_bus, const unsigned int t_p, char * arch);
  // Retorna el nombre del archivo y/o directorio que se encuentren en el PATH,
  // segun los atributos de busqueda indicados en ATRIB_BUS, si T_P es distinto de cero
  // inicializa la busqueda en caso contrario la ontinua apartir del ultimo archivo
  // encontrado
  // Atributos (_A_ARCH)   Archivo
  //           (_A_SUBDIR) Subdirectorios
  // Nombre del archivo: NB_AR_DI.CPP


int numero_drives_logicos(const unsigned int n_m_d = 10);
  // Retorna el numero de drives logicos habilitados en la computadora, la variable N_M_D
  // indica el numero maximo de drivers a leer
  // Nombre del archivo: N_DRV_LG.CPP


void nombre_drive_logico(const unsigned int t_p, const unsigned int n_m_d, char *dirve);
  // Regresa el nombre del drive logico instalado en el sistema con la mascara [- -]
  // si T_P es distinto de cero inicializa la busqueda, en caso contrario continua esta
  // apartir del ultimo drive encontrado, si son todos retorna un puntero nulo
  // la variable N_M_D indica el numero maximo de drivers a leer
  // Nombre del archivo: NB_DRV_L.CPP


int cambia_directorio(const char *path);
  // Cambia la actual trayectoria por la especificada en PATH
  // Nombre del archivo: CAM_DIR.CPP


int cambia_unidad(const char *path);
  // Cambia la actual unida por la especificada en el PATH (solo toma el primer caracter como unida)
  // Nombre del archivo: CAM_UNI.CPP

//void cambia_ext_path(char *path, const char *ext);
//  // Dado un path PATH cambia la extencion a la indicada en EXT
//  // Nombre del archivo: CAM_EXT.CPP

//void cambia_ext_path(const char *path, const char *ext, char *pathmod);
//  // Dado un path PATH cambia la extencion a la indicada en EXT dejandolo en PATHMOD
//  // Nombre del archivo: CAM_EXT.CPP

void encripta_cadena(const char *cad, char *cad_encp);
  // Encripta la cadena CAD retornando una cadena encriptada
  // Nombre del archivo: ENCRYTSP.CPP

void graba_pantalla_texto(const int x1, const int y1, const int x2, const int y2, const char *archivo);
  // Graba el contenido de un recuadro de la pantalla en el archivo ARCHIVO
  // Nombre del archivo: GRABAPT.CPP

void lee_pantalla_texto(const int x1, const int y1, const int x2, const int y2, const char *archivo);
  // Lee una pantalla previamente grabada y la pone en las coordenadas indicadas
  // Nombre del archivo: LEEPT.CPP


void graba_pantalla_grafica(const int x1, const int y1, const int x2, const int y2, const char *archivo);
  // Graba el contenido de un recuadro de la pantalla en el archivo ARCHIVO
  // Nombre del archivo: GRABAPG.CPP


void lee_pantalla_grafica(const int x1, const int y1, const char *archivo);
  // Lee en contenido de un recuadro apartir de un archivo insertandolo en la pantalla en la posicion X1,Y1
  // Nombre del archivo: LEEPG.CPP


void Graba_imagen_sin_formato(const char *arch, const int x1, const int y1, const int x2, const int y2);
  // Graba imagen grafica sin formato alguno, tomando pixel por pixel
  // Nombre del archivo: GRA_IMG.CPP

void Lee_imagen_sin_formato(const char *arch, const int x1, const int y1, const int x2, const int y2);
  // Lee imagen grafica sin formato alguno, leyendo pixel por pixel
  // Nombre del archivo: LEE_IMG.CPP


void Nombre_archivo_temporal(char *archivo);
  // Genera un nombre de archivo unico
  // Nombre del archivo: ARCHTMP.CPP


int Redireccina(const int opc, const char *arch);
  // Redirecciona la entrada, salida, error, auxiliar e impresion estandar al archivo ARCH retornando -1 si hay algun error
  // (1) Entrada   standar al archivo
  // (2) Salida    standar al archivo
  // (3) Error     standar al archivo
  // (4) Auxiliar  standar al archivo
  // (5) Impresora standar al archivo
  // (6) Archivo a la entrada standar
  // Nombre del archivo: REDIR.CPP


void Reinicializa_sistema(const int tp);
  // Reinicializa el sistema
  // (1) Reinicializacion en frio
  // (0) Reinicializacion en caliente
  // Nombre del archivo: RESTAR.CPP


int Opcion(const char *txt1, const char *txt2="", const char *txt3="", const char *txt4="", const char *txt5="");  // Presenta una ventana hasta con 5 lineas de texto aceptando la opcion aceptar y Cancelar, retornando
  // (1)   Aceptar
  // (2)   Cancelar
  // (3)   Cerrado de la ventana



#endif



