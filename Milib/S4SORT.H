/* s4sort.h   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */ 

typedef struct
{
   char  S4PTR *ptr ;       /* Pointer to the starting memory */
   unsigned  pos ;    /* The current position withing 'ptr' */
   unsigned  len ;    /* The current data length pointed to by 'ptr' */

   long   disk ;      /* Current Disk Position, Offset from start of 'spool' */
	              /* >= 'spool_bytes' means nothing is on disk. */
   int    spool_i ;   /* The spool number of this spool  */
} S4SPOOL ;

typedef struct
{
   C4CODE S4PTR *code_base ;

   H4FILE        file ;
   char          file_name_buf[14] ;
   H4SEQ_WRITE   seqwrite ;
   char         S4PTR *seqwrite_buffer ;
   S4SPOOL S4PTR *spool_pointer ;
   int      spools_n ;      /* The current # of "spools" */
   int      spools_max ;    /* The # of "spools" memory is allocated for */

   char S4PTR **pointers ;
   int   pointers_i ;    /* The next pointers to return using 'get'. */
   int   pointers_used ; /* The number of pointers assigned so far. */
   int   pointers_init ; /* The number of pointers initialied so far. */
   int   pointers_max ;  /* The number of pointers for which memory */
			 /* has been allocated. */

   L4LIST pool ;      /* A number of memory pools for sorting */
   Y4MEMORY_TYPE S4PTR *pool_memory ;
   int    pool_n ;        /* The number of pools */
   int    pool_entries ;  /* Number of record entries in each pool */

   int    sort_len ;

   long     spool_disk_len ; /* The # of bytes in each disk spool */
   unsigned spool_mem_len ;  /* The # of bytes in each memory spool */

   int    info_offset ;   /* The spot to put the corresponding info. */
   int    info_len ;
   int    tot_len ;       /* The total length of each pool entry. */

   long   is_mem_avail ;
   S4CMP_FUNCTION *cmp ;
} S4SORT ;

/* The 'len' is passed to the compare function as its third parameter */
/* The compare function returns '(int) 1' iff 'v1>v2'. */
/* s4quick also assumes that there is a record number after the sort data */

#ifdef __cplusplus
   extern "C" {
#endif

void  s4quick( void S4PTR **, int, S4CMP_FUNCTION *, int ) ;
void  s4delete_spool_entry( S4SORT S4PTR * ) ;
int   s4flush( S4SORT S4PTR * ) ;   /* Flushes current entries to disk */
int   s4get_ptr_ptr( S4SORT S4PTR *, char S4PTR ** ) ;
void  s4init_pointers( S4SORT S4PTR *, char S4PTR *, unsigned ) ;
long  s4needed(S4SORT S4PTR *) ; /* Returns an estimate on the amount of memory needed */
int   s4next_spool_entry(S4SORT S4PTR *) ;

int   S4FUNCTION s4free( S4SORT S4PTR * ) ;    /* Frees any allocated memory and uninitializes */
int   S4FUNCTION s4get( S4SORT S4PTR *,  long S4PTR *, void S4PTR **, void S4PTR ** ) ;
int   S4FUNCTION s4get_init( S4SORT S4PTR * ) ;
int   S4FUNCTION s4init( S4SORT S4PTR *, C4CODE S4PTR *, int, int ) ;
int   S4FUNCTION s4put( S4SORT S4PTR *, long, void S4PTR *, void S4PTR * ) ;

#ifdef __cplusplus
   }
#endif
