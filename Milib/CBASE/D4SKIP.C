/* d4skip.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int  S4FUNCTION d4skip( D4DATA *d4, long n )
{
   T4TAG *tag ;
   int rc, save_flag, old_eof_flag ;
   long start_rec, new_rec, count, n_skipped ;
   char *key_value ;
   C4CODE *c4 ;

   if ( d4 == 0 )  return -1L ;
   c4 =  d4->code_base ;
   tag =  d4tag_selected(d4) ;

   if ( c4->error_code < 0 )  return -1 ;

   if ( tag == 0 )
   {
      d4->bof_flag =  0 ;
      start_rec =  d4->rec_num ;
      new_rec =  start_rec+n ;
      if ( new_rec > 0L )
      {
         save_flag =  c4->go_error ;
         c4->go_error =  0 ;
         rc = d4go(d4,new_rec) ;
         c4->go_error =  save_flag ;
         if ( rc >= 0 && rc != r4entry )  return rc ;
      }

      count =  d4reccount(d4) ;
      if ( count <= 0L || new_rec > count )
      {
         if ( count <= 0L )
         {
            if ( count < 0 )  return -1 ;
            if ( (rc = d4go_eof(d4)) != r4eof )  return rc ;
            d4->bof_flag =  1 ;
         }
         if ( n < 0 )
         {
            d4->bof_flag =  1 ;
            return r4bof ;
         }
         else
            return d4go_eof(d4) ;
      }

      if ( new_rec < 1L )
      {
         old_eof_flag =  d4->eof_flag ;
         rc =  d4go(d4,1L) ;  if ( rc != 0 )  return rc ;
         d4->bof_flag =  1 ;
         d4->eof_flag =  old_eof_flag ;
         return r4bof ;
      }

      return d4go(d4,new_rec) ;
   }
   else
   {
      if ( d4->eof_flag )
      {
         if ( n >= 0 ) return d4go_eof(d4) ;

         rc =  d4bottom(d4) ;  if ( rc !=  0 && rc != r4eof )  return rc ;
         if ( rc == r4eof )
         {
            if ( (rc = d4go_eof(d4)) != r4eof )  return rc ;
            return r4bof ;
         }
         n++ ;
         d4->rec_num =  t4recno(tag) ;
      }

      if ( n == 0 )  return 0 ;
      d4->bof_flag =  0 ;

      if ( d4->record_changed )
         if ( d4flush_record(d4) < 0 )  return -1 ;

      if ( t4recno(tag) == d4->rec_num )
      {
         rc =  t4lock(tag) ;
         if ( rc != 0 )  return rc ;
      }

      if ( t4recno(tag) != d4->rec_num )
      {
         rc =  d4lock(d4, d4->rec_num ) ;  if ( rc != 0 )  return rc ;
         d4go( d4, d4->rec_num ) ;

         e4key( tag->expr, &key_value ) ;

         rc = t4go(tag, key_value, d4->rec_num ) ;  if ( rc < 0 )  return -1 ;
         if ( rc == r4locked )  return r4locked ;
         if ( rc > 0 )
         {
            if ( c4->skip_error )
               return e4error( c4, e4entry, d4->file.name, (char *) 0 ) ;
            else
               return r4entry ;
         }
      }

      #ifdef S4FOX
         if ( tag->header.descending )
            n_skipped = -t4skip(tag,-n) ;
         else
            n_skipped =  t4skip(tag,n) ;
         if ( n > 0  &&  n_skipped != n )
            return d4go_eof(d4) ;
      #else
         n_skipped =  t4skip(tag,n) ;
         if ( n > 0  &&  n_skipped != n )
            return d4go_eof(d4) ;
      #endif

      if ( (rc = d4go(d4, t4recno(tag))) != 0 )  return rc ;
      if ( n == n_skipped )  return 0 ;
      #ifdef S4FOX
         if ( ( n < 0 && !tag->header.descending ) ||  ( n < 0 && tag->header.descending ) )
      #else
         if ( n < 0 )
      #endif
      {
         d4->bof_flag =  1 ;
         return r4bof ;
      }
   }
   return 0 ;
}

