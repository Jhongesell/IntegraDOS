/* d4tag.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

void S4FUNCTION d4tag_select( D4DATA *d4, T4TAG *t4 )
{
   #ifdef S4DEBUG
      if ( t4 != 0 )
         if ( t4->index->data != d4 )
            e4severe( e4parm, "d4tag_select()", (char *) 0 ) ;
   #endif

   if ( t4 == 0 )
      d4->indexes.selected =  0 ;
   else
   {
      d4->indexes.selected     =  (void *) t4->index ;
      t4->index->tags.selected =  (void *) t4 ;
   }
}

T4TAG *S4FUNCTION d4tag( D4DATA *d4, char *tag_name )
{
   char tag_lookup[11] ;
   T4TAG *tag_on ;

   if ( d4 == 0 )  return 0 ;

   u4ncpy( tag_lookup, tag_name, sizeof(tag_lookup) ) ;

   c4upper( tag_lookup ) ;

   for( tag_on = 0;;)
   {
      tag_on =  d4tag_next( d4, tag_on ) ;
      if ( tag_on == 0 )   break ;

      if ( strcmp( tag_on->alias, tag_lookup) == 0 )
         return tag_on ;
   }

   if ( d4->code_base->tag_name_error )
      e4error( d4->code_base, e4tag_name, tag_name, (char *) 0 ) ;
   return 0 ;
}

T4TAG *S4FUNCTION d4tag_default( D4DATA *d4 )
{
   T4TAG *tag ;
   I4INDEX *index ;

   if ( d4 == 0 )  return 0 ;

   tag =  d4tag_selected( d4 ) ;
   if ( tag != 0 )  return tag ;

   index =  (I4INDEX *) l4first( &d4->indexes ) ;
   if ( index != 0 )
   {
      tag =  (T4TAG *) l4first( &index->tags ) ;
      if ( tag != 0 )  return tag ;
  }
 
   return 0 ;
}

T4TAG *S4FUNCTION d4tag_selected( D4DATA *d4 )
{
   I4INDEX *index ;
   T4TAG   *tag ;

   if ( d4 == 0 )  return 0 ;

   index =  (I4INDEX *) d4->indexes.selected ;
   if ( index != 0 )
   {
      tag =  (T4TAG *) index->tags.selected ;
      if ( tag != 0 )  return tag ;
   }

   return 0 ;
}

T4TAG *S4FUNCTION d4tag_next( D4DATA *d4, T4TAG *tag_on )
{
   I4INDEX *i4 ;
   if ( tag_on == 0 )
   {
      i4 =  (I4INDEX *) l4first( &d4->indexes ) ;
      if ( i4 == 0 )  return 0 ;
   }
   else
      i4 =  tag_on->index ;

   tag_on =  (T4TAG *) l4next( &i4->tags, tag_on ) ;
   if ( tag_on != 0 )  return tag_on ;

   i4 =  (I4INDEX *) l4next( &d4->indexes, i4 ) ;
   if ( i4 == 0 )  return 0 ;

   return (T4TAG *) l4first( &i4->tags ) ;
}

T4TAG *S4FUNCTION d4tag_prev( D4DATA *d4, T4TAG *tag_on )
{
   I4INDEX *i4 ;
   if ( tag_on == 0 )
   {
      i4 =  (I4INDEX *) l4last( &d4->indexes ) ;
      if ( i4 == 0 )  return 0 ;
   }
   else
      i4 =  tag_on->index ;

   tag_on =  (T4TAG *) l4prev( &i4->tags, tag_on ) ;
   if ( tag_on != 0 )  return tag_on ;

   i4 =  (I4INDEX *) l4prev( &d4->indexes, i4 ) ;
   if ( i4 == 0 )  return 0 ;

   return (T4TAG *) l4last( &i4->tags ) ;
}

