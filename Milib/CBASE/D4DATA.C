/* d4data.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

char *S4FUNCTION d4alias( D4DATA *data )
{
   return data->alias ;
}

void S4FUNCTION d4alias_set( D4DATA *data, char *new_alias )
{
   c4upper( new_alias ) ;
   u4ncpy( data->alias, new_alias, sizeof(data->alias) ) ;
}

int S4FUNCTION d4append_data( D4DATA *d4 )
{
   long count, pos ;
   int  rc ;

   count =  d4reccount(d4) ;  if ( count < 0L )  return -1 ;
   d4->file_changed =  1 ;
   pos =  d4rec_pos( d4, count+1L ) ;
   d4->record[d4->record_width] = 0x1A ;

   rc =  h4write( &d4->file, pos, d4->record, (d4->record_width+1) ) ;
   if ( rc >= 0 )
   {
      d4->rec_num =  count + 1L ;
      d4->record_changed =  0 ;
      if ( d4lock_test_append(d4) )
      d4->header.num_recs =  count + 1L ;
   }

   d4->record[d4->record_width] =  0 ;

   return rc ;
}

int S4FUNCTION d4append_blank( D4DATA *d4 )
{
   int rc ; 

   rc =  d4flush_record(d4) ;    if ( rc != 0 )  return rc ;
   rc =  d4append_start(d4,0) ;  if ( rc != 0 )  return rc ;

   memset( d4->record, ' ', d4->record_width ) ;
   return d4append(d4) ;
}

void S4FUNCTION d4blank( D4DATA *d4 )
{
   if ( d4 == 0 )  return ;
   memset( d4->record, ' ', d4->record_width ) ;
   d4->record_changed =  1 ;
}

int S4FUNCTION d4bof( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 )  return -1 ;
   return d4->bof_flag ;
}

int S4FUNCTION d4bottom( D4DATA *d4 )
{
   T4TAG *tag ;
   long rec ;
   int  rc ;

   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 )  return -1 ;

   tag =  d4tag_selected(d4) ;
   if ( tag == 0 )
   {
      rec =  d4reccount(d4) ;
      if ( rec > 0L )
         return d4go(d4, rec) ;
   }
   else
   {
      if (d4flush_record( d4 ) < 0) return -1 ; 
      if ( (rc = t4bottom(tag)) != 0 )  return rc ;
      if ( ! t4eof(tag) )
         return d4go(d4, t4recno(tag) ) ;
   }

   d4->bof_flag =  1 ;
   return d4go_eof(d4) ;
}

void S4FUNCTION d4delete( D4DATA *d4 )
{
   if ( d4 == 0 )  return ;
   if ( *d4->record != '*' )
   {
      *d4->record = '*' ;
      d4->record_changed =  1 ;
   }
}

int S4FUNCTION d4deleted( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;
   return *d4->record != ' ' ;
}

int S4FUNCTION d4eof( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 )  return -1 ;
   return d4->eof_flag ;
}

int S4FUNCTION d4free_blocks( D4DATA *d4 )
{
   T4TAG *tag_on ;

   for( tag_on = 0; tag_on = (T4TAG *) d4tag_next(d4, tag_on); )
      if ( t4free_all(tag_on) < 0 )  return -1 ;

   return 0 ;
}

I4INDEX * S4FUNCTION d4index( D4DATA *d4, char *index_name )
{
   char index_lookup[258], current[258] ;
   I4INDEX *index_on ;

   if ( d4 == 0 )  return 0 ;

   u4name_piece( index_lookup, sizeof(index_lookup), index_name, 1, 0 ) ;
   c4upper( index_lookup ) ;

   for( index_on = 0; index_on = (I4INDEX *) l4next( &d4->indexes, index_on); )
   {
      #ifdef N4OTHER
         u4name_piece( current, sizeof(current), index_on->alias, 1, 0 ) ;
      #else
         u4name_piece( current, sizeof(current), index_on->file.name, 1, 0 ) ;
      #endif
      c4upper( current ) ;
      if ( strcmp( current, index_lookup) == 0 )    /* check out d4->alias ? */
         return index_on ;
   }

   return 0 ;
}

int S4FUNCTION d4num_fields( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 ) return -1 ;
   return d4->n_fields ;
}

int S4FUNCTION d4read( D4DATA *d4, long rec, char *ptr )
{
   unsigned len ;

   len =  d4->record_width ;

   len =  h4read( &d4->file, d4rec_pos(d4,rec), ptr, len ) ;
   if ( d4->code_base->error_code < 0 )  return -1 ;

   if ( len != d4->record_width )   return r4entry ;
   return 0 ;
}

int S4FUNCTION d4read_old( D4DATA *d4, long rec )
{
   int rc ;

   if ( rec <= 0 )
   {
      d4->rec_num_old =  rec ;
      memset( d4->record_old, ' ', d4->record_width ) ;
   }
   if ( d4->rec_num_old == rec )  return 0 ;

   d4->rec_num_old =  -1 ;
   rc =  d4read( d4, rec, d4->record_old) ;
   if ( rc < 0 )  return -1 ;
   if ( rc > 0 )
      memset( d4->record_old, ' ', d4->record_width ) ;
   d4->rec_num_old =  rec ;

   return 0 ;
}

void S4FUNCTION d4recall( D4DATA *d4 )
{
   if ( d4 == 0 )  return ;

   if ( *d4->record != ' ' )
   {
      *d4->record = ' ' ;
      d4->record_changed =  1 ;
   }
}

long S4FUNCTION d4reccount( D4DATA *d4 )
{
   long count ;

   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 ) return -1 ;
   if ( d4->header.num_recs >= 0L )  return d4->header.num_recs ;

   count =  h4length( &d4->file ) ;
/*   count-- ; */
   if ( count < 0L )  return -1L ;
   count =  (count-d4->header.header_len)/ d4->record_width ;
   if ( d4lock_test_append(d4) )
      d4->header.num_recs =  count ;

   return count ;
}

long S4FUNCTION d4recno( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;
   return d4->rec_num ;
}

char *S4FUNCTION d4record( D4DATA *d4 )
{
   if ( d4 == 0 )  return 0 ;
   return d4->record ;
}

long S4FUNCTION d4record_width( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1L ;
   return (long) d4->record_width ;
}

long S4FUNCTION d4rec_pos( D4DATA *d4, long rec )
{
   return d4->header.header_len + d4->record_width * (rec - 1) ;
}

int S4FUNCTION d4reindex( D4DATA *d4 )
{
   I4INDEX *index_on ;
   int rc ;

   if ( (rc = d4lock_file(d4)) != 0 )  return rc ;
   if ( (rc = d4lock_index(d4)) != 0 )  return rc ;

   for ( index_on = 0; index_on = (I4INDEX *) l4next(&d4->indexes, index_on);)
      if ( i4reindex(index_on) < 0 )  return -1 ;

   return 0 ;
}

int S4FUNCTION d4top( D4DATA *d4 )
{
   T4TAG *tag ;
   int rc, save_flag ;
   C4CODE *c4 ;

   if ( d4 == 0 )  return -1 ;

   c4 =  d4->code_base ;
   if ( c4->error_code < 0 )  return -1 ;

   tag =  d4tag_selected(d4) ;

   if ( tag == 0 )
   {
      save_flag =  c4->go_error ;
      c4->go_error =  0 ;
      rc = d4go(d4,1L) ;
      c4->go_error =  save_flag ;
      if ( rc <= 0 )  return rc ;

      if ( d4reccount(d4) != 0L )
         return d4go(d4,1L) ;
   }
   else
   {
      if (d4flush_record( d4 ) < 0) return -1 ; 
      if ( (rc = t4top(tag)) < 0 )  return -1 ;
      if ( ! t4eof(tag) )
         return d4go(d4, t4recno(tag)) ;
   }

   d4->bof_flag =  1 ;
   return d4go_eof(d4) ;
}

int S4FUNCTION d4update_header( D4DATA *d4, int do_time_stamp, int do_count )
{
   long pos ;
   unsigned len ;

   if ( d4 == 0 )  return -1 ;

   pos = 0L ;
   len = sizeof(d4->header) ;
   if ( do_time_stamp )
      u4yymmdd( &d4->header.yy ) ;
   else
   {
      pos += 4 ;
      len -= 4 ;
   }

   if ( do_count )
       d4reccount(d4) ;
   else
       len -= (sizeof(d4->header.num_recs) +sizeof(d4->header.header_len)) ;
   if ( h4write( &d4->file, pos, (char *)&d4->header + pos,len) < 0 )
      return -1 ;
   d4->file_changed =  0 ;
   return 0 ;
}
