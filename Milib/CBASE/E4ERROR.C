/* e4error.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef S4WINDOWS
   #include <windows.h>
   #ifdef __TURBOC__
      #if __TURBOC__ == 0x297     /* if Borland C++ 2.0 */
         #ifdef __cplusplus
	    extern "C"{ void FAR PASCAL FatalAppExit(WORD,LPSTR) ; }
         #else
	    void FAR PASCAL FatalAppExit(WORD,LPSTR) ;
         #endif
      #endif
   #else
      #ifdef __ZTC__
         void FAR PASCAL FatalAppExit(unsigned short,LPSTR);
      #else
         void FAR PASCAL FatalAppExit(WORD,LPSTR);
      #endif
   #endif
#else
   #ifdef S4UNIX
      #include <tinfo.h>
   #else
      #include <conio.h>
   #endif
#endif

typedef struct error_data_st
{
   int   error_num ;
   char *error_data ;
}  ERROR_DATA ;


#ifdef S4LANGUAGE

#ifdef S4GERMAN

ERROR_DATA e4error_data[] =
{
   /* Allgemeine Fehler beim Diskzugriff  (General Disk Access Errors) */
   { e4create,         "Anlegen einer Datei" },
   { e4open,           "\216ffnen einer Datei" },
   { e4read,           "Lesen einer Datei" },
   { e4seek,           "Suchen einer Position in der Datei " },
   { e4write,          "Schreiben einer Datei" },
   { e4close,          "Schlie\341en einer Datei" },
   { e4remove,         "L\224schen einer Datei" },
   { e4lock,           "Locken einer Datei" },
   { e4unlock,         "Freigeben einer Datei" },
   { e4len,            "Festlegen der L\204nge einer Datei" },
   { e4len_set,        "Einstellen der L\204nge einer Datei" },
   { e4rename,         "Umnennen einer Datei" },

   /* Datenbank spezifische Fehler (Database Specific Errors) */
   { e4data,           "Datei is keiner DatenBank" },
   { e4record_len,     "Datensatzl\204nge zu gro\341" },
   { e4field_name,     "Unbekannter Feldname" },
   { e4field_type,     "Feldtyp" },

   /* Indexdatei spezifische Fehler  (Index File Specific Errors) */
   { e4index,          "Datei is keine Indexdatei" },
   { e4entry,          "Indexdatei is veraltet" },
   { e4unique,         "Schul\201sel ist schon einmal vorhanden" },
   { e4tag_name,       "Name des 'Tag'"},

   /* Fehler bei der Bewertung von Ausdr\201cken   (Expressions Evaluation Errors) */
   { e4comma_expected, "\",\" oder \")\" erwartet" },
   { e4complete,       "Ausdruck ist nich vollst\204ndig" },
   { e4data_name,      "Keine offene Datenbank" },
   { e4num_parms,      "Ung\201ltige Anzahl von Parametern im Ausdruck"},
   { e4overflow,       "\232berlauf bei der Auswertung eines Ausdrucks" },
   { e4right_missing,  "Rechte Klammer im Ausdruck fehlt" },
   { e4type_sub,       "Ein Parameter oder Operator hat einen ung\201ltigen Typ" },
   { e4unrec_function, "Unbekannte Funktion im Ausdruck" },
   { e4unrec_operator, "Unbekannter Operator im Ausdruck" },
   { e4unrec_value,    "Unbekannter Wert im Ausdruck"} ,
   { e4unterminated,   "Nicht abgeschlossene Zeichenkette im Ausdruck"} ,

   /* Kritische Fehler  (Critical Errors) */
   { e4memory,         "Kein Speicher mehr verf\201gbar"} ,
   { e4info,           "Unerwartete Information" },
   { e4parm,           "Unerwarteter Parameter"},
   { e4result,         "Unerwartetes Ergebnis"},
   { 0, 0 },
} ;

#endif   /*  ifdef S4GERMAN  */

#ifdef S4FRENCH

ERROR_DATA e4error_data[] =
{
   /* General Disk Access Errors */
   { e4create,         "Creating File" },
   { e4open,           "Opening File" },
   { e4read,           "Reading File" },
   { e4seek,           "Seeking to File Position" },
   { e4write,          "Writing to File" },
   { e4close,          "Closing File" },
   { e4remove,         "Removing File" },
   { e4lock,           "Locking File" },
   { e4unlock,         "Unlocking File" },
   { e4len,            "Determining File Length" },
   { e4len_set,        "Setting File Length" },
   { e4rename,         "Renaming File" },

   /* Database Specific Errors */
   { e4data,           "File is not a Data File" },
   { e4record_len,     "Record Length is too Large" },
   { e4field_name,     "Unrecognized Field Name" },
   { e4field_type,     "Unrecognized Field Type" },

   /* Index File Specific Errors */
   { e4index,          "Not a Correct Index File" },
   { e4entry,          "Tag Entry Missing" },
   { e4unique,         "Unique Key Error" },
   { e4tag_name,       "Tag Name not Found" },

   /* Expression Evaluation Errors */
   { e4comma_expected, "Comma or Bracket Expected" },
   { e4complete,       "Expression not Complete" },
   { e4data_name,      "Data File Name not Located" },},
   { e4num_parms,      "Number of Parameters is Wrong" },
   { e4overflow,       "Overflow while Evaluating Expression" },
   { e4right_missing,  "Right Bracket Missing" },
   { e4type_sub,       "Sub-expression Type is Wrong" },
   { e4unrec_function, "Unrecognized Function" },
   { e4unrec_operator, "Unrecognized Operator" },
   { e4unrec_value,    "Unrecognized Value"} ,
   { e4unterminated,   "Unterminated String"} ,

   /* Critical Errors */
   { e4memory,         "Out of Memory"} ,
   { e4info,           "Unexpected Information"} ,
   { e4parm,           "Unexpected Parameter"} ,
   { e4result,         "Unexpected Result"} ,
   { 0, 0 },
} ;

#endif   /*  ifdef S4FRENCH  */

#else   /* ifndef S4LANGUAGE  */

ERROR_DATA e4error_data[] =
{
   /* General Disk Access Errors */
   { e4create,       ""}, // "Creating File" },
   { e4open,         ""}, // "Opening File" },
   { e4read,         ""}, //   "Reading File" },
   { e4seek,         ""}, //   "Seeking to File Position" },
   { e4write,        ""}, //   "Writing to File" },
   { e4close,        ""}, //   "Closing File" },
   { e4remove,       ""}, //   "Removing File" },
   { e4lock,         ""}, //   "Locking File" },
   { e4unlock,       ""}, //   "Unlocking File" },
   { e4len,          ""}, //   "Determining File Length" },
   { e4len_set,      ""}, //   "Setting File Length" },
   { e4rename,       ""}, //   "Renaming File" },

   /* Database Specific Errors */
   { e4data,         ""}, //   "File is not a Data File" },
   { e4record_len,   ""}, //   "Record Length is too Large" },
   { e4field_name,   ""}, //   "Unrecognized Field Name" },
   { e4field_type,   ""}, //   "Unrecognized Field Type" },

   /* Index File Specific Errors */
   { e4index,        ""}, //   "Not a Correct Index File" },
   { e4entry,        ""}, //   "Tag Entry Missing" },
   { e4unique,       ""}, //   "Unique Key Error" },
   { e4tag_name,     ""}, //   "Tag Name not Found" },

   /* Expression Evaluation Errors */
   { e4comma_expected, ""}, // "Comma or Bracket Expected" },
   { e4complete,       ""}, // "Expression not Complete" },
   { e4data_name,      ""}, // "Data File Name not Located" },
   { e4num_parms,      ""}, // "Number of Parameters is Wrong" },
   { e4overflow,       ""}, // "Overflow while Evaluating Expression" },
   { e4right_missing,  ""}, // "Right Bracket Missing" },
   { e4type_sub,       ""}, // "Sub-expression Type is Wrong" },
   { e4unrec_function, ""}, // "Unrecognized Function" },
   { e4unrec_operator, ""}, // "Unrecognized Operator" },
   { e4unrec_value,    ""}, // "Unrecognized Value"} ,
   { e4unterminated,   ""}, // "Unterminated String"} ,

   /* Critical Errors */
   { e4memory,         ""}, // "Out of Memory"} ,
   { e4info,           ""}, // "Unexpected Information"} ,
   { e4parm,           ""}, // "Unexpected Parameter"} ,
   { e4result,         ""}, // "Unexpected Result"} ,
   { 0, 0 },
} ;

#endif   /*  ifdef  S4LANGUAGE  */

int S4FUNCTION e4error_set( C4CODE S4PTR *c4, int new_err_no )
{
   int old_err_no ;

   old_err_no =  c4->error_code ;
   c4->error_code =  new_err_no ;
   return old_err_no ;
}

void S4FUNCTION e4exit_test( C4CODE S4PTR *c4 )
{
   if ( c4->error_code < 0 )  e4exit(c4) ;
}

#ifdef S4WINDOWS
   void  S4FUNCTION e4exit( C4CODE S4PTR *c4 )
   {
      FatalAppExit( 0, "Exiting Application." ) ;
   }

   int S4ERROR e4error( C4CODE S4PTR *c4, int err_no, char *msg, ... )
   {
      char error_str[257], *ptr ;
      int pos, i ;
      WORD wType ;
      va_list  arg_marker  ;

      va_start( arg_marker, msg ) ;

      #ifdef S4DEBUG
         if ( c4->hWnd == 0 )
            FatalAppExit( 0, "C4CODE.hWnd was not assigned a window handle" ) ;
      #endif

      c4->error_code =  err_no ;

      strcpy( error_str, "Error #: " ) ;
      c4ltoa45( err_no, (char far *)error_str+9, 4 ) ;
      pos =  13 ;

      error_str[pos++] = '\n' ;

      for ( i=0; i <  (int) e4error_data[i].error_data != 0; i++ )
         if ( e4error_data[i].error_num == err_no )
	 {
            strcpy( error_str+pos, e4error_data[i].error_data ) ;
            pos +=  strlen( e4error_data[i].error_data ) ;
            error_str[pos++] = '\n' ;
            break ;
         }

      ptr =  msg ;
      for (; ptr != (char *) 0; ptr =  va_arg(arg_marker, char *))
      {
	 if ( strlen(ptr)+pos+3 >=  sizeof(error_str) )  break ;
         strcpy( error_str+pos, ptr ) ;
         pos +=  strlen(ptr) ;
         error_str[pos++] = '\n' ;
      }

      error_str[pos] =  0 ;

      wType =  MB_OK | MB_ICONSTOP ;

      if ( err_no == e4memory )
         wType |=  MB_SYSTEMMODAL ;

      if ( MessageBox( c4->hWnd, error_str, NULL, wType ) == 0 )
         e4severe( e4memory, (char *) 0 ) ;

      return err_no ;
   }

   void  S4ERROR e4severe( int err_no, char *msg, ... )
   {
      char error_str[257];
      char *ptr;
      int pos, i ;
      WORD wType ;

      va_list  arg_marker  ;
      va_start( arg_marker, msg ) ;

      strcpy( error_str, "Error #: " ) ;
      c4ltoa45( err_no, (char far *)error_str+9, 4 ) ;
      pos =  13 ;

      error_str[pos++] = ';' ;
      error_str[pos++] = ' ' ;

      for ( i=0; i < (int) e4error_data[i].error_data != 0; i++ )
	 if ( e4error_data[i].error_num == err_no )
	 {
	    strcpy( error_str+pos, e4error_data[i].error_data ) ;
	    pos +=  strlen( e4error_data[i].error_data ) ;
	    error_str[pos++] = ';' ;
	    error_str[pos++] = ' ' ;
	    break ;
	 }

      ptr =  msg ;
      for (; ptr != (char *) 0; ptr =  va_arg(arg_marker, char *))
      {
	 if ( strlen(ptr)+pos+4 >=  sizeof(error_str) )  break ;
	 strcpy( error_str+pos, ptr ) ;
	 pos +=  strlen(ptr) ;
	 error_str[pos++] = ';' ;
	 error_str[pos++] = ' ' ;
      }

      error_str[pos] =  0 ;

      FatalAppExit( 0, error_str ) ;
   }

#else   /*  ifndef S4WINDOWS  */

   void S4FUNCTION e4exit( C4CODE S4PTR *c4 )
   {
      if ( c4 == 0 )
         exit(0) ;
      else
         exit( c4->error_code ) ;
   }

   static void  e4error_out( char *ptr )
   {
      write( 1, ptr, (unsigned int) strlen(ptr) ) ;
   }

   static void display( int err_no )
   {
      char buf[11] ;
      int i ;

      c4ltoa45( (long) err_no, buf, 6 ) ;
      buf[6] =  0 ;
      e4error_out( buf ) ;

      for ( i = 0; e4error_data[i].error_data != 0; i++ )
         if ( e4error_data[i].error_num == err_no )
	 {
	    e4error_out( "\r\n" ) ;
	    e4error_out( e4error_data[i].error_data ) ;
            break ;
         }
   }

#ifdef S4VARARGS
int S4ERROR e4error(c4, err_no, va_alist )
C4CODE S4PTR *c4 ;
int err_no ;
va_dcl 
#else
int S4ERROR e4error( C4CODE S4PTR *c4, int err_no, char *msg, ... )
#endif
{
   char *ptr ;
   va_list  arg_marker  ;

   #ifdef  S4VARARGS
      va_start( arg_marker ) ;
      ptr =  va_arg(arg_marker, char *) ;
   #else
      va_start( arg_marker, msg ) ;
      ptr =  msg ;
   #endif

   c4->error_code =  err_no ;

   e4error_out( "\r\n\r\nError Number" ) ;
   display ( err_no ) ;

   for (; ptr != (char S4PTR *) 0; ptr =  va_arg(arg_marker, char *))
   {
      e4error_out( "\r\n" ) ;
      e4error_out( ptr ) ;
   }

   e4error_out( "\r\nPress a key ..." ) ;
   getch() ;

   return( err_no ) ;
}

#ifdef S4VARARGS                  
void S4ERROR e4severe(err_no, va_alist )
int err_no ;
va_dcl 
#else
void S4ERROR e4severe( int err_no, char *msg, ... )
#endif
{
   char S4PTR *ptr ;
   va_list  arg_marker  ;

   #ifdef  S4VARARGS
      va_start( arg_marker ) ;
      ptr =  va_arg(arg_marker, char *) ;
   #else
      va_start( arg_marker, msg ) ;
      ptr =  msg ;
   #endif

   e4error_out( "\r\n\r\nSevere Error Number" ) ;
   display( err_no ) ;

   for (; ptr != (char S4PTR *) 0; ptr =  va_arg(arg_marker, char *))
   {
      e4error_out( "\r\n" ) ;
      e4error_out( ptr ) ;
   }

   e4error_out( "\r\nPress a key ..." ) ;
   getch() ;

   exit(1) ;
}
#endif
