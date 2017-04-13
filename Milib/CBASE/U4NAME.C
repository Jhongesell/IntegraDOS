/* u4name.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

void S4FUNCTION u4name_ext( char *name, int len_result, char *new_ext, int do_replace )
{
   int  file_name_len, ext_pos, on_pos, ext_len ;

   ext_pos =  file_name_len =  strlen(name) ;

   for( on_pos = ext_pos-1;; on_pos-- )
   {
      if ( name[on_pos] == '.' )
      {
	 ext_pos =  on_pos ;
	 break ;
      }
      if ( name[on_pos] == '\\' )  break ;
      if ( on_pos == 0 )  break ;
   }

   if ( file_name_len != ext_pos &&  !do_replace )
   {
      c4upper( name ) ;
      return ;
   }

   if ( *new_ext == '.' )  new_ext++ ;
   ext_len =  strlen(new_ext) ;

   if ( ext_len > 3 )  ext_len =  3 ;
   if ( len_result <= ext_pos + ext_len + 2 )
      e4severe( e4result, "u4name_ext()", (char *) 0 ) ;

    name[ext_pos++] = '.' ;
    strcpy( name+ ext_pos, new_ext ) ;

    c4upper(name) ;
}

void  S4FUNCTION u4name_piece( char *result, int len_result, char *from,
		  int give_path, int give_ext )
{
   unsigned name_pos, ext_pos, on_pos, pos, new_len, from_len ;
   int are_past_ext ;

   name_pos = 0 ;
   are_past_ext =  0 ;
   ext_pos =  from_len =  strlen(from) ;
   if ( ext_pos == 0 )
   {
      *result =  0 ;
      return ;
   }

   for( on_pos = ext_pos-1;; on_pos-- )
   {
      switch ( from[on_pos] )
      {
	 case '\\':
	 case ':':
	    if (name_pos == 0)  name_pos =  on_pos + 1 ;
	    are_past_ext =  1 ;
	    break ;

	 case '.':
	    if ( ! are_past_ext )
	    {
	       ext_pos =  on_pos ;
	       are_past_ext =  1 ;
	    }
	    break ;
      }

      if ( on_pos == 0 )  break ;
   }

   pos = 0 ;
   new_len =  from_len ;
   if ( ! give_path )
   {
      pos =  name_pos ;
      new_len -=  name_pos ;
   }

   if ( ! give_ext )
      new_len -=  from_len - ext_pos ;

   if ( new_len >= (unsigned) len_result )
      e4severe( e4result, "u4name_piece()", (char *) 0 ) ;

   memcpy( result, from+ pos, new_len ) ;
   result[new_len] =  0 ;
   c4upper(result) ;
}

/* u4name_char.c  Returns TRUE iff it is a valid dBase field or function name character */
int S4FUNCTION u4name_char( char ch)
{
   return ( ch>='a' && ch<='z'  ||
	    ch>='A' && ch<='Z'  ||
	    ch>='0' && ch<='9'  ||
	    ch=='\\'  ||  ch=='.'  || ch=='_'  ||  ch==':' ) ;
}

/* returns the length of the path in from, and copies the path in from to result */

#ifdef N4OTHER
int S4FUNCTION u4name_path( char *result, int len_result, char *from )
{
   int  file_name_len, ext_pos, on_pos, ext_len ;

   u4name_piece( result, len_result, from, 1, 0 ) ;
   for( on_pos = 0 ; result[on_pos] != 0 ; on_pos++ ) ;

   for( ; on_pos >= 0 ; on_pos-- )
      if( result[on_pos] == '\\' || result[on_pos] == '\:' ) break ;   /* end of path */

   if( on_pos < len_result )
   result[++on_pos] = '\0' ;
   return on_pos ;
}
#endif
