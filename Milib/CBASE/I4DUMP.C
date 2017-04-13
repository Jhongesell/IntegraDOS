/* i4dump.c   (C)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved.

Displays the contents of a tag.
Currently uses printf */

/* not supported for FoxPro compact index file structures */
#ifdef S4MDX    /*  ifndef S4FOX  */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

static void output( int h, char * s )
{
  write( h, s, strlen(s) ) ;
}

static int  t4dump_do( T4TAG *t4, int out_handle, int level, int display_all )
{
   char  out_buf[200] ;
   int rc, i_key ;
   B4BLOCK *block_on ;
   char  buf[ I4MAX_KEY_SIZE ] ;
   int   len ;

   rc =  t4down(t4) ;  if ( rc != 0 )  return rc ;
   block_on =  t4block(t4) ;

   if ( b4leaf(block_on) && ! display_all )
   {
      t4up(t4) ;
      return 0 ;
   }

   if ( b4leaf( block_on) )
   {
      sprintf( out_buf, "\r\n\r\nLeaf B4BLOCK #: %ld   Level %d\r\n\r\n", block_on->file_block, level ) ;
      output( out_handle, out_buf ) ;
      output( out_handle, "Record Number   Key\r\n" ) ;
   }
   else
   {
      sprintf( out_buf, "\r\n\r\nBranch B4BLOCK #: %ld   Level %d\r\n\r\n", block_on->file_block, level ) ;
      output( out_handle, out_buf ) ;
      output( out_handle, "H4FILE B4BLOCK      Key\r\n" ) ;
   }

   for ( i_key = 0; i_key < block_on->n_keys; i_key++ )
   {
      sprintf( out_buf, "\r\n%10ld      ", b4key(block_on,i_key)->num ) ;

      output( out_handle, out_buf ) ;
      if ( t4type(t4) == t4str )
      {
         len =  50 ;
         if ( len > t4->header.value_len )  len =  t4->header.value_len ;

         memcpy( buf, b4key_key(block_on, i_key), len ) ;
         buf[len] =  0 ;
         output( out_handle, buf ) ;
      }
   }

   if ( b4leaf( block_on) )
   {
      t4up(t4) ;
      return 0 ;
   }

   sprintf( out_buf, "\r\n%10ld      ", b4key( block_on, i_key)->num ) ;
   output( out_handle, out_buf ) ;

   block_on->key_on = 0 ;
   do
   {
      rc = t4dump_do( t4, out_handle, level+1, display_all ) ;
      if ( rc > 0 )  e4severe( e4result, "t4dump_do()", (char *) 0 ) ;
      if ( rc < 0 )  return -1 ;
   } while ( b4skip( block_on, 1) == 1 ) ;

   t4up(t4) ;
   return 0 ;
}

int S4FUNCTION t4dump( T4TAG *t4, int out_handle, int display_all )
{
   int rc ;
   if ( (rc = t4lock(t4)) != 0 )  return rc ;
   if ( t4free_all(t4) < 0 )  return -1 ;
   output( out_handle, "\r\n\r\n" ) ;
   return t4dump_do( t4, out_handle, 0, display_all ) ;
}

#endif

