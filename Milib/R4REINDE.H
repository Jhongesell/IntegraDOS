/* r4reindex.h   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

typedef struct
{           
   /* must be before n_keys  */
   #ifdef S4CLIPPER                     
      short  *block_index ;
      char   *data;
   #endif

   #ifdef S4FOX
      B4STD_HEADER header ;
   #else
      short  n_keys ;
   #endif

   #ifdef S4MDX
      char   dummy[6] ;
      char   info[2] ;
   #endif

   #ifdef S4NDX
      char   dummy[2] ;
      char   info;
   #endif
} R4BLOCK_DATA ;

typedef struct  /* R4REINDEX  For 'i4reindex' */
{
   #ifndef N4OTHER
      T4TAG S4PTR *tag ;
      I4INDEX      S4PTR *index ;
      int           n_tags ;
   #endif

   #ifdef S4FOX
      char      *cur_pos ;
      B4NODE_HEADER node_hdr ;
      int        n_blocks_used ;
   #else
      #ifdef S4CLIPPER
         B4KEY_DATA *stranded ;
         int  lastblock_inc ;        /* How much to increment 'lastblock' by */
      #else
         int  lastblock_inc ;        /* How much to increment 'lastblock' by */
         unsigned  blocklen ;
      #endif
   #endif

   int  grouplen ;             /* Entire key length */
   int  valuelen ;             /* Length of sorted portion of key */
   int  keysmax ;
   long lastblock ;            /* The block last written to */
   long  key_count ;

   S4SORT        sort ;

   H4SEQ_WRITE   seqwrite ;    
   char         S4PTR *buffer ;      /* Buffer for 'seqwrite' and 'seqread' */
   int           buffer_len ;

   R4BLOCK_DATA S4PTR *start_block ;
   int           n_blocks ;
   int           min_keysmax;  /* To calculate the number of blocks */

   D4DATA       S4PTR *data ;
   C4CODE       S4PTR *code_base ;
} R4REINDEX ;

#ifdef __cplusplus
   extern "C" {
#endif

void r4reindex_free( R4REINDEX S4PTR * ) ;

#ifdef N4OTHER
   int r4reindex_tag_headers_calc( R4REINDEX S4PTR *, T4TAG S4PTR * ) ;
   int r4reindex_supply_keys( R4REINDEX S4PTR *, T4TAG S4PTR * ) ;
   int r4reindex_write_keys( R4REINDEX S4PTR *, T4TAG S4PTR * ) ;
   int r4reindex_tag_headers_write( R4REINDEX S4PTR *, T4TAG S4PTR * ) ;
   int r4reindex_init( R4REINDEX S4PTR *, T4TAG S4PTR * ) ;
#else
   int r4reindex_tag_headers_calc( R4REINDEX S4PTR * ) ;
   int r4reindex_supply_keys( R4REINDEX S4PTR * ) ;
   int r4reindex_write_keys( R4REINDEX S4PTR * ) ;
   int r4reindex_tag_headers_write( R4REINDEX S4PTR * ) ;
   int r4reindex_init( R4REINDEX S4PTR *, I4INDEX S4PTR * ) ;
#endif
int r4reindex_blocks_alloc( R4REINDEX S4PTR * ) ;
T4TAG S4PTR *r4reindex_find_i_tag( R4REINDEX S4PTR *, int ) ;

/* Write key functions */
#ifdef S4FOX
   int r4reindex_add( R4REINDEX S4PTR *, long, char S4PTR *, char S4PTR *, int S4PTR * ) ;
   int r4reindex_finish( R4REINDEX S4PTR *, char S4PTR * ) ;
   int r4reindex_todisk( R4REINDEX S4PTR *, char S4PTR * ) ;
#else
   int r4reindex_add( R4REINDEX S4PTR *, long, char S4PTR * ) ;
   int r4reindex_finish( R4REINDEX S4PTR * ) ;
   #ifdef S4CLIPPER
      int r4reindex_todisk( R4REINDEX S4PTR *, long, char S4PTR * ) ;
   #else
      int r4reindex_todisk( R4REINDEX S4PTR * ) ;
   #endif
#endif

#ifdef __cplusplus
   }
#endif
