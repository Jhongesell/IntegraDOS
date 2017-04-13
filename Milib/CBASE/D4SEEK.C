/* d4seek.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int  S4FUNCTION d4seek_double( D4DATA *d4, double dkey )
{
   T4TAG *tag ;
   int    rc, rc2 ;
   char buf[I4MAX_KEY_SIZE] ;
   #ifdef S4CLIPPER
      int len ;
   #endif

   if ( d4 == 0 )  return -1L ;
   tag =  d4tag_default(d4) ;

   if ( d4->code_base->error_code < 0 )  return -1 ;
   if ( tag == 0 )  return r4no_tag ;

   rc =  d4flush_record(d4) ;  if ( rc != 0 )  return rc ;

   #ifdef S4CLIPPER
      if ( tag->dtok == 0 )
      {
         len = tag->header.key_len ;
	 c4dtoa45( dkey, buf, len, tag->header.key_dec ) ;
	 if ( buf[0] == '*' )  return -1 ;  /* unknown overflow result */
	 c4clip( buf, len ) ;
      }
      else
   #else
      #ifdef S4DEBUG
         if ( tag->dtok == 0 )
       e4severe( e4info, "d4seek(double)", E4_INFO_WT4, (char *) 0 ) ;
      #endif
   #endif

   tag->dtok( buf, dkey ) ;

   #ifdef S4MDX
      rc =  t4seek( tag, buf, tag->header.value_len ) ;
   #else
      rc =  t4seek( tag, buf, tag->header.key_len ) ;
   #endif

   if ( rc == r4locked )  return r4locked ;

   if ( t4eof(tag) )  return d4go_eof(d4) ;
   rc2 =  d4go( d4, t4recno(tag) ) ;
   if ( rc2 != 0 )  return rc2 ;
   return rc ;
}

int  S4FUNCTION d4seek( D4DATA *d4, char *str )
{
   T4TAG *tag ;
   int    rc, rc2, str_len ;
   char buf[I4MAX_KEY_SIZE] ;

   if ( d4 == 0 )  return -1L ;
   tag =  d4tag_default(d4) ;

   if ( d4->code_base->error_code < 0 )  return -1 ;
   if ( tag == 0 )  return r4no_tag ;

   rc =  d4flush_record(d4) ;  if ( rc != 0 )  return rc ;

   str_len =  strlen(str) ;
   (*tag->stok)( buf, str, str_len ) ;

   #ifdef S4MDX
      if ( tag->header.type != 'C' )
         str_len =  tag->header.value_len ;
   #else
      if ( tag->expr->type != t4str )
         str_len =  tag->header.key_len ;
   #endif

   rc =  t4seek( tag, buf, str_len ) ;
   if ( t4eof(tag) )  return d4go_eof(d4) ;

   rc2 =  d4go( d4, t4recno(tag) ) ;
   if ( rc2 != 0 )  return rc2 ;
   return rc ;
}


