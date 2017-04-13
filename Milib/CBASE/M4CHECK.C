/* m4check.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"

/* not supported for FoxPro FPT memo files */
#ifndef S4MFOX
#ifndef S4MNDX

#ifdef __TURBOC__
   #pragma hdrstop
#endif

S4FUNCTION m4check( M4FILE *m4 )
{
   F4FLAG flags ;
   D4DATA *d4 ;
   int  rc, i_field ;
   long num_blocks, memo_id ;
   M4CHAIN_ENTRY cur ;

   d4 =  m4->data ;
   rc =  d4lock_file(d4) ;
   if ( rc != 0 )  return rc ;

   if ( (rc = d4flush_all(d4)) != 0)  return rc ;

   if ( f4flag_init( &flags, d4->code_base, h4length(&m4->file)/m4->block_size ) < 0 )
      return e4memory ;

   /* Set flags for the data file entries */
   for ( d4top(d4); ! d4eof(d4); d4skip(d4,1L) )
   {
      for ( i_field =  0;  i_field < d4->n_fields_memo; i_field++ )
      {
	 num_blocks =  (m4len(d4->fields_memo[i_field].field) + m4->block_size - 1)/ m4->block_size ;
	 memo_id =  f4long( d4->fields_memo[i_field].field ) ;

	 if ( f4flag_is_any_set( &flags, memo_id, num_blocks ) )
	    return e4error( d4->code_base, e4info, "m4check()", E4_INFO_AME, (char *) 0 ) ;
	 if ( f4flag_set_range( &flags, memo_id, num_blocks ) < 0 )
	    return -1 ;
      }
   }

   /* Set flags for the free chain */
   memset( (void *)&cur, 0, sizeof(cur) ) ;
   m4file_chain_skip( m4, &cur ) ;  /* Read in root */

   for ( m4file_chain_skip(m4,&cur); cur.next != -1; m4file_chain_skip(m4,&cur))
   {
      rc = f4flag_is_any_set( &flags, cur.block_no, cur.num) ;
      if ( rc < 0 )  return -1 ;
      if ( rc )
         return e4error( d4->code_base, e4info, "m4check()", E4_INFO_AME, (char *) 0 ) ;
      if ( f4flag_set_range( &flags, cur.block_no, cur.num ) < 0 )  return -1 ;
   }

   rc =  f4flag_is_all_set( &flags, 1, h4length(&m4->file)/m4->block_size-1 ) ;
   u4free( flags.flags ) ;
   if ( rc == 0 )
      return e4error( d4->code_base, e4result, "m4check()", E4_RESULT_WAS, (char *) 0 ) ;
   if ( rc < 0 )  return -1 ;

   return 0 ;
}

#endif  /*   ifndef S4MFOX   */
#endif  /*   ifndef S4MNDX   */
