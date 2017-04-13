/* x4filter.h   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

typedef int S4CALL X4FUNCTION( void S4PTR * ) ;

#ifdef S4DLL
   #define x4init( p1, p2, p3, p4 )    x4init_work( p1, p2, (X4FUNCTION S4PTR *) MakeProcInstance((FARPROC) p3, p2->code_base->hInst), p4 )
#else
   #define x4init( p1, p2, p3, p4 )    x4init_work( p1, p2, p3, p4 )
#endif

typedef struct
{
   D4DATA     S4PTR *data ;
   X4FUNCTION *routine ;
   void       S4PTR *routine_data ;
} X4FILTER ;

#ifdef __cplusplus
   extern "C" {
#endif

int  x4filter_go_eof( X4FILTER S4PTR * ) ;
int  x4find_good( X4FILTER S4PTR *, int) ;  /* Go to the first legitimate record, current? */
int  x4seek_do( X4FILTER S4PTR *, int ) ;

int  S4FUNCTION x4bottom( X4FILTER S4PTR * ) ;
void S4FUNCTION x4init_work( X4FILTER S4PTR *, D4DATA S4PTR *, X4FUNCTION *, void S4PTR * ) ;
int  S4FUNCTION x4filter_test( X4FILTER S4PTR * ) ;    /* TRUE - keep record. */
int  S4FUNCTION x4go( X4FILTER S4PTR *, long ) ;
int  S4FUNCTION x4seek( X4FILTER S4PTR *, char S4PTR * ) ;
int  S4FUNCTION x4seek_double( X4FILTER S4PTR *, double ) ;
int  S4FUNCTION x4skip( X4FILTER S4PTR *, long ) ;
int  S4FUNCTION x4top( X4FILTER S4PTR * ) ;

#ifdef __cplusplus
   }
#endif
