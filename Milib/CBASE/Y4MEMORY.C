/* y4memory.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef S4DO_PRINT
   int v4print =  1 ;
#endif

#ifdef S4WINDOWS
   #include <windows.h>
#endif

#define y4num_types 10

typedef struct
{
   L4LINK  link ;
   Y4MEMORY_TYPE  types[y4num_types] ;
} Y4MEMORY_GROUP ; 

static L4LIST  avail;   /* A list of available Y4MEMORY_TYPE entries */
static L4LIST  used ;   /* A list of used Y4MEMORY_TYPE entries */
static L4LIST  groups ; /* A list of Allocated Y4MEMORY_TYPE groups */

#ifdef S4DEBUG

static char **y4test_pointers ;
static int    y4num_pointer =  -1 ;
static int    y4num_used =  0 ;

#define y4extra_chars 10
#define y4extra_tot   (y4extra_chars*2 + sizeof(unsigned))
#define y4check_char  0x55

/* Returns the pointer to be returned; is passed the pointer allocated by malloc ... */
static char *y4fix_pointer( char *start_ptr, unsigned large_len )
{
   char *return_ptr ;
   unsigned pos ;

   memset( start_ptr, y4check_char, y4extra_chars ) ;
   return_ptr =  start_ptr + y4extra_chars ;

   memcpy( return_ptr, &large_len, sizeof(large_len) ) ;
   pos =  large_len-y4extra_chars ;
   memset( start_ptr+ pos, y4check_char, y4extra_chars ) ;

   return return_ptr + sizeof(unsigned) ;
}

/* Returns the pointer allocated by malloc; */
/* passed by pointer returned by 'y4fix_pointer' */
static char *y4check_pointer( char *return_ptr )
{
   unsigned pos, *large_len_ptr ;
   char *malloc_ptr, *test_ptr ;
   int i, j ;

   large_len_ptr =  (unsigned *) (return_ptr - sizeof(unsigned)) ;
   malloc_ptr =  return_ptr - sizeof(unsigned) - y4extra_chars ;

   for ( j =0; j < 2; j++ )
   {
      if (j == 0)
         test_ptr =  malloc_ptr ;
      else
      {
	 pos =  *large_len_ptr - y4extra_chars ;
         test_ptr =  malloc_ptr + pos ;
      }

      for ( i=0; i<y4extra_chars; i++ )
         if ( test_ptr[i] != y4check_char )
          e4severe( e4result, return_ptr, (char *) 0 ) ;
/*        e4severe( e4result, E4_RESULT_COM, (char *) 0 ) ; */
   }
   return malloc_ptr ;
}

static void y4push_pointer( char *ptr )
{
   #ifdef S4WINDOWS
      HANDLE  handle, *h_ptr, *old_h_ptr ;
      h_ptr =  (HANDLE *) 0 ;
   #endif

   if ( y4num_pointer < 0 )
   {
      #ifdef S4WINDOWS
         #ifdef S4MEDIUM
            handle =  LocalAlloc( LMEM_FIXED | LMEM_ZEROINIT, (WORD) sizeof(char *) * 100 + sizeof(HANDLE) ) ;

            if ( handle == (HANDLE) 0 )
               e4severe( e4memory, E4_MEMORY_YPU, (char *) 0 ) ;

            h_ptr =  (HANDLE *) LocalLock( handle ) ;
         #else
            #ifdef __DLL__
               handle =  GlobalAlloc( GMEM_FIXED | GMEM_DDESHARE | GMEM_ZEROINIT, (DWORD) sizeof(char *) * 100 + sizeof(HANDLE) ) ;
            #else
               handle =  GlobalAlloc( GMEM_FIXED | GMEM_ZEROINIT, (DWORD) sizeof(char *) * 100 + sizeof(HANDLE) ) ;
            #endif

            if ( handle == (HANDLE) 0 )
               e4severe( e4memory, E4_MEMORY_YPU, (char *) 0 ) ;

            h_ptr =  (HANDLE *) GlobalLock( handle ) ;
         #endif
         *h_ptr++ =  handle ;
         y4test_pointers = (char **) h_ptr ;
      #else
         y4test_pointers =  (char **) malloc( sizeof(char *) * 100 ) ;
      #endif
      y4num_pointer =  100 ;
   }
   if ( y4num_pointer == y4num_used )
   {
      y4num_pointer += 100 ;
      if ( y4num_pointer > 10000 )
         e4severe( e4result, E4_MEMORY_YPU, E4_RESULT_TOO, (char *) 0 ) ;

      #ifdef S4WINDOWS
         old_h_ptr =  (HANDLE *) (y4test_pointers) ;
         old_h_ptr-- ;  /* get the actual handle */

         #ifdef S4MEDIUM
            handle =  LocalReAlloc( *old_h_ptr, (WORD) sizeof(char *) * y4num_pointer + sizeof(HANDLE), LMEM_MOVEABLE ) ;

            if ( handle == (HANDLE) 0 )
               e4severe( e4memory, E4_MEMORY_YPU, (char *) 0 ) ;

            h_ptr =  (HANDLE *) LocalLock( handle ) ;
         #else
            #ifdef __DLL__
               handle =  GlobalReAlloc( *old_h_ptr, (DWORD) sizeof(char *) * y4num_pointer + sizeof(HANDLE), GMEM_MOVEABLE ) ;
            #else
               handle =  GlobalReAlloc( *old_h_ptr, (DWORD) sizeof(char *) * y4num_pointer + sizeof(HANDLE), GMEM_MOVEABLE ) ;
            #endif

            if ( handle == (HANDLE) 0 )
               e4severe( e4memory, E4_MEMORY_YPU, (char *) 0 ) ;

            h_ptr =  (HANDLE *) GlobalLock( handle ) ;
         #endif
         *h_ptr++ =  handle ;
         y4test_pointers = (char **) h_ptr ;
      #else
         y4test_pointers = (char **) realloc( y4test_pointers, sizeof(char *) *y4num_pointer ) ;
      #endif
   }

   if ( y4test_pointers == 0 )
      e4severe( e4memory, E4_MEMORY_YPU, (char *) 0 ) ;

   y4test_pointers[y4num_used++] =  ptr ;
}

static void y4pop_pointer( char *ptr )
{
   int i ;

   for ( i=y4num_used-1; i>=0; i-- )
      if ( y4test_pointers[i] == ptr )
      {
         /* This 'memmove' may create compile warning */
         memmove( y4test_pointers+i, y4test_pointers+i+1, (size_t) (sizeof(char *) * (y4num_used-i-1))) ;
         y4num_used-- ;
         return ;
      }

   e4severe( e4result, E4_MEMORY_YPO, E4_RESULT_FRE, (char *) 0 ) ;
}

void S4FUNCTION y4check_memory()
{
   int i ;

   for ( i = 0; i < y4num_used; i++ )
      y4check_pointer( y4test_pointers[i] ) ;
}

int S4FUNCTION y4free_check( int max_left )
{
   #ifdef S4DO_PRINT
      int i ;
      if ( v4print )
         for ( i = 0; i < y4num_used; i++ )
	    fprintf( v4print, "\r\ny4free_check: %p", y4test_pointers[i] ) ;
   #endif

   if ( y4num_used > max_left )
      e4severe( e4result, "y4free_check()", E4_RESULT_FRE, (char *) 0 ) ;

   return ( y4num_used ) ;
}
#endif

Y4MEMORY_TYPE *S4FUNCTION y4memory_type( int start, unsigned unit_size, int expand, int is_temp)
{
   Y4MEMORY_TYPE *on_type ;

   #ifdef S4DEBUG
      unit_size += 2*y4extra_chars + sizeof(unsigned) ;
   #endif

   if ( ! is_temp )
      for( on_type = 0; on_type =  (Y4MEMORY_TYPE *) l4next(&used,on_type); )
      {
         if ( on_type->unit_size == unit_size  &&  on_type->n_repeat > 0 )
         {
            /* Match */
            if ( start > on_type->unit_start )
               on_type->unit_start =  start ;
            if ( expand> on_type->unit_expand)
               on_type->unit_expand=  expand ;
            on_type->n_repeat++ ;
            return on_type ;
         }
      }

   /* Allocate memory for another Y4MEMORY_TYPE */

   on_type =  (Y4MEMORY_TYPE *) l4last( &avail ) ;
   if ( on_type == 0 )
   {
      Y4MEMORY_GROUP *group ;
      int i ;

      group = (Y4MEMORY_GROUP *) u4alloc(sizeof(Y4MEMORY_GROUP)) ;
      if ( group == 0 )  return 0 ;

      for ( i = 0; i<y4num_types; i++ )  l4add( &avail, group->types+i ) ;
      on_type =  (Y4MEMORY_TYPE *) l4last( &avail ) ;
      l4add( &groups, group ) ;
   }

   l4remove( &avail, on_type ) ;
   memset( (void *)on_type, 0, sizeof(Y4MEMORY_TYPE) ) ;
   l4add( &used, on_type ) ;

   on_type->unit_start =  start ;
   on_type->unit_size  =  unit_size ;
   on_type->unit_expand=  expand ;
   on_type->n_repeat =  1 ;
   on_type->n_used   =  0 ;
   if ( is_temp )  on_type->n_repeat =  -1 ;

   #ifdef S4DO_PRINT
      if ( v4print )
	 fprintf( v4print, "\r\n    Y4MEMORY_TYPE: %p", on_type ) ;
   #endif

   return on_type ;
}

void *S4FUNCTION y4memory_type_alloc( Y4MEMORY_TYPE **type_ptr_ptr,
           int start, unsigned unit_size, int expand, int is_temp)
{
   if ( *type_ptr_ptr == 0 )
   {
      *type_ptr_ptr =  y4memory_type( start, unit_size, expand, is_temp ) ;
      if ( *type_ptr_ptr == 0 )  return 0 ;
   }

   return y4alloc( *type_ptr_ptr ) ;
}

void S4FUNCTION y4release( Y4MEMORY_TYPE *memory_type )
{
   void *ptr ;

   if ( memory_type == 0 )  return ;

   memory_type->n_repeat-- ;
   if ( memory_type->n_repeat <= 0 )
   {
      for( ; ptr = l4pop( &memory_type->chunks); )
	 u4free( ptr ) ;

      l4remove( &used, memory_type ) ;
      l4add( &avail, memory_type ) ;

    #ifdef S4DO_PRINT 
	 if ( v4print ) 
	    fprintf( v4print, "\r\n    Y4RELEASE: %p", memory_type ) ; 
    #endif 
   }
}

static void *y4alloc_low( Y4MEMORY_TYPE *memory_type )
{
   L4LINK *next_piece ;
   Y4CHUNK *new_chunk ;
   #ifdef S4DEBUG
      char *ptr ;
   #endif

   if ( memory_type == 0 )  return 0 ;
   next_piece =  (L4LINK *) l4pop( &memory_type->pieces ) ;

   if ( next_piece != 0 )
   {
      #ifdef S4DEBUG
         memory_type->n_used++ ;
         ptr =  y4fix_pointer( (char *) next_piece, memory_type->unit_size ) ;

	 #ifdef S4DO_PRINT
	    if ( v4print )
	       fprintf(v4print, "\r\n  Y4ALLOC:  %p", ptr);
	 #endif

         y4push_pointer( ptr ) ;
         return ptr ;
      #else
         return next_piece ;
      #endif
   }

   if ( (new_chunk =  y4alloc_chunk( memory_type )) == 0 )  return 0 ;
   l4add( &memory_type->chunks, &new_chunk->link ) ;

   memory_type->n_used++ ;
   #ifdef S4DEBUG
      ptr =  y4fix_pointer( (char *) l4pop(&memory_type->pieces),
			    memory_type->unit_size ) ;
      #ifdef S4DO_PRINT
	 if ( v4print )
	    fprintf(v4print, "\r\n  Y4ALLOC:  %p", ptr);
      #endif

      y4push_pointer( ptr ) ;
      return ptr ;
   #else
      return l4pop( &memory_type->pieces ) ;
   #endif
}

void *S4FUNCTION y4alloc( Y4MEMORY_TYPE *memory_type )
{
   void *ptr ;
   ptr =  y4alloc_low( memory_type ) ;
   if ( ptr != 0 )
      #ifdef S4DEBUG
	 memset( ptr, 0, memory_type->unit_size - y4extra_tot ) ;
      #else
         memset( ptr, 0, memory_type->unit_size ) ;
      #endif
   return ptr ;
}

void S4FUNCTION y4free( Y4MEMORY_TYPE *memory_type, void *free_ptr )
{
   memory_type->n_used-- ;
   #ifdef S4DEBUG
      if ( memory_type->n_used < 0 )  e4severe( e4result, "y4free()", (char *) 0 ) ;

    #ifdef S4DO_PRINT 
	 if ( v4print ) 
	    fprintf(v4print, "\r\n  Y4FREE:  %p", free_ptr ) ; 
    #endif 

      y4pop_pointer( (char *) free_ptr ) ;
      l4add( &memory_type->pieces, (L4LINK *) y4check_pointer( (char *) free_ptr )   ) ;
      memset( &free_ptr, 0, sizeof(free_ptr) ) ;
   #else
      l4add( &memory_type->pieces, (L4LINK *) free_ptr ) ;
   #endif
}

Y4CHUNK *S4FUNCTION y4alloc_chunk( Y4MEMORY_TYPE *type_ptr )
{
   Y4CHUNK *chunk_ptr ;
   int  n_allocate, i ;
   char *ptr ;

   n_allocate = type_ptr->unit_expand ;
   if ( l4last( &type_ptr->chunks ) == 0 )  n_allocate =  type_ptr->unit_start ;

   chunk_ptr =  (Y4CHUNK *)
      u4alloc( sizeof(L4LINK)+ (long)n_allocate*type_ptr->unit_size ) ;
   if ( chunk_ptr == 0 )  return 0 ;
   ptr =  (char *) &chunk_ptr->data ;
   for ( i = 0; i < n_allocate; i++ )
      l4add( &type_ptr->pieces, (L4LINK *) (ptr+i*type_ptr->unit_size) ) ;

   return  chunk_ptr ;
}

#ifdef S4MAX
   long  y4max_memory =  0x4000 ;
   long  u4allocated  =  0L ;
   #ifndef S4DEBUG
      S4DEBUG should be set with S4MAX (force compile error.)
   #endif
#endif

void * S4FUNCTION u4alloc( long n )
{
   size_t s ;
   char *ptr ;

   #ifdef S4DEBUG
      if ( n == 0L )  e4severe( e4parm, "u4alloc()", E4_PARM_ZER, (char *) 0 ) ;
      n += y4extra_chars*2 + sizeof(unsigned) ;
   #endif

   #ifdef S4MAX
      /* Assumes 'y4max_memory' is less than the actual maximum */
      if ( u4allocated + n >  y4max_memory )  return 0 ;
      u4allocated +=  n ;
   #endif

   s = (size_t) n ;
   if ( n > (long) s )  return 0 ;

   #ifdef S4WINDOWS
   {
      HANDLE  handle, *h_ptr ;
      h_ptr =  (HANDLE *) 0 ;

      #ifdef S4MEDIUM
	 #ifdef __DLL__
	    Error, cannot create a Medium memory model DLL with Borland C++ 2.0
	 #endif
	 handle =  LocalAlloc( LMEM_FIXED | LMEM_ZEROINIT, (WORD) s+ sizeof(HANDLE) ) ;

	 if ( handle == (HANDLE) 0 )  return 0 ;

	 h_ptr =  (HANDLE *) LocalLock( handle ) ;
      #else
	 #ifdef __DLL__
	    handle =  GlobalAlloc( GMEM_FIXED | GMEM_DDESHARE | GMEM_ZEROINIT, (DWORD) s+ sizeof(HANDLE) ) ;
	 #else
	    handle =  GlobalAlloc( GMEM_FIXED | GMEM_ZEROINIT, (DWORD) s+ sizeof(HANDLE) ) ;
	 #endif

      if ( handle == (HANDLE) 0 )  return 0 ;

      h_ptr =  (HANDLE *) GlobalLock( handle ) ;
      #endif
      *h_ptr++ =  handle ;
      ptr = (char *) h_ptr ;
   }
   #else
      ptr =  (char *) malloc( s ) ;

      if ( ptr == 0 )  return 0 ;
      #ifndef S4DEBUG
         memset( ptr, 0, s ) ;
      #endif
   #endif

   #ifdef S4DEBUG
      ptr =  y4fix_pointer( ptr, s ) ;
      y4push_pointer( ptr ) ;
      memset( ptr, 0, s-y4extra_chars*2 - sizeof(unsigned) ) ;
   #endif

   #ifdef S4DO_PRINT
      if ( v4print )
	 fprintf(v4print, "\r\nU4ALLOC:  %p   # bytes alloc: %ld", ptr, n);
   #endif

   return (char *) ptr ;
}

void S4FUNCTION u4free( void *ptr )
{
   #ifdef S4WINDOWS
      HANDLE  hand ;
   #endif
   #ifdef S4MAX
      unsigned *amount ;
   #endif

   if ( ptr == 0 ) return ;

   #ifdef S4MAX
      amount =  (unsigned *) ptr ;
      u4allocated -=  amount[-1] ;
   #endif

   #ifdef S4WINDOWS
      #ifdef S4DEBUG
         y4pop_pointer( (char *) ptr ) ;
	 hand =  ((HANDLE *) y4check_pointer( (char *) ptr ))[-1] ;
      #else
         hand =  ((HANDLE *) ptr)[-1] ;
      #endif

      #ifdef S4MEDIUM
         hand =  LocalFree( hand ) ;
      #else
         hand =  GlobalFree( hand ) ;
      #endif

      if ( hand != (HANDLE) 0 )
         e4severe( e4memory, E4_MEMORY_ERR, (char *) 0 ) ;
   #else
      #ifdef S4DEBUG
         y4pop_pointer( (char *) ptr ) ;

         #ifdef S4DO_PRINT 
            if ( v4print ) 
               fprintf(v4print, "\r\nU4FREE:  %p", ptr ); 
         #endif 

	 free(y4check_pointer( (char *) ptr )  ) ;
      #else
         free( ptr ) ;
      #endif
   #endif
}

void S4FUNCTION y4reset()
{
   Y4MEMORY_TYPE *on_type ;
   L4LINK *on_chunk, *on_group ;
   #ifdef S4LOCK_CHECK
      e4severe( e4result, E4_RESULT_S4L, (char *) 0 ) ;
   #endif

   for( on_type = 0; on_type = (Y4MEMORY_TYPE *) l4next(&used,on_type); )
   {
      while( on_chunk = (L4LINK *) l4pop( &on_type->chunks) ) 
	 u4free( on_chunk ) ;
   }

   while( on_group = (L4LINK *) l4pop(&groups) )
      u4free( on_group ) ;

   y4init() ;
}

void S4FUNCTION y4init()
{
   memset( (void *)&avail, 0, sizeof(avail) ) ;
   memset( (void *)&used,  0, sizeof(used) ) ;
   memset( (void *)&groups,0, sizeof(groups) ) ;
}
