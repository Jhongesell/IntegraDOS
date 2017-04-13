/* c4code.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef __TURBOC__
   #ifndef __DLL__
   #ifdef S4DEBUG
      extern unsigned _stklen ;
   #endif
   #endif
#endif

char m4null_char = '\0' ;
unsigned  e4buf_len = 0 ;
char  *e4buf = 0 ;

#ifdef __DLL__
   #include <windows.h>
   int PASCAL LibMain( HANDLE hInstance, WORD wDataSeg, WORD cbHeapSize, LPSTR lpCmdLine )
   {
      return 1 ;
   }
#endif

void S4FUNCTION d4init( C4CODE *c4 )
{
   memset( (void *)c4, 0, sizeof(C4CODE) ) ;
   #ifndef S4DLL
      y4init() ;
   #endif

   #ifdef S4DEBUG
      /* if a DLL, can't check the stack length since this is a separate executable */
      #ifndef __DLL__
      #ifdef __TURBOC__
         if ( _stklen < 5000 ) /* 5000 seems to be an appropriate minimum */
            e4severe( e4result, E4_RESULT_STC, (char *) 0 ) ;
      #endif
      #endif
      c4->debug_int =  0x5281 ; /* Some random value for double checking. */
   #endif

   c4->mem_size_block   =  0x400 ;   /* 1024 */

   c4->mem_size_sort_pool   =  0xF000 ;  /* 61440 */
   c4->mem_size_sort_buffer =  0x1000 ;  /* 4096 */
   c4->mem_size_buffer      =  0x4000 ;  /* 16384 */

   c4->default_unique_error =  r4unique_continue ;
   u4ncpy( c4->date_format, "MM/DD/YY", sizeof(c4->date_format) ) ;

   #ifdef S4CLIPPER
      c4->numeric_str_len  = 10 ;    /* default length for clipper numeric keys is 10  */
      c4->decimals         =  0 ;
   #endif

   /* Flags initialization */
   c4->go_error=  c4->open_error=  c4->create_error =  c4->skip_error =  
      c4->tag_name_error =  c4->auto_open =  c4->field_name_error =
           c4->read_lock =  c4->safety =  c4->wait =  1;

   c4->mem_start_index= c4->mem_expand_index = 
                        c4->mem_expand_data  = c4->mem_start_data =  5 ;
   c4->mem_start_block= c4->mem_expand_block = 
         c4->mem_start_tag  = c4->mem_expand_tag =  10 ;
}


int S4FUNCTION d4init_undo( C4CODE *c4 )
{
   d4close_all(c4) ;

   y4release( c4->index_memory ) ;
   c4->index_memory =  0 ;

   y4release( c4->data_memory ) ;
   c4->data_memory =  0 ;

   y4release( c4->tag_memory ) ;
   c4->tag_memory =  0 ;

   u4free( e4buf ) ;
   e4buf_len = 0 ;
   e4buf = 0 ;

   return c4->error_code ;
}

int S4FUNCTION d4close_all( C4CODE *c4 )
{
   D4DATA *data_on, *data_next ;
   int rc, rc_return ;

   rc = rc_return = 0 ;

   for ( data_next = (D4DATA *) l4first(&c4->data_list);; )
   {
      data_on =  data_next ;
      if ( data_on == 0 )  break ;
      data_next = (D4DATA *) l4next( &c4->data_list, data_next);

      if ( (rc = d4close(data_on)) < 0 )  rc_return =  rc ;
   }

   if ( c4->error_code < 0 )  return -1 ;
   return rc_return ;
}

D4DATA *S4FUNCTION d4data( C4CODE *c4, char *alias_name )
{
   char buf[12] ;
   D4DATA *data_on, *data_result ;

   if ( c4->error_code < 0 )  return 0 ;

   u4ncpy( buf, alias_name, sizeof(buf) ) ;
   c4upper( buf ) ;

   data_on =  data_result =  0 ;

   while( data_on =  (D4DATA *) l4next( &c4->data_list, data_on) )
      if ( strcmp(buf, data_on->alias) == 0 )
      {
         #ifdef S4DEBUG
       if ( data_result != 0 )  e4severe( e4info, "d4data()", E4_INFO_DUP, (char *) 0 ) ;
       data_result =  data_on ;
    #else
       data_result =  data_on ;
       break ;
    #endif
      }
   return data_result ;
}

