/* e4functi.c  (c)Copyright Sequiter Software Inc., 1991-1992.	All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifndef S4NO_POW
   #include <math.h>
#else
   typedef void E4FUNC( E4PARMS * ) ;
#endif

E4FUNCTIONS  v4functions[] =
{
   /* I4FIELD_STR, 0 */
   {  0, 0,     "",  0,      0,  t4str, 0 },

   /* I4FIELD_LOG, 1 */
   {  1, 0,     "",  0,      0,  t4log, 0 },

   /* I4FIELD_DATE_D, 2 */
   {  2, 0,     "",  0,      0,  t4date_doub, 0 },

   /* I4FIELD_DATE_S, 3 */
   {  3, 0,     "",  0,      0,  t4date_str, 0 },

   /* I4FIELD_NUM_D, 4 */
   {  4, 0,     "",  0,      0,  t4num_doub, 0 },

   /* I4FIELD_NUM_S, 5 */
   {  5, 0,     "",  0,      0,  t4num_str, 0 },

   /* I4STRING, 6 */
   {  6, 0,     "",  0,      0,  t4str, 0 },

   /* I4DOUBLE, 7 */
   {  7, 0,     "",  0,      0,  t4num_doub, 0 },

   {  8, 6,     ".TRUE.", e4true_eval,  0,  t4log, 0 },
   {  8, 3,     ".T.",    e4true_eval,  0,  t4log, 0 },
   { 10, 7,     ".FALSE.",e4false, 0,  t4log, 0 },
   { 10, 3,     ".F.",    e4false, 0,  t4log, 0 },
   { 12, 5,     ".NOT.",  e4not,   5,  t4log, 1, t4log },

   #ifdef S4NO_POW
   { 17, 0,"", (E4FUNC *) 0,       9, t4num_doub, 2, t4num_doub, t4num_doub },
   { 17, 0,"", (E4FUNC *) 0,       9, t4num_doub, 2, t4num_doub, t4num_doub },
   #else
   { 17, 1,"^",       e4power,       9, t4num_doub, 2, t4num_doub, t4num_doub },
   { 17, 2,"**",      e4power,       9, t4num_doub, 2, t4num_doub, t4num_doub },
   #endif

   { 19, 1,"#",       e4not_equal,  6, t4log, 2, t4str, t4str },
   { 19, 2,"<>",      e4not_equal,  6, t4log, 2, t4str, t4str },
   { 19, 2,"<>",      e4not_equal,  6, t4log, 2, t4num_doub, t4num_doub },
   { 19, 2,"<>",      e4not_equal,  6, t4log, 2, t4date_doub, t4date_doub },
   { 19, 2,"<>",      e4not_equal,  6, t4log, 2, t4log, t4log },

   { 21, 2,">=",      e4greater_eq, 6, t4log, 2, t4str, t4str },
   { 21, 2,">=",      e4greater_eq, 6, t4log, 2, t4num_doub, t4num_doub },
   { 21, 2,">=",      e4greater_eq, 6, t4log, 2, t4date_doub, t4date_doub },

   { 23, 2,"<=",      e4less_eq,    6, t4log, 2, t4str, t4str },
   { 23, 2,"<=",      e4less_eq,    6, t4log, 2, t4num_doub, t4num_doub },
   { 23, 2,"<=",      e4less_eq,    6, t4log, 2, t4date_doub, t4date_doub },

   { 25, 1,"+",       e4add,         7, t4num_doub,  2, t4num_doub, t4num_doub },
   { 25, 1,"+",       e4add,         7, t4date_doub, 2, t4num_doub, t4date_doub },
   { 25, 1,"+",       e4add,         7, t4date_doub, 2, t4date_doub, t4num_doub },
   { 25, 1,"+",       e4concatenate, 7, t4str,       2, t4str, t4str },

   { 30, 1,"-",       e4sub,         7, t4num_doub,  2, t4num_doub,    t4num_doub },
   { 30, 1,"-",       e4sub,         7, t4num_doub,  2, t4date_doub, t4date_doub },
   { 30, 1,"-",       e4sub,         7, t4date_doub, 2, t4date_doub, t4num_doub },
   { 30, 1,"-",       e4concat_two,  7, t4str, 2, t4str, t4str },

   { 40, 1,"*",       e4multiply,    8, t4num_doub, 2, t4num_doub, t4num_doub },
   { 50, 1,"/",       e4divide,      8, t4num_doub, 2, t4num_doub, t4num_doub },

   { 70, 1,"$",       e4contain,    6, t4log, 2, t4str, t4str },

   { 80, 1,"=",       e4equal,      6, t4log, 2, t4str, t4str },
   { 80, 1,"=",       e4equal,      6, t4log, 2, t4num_doub, t4num_doub },
   { 80, 1,"=",       e4equal,      6, t4log, 2, t4date_doub, t4date_doub },
   { 80, 1,"=",       e4equal,      6, t4log, 2, t4log, t4log },

   {100, 1,">",       e4greater,    6, t4log, 2, t4str, t4str },
   {100, 1,">",       e4greater,    6, t4log, 2, t4num_doub, t4num_doub },
   {100, 1,">",       e4greater,    6, t4log, 2, t4date_doub, t4date_doub },

   {110, 1,"<",       e4less,       6, t4log, 2, t4str, t4str },
   {110, 1,"<",       e4less,       6, t4log, 2, t4num_doub, t4num_doub },
   {110, 1,"<",       e4less,       6, t4log, 2, t4date_doub, t4date_doub },

   {150, 4,".OR.",    e4or,         3, t4log, 2, t4log, t4log },
   {160, 5,".AND.",   e4and,        4, t4log, 2, t4log, t4log },

   {170, 4,"STOD",    e4stod,    0,  t4date_doub, 1, t4str },
   {175, 4,"CTOD",    e4ctod,    0,  t4date_doub, 1, t4str },
   {180, 4,"DTOS",    e4dtos,    0,  t4str, 1, t4date_str },
   {180, 4,"DTOS",    e4dtos,    0,  t4str, 1, t4date_doub },
   {185, 4,"DTOC",    e4dtoc,    0,  t4str, 1, t4date_str },
   {185, 4,"DTOC",    e4dtoc,    0,  t4str, 1, t4date_doub },
   {190, 4,"DATE",    e4date,    0,  t4date_doub, 0 },
   {194, 3,"DAY",     e4day,     0,  t4num_doub, 1, t4date_doub },
   {194, 3,"DAY",     e4day,     0,  t4num_doub, 1, t4date_str },
   {196, 5,"MONTH",   e4month,   0,  t4num_doub, 1, t4date_doub },
   {196, 5,"MONTH",   e4month,   0,  t4num_doub, 1, t4date_str },
   {198, 4,"YEAR",    e4year,    0,  t4num_doub, 1, t4date_doub },
   {198, 4,"YEAR",    e4year,    0,  t4num_doub, 1, t4date_str },

   {200, 7,"DELETED", e4deleted, 0,  t4log, 0 },
   {210, 3,"DEL",     e4del,     0,  t4str, 0 },
   {220, 3,"IIF",     e4iif,     0,  t4str, 3, t4log, t4str, t4str },
   {220, 3,"IIF",     e4iif,     0,  t4num_doub, 3, t4log, t4num_doub, t4num_doub },
   {220, 3,"IIF",     e4iif,     0,  t4log, 3, t4log, t4log, t4log },
   {220, 3,"IIF",     e4iif,     0,  t4date_doub, 3, t4log, t4date_doub, t4date_doub },
   {230, 8,"RECCOUNT",e4reccount,0,  t4num_doub,  0 },
   {240, 5,"RECNO",   e4recno,   0,  t4num_doub,  0 },
   {250, 3,"STR",     e4str,     0,  t4str, 3, t4num_doub, t4num_doub, t4num_doub },
   {260, 6,"SUBSTR",  e4substr,  0,  t4str, 3, t4str, t4num_doub, t4num_doub },
   {270, 4,"TIME",    e4time,    0,  t4str, 0 },
   {280, 5,"UPPER",   e4upper,   0,  t4str, 1, t4str },
   {290, 3,"VAL",     e4val,     0,  t4num_doub, 1, t4str },
   {-1},
} ;

void e4parms_init( E4PARMS *p4, D4DATA *data )
{
   memset( (void *)p4, 0, sizeof(E4PARMS) ) ;

   p4->data   =  data ;
   p4->code_base =  data->code_base ;
   p4->on_parm =  0 ;
}

int  e4parms_space_reserve( E4PARMS *p4, unsigned new_len )
{
   if ( p4->code_base->error_code < 0 )  return -1 ;

   p4->parms[p4->on_parm].len =  new_len ;

   if ( e4buf_len < new_len+ p4->parms[p4->on_parm].pos )
   {
      if ( u4alloc_again( p4->code_base, &e4buf, &e4buf_len,
	   new_len + p4->parms[p4->on_parm].pos ) < 0 )
         return -1 ;
   }
   return  0 ;
}

void  e4parms_set( E4PARMS *p4, void *ptr, unsigned len, int parm_type )
{
   if ( e4parms_space_reserve(p4, len) < 0 )  return ;

   memmove( e4buf+ p4->parms[p4->on_parm].pos, ptr, len ) ;
   p4->parms[p4->on_parm].type =  parm_type ;
}

void  e4parms_set_logical( E4PARMS *p4, int true_or_false )
{
   int *i_ptr ;
   e4parms_pos_align(p4) ;
   #ifdef S4FOX
      if ( e4parms_space_reserve(p4,sizeof(char)) < 0 )  return ;
   #else
      if ( e4parms_space_reserve(p4,sizeof(int)) < 0 )  return ;
   #endif

   i_ptr  =  (int *) (e4buf+ p4->parms[p4->on_parm].pos) ;
   *i_ptr =  true_or_false ;
   p4->parms[p4->on_parm].type =  t4log ;
}

void e4parms_set_double( E4PARMS *p4, double d )
{
   double *d_ptr ;

   e4parms_pos_align(p4) ;
   if ( e4parms_space_reserve(p4,sizeof(double)) < 0 )  return ;

   d_ptr  =  (double *) (e4buf+ p4->parms[p4->on_parm].pos) ;
   *d_ptr =  d ;
   p4->parms[p4->on_parm].type =  t4num_doub ;
}

double *e4parms_double_ptr( E4PARMS *p4, int i )
{
   return (double *) (e4buf+ p4->parms[p4->on_parm+i].pos) ;
}

#ifdef S4FOX
/* note that the fox version returns a char pointer, instead of int */ 
char *e4parms_int_ptr( E4PARMS *p4, int i )
{
   return (char *) (e4buf+ p4->parms[p4->on_parm+i].pos) ;
}
#else
int *e4parms_int_ptr( E4PARMS *p4, int i )
{
   return (int *) (e4buf+ p4->parms[p4->on_parm+i].pos) ;
}
#endif

void e4parms_fix_pos( E4PARMS *p4)
{
   if ( p4->parms[p4->on_parm+1].pos != ( p4->parms[p4->on_parm].pos + p4->parms[p4->on_parm].len ))
   {
      memcpy( e4buf+p4->parms[p4->on_parm].len, e4buf+p4->parms[p4->on_parm+1].pos, p4->parms[p4->on_parm+1].len);
      p4->parms[p4->on_parm+1].pos =  p4->parms[p4->on_parm].pos + p4->parms[p4->on_parm].len;   
   }
}

void e4parms_pos_align( E4PARMS *p4 )
{
   int  extra_len ;

   extra_len =  sizeof(double) - p4->parms[p4->on_parm].pos % sizeof(double) ;
   if ( extra_len == sizeof(double) )  extra_len =  0 ;
   p4->parms[p4->on_parm].pos +=  extra_len ;
}

int e4parms_check( E4PARMS *p4 )
{
   if ( p4->code_base->error_code < 0 )  return -1 ;
   if ( p4->on_parm+2 >= E4PARM_STACK_SIZE || p4->on_parm < 0 )
      return e4error( p4->code_base, e4overflow, (char *) 0 ) ;
   return 0 ;
}

int e4parms_pop( E4PARMS *p4, int n )
{
   p4->on_parm -= n ;
   if ( e4parms_check(p4) < 0 )  return -1 ;
   return 0 ;
}

int  e4parms_push( E4PARMS *p4 )
{
   if ( e4parms_check(p4) < 0 )  return -1 ;
   p4->parms[p4->on_parm+1].pos =  p4->parms[p4->on_parm].pos + p4->parms[p4->on_parm].len ;
   p4->on_parm++ ;
   return 0 ;
}

/* Function rules

   1.  Place the result back into the first parameter.
   2.  If the result is of length greater than the length of
       the first parameter, be aware that the result will overwrite
       the second, ...
   3.  If the type of the result is different than the type
       of the first parameter, set the type.
   4.  If the length of the result is different from the length
       of the first parameter, set the resulting length.
   5.  If there is an error, set a length of '-1'.
*/

void  e4true_eval( E4PARMS *p4 )
{
   e4parms_set_logical( p4, 1 ) ;
}

void  e4false( E4PARMS *p4 )
{
   e4parms_set_logical( p4, 0 ) ;
}

void  e4add( E4PARMS *p4 )
{
   e4parms_set_double( p4, *e4parms_double_ptr(p4,0) + *e4parms_double_ptr(p4,1) ) ;
   if ( p4->parms[p4->on_parm+1].type == t4date_doub )
      p4->parms[p4->on_parm].type =  t4date_doub ;
}

void  e4concatenate( E4PARMS *p4 )
{
   e4parms_fix_pos( p4 );
   p4->parms[p4->on_parm].len +=  p4->parms[p4->on_parm+1].len ;
}

void  e4sub( E4PARMS *p4 )
{
   int final_type ;

   final_type =  t4date_doub ;
   if ( p4->parms[p4->on_parm].type == p4->parms[p4->on_parm+1].type )
      final_type =  t4num_doub ;

   e4parms_set_double( p4, *e4parms_double_ptr(p4,0) - *e4parms_double_ptr(p4,1) ) ;
   p4->parms[p4->on_parm].type =  final_type ;
}

void  e4concat_two( E4PARMS *p4 )
{
   char *ptr ;
   int pos, n_blanks ;

   ptr =  e4buf + p4->parms[p4->on_parm].pos ;
   for ( pos = p4->parms[p4->on_parm].len-1; pos >= 0; pos-- )
      if ( ptr[pos] != ' ' && ptr[pos] != 0 )  break ;

   n_blanks =   p4->parms[p4->on_parm].len - ++pos ;
   ptr +=  pos ;

   memmove( ptr, e4buf+p4->parms[p4->on_parm+1].pos, p4->parms[p4->on_parm+1].len ) ;
   memset( ptr+p4->parms[p4->on_parm+1].len, (int) ' ', (size_t) n_blanks ) ;

   p4->parms[p4->on_parm].len =  p4->parms[p4->on_parm].len + p4->parms[p4->on_parm+1].len ;
}

void  e4multiply( E4PARMS *p4 )
{
   *e4parms_double_ptr(p4,0) *=  *e4parms_double_ptr(p4,1) ;
}

void  e4divide( E4PARMS *p4 )
{
   *e4parms_double_ptr(p4,0) /= *e4parms_double_ptr(p4,1) ;
}

#ifndef S4NO_POW
void  e4power( E4PARMS *p4 )
{
   *e4parms_double_ptr(p4,0) =  pow( *e4parms_double_ptr(p4,0), *e4parms_double_ptr(p4,1) ) ;
}
#endif

void  e4not_equal( E4PARMS *p4 )
{
   int  rc, len ;

   switch( p4->parms[p4->on_parm+1].type )
   {
      case t4str:
         len =  (p4->parms[p4->on_parm].len < p4->parms[p4->on_parm+1].len) ? p4->parms[p4->on_parm].len : p4->parms[p4->on_parm+1].len ;
	 rc =  memcmp( e4buf+p4->parms[p4->on_parm].pos, e4buf+p4->parms[p4->on_parm+1].pos, (size_t) len) ;

         if ( rc == 0 )
            if ( p4->parms[p4->on_parm].len < p4->parms[p4->on_parm+1].len )
	       rc = -1 ;
         break ;

      case t4num_doub:
      case t4date_doub:
         if ( *e4parms_double_ptr(p4,0) <  *e4parms_double_ptr(p4,1) )
            rc =  -1 ;
         else
         {
            if ( *e4parms_double_ptr(p4,0) == *e4parms_double_ptr(p4,1) )
               rc =  0 ;
            else
               rc =  1 ;
         }
         break ;

      case t4log:
         /*  Equal if .T. and .T. or .F. .and .F. */
         rc = ! (*e4parms_int_ptr(p4,0) &&  *e4parms_int_ptr(p4,1) ||
               ! *e4parms_int_ptr(p4,0) &&  ! *e4parms_int_ptr(p4,1) ) ;
         break ;
   }       

   e4parms_set_logical( p4, rc ) ;
}

void  e4equal( E4PARMS *p4 )
{
   e4not_equal( p4 ) ;
   *e4parms_int_ptr(p4,0) =  ! *e4parms_int_ptr(p4,0) ;
}

void  e4greater( E4PARMS *p4 )
{
   e4not_equal( p4 ) ;
   *e4parms_int_ptr(p4,0) =  *e4parms_int_ptr(p4,0) > 0 ;
}

void  e4less( E4PARMS *p4 )
{
   e4not_equal( p4 ) ;
   *e4parms_int_ptr(p4,0) =  *e4parms_int_ptr(p4,0) < 0 ;
}

void  e4greater_eq( E4PARMS *p4 )
{
   e4not_equal( p4 ) ;
   *e4parms_int_ptr(p4,0) =  *e4parms_int_ptr(p4,0) >= 0 ;
}

void  e4less_eq( E4PARMS *p4 )
{
   e4not_equal( p4 ) ;
   *e4parms_int_ptr(p4,0) =  *e4parms_int_ptr(p4,0) <= 0 ;
}

void  e4not( E4PARMS *p4 )
{
   *e4parms_int_ptr(p4,0) =   ! *e4parms_int_ptr(p4,0) ;
}

void  e4or( E4PARMS *p4 )
{
   *e4parms_int_ptr(p4,0) =   *e4parms_int_ptr(p4,0)  ||  *e4parms_int_ptr(p4,1) ;
}

void  e4and( E4PARMS *p4 )
{
   *e4parms_int_ptr(p4,0) =   *e4parms_int_ptr(p4,0)  &&  *e4parms_int_ptr(p4,1) ;
}

void  e4stod( E4PARMS *p4 )
{
   e4parms_set_double( p4, (double) a4long(e4buf+p4->parms[p4->on_parm].pos)) ;
   p4->parms[p4->on_parm].type =  t4date_doub ;
}

void  e4dtos( E4PARMS *p4 )
{
   double *d_ptr ;
   char buf[9] ;

   if ( p4->parms[p4->on_parm].type == t4date_doub )
   {
      d_ptr =  e4parms_double_ptr(p4,0) ;
      
      a4assign( buf, (long) (*d_ptr) ) ;
      e4parms_set( p4, buf, 8, t4str ) ;
   }
   else
      p4->parms[p4->on_parm].type =  t4str ;
}

void  e4ctod( E4PARMS *p4 )
{
   char buf[9] ;

   a4init( buf, e4buf+p4->parms[p4->on_parm].pos, p4->code_base->date_format) ;
   e4parms_set_double( p4, (double) a4long(buf) ) ;
   p4->parms[p4->on_parm].type =  t4date_doub ;
}

void  e4dtoc( E4PARMS *p4 )
{
   char buf[9] ;
   int   len ;

   e4dtos( p4 ) ;

   len =  strlen( p4->code_base->date_format ) ;
   if ( e4parms_space_reserve( p4, len ) < 0 ) return ;

   strncpy(buf,e4buf+p4->parms[p4->on_parm].pos, sizeof(buf)) ;
   a4format( buf, e4buf, p4->code_base->date_format ) ;

   p4->parms[p4->on_parm].len =  len ;
}

void  e4date( E4PARMS *p4 )
{
   char buf[9] ;

   a4today( buf ) ;
   e4parms_set_double( p4, (double) a4long(buf) ) ;
   p4->parms[p4->on_parm].type =  t4date_doub ;
}

void  e4day( E4PARMS *p4 )
{
   e4dtos( p4 ) ;
   e4parms_set_double( p4, c4atod( (e4buf+p4->parms[p4->on_parm].pos)+6, 2) ) ;
}

void  e4month( E4PARMS *p4 )
{
   e4dtos( p4 ) ;
   e4parms_set_double( p4, c4atod( (e4buf+p4->parms[p4->on_parm].pos)+4, 2) ) ;
}

void  e4year( E4PARMS *p4 )
{
   e4dtos( p4 ) ;
   e4parms_set_double( p4, c4atod( (e4buf+p4->parms[p4->on_parm].pos), 4) ) ;
}

void  e4del( E4PARMS *p4 )
{
   e4parms_set( p4, p4->data->record, 1, t4str ) ;
}

void  e4deleted( E4PARMS *p4 )
{
   e4parms_set_logical( p4, d4deleted(p4->data) ) ;
}

void  e4iif( E4PARMS *p4 )
{
   int i ;

   i =  *e4parms_int_ptr(p4,0) ? 1 : 2 ;
   e4parms_set( p4, e4buf+p4->parms[p4->on_parm+i].pos,
                    p4->parms[p4->on_parm+i].len,
                    p4->parms[p4->on_parm+i].type ) ;
}

void  e4reccount( E4PARMS *p4 )
{
   e4parms_set_double( p4, (double) d4reccount( p4->data ) ) ;
}

void  e4recno( E4PARMS *p4 )
{
   e4parms_set_double( p4, (double) d4recno( p4->data ) ) ;
}

void  e4str( E4PARMS *p4 )
{
   int len, dec ;

   len =  (int) *e4parms_double_ptr(p4,1) ;
   dec =  (int) *e4parms_double_ptr( p4, 2) ;

   if ( e4parms_space_reserve( p4, (unsigned) len ) < 0 )  return ;

   c4dtoa45( *e4parms_double_ptr(p4,0), e4buf+p4->parms[p4->on_parm].pos, len, dec) ;

   p4->parms[p4->on_parm].type =  t4str ;
   p4->parms[p4->on_parm].len =  len ;
}

void  e4substr( E4PARMS *p4 )
{
   unsigned i_temp, start_pos, len ;

   i_temp =  (int) *e4parms_double_ptr(p4,1) ;
   start_pos =  i_temp - 1 ;
   if ( (int) start_pos < 0 )  start_pos =  0 ;
   if ( start_pos > p4->parms[p4->on_parm].len )  start_pos =  p4->parms[p4->on_parm].len ;

   len =  (int) *e4parms_double_ptr(p4,2) ;
   if ( len > p4->parms[p4->on_parm].len - start_pos )
      len =  p4->parms[p4->on_parm].len - start_pos ;

   e4parms_set( p4, (e4buf+p4->parms[p4->on_parm].pos)+start_pos, len, t4str ) ;
}

void  e4time( E4PARMS *p4 )
{
   char buf[9] ;
   a4time_now( buf ) ;
   e4parms_set( p4, buf, 8, t4str ) ;
}

void  e4upper( E4PARMS *p4 )
{
   if ( e4parms_space_reserve( p4, p4->parms[p4->on_parm].len+1 ) < 0 )  return ;
   p4->parms[p4->on_parm].len-- ;
   e4buf[ p4->parms[p4->on_parm].pos + p4->parms[p4->on_parm].len ] = 0 ;
   c4upper( e4buf+p4->parms[p4->on_parm].pos ) ;
}

void  e4val( E4PARMS *p4 )
{
   e4parms_set_double( p4, c4atod(e4buf+p4->parms[p4->on_parm].pos, p4->parms[p4->on_parm].len) );
}

/* Is the first string contained in the second */
void  e4contain( E4PARMS *p4 )  
{
   int   a_len, comp_len, i ;
   char  first_char, *b_ptr ;

   a_len      =  p4->parms[p4->on_parm].len ;
   first_char =  e4buf[p4->parms[p4->on_parm].pos] ;
   comp_len   =  p4->parms[p4->on_parm+1].len - a_len ;
   b_ptr      =  e4buf+p4->parms[p4->on_parm+1].pos ;

   /* See if there is a match */
   for ( i=0; i <= comp_len; i++ )
      if ( first_char == b_ptr[i] )
	 if ( memcmp( e4buf+p4->parms[p4->on_parm].pos, b_ptr+i, (size_t) a_len ) == 0 )
         {
	    e4parms_set_logical(p4, 1) ;
	    return ;
	 }

   e4parms_set_logical(p4, 0) ;
}


