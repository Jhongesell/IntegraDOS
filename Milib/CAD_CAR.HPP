// Autor:     Antonio Carrillo Ledesma.
// R.F.C.:    CALA-691229-TV5
// Direcci¢n: Amsterdam 312 col. Hipodromo Condesa
// TelÇfono:  5-74-43-53

// Propiedad intelectual, todos los derechos reservados conforme a la ley, registro en tr†mite
// Revisi¢n  3.1-C


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//           Rutinas de manupulaci¢n de cadenas y/o caracteres              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#ifndef __CAD_CAR_HPP__
#define __CAD_CAR_HPP__


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//            Rutinas que modifican s¢lo una porci¢n de la cadena           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

unsigned int s_trim(const char *cad, char *cadobj);
  // Quita los espacios que existan al inicio y fin de la cadena CAD, dejandola en la cadena CADOBJ
  // retornando el tama§o de la cadena
  // Nombre del archivo: S_TRIM.CPP

unsigned int  s_trim(char *cad);
  // Quita los espacios que existan al inicio y fin de la cadena CAD, retorna la longitud de la cadena
  // Nombre del archivo: S_TRIMP.CPP


unsigned int s_ltrim(const char *cad, char *cadobj);
  // Quita los espacios que existan al inicio de la cadena CAD, dejandola en CADOBJ
  // y retornando la longitud de ella
  // Nombre del archivo: S_LTRIM.CPP

unsigned int s_rtrim(const char *cad, char *cadobj);
  // Quita los espacios que existan al final de la cadena CAD dejandola en CADOBJ
  // y retornando la longitud de ella
  // Nombre del archivo: S_RTRIM.CPP

unsigned int s_substr(const char *cad, const unsigned int p_i, const unsigned int n_c, char *cadobj);
  // Extrae de la cadena CAD el n£mero de caracteres indicados en N_C a partir de la posici¢n P_I partiendo
  // de cero, dejando esta en la CADOBJ, retornando el tama§o de la cadena
  // Nombre del archivo: S_SUBSTR.CPP

unsigned int s_lsubstr(const char *cad, const unsigned int n_c, char *cadobj);
  // Extrae de la cadena los primeros N_C caracteres, dejandola en CADOBJ
  // Nombre del archivo: S_LSUBST.CPP

unsigned int s_rsubstr(const char *cad, const unsigned int n_c, char *cadobj);
  // Extrae de la cadena los £ltimos N_C de la cadena, dejandola el CADOBJ
  // Nombre del archivo: S_RSUBST.CPP

//unsigned int Longitud_cadena_sin_esp_fin(const char *cad);
  // Retorna la longitud de la cadena sin los espacios al final de esta
  // Nombre del archivo: S_LGCDSP.CPP


////////////////
////////////////
////////////////


void s_nocar(char *cad, const char car, const unsigned int t_p = 1);
  // Quita de la cadena CAD, la(s) ocurrencia(s) del caracter CAR
  // Si T_P es (1) Extrae todas las ocurrencias del caracter CAR
  //           (2) Extrae s¢lo la primera ocurrencia del caracter
  //           (3) extrae s¢lo la £ltima ocurrencia del caracter
  // Nombre del archivo: S_NOCAR.CPP


void s_nocar(const char *cad, const char car, char *xcad, const unsigned int t_p = 1);
  // Quita de la cadena CAD, la(s) ocurrencia(s) del caracter CAR
  // Si T_P es (1) Extrae todas las ocurrencias del caracter CAR
  //           (2) Extrae s¢lo la primera ocurrencia del caracter CAR
  //           (3) extrae s¢lo la £ltima ocurrencia del caracter CAR
  // Nombre del archivo: S_NOCARP.CPP


//void s_borra_caracter(char *cad, const unsigned int pos);
  // Borra de la cadena CAD el caracter especificado por la posici¢n POS,
  // recorriendo el texto subsiguiente sin alterar su longitud
  // Nombre del archivo: S_BORCAR.CPP

//void s_borra_caracter(const char *cad, const int pos, char *cadobj);
  // Borra de la cadena CAD el caracter especificado por la posici¢n POS, recorriendo
  // el texto subsiguiente sin alterar su longitud, retornando esta en XCAD
  // Nombre del archivo: S_BORCAP.CPP

//void s_borra_cadena(char *cad);
  // Borra el contenido de la cadena, respetando la longitud de esta
  // Nombre del archivo:S_BORRA.CPP

//void s_borra_cadena_pos_fin(char *cad, const unsigned int pos);
  // Borra desde la actual posici¢n hasta el final de la cadena, respetando la longitud de esta
  // Nombre del archivo:S_BORRAF.CPP

//void s_borra_cadena_ini_pos(char *cad, const unsigned int pos);
  // Borra desde el inicio hasta la actual posici¢n la cadena, respetando la longitud de esta
  // Nombre del archivo:S_BORRAI.CPP

// No se ha revisado
void s_borra_palabra_sig(char *cad, const unsigned int pos);
  // Borra la palabra siguiente que se encuentre despues de la posicion POS
  // Nombre del archivo:S_BORRPL.CPP

void s_nocad(const char *cad, const char *x_cad, char *cadobj);
  // Elimina de la cadena CAD los caracteres de la cadena X_CAD, retornandola en la cadena CADOBJ
  // Nombre del archivo: S_NOCAD.CPP

void s_remplaza(char *cad, const char car, const char x_car);
  // Remplaza de la cadena CAD el caracter CAR por X_CAR
  // Nombre del archivo: S_REMPLZ.CPP

void s_remplaza(const char *cad, const char car, const char x_car, char *xcad);
  // Remplaza de la cadena CAD el caracter CAR por X_CAR, dejandola en XCAD
  // Nombre del archivo: S_REMPZP.CPP


void s_ins_car(char *cad, const unsigned int pos, const char car);
  // Inserta en la cadena CAD en la posici¢n POS el caracter CAR sin alterar longitud
  // Nota: La posici¢n inicia en 0
  // Nombre del archivo: S_INSCAR.CPP


//void Busqueda_remplaza_cadenas(char *cad, const char *bus, const char *rem);
//  // Remplaza de la cadena CAD la cedena buscada BUS por la cedena REM (todas las ocurrencias)
//  // Nombre del archivo: S_BUSREM.CPP

//int Busqueda_remplaza_expresiones(char *exp, const char *bus, const char *rem);
//  // Remplaza de la cadena CAD la cedena buscada BUS por la cedena REM (todas las ocurrencias)
//  // Retorna 0 si no se realizo remplazo alguno, en caso contrario retorna 1
//  // Nombre del archivo: S_B_R_E.CPP

void centra_cadena(const char *cad, char *x_cad);
  // Centra la cadena CAD a la longitud de esta, dejandola en x_cad
  // Nombre del archivo: CENCAD.CPP

// No se ha revisado
//void s_acom_esp_cad(const char *cad, char *xcad, const unsigned int lg);
  // Acomoda la cadena CAD en la cadena XCAD en rangos de LG longitud, si una palabra cae en
  // dicho rango es recorrida al sigiente rellenando esa porcion con espacios
  // Nombre del archivo: S_A_E_C.CPP




//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//             Rutinas que extraen una porci¢n de la cadena                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////



void s_solo_car_alf(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres alfabeticos de la cadena CAD
  // si T_P es (1) extrae todos los caracteres de la cadena
  //           (2) extrae s¢lo los primeros caracteres de la cadena
  //           (3) extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean alfabÇticos
  //           (5) Extrae s¢lo los primeros caracteres que no sean alfabÇticos
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean alfabÇticos
  // Acepta los siguientes caracteres como alfabÇticos [ABCDEFGHIJKLMNOPQRSTUVWXYZZ
  // abcdefghijklmnopqrstuvwxyz†ÖÇä°¢¢£óÅ§• -_]ç
  // Nombre del archivo: S_SOLALF.CPP

void s_solo_car_alfnum(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres alfanumericos de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean alfanumericos
  //           (5) Extrae s¢lo los primeros caracteres que no sean alfanumericos
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean alfanumericos
  // Acepta los siguientes caracteres como alfanumericos [ABCDEFGHIJKLMNOPQRSTUVWXYZZ
  // abcdefghijklmnopqrstuvwxyz†ÖÇä°¢¢£óÅ§• -_0123456789.+]ç
  // Nombre del archivo: S_SOLAFN.CPP

void s_solo_car_dig(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres digitos de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean digitos
  //           (5) Extrae s¢lo los primeros caracteres que no sean digitos
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean digitos
  // Acepta loa siguientes caracteres como digitos [0123456789+-.]
  // Nombre del archivo: S_SOLDIG.CPP

void s_solo_car_num(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres numericos de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean numericos
  //           (5) Extrae s¢lo los primeros caracteres que no sean numericos
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean numericos
  // Acepta los siguientes caracteres como numericos [0123456789]
  // Nombre del archivo: S_SOLNUM.CPP

void s_solo_car_punt(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres de puntuaci¢n de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean de puntuaci¢n
  //           (5) Extrae s¢lo los primeros caracteres que no sean de puntuaci¢n
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean de puntuaci¢n
  // Acepta los siguientes caracteres como de puntuaci¢n ≠"#$%&`()*+'-./:;<>?@=\[]^_{}|~®
  // Nombre del archivo: S_SOLPTC.CPP

void s_solo_car_extleng(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres de la extenci¢n del lenguaje de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean extenci¢n del lenguaje
  //           (5) Extrae s¢lo los primeros caracteres que no sean extenci¢n del lenguaje
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean extenci¢n del lenguaje
  // Acepta los siguientes caracteres como de extenci¢n del lenguaje 0x80 al 0xA7
  // Nombre del archivo: S_SOLEXL.CPP

void s_solo_car_ctrl(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres de control (exepto 0x00) de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean de control
  //           (5) Extrae s¢lo los primeros caracteres que no sean de control
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean de control
  // Acepta los siguientes caracteres como de control 0x01 al 0x1F
  // Nombre del archivo: S_SOLCTR.CPP

void s_solo_car_graf(const char *cad, const unsigned int t_p, char *xcad);
  // Rutina que extrae los caracteres gr†ficos de la cadena CAD
  // si T_P es (1) Extrae todos los caracteres de la cadena
  //           (2) Extrae s¢lo los primeros caracteres de la cadena
  //           (3) Extrae s¢lo los £ltimos caracteres de la cadena
  //           (4) Extrae todos aquellos caracteres que no sean gr†ficos
  //           (5) Extrae s¢lo los primeros caracteres que no sean gr†ficos
  //           (6) Extrae s¢lo los £ltimos caracteres que no sean gr†ficos
  // Acepta los siguientes caracteres como gr†ficos 0xA9 al 0xFF
  // Nombre del archivo: S_SOLGRF.CPP




/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//         Rutinas que retornan una cadena inicializada o rellena          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////




void s_rellena(char *cad, const char car, const unsigned int lg);
  // Rellena la cadena CAD con el caracter CAR hasta una longitud de L_G
  // Si la cadena es mas grande no la modifica
  // Nombre del archivo: S_RELLEP.CPP

void s_rellena(const char *cad, const char car, const unsigned int lg, char *xcad);
  // Rellena la cadena CAD con el caracter CAR hasta una longitud de L_G,
  // Si la cadena es mas grande no la modifica, retornando un puntero a ella
  // Nombre del archivo: S_RELLEN.CPP

void s_ini_cad(char *cad, const char car, const unsigned int lg);
  // Inicializa la cadena CAD con el caracter CAR de L_G longitud
  // Nombre del archivo: S_INICAD.CPP

void s_ini_num(char *cad, const unsigned int lg);
  // Inicializa una cadena CAD con un cero a la derecha de longitud N_C
  // Nombre del archivo: S_ININUM.CPP

void s_repetir(char *x_cad, const char *cad, const unsigned int num);
  // Repite la cadena CAD el n£mero de veces indicada en NUM dejandola en x_cad
  // Nombre del archivo: S_REPET.CPP

/*
char *s_repetir(const char *cad, const unsigned int num);
  // Repite la cadena CAD el n£mero de veces indicado el NUM, retornando un puntero a ella
  // Nombre del archivo: S_REPETP.CPP
*/




/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//        Rutinas de busquedad, sorteo o comparaci¢n de cadenas            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


//int s_busca_caracter(const char *cad, const char car, unsigned int &pos);
  // Indica la posici¢n que fuÇ encontrado el caracter CAR en la cadena CAD en la variable POS
  // retorna 0 si no lo encuentra
  // Nombre del archivo: S_BUSCAR.CPP

//int s_busca_caracteres(const char *cad, const char *cars, unsigned int &pos);
  // Indica la posici¢n en que fue encontrado cualquier caracter de la cadena CARS
  // en la cadena CAD e indicando su posicion en la variable POS, si no fue encontrado 
  // retorna 0, si fue encontrado retorna 1
  // Nombre del archivo: S_BUSCRS.CPP

//int Busca_cadena(const char *cad, const char *x_cad, unsigned int &pos);
  // Busca en la cadena CAD los caracteres de la cadena X_CAD, retornando la
  // posici¢n donde inicia esta, retorna 0 si no lo encuentra
  // Nombre del archivo: S_BUSCAD.CPP

//int Busca_expresion(const char *cad, const char *exp, unsigned int &pos);
//  // Busca una expresion EXP, dentro de la cadena CAD (expresion es una variable o funcion)
//  // La expresion buscada estara delimitada por
//  //    Al inicio {}[]()=,.*-/+  o el inicio de la cadena
//  //    Al final  {}[]()=,.*-/+ o fin de la cadena
//  // retornando la posici¢n donde inicia esta, retorna 0 si no lo encuentra
//  // Nombre del archivo: S_BUSEXP.CPP


int s_bus_ult_car(const char *cad, const char car, unsigned int &pos);
  // Indica la posici¢n en que fuÇ encontrado el ultimo caracter CAR en la cadena CAD
  // retorna 0 si no lo encuentra
  // Nombre del archivo: S_BUSCRU.CPP

// No se ha revisado
//int s_bus_todos_car(const char *cad, const char car, const unsigned int t_p = 0);
  // Indica la posici¢n en que fuÇ encontrado el caracter CAR en la cadena CAD,
  // inicia en 0, retorna -1 si no lo encuentra
  // Si T_P es (0) Encuentra la primera ocurrencia
  //           (1) Encuentra la siguiente ocurrencia (Utiliza variable est†tica
  //               para guardar la posici¢n de la £ltima ocurrencia)
  // Nombre del archivo: S_BUSCRT.CPP

// No se ha revisado
//int s_bus_ult_cad(const char *cad, const char *x_cad, const unsigned int pos = 0);
  // Busca en la cadena CAD los caracteres de la cadena X_CAD, retornando la
  // posici¢n donde inicia la £ltima ocurrencia, retornando -1 si no la encuentra
  // Parte la busqueda de la posicion POS
  // Nombre del archivo: S_BUSCDU.CPP

// No se ha revisado
//int s_bus_todas_cad(const char *cad, const char *x_cad, const unsigned int t_p = 0);
  // Busca en la cadena CAD los caracteres de la cadena X_CAD, retornando la
  // posici¢n donde inicia esta, retornando -1 si no la encuentra
  // Si T_P es (0) Encuentra la primera ocurrencia
  //           (1) Encuentra la siguiente ocurrencia (Utiliza variable est†tica
  //               para guardar la posici¢n de la £ltima ocurrencia)
  // Nombre del archivo: S_BUSCDT.CPP

char *s_bus_palabra_sig(const char *cad, const unsigned int pos, const unsigned int t_p);
  // Retorna la palabra siguiente de la cadena CAD, a partir de la posicion POS hasta POSF
  // Si POSF es cero asume hasta el final de la cadena
  // Si T_P es (0) busca la palabra siguiente (alfanumerico y espacio)
  //           (1) busca la palabra siguiente (alfanumerico sin espacios)
  //           (2) Busca la palabra siguiente (numerico, punto y signo menos)
  //           (3) Busca la palabra siguiente (numerico)
  // Nombre del archivo: S_BUSPLS.CPP

char *s_bus_palabra_ant(const char *cad, const unsigned int pos, const unsigned int t_p);
  // Retorna la palabra anterior de la cadena CAD, a partir de la posicion POSI hasta POSF
  // Si POSF es cero asume hasta el final de la cadena
  // Si T_P es (0) busca la palabra siguiente (alfanumerico y espacio)
  //           (1) busca la palabra siguiente (alfanumerico sin espacios)
  //           (2) Busca la palabra siguiente (numerico, punto y signo menos)
  //           (3) Busca la palabra siguiente (numerico)
  // Nombre del archivo: S_BUSPLA.CPP

//int s_pos_palabra_sig(const char *cad, const int pos, const unsigned int t_p = 0);
  // Retorna la posicion donde inicia la palabra siguiente en la cadena CAD
  // a partir de la posicion POSI hasta la posicion POSF, retornando -1 si no la encuentra
  // Si T_P es (0) busca la palabra siguiente (alfanumerico sin espacio)
  //           (1) busca la palabra siguiente (alfanumerico con espacios)
  //           (2) Busca la palabra siguiente (numerico, punto y signo menos)
  //           (3) Busca la palabra siguiente (numerico)
  // Nombre del archivo: S_POSPLS.CPP

//int s_pos_palabra_ant(const char *cad, const int pos, const unsigned int t_p = 0);
  // Retorna la posicion donde inicia la palabra anterior en la cadena CAD
  // a partir de la posicion POSI hasta la posicion POSF, retornando -1 si no la encuentra
  // Si T_P es (0) busca la palabra siguiente (alfanumerico sin espacio)
  //           (1) busca la palabra siguiente (alfanumerico con espacios)
  //           (2) Busca la palabra siguiente (numerico, punto y signo menos)
  //           (3) Busca la palabra siguiente (numerico)
  // Nombre del archivo: S_POSPLA.CPP

////////////////////////////////
// revisar que se quiere hacer
int s_pos_sig_car_dist(const char *cad,const char car, const int pos);
  // Retorna la posicion del siguiente caracter distinto de CAR en la cadena CAD
  // la busqueda inicia el POS
  // retorna -1 si no es encontrado
  // Nombre del archivo: S_PSCARD.CPP

int s_pos_ant_car_dist(const char *cad,const char car, const int pos);
  // Retorna la posicion del anterior caracter distinto de CAR en la cadena CAD
  // la busqueda inicia el POS
  // retorna -1 si no es encontrado
  // Nombre del archivo: S_PACARD.CPP


int s_pos_sig_cad_dist(const char *cad,const char *xcad, const unsigned int posi = 0, const unsigned int posf = 0);
  // Retorna la posicion del siguiente  caracter  distinto al de la cadena XCAD en la cadena CAD
  // la busqueda inicia el POSI y termina en POSF. Si POSF es cero asume hasta el final de la cadena
  // Nombre del archivo: S_PSCADD.CPP

int s_pos_ant_cad_dist(const char *cad,const char *xcad, const unsigned int posi = 0, const unsigned int posf = 0);
  // Retorna la posicion del anterior caracter distinto al de la cadena XCAD en la cadena CAD
  // la busqueda inicia el POSI y termina en POSF. Si POSF es cero asume hasta el final de la cadena
  // Nombre del archivo: S_PACADD.CPP

//unsigned int s_n_veces_car(const char *cadena, const char car);
  // Cuenta el numero de veces que un caracter CAR aparece en una cadena CEDENA, empesando en la
  // Nombre del archivo: S_NVCAR.CPP

//unsigned int s_n_veces_cad(const char *cadena, const char *cbus);
  // Cuenta el numero de veces que una cadena CBUS aparece en la cadena CADENA, empesando en la
  // Nombre del archivo: S_NVCAD.CPP

char *sort_cad(const char *cad, const unsigned int ord);
  // Ordena una cadena de menor a mayor si ORD = 1, en caso contrario de mayor a menor,
  // retornando un puntero a ella
  // Nombre del archivo: S_SORT.CPP

//int  s_compara(const char *cad1, const char *cad2);
  // Compara la cadena CAD1 con la cadena CAD2 regresando verdadero si son iguales
  // Nombre del archivo: S_COMP.CPP



/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//         Rutinas de conversi¢n de cadenas y/o caracteres                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////




void s_conv_may(char *cad);
  // Los caracteres de la cadena CAD se convierten a may£sculas
  // Nombre del archivo: S_MAY.CPP
/*
char *s_conv_may_p(const char *cad, const unsigned int posi = 0, const unsigned int posf = 0);
  // De los caracteres de la cadena CAD retorna un puntero con estos,
  //  convertidos en may£sculas
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena
  // Nombre del archivo: S_MAY_P.CPP
*/
void s_conv_min(char *cad);
  // Los caracteres de la cadena CAD se convierten a min£sculas
  // Nombre del archivo: S_MIN.CPP
/*
char *s_conv_min_p(const char *cad, const unsigned int posi = 0, const unsigned int posf = 0);
  // De los caracteres de la cadena CAD retorna un puntero con estos,
  //  convertidos en min£sculas
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena
  // Nombre del archivo: S_MIN_P.CPP
*/
char s_conv_may_min(const char car, const unsigned int t_p);
  // Comvierte el caracter CAR segun la siguiente convicci¢n
  // Si T_P es (1) comvierte de mayuscula a miniscula y vice versa
  //           (2) convierte a minuscula
  //           (3) convierte a mayuscula
  // Nota: Trabaja tambien con caracteres invertidos (†Ç°¢£ÖäïóÅ§•)ç
  // Nombre del archivo: S_C_M_M.CPP

void s_intercambia_var(char *cad1, char *cad2);
  // Intercambia el valor de las cadenas CAD1 y CAD2
  // Nombre del archivo: S_INTVAR.CPP

// falta documentaci¢n
void s_conv_double_char(const double num, const unsigned int ent, const unsigned int dec, char *cad);
   // Convierte un double a cadena, con ENT entreos y DEC decimales redondeados



/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//     Rutinas que formatean o quitan el formato de una cadena             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////



void s_r_izq(char *cad, const unsigned int lg, const char car);
  // Regresa en la cadena CAD un a una cadena rellena con el caracter CAR por la
  // izquierda de longitud LG, si esta es mayor no le hace nada
  // Nombre del archivo: S_R_IZP.CPP

void s_r_izq(const char *cad, const unsigned int lg, const char car, char *cadobj);
  // Regresa de una cadena CAD en la CADOBJ un a una cadena rellena con el caracter CAR por la
  // izquierda de longitud LG, si esta es mayor no le hace nada
  // Nombre del archivo: S_R_IZ.CPP

void s_r_t(char *cad, const unsigned int lg, const char car);
  // Rellena o trunca la cadena CAD a una determinada longitud LG rellenando
  // con el caracter CAR por la derecha
  // Nombre del archivo: S_R_T_P.CPP

void s_r_t(const char *cad, const unsigned int lg, const char car, char *cadobj);
  // Reyena o trunca la cadena CAD a una determinada longitud LG reyenando
  // con el caracter CAR por la derecha
  // Nombre del archivo: S_R_T.CPP

void s_formato(const char *cad, const unsigned int n_e, const unsigned int n_d, const unsigned int t_p, char *cadobj);
  // Formatea una cadena numerica CAD con comas con N_E enteros y N_D decimales,
  // dejandola en la variable CADOBJ, la cadena resultante puede ser de mayor longitud
  // Si T_P es (0) no se combierten las comas en apostrofes en los miles
  //           (1) si se combierten las comas en apostrofes en los miles
  // Nombre del archivo: S_FORMAT.CPP
  // Nota: La cadena numerica esta cargada hacia la derecha, se suprimen cifras no significativas

void s_sin_formato(const char *cad, char *xcad);
  // Rutina que quita el formato de comas y postrofes de una cadena de digitos,
  // retornando esta en XCAD, respeta la longitud de la cadena CAD
  // Nombre del archivo: S_S_FORM.CPP
  // Nota: La cadena numerica esta cargada hacia la izquierda, no se suprimen cifas no significativas

void s_for_num(const char *cad, const unsigned int n_e, const unsigned int n_d, char *s_for);
  // Rutina que ajusta la cadena numerica a una determinada longitud N_E + N_D + 1
  // con n£mero de decimales N_D y n£mero de enteros N_E
  // Nombre del archivo: S_FORNUM.CPP
  // Nota: La cadena numerica esta cargada hacia la izquierda, se suprimen cifras no significativas


void s_conv_numero_a_letra(const char *cad, char *cad_ent, char *cad_dec);
  // Rutina que convierte una cadena numerica, en su equivalente escito, separado en cantidad entera
  // y cantidad decimal (falta optimizar)
  // Nombre del archivo: S_NUMLET.CPP


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//      Rutinas que verifican el tipo de informaci¢n de una cadena         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// Modificar el fuente quitar posi y posf
//int s_es_numero(const char *cad);
  // Rutina que retorna verdadero si la cadena s¢lo contiene una expresion numerica valida
  // (ya sea en formato exponencial o numerico ordinario) en caso contrario retorna (0)
  // Nombre del archivo: S_ESNUM.CPP

// Modificar el fuente quitar posi y posf
int s_es_digito(const char *cad);
  // Rutina que retorna verdadero si la cadena s¢lo contiene n£meros
  // en caso contrario retorna (0)
  // Nombre del archivo: S_ESDIG.CPP

//int s_es_alfabetica(const char *cad);
  // Rutina que retorna verdadero si la cadena s¢lo contiene caracteres alfabeticos, espacios, gui¢n o subrrayado
  // en caso contrario retorna (0)
  // Soporta caracteres como †ÖÇä°¢ï£óÅ§•ç
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena
  // Nombre del archivo: S_ESALF.CPP

int s_es_alfanumerica(const char *cad);
  // Rutina que retorna verdadero si no la cadena s¢lo contiene caracteres alfabeticos, numericos, espacios, gui¢n, mas o subrrayado
  // en caso contrario retorna (0)
  // Soporta caracteres como †ÖÇä°¢ï£óÅ§•ç
  // Nombre del archivo: S_ESALFN.CPP

int s_es_mayusculas(const char *cad);
  // Rutina que retorna verdadero si no contiene caracteres alfabeticos en min£sculas
  // en caso contrario retorna (0)
  // Soporta caracteres como †ÖÇä°¢ï£óÅ§•ç
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena
  // Nombre del archivo: S_ESMAY.CPP

int s_es_minusculas(const char *cad);
  // Rutina que retorna verdadero si no contiene caracteres alfabeticos en may£sculas
  // en caso contrario retorna (0)
  // Soporta caracteres como †ÖÇä°¢ï£óÅ§•ç
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena
  // Nombre del archivo: S_ESMIN.CPP

int s_es_espacios(const char *cad);
  // Rutina que retorna verdadero si solo contiene caracteres espacios en caso contrario retorna (0)
  // iniciando en la posicion POSI y terminando en la posicion POSF, si POSF es
  // cero es hasta el final de la cadena
  // Nombre del archivo: S_ESCESP.CPP

//int s_es_car_alf(const char car);
  // Retorna (1) si el carcter es alfabetico, en caso contrario retorna (0)
  // Acepta los siguientes caracteres como alfabeticos [ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz]
  // Nombre del archivo: S_ESCALF.CPP

int s_es_car_alf_ace(const char car);
  // Retorna (1) si el carcter es alfabetico, en caso contrario retorna (0)
  // Acepta los siguientes caracteres como alfabeticos [ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz†ÖÇäç°¢¢£óÅ§• -_]
  // Nombre del archivo: S_ESCALF.CPP

//int s_es_car_num(const char car);
  // Retorna (1) si el carcter es numerico, en caso contrario retorna (0)
  // Acepta los siguientes caracteres como numericos [0123456789.+-]
  // Nombre del archivo: S_ESCNUM.CPP

//int s_es_car_dig(const char car);
  // Retorna (1) si el carcter es digito, en caso contrario retorna (0)
  // Acepta los siguientes caracteres como digitos [0123456789]
  // Nombre del archivo: S_ESCDIG.CPP

//int s_es_car_alfnum(const char car);
  // Retorna (1) si el carcter es alfanumerico, en caso contrario retorna (0)
  // Acepta los siguientes caracteres como alfanumerico [ABCDEFGHIJKLMNOPQRSTUVWXYZ
  // abcdefghijklmnopqrstuvwxyz†ÖÇä°¢¢£óÅ§• -_0123456789+.]ç
  // Nombre del archivo: S_ESCAFN.CPP

int s_es_car_acent(const char car);
  // Si el caracter CAR es †ÖÇä°¢ï£óÅ§ regresa (1)  [exepto •]
  // en caso contrario regresa (0)
  // Nombre del archivo: S_ESCVOA.CPP

int s_es_car_ctrl(const char car);
  // Si el caracter CAR es del rango 0x01 a 0x1F raegresa (1) [Caracteres de control]
  // en caso contrario regresa (0)
  // Nombre del archivo: S_ESCCTR.CPP

int s_es_car_extleng(const char car);
  // Si el caracter CAR es del rango 0x80 a 0xA7 raegresa (1)  [Caracteres de extenci¢n del lenguaje]
  // en caso contrario regresa (0)
  // Nombre del archivo: S_ESCEXL.CPP

int s_es_car_graf(const char car);
  // Si el caracter CAR es del rango 0xA9 a 0xFF  raegresa (1) [Caracteres gr†ficos]
  // en caso contrario regresa (0)
  // Nombre del archivo: S_ESCGRF.CPP

int s_es_car_punt(const char car);
  // Si el caracter CAR es ≠"#$%&`()*+'-./:;<>?@=\[]^_{}|~®  raegresa (1) [Caracteres de puntuaci¢n]
  // en caso contrario regresa (0)
  // Nombre del archivo: S_ESCPTC.CPP

int s_es_car_may(const char car);
  // Rutina que retorna verdadero si el caracter es letra mayoscula o •
  // en caso contrario retorna (0)
  // Soporta caracteres como †ÖÇä°¢ï£óÅ§•ç
  // Nombre del archivo: S_ESCMAY.CPP

int s_es_car_min(const char car);
  // Rutina que retorna verdadero si el caracter es letra minuscula o †ÖÇä°¢ï£óÅ§ç
  // en caso contrario retorna (0)
  // Soporta caracteres como †ÖÇä°¢ï£óÅ§•ç
  // Nombre del archivo: S_ESCMIN.CPP










#endif

