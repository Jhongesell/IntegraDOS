/* l4link.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#define  LINK_PTR(p)  ((L4LINK *)p)

void *S4FUNCTION l4first( L4LIST *l4 )
{
   if ( l4->last == 0 )  return 0 ;
   return l4->last->n ;
}

void *S4FUNCTION l4next( L4LIST *l4, void *link )
{
   if ( link == l4->last )  return 0 ;
   if ( link == 0 )  return l4first(l4) ;

   return LINK_PTR(link)->n ;
}

void *S4FUNCTION l4prev( L4LIST *l4, void *link )
{
   if ( link == 0 )  return l4->last ;
   if ( l4->last->n == link )  return 0 ;

   return LINK_PTR(link)->p ;
}

void *S4FUNCTION l4last( L4LIST *l4 )
{
   return l4->last ;
}

void  S4FUNCTION l4add_before( L4LIST *l4, void *anchor, void *new_link )
{
   #ifdef S4DEBUG
      if ( new_link == 0 )
	 e4severe( e4parm, "l4add_after()", (char *) 0 ) ;
   #endif

   if ( l4->last == 0 )
   {
      l4->last =  (L4LINK *) LINK_PTR(new_link)->p
               = (L4LINK *) LINK_PTR(new_link)->n = (L4LINK *) new_link ;
   }
   else
   {
      LINK_PTR(new_link)->n = (L4LINK *) anchor ;
      LINK_PTR(new_link)->p =  LINK_PTR(anchor)->p ;
      LINK_PTR(anchor)->p->n= (L4LINK *) new_link ;
      LINK_PTR(anchor)->p   = (L4LINK *) new_link ;
   }

   l4->n_link++ ;
   #ifdef S4DEBUG
      l4check(l4) ;
   #endif

   return ;
}


void S4FUNCTION l4add_after( L4LIST *l4, void *anchor, void *new_link )
{
   #ifdef S4DEBUG
      if ( new_link == 0 )
         e4severe( e4parm, "l4add_after()", (char *) 0 ) ;
   #endif

   if ( l4->last == 0 )
   {
      l4->last =  (L4LINK *) LINK_PTR(new_link)->p
               = (L4LINK *) LINK_PTR(new_link)->n = (L4LINK *) new_link ;
   }
   else
   {
      LINK_PTR(new_link)->p =  (L4LINK *) anchor ;
      LINK_PTR(new_link)->n =  LINK_PTR(anchor)->n ;
      LINK_PTR(anchor)->n->p=  (L4LINK *) new_link ;
      LINK_PTR(anchor)->n   =  (L4LINK *) new_link ;
      if ( anchor == l4->last )  l4->last =  (L4LINK *) new_link ;
   }

   l4->n_link++ ;
   #ifdef S4DEBUG
      l4check(l4) ;
   #endif

   return ;
}

void S4FUNCTION l4add( L4LIST *l4, void *new_link )
{
   l4add_after( l4, l4->last, new_link ) ;
}

void S4FUNCTION l4remove( L4LIST *l4, void *remove_link )
{
   #ifdef S4DEBUG
      L4LINK *link_on ;
   #endif

   if ( remove_link == 0 )  return ;

   #ifdef S4DEBUG
      /* Make sure the link being removed is on the linked list ! */
      for ( link_on = 0; link_on =  (L4LINK *) l4next(l4,link_on); )
         if ( link_on == remove_link )  break ;

      if ( link_on != remove_link )
         e4severe( e4info, "l4remove()", E4_INFO_LIN, (char *) 0 ) ;
   #endif

   if ( l4->selected == remove_link )
   {
      l4->selected =  LINK_PTR(remove_link)->p ;
      if ( l4->selected == remove_link )  l4->selected =  0 ;
   }

   LINK_PTR(remove_link)->p->n = LINK_PTR(remove_link)->n ;
   LINK_PTR(remove_link)->n->p = LINK_PTR(remove_link)->p ;
   if ( remove_link == l4->last )
   {
      if ( l4->last->p == l4->last )
	 l4->last =  0 ;
      else
	 l4->last =  l4->last->p ;
   }

   l4->n_link-- ;

   #ifdef S4DEBUG
      l4check(l4) ;
   #endif

   return ;
}

void *S4FUNCTION l4pop( L4LIST *l4 )
{
   L4LINK *p ;

   p = l4->last ;
   l4remove( l4, l4->last ) ;
   return p ;
}

#ifdef S4DEBUG
void l4check( L4LIST *l4 )
{
   /* Check the Linked List */
   L4LINK *on_link ;
   int i ;

   on_link =  l4->last ;
   if ( on_link == 0 )
   {
      if ( l4->n_link != 0 )  e4severe( e4info, "l4check() 1", E4_INFO_CRL, (char *) 0) ;
      return ;
   }

   for ( i = 1; i <= l4->n_link; i++ )
   {
      if ( on_link->n->p != on_link  ||  on_link->p->n != on_link )
	 e4severe( e4info, "l4check() 2", E4_INFO_CRL, (char *) 0 ) ;

      on_link =  on_link->n ;

      if ( i == l4->n_link || on_link == l4->last )
         if ( i != l4->n_link || on_link != l4->last )
	    e4severe( e4info, "l4check", E4_INFO_WRO, (char *) 0 ) ;
   }
}
#endif
