/* f4memo.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION m4assign( F4FIELD *f4, char *ptr )
{
   return m4assign_n( f4, ptr, (unsigned) strlen(ptr) ) ;
}

int S4FUNCTION m4assign_n( F4FIELD *f4, char *ptr, unsigned len )
{
   C4CODE *c4 ;
   F4MEMO *mfield ;
   int rc ;

   c4 =  f4->data->code_base ;
   if ( c4->error_code < 0 )  return -1 ;

   mfield =  f4->memo ;
   if ( mfield == 0 )
   {
      f4assign_n( f4, ptr, len ) ;
      return 0 ;
   }

   if ( (rc = m4set_len( f4, len )) != 0 )
      return  rc ;

   memcpy( mfield->contents, ptr, (size_t) len ) ;
   return 0 ;
}

int  S4FUNCTION m4flush( F4FIELD *f4 )
{
   int rc ;

   if ( f4->memo == 0 )
      return 0 ;

   if ( f4->memo->is_changed )
   {
      rc =  m4write( f4 ) ;
      if ( rc != 0 )  return rc ;
   }
   return 0 ;
}

void  S4FUNCTION m4free( F4FIELD *f4 )
{
   F4MEMO *mfield ;
   mfield =  f4->memo ;
   if ( mfield == 0 )  return ;

   if ( mfield->len_max > 0 )
      u4free( mfield->contents ) ;
   else
      memset( (void *)&mfield->contents, 0, sizeof(mfield->contents) ) ;

   mfield->status  =  1 ;
   mfield->len_max =  0 ;
   return ;
}

unsigned  S4FUNCTION m4len( F4FIELD *f4 )
{
   if ( f4->memo == 0 )
      return (unsigned) f4->len ;
     
   if ( f4->memo->status == 1 )
   {
      if ( m4read( f4 ) != 0 )  return 0 ;
      f4->memo->status =  0 ;
   }
   return f4->memo->len ;
}

unsigned S4FUNCTION m4ncpy( F4FIELD *f4, char *ptr, unsigned n )
{
   unsigned  num_cpy ;
   C4CODE   *c4 ;

   c4 = f4->data->code_base ;
   if ( c4->error_code < 0 )  return 0 ;
   c4->error_code =  0 ;

   if ( f4->memo == 0 )
      return f4ncpy( f4, ptr, n ) ;

   num_cpy =  m4len(f4) ;
   if ( n < num_cpy )  num_cpy =  n ;

   memcpy( ptr, m4ptr(f4), (size_t) num_cpy ) ;

   if ( num_cpy < n )  ptr[num_cpy] =  '\000' ;

   return( num_cpy ) ;
}

char *S4FUNCTION m4ptr( F4FIELD *f4 )
{
   if ( f4->memo == 0 )
      return f4ptr(f4) ;

   if ( f4->memo->status == 1 )
   {
      if ( m4read(f4) != 0 )  return 0 ;
      f4->memo->status =  0 ;
   }
   return f4->memo->contents ;
}

extern char m4null_char ;

int S4FUNCTION m4read( F4FIELD *f4 )
{
   int  rc ;
   F4MEMO *mfield ;

   mfield =  f4->memo ;
   mfield->is_changed =  0 ;

   if ( d4recno( f4->data ) < 0 )
   {
      mfield->len =  0 ;
      return mfield->len ;
   }

   rc =  d4validate_memo_ids( f4->data ) ;  if ( rc != 0 )  return rc ;
   rc =  m4read_low( f4 ) ;  if ( rc != 0 )  return -1 ;

   if ( mfield->len_max > 0 )
      mfield->contents[mfield->len] =  0 ;
   else
      mfield->contents =  &m4null_char ;

   return 0 ;
}

int S4FUNCTION m4read_low( F4FIELD *f4 )
{
   F4MEMO *mfield ;
   int rc ;

   mfield =  f4->memo ;
   mfield->len =  mfield->len_max ;
   rc = m4file_read( &f4->data->memo_file, f4long(f4),
		     &mfield->contents, &mfield->len ) ;
   if ( mfield->len > mfield->len_max )
      mfield->len_max =  mfield->len ;

   return rc ;
}

void S4FUNCTION m4reset( F4FIELD *f4 )
{
   f4->memo->len =  0 ;
   f4->memo->status =  1 ;
}

int S4FUNCTION m4set_len( F4FIELD *f4, unsigned len )
{
   F4MEMO *mfield ;

   mfield =  f4->memo ;
   if ( mfield == 0 )  return 0 ;

   if ( mfield->len_max <= len )
   {
      if ( mfield->len_max > 0 )
         u4free( mfield->contents ) ;
      mfield->len_max =  1 + len ;

      mfield->contents =  (char *) u4alloc( mfield->len_max ) ;

      if ( mfield->contents == 0 )
      {
         mfield->len_max =  0 ;
         mfield->status  =  1 ;
         return e4error( f4->data->code_base, e4memory, (char *) 0 ) ;
      }
   }

   mfield->len =  len ;
   mfield->status = 0 ;
   mfield->is_changed =  1 ;
   f4->data->record_changed =  1 ;

   return 0 ;
}

char *S4FUNCTION m4str( F4FIELD *f4 )
{
   if ( f4->memo == 0 )
      return f4str(f4) ;

   return m4ptr(f4) ;
}

int S4FUNCTION m4write( F4FIELD *f4 )
{
   int rc ;
   long memo_id, new_id ;

   rc = d4validate_memo_ids( f4->data ) ;  if ( rc != 0 )  return rc ;
   memo_id =  f4long( f4 ) ;
   new_id  =  memo_id ;

   rc =  m4file_write( &f4->data->memo_file, &new_id, f4->memo->contents, f4->memo->len ) ;
   if ( rc != 0 )  return rc ;

   if ( new_id != memo_id )
      f4assign_long( f4, new_id ) ;

   f4->memo->is_changed =  0 ;
   return  0 ;
}
