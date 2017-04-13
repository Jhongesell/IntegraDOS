/* d4positi.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION d4position_set( D4DATA *d4, double pos )
{
   T4TAG *tag_on ;
   long   new_rec, count ;
   int    rc ;

   if ( d4 == 0 )  return -1 ;

   if ( pos > 1.0 )  return d4go_eof(d4) ;
   if ( pos <= 0 )   return d4top(d4) ;

   tag_on =  d4tag_selected(d4) ;

   if ( tag_on == 0 )
   {
      count =  d4reccount(d4) ;
      if ( count <= 0L )  return d4go_eof(d4) ;

      new_rec = (long) (pos * ((double) count - 1) + 1.5) ;
      if ( new_rec > count ) new_rec = count ;
   }
   else
   {
      if ( (rc = t4position_set(tag_on,pos)) != 0 )  return rc ;
      if ( rc == r4eof )  return  d4go_eof(d4) ;

      new_rec =  t4recno(tag_on) ;
   }

   return d4go(d4,new_rec) ;
}

int S4FUNCTION d4position2( D4DATA *d4, double *result )
{
   *result = d4position( d4 ) ;
   if (*result < 0.0) 
      return -1;
   return 0;
}

double S4FUNCTION d4position( D4DATA *d4 )
{
   T4TAG *tag_on ;
   long   count ;
   int    rc, len ;
   char  *result ;

   if ( d4eof(d4) ) return 1.1 ;

   tag_on =  d4tag_selected( d4 ) ;

   if ( tag_on == 0 || d4->rec_num <= 0L )
   {
      count =  d4reccount(d4) ;
      if ( count < 0 )  return -1.0 ;
      if ( count == 0  ||  d4->rec_num <= 0L )  return 0.0 ;

      return  (double) ( d4->rec_num - 1) / ( count - (count != 1 ) );
   }
   else
   {
      if ( t4recno(tag_on) == d4->rec_num )
         return t4position(tag_on) ;

      if ( (len = e4key( tag_on->expr, &result)) < 0 )  return -1.0 ;
      rc =  t4seek( tag_on, result, len ) ;
      if ( rc != 0  && rc != r4eof && rc != r4after )  return -1.0 ;

      return t4position(tag_on) ;
   }
}

