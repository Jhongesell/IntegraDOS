/* c4window.h  Header file for CodeWindows  (c)Copyright Sequiter Software Inc., 1991.  All rights reserved. */

#define OEMRESOURCE           /* added for LoadBitmap call, combo_arrow ... */

#include <windows.h>
#include "d4all.h"

#define c4control    0 
#define c4list       1 
#define c4edit       2 
#define c4combo      4 
#define c4button     8 
#define c4display   16 

#define c4return    13
#define c4cancel    27

#define c4entrywin   0 
#define c4browsewin  1 

#define e4cw       -1 

#define s4max_position  30000 
#define s4max_pix       30000

#define  m4blank         1
#define  m4copy	         2
#define  m4delete        3
#define  m4undelete      4
#define  m4pack	         5
#define  m4find          6
#define  m4select        7
#define  m4bottom        8
#define  m4top	         9
#define  m4go           10 
#define  m4help	        11
#define  m4reindex      12 
#define  m4down         13
#define  m4up           14
#define  m4page_up      15
#define  m4page_down    16
#define  m4return       17
#define  m4init         18
#define  m4left 	19
#define  m4right        20
#define  m4page_left    21
#define  m4page_right   22

#define  d4go_new      104
#define  d4go_current  105
#define  d4go_count    106
#define  d4go_ok       107
#define  d4go_cancel   108

#define  m4find_tag    103
#define  m4find_value  104

#define  WM_USER_CLOSE  WM_USER+1

struct B4BROWSE_st ;
struct G4BUTTON_st ;
struct G4EDIT_st   ;
struct G4COMBO_st  ;
struct W4ENTRY_st  ;

typedef long (FAR PASCAL *C4WNDPROC)( unsigned, unsigned, unsigned, long ) ;

typedef int S4CALL S4BUTTON  ( struct G4BUTTON_st * ) ;
typedef int S4CALL S4VALID   ( struct G4EDIT_st *, void * ) ;
typedef int S4CALL S4RECORD  ( struct W4ENTRY_st *, void * ) ;
typedef int S4CALL S4DELETE  ( struct W4ENTRY_st *, int, void * ) ;
typedef int S4CALL S4FORMULA ( struct W4ENTRY_st *, char *, void * ) ;

#ifdef S4DLL
   #define g4button( p1, p2, p3, p4, p5, p6, p7, p8, p9 )  g4button_work( p1, p2, p3, p4, p5, p6, p7, (S4BUTTON *) MakeProcInstance((FARPROC) p8, p1->gw.cw->cb.hInst), p9 )
   #define g4valid( p1, p2, p3 )  g4valid_work( p1, (S4VALID *) MakeProcInstance((FARPROC) p2, p1->ca.gw.cw->cb.hInst), p3 )
   #define b4record_function( p1, p2, p3 ) b4record_function_work( p1, (S4RECORD *) MakeProcInstance((FARPROC) p2, p1->gw.cw->cb.hInst), p3 )
   #define b4delete_function( p1, p2, p3 ) b4delete_function_work( p1, (S4DELETE *) MakeProcInstance((FARPROC) p2, p1->gw.cw->cb.hInst), p3 ) 
   #define g4button_function( p1, p2, p3 ) g4button_function_work( p1, (S4BUTTON *) MakeProcInstance((FARPROC) p2, p1->ca.gw.cw->cb.hInst), p3 )
#else
   #define g4button( p1, p2, p3, p4, p5, p6, p7, p8, p9 )  g4button_work( p1, p2, p3, p4, p5, p6, p7, p8, p9 )
   #define g4valid( p1, p2, p3 )  g4valid_work( p1, p2, p3 )
   #define b4record_function( p1, p2, p3 ) b4record_function_work( p1, p2, p3 )
   #define b4delete_function( p1, p2, p3 ) b4delete_function_work( p1, p2, p3 )
   #define g4button_function( p1, p2, p3 ) g4button_function_work( p1, p2, p3 )
#endif

typedef struct
{
   C4CODE cb ;
   int    TypeOver ;
   int    bad_date ;

   FARPROC button4_proc ;
   FARPROC combo4_proc ;
   FARPROC list4_proc ;

   HBITMAP combo_arrow ;
} C4WINDOWS ;

typedef struct
{
   C4WINDOWS S4PTR *cw ;
   HWND      hWindow ;
   int       Id ;          /* ID of Child Control Window */
   long      Style ;
   FARPROC   OldWindowProc ;
   int       IsDestroyed ;
   int       hEnable ;
} W4GENERAL ;

typedef struct W4ENTRY_st
{
   W4GENERAL gw ;
   L4LIST    ControlList ;

   int       Pos ;             /* Current Caret Position; -1 Unknown */
   int       return_code ;     /* 0 - External; VK_ESCAPE, VK_RETURN */
   int       max_id ;          /* next available id for child */
   int       isReady ;         /* =0 not ready to paint, =1 ready to paint */

   int       hasMessageLoop ;  /* =1 if g4read called, =0 if not */

   struct B4BROWSE_st S4PTR *bw ;
} W4ENTRY ;

typedef struct
{
   L4LINK    link ;
   W4GENERAL gw ;

   int   is_default ;  /* for defpushbutton style ... */
   int   area_type ;   /* c4button, c4list, c4edit, c4combo */
   void  S4PTR *area ;        /* G4BUTTON *, G4LIST *, G4EDIT *, G4COMBO * */

   HWND     hParent ;
   W4ENTRY  S4PTR *ew ;
} G4CONTROL ;

typedef struct
{
   G4CONTROL   ca ;

   char        S4PTR *pStr ;
   char        S4PTR *pResult ;
   int         pResultLen ;
   C4WINDOWS   S4PTR *cw ;
   char        S4PTR *select ;      /* The string to use as the selected entry */

   struct G4COMBO_st S4PTR *combo ;          /* !=0 for combobox, =0 other */
} G4LIST ;

typedef struct G4EDIT_st
{
   G4CONTROL ca ;

   F4FIELD   S4PTR *data_field ; /* =0 if using a buffer */
   int       edit_changed ; /* =0 if contents unchanged, =1 if changed */

   char      S4PTR *Buffer ;
   char      S4PTR *BufferPrev ;
   int       nChars ;
   int       Decimals ;
   int       EntryType ;  /* t4str, t4log, t4num_str, t4date_str */
   int       Margin ;     /* Logical x caret position when at pos zero. */
   int       MarginTotal ;
   int       MaxLen ;     /* The maximum # of characters to enter */
   int       isModify ;   /* T/F "Can modify?" flag */
   int       Offset ;
   char      S4PTR *Pict ;
   int       PictLen ;
   int       Pos ;
   char      S4PTR *pResult ;
   int       pResultLen ;
   int       SelectAnchor ;  /* -1 - None; else, starting selection position */
   int       isUpper ;       /* 1 uppercase */

   int       width ;
   int       y ;
   int       x ;

   S4VALID  *valid ;
   void      S4PTR *valid_data ;

   struct G4COMBO_st S4PTR *combo ;    /* =0 not part of combobox */
} G4EDIT ;

typedef struct G4BUTTON_st
{
   G4CONTROL ca ;

   int ButtonType ;
   char *isChecked ;

   S4BUTTON *routine ;
   void      S4PTR *routine_data ;

   struct G4COMBO_st S4PTR *combo ;
} G4BUTTON ;

typedef struct G4COMBO_st
{
   G4EDIT     edit ;        /* took out pointer ... */
   G4LIST     S4PTR *list ;
   G4BUTTON   S4PTR *button ;

   int is_down ;
   int isDrop ;                   /* Is it a drop down combo box */

   int x ;
   int y ;
   int width ;
   int height ;
} G4COMBO ;

typedef struct
{
   G4CONTROL ca ;

   char S4PTR *text ;
   unsigned text_len ;

   int x ;
   int y ;
   int width ;
} G4DISPLAY ;

typedef struct B4BROWSE_st
{
   W4ENTRY entry ;
   L4LIST  BrowseList ;

   D4DATA  S4PTR *pData ;
   long    CurRec ;       /* The record number at position 'CurPos' */
   int     CurPos ;

   int     Appending ;    /* Edit controls contain append data. */
      	                  /* In this case, CurRec is 'DataFile->reccount()+1' */
   int     do_browse ;    /* =1 if BrowseWindow */

   int     Pixels ;       /* Number of pixels in each line */
   int     PixelsExtra ;  /* The # of extra pixels before & after an EditBox */
   int     MarginTop ;    /* For Edit Controls */
   int     HeightEdit ;   /* Height of Edit Control */

   int     MaxWidth ;     /* both used for scroll bar stuff */
   int     MaxHeight ;    
   int     MaxPos ;

   S4RECORD *record ;
   void      S4PTR *record_data ;
   S4DELETE *del ;
   void      S4PTR *del_data ;
} B4BROWSE ;

typedef struct
{
   L4LINK    link ;
   W4GENERAL gw ;

   B4BROWSE  S4PTR *bw ;
   G4EDIT    S4PTR *ea ;
   G4DISPLAY S4PTR *sa ;

   int LeftPos ;       /* Left Position of Edit Control */
   int WidthBrowse ;   /* Width of Browse Window */
   int WidthControl ;  /* Control Width for Moving Window */
   int UpdateStart, UpdateEnd ;
} B4AREA ;

typedef struct
{
   LONG  Style ;
   BYTE  ItemCount ;
   short X, Y, CX, CY ;
} C4DIALOGINFO ;

typedef struct
{
   short  X, Y, CX, CY ;
   short  Id ;             /* Control ID */
   LONG   Style ;
} C4CONTROLITEM ;

typedef struct
{
   C4WINDOWS S4PTR *cw ;
   HDC hDC ;
} P4PRINT ;

long FAR PASCAL _export browse4_area_proc ( HWND, unsigned, WORD, LONG ) ;
long FAR PASCAL _export browse4_proc      ( HWND, unsigned, WORD, LONG ) ;
long FAR PASCAL _export button4_proc      ( HWND, unsigned, WORD, LONG ) ;
long FAR PASCAL _export edit4_proc        ( HWND, unsigned, WORD, LONG ) ;
long FAR PASCAL _export entry4_proc       ( HWND, unsigned, WORD, LONG ) ;
long FAR PASCAL _export list4_proc        ( HWND, unsigned, WORD, LONG ) ;
long FAR PASCAL _export combo4_proc       ( HWND, unsigned, WORD, LONG ) ;

void browse4_area_create          ( W4ENTRY S4PTR * ) ;
int  browse4_area_process_message ( B4AREA S4PTR *, WORD, WORD, LONG ) ;
void browse4_area_updateposcalc   ( B4AREA S4PTR * ) ;
int  browse4_process_message      ( B4BROWSE S4PTR *, WORD, WORD, LONG ) ;
int  browse4_commandprepare       ( B4BROWSE S4PTR * ) ;
int  browse4_current              ( B4BROWSE S4PTR *, G4EDIT S4PTR * ) ;
int  browse4_display              ( B4BROWSE S4PTR * ) ;
int  browse4_display_pos          ( B4BROWSE S4PTR *, int, int, HBRUSH ) ;
void browse4_find                 ( B4BROWSE S4PTR * ) ;
void browse4_go                   ( B4BROWSE S4PTR * ) ;
int  browse4_horzthumb            ( B4BROWSE S4PTR *, int ) ;
int  browse4_movecontrols         ( B4BROWSE S4PTR *, int ) ;
int  browse4_newposition          ( B4BROWSE S4PTR *, int ) ;
int  browse4_newposrec            ( B4BROWSE S4PTR *, int, long ) ;
int  browse4_npositions           ( B4BROWSE S4PTR * ) ;
void browse4_pack                 ( B4BROWSE S4PTR * ) ;
int  browse4_process_key          ( B4BROWSE S4PTR *, WORD, WORD, LONG ) ;
int  browse4_refresh              ( B4BROWSE S4PTR * ) ;
int  browse4_refreshhorzscrollpos ( B4BROWSE S4PTR * ) ;
int  browse4_refreshscrollpos     ( B4BROWSE S4PTR * ) ;
void browse4_reindex              ( B4BROWSE S4PTR * ) ;
int  browse4_resized              ( B4BROWSE S4PTR * ) ;
int  browse4_scrollleft           ( B4BROWSE S4PTR * ) ;
int  browse4_scrollpageleft       ( B4BROWSE S4PTR * ) ;
int  browse4_scrollpageright      ( B4BROWSE S4PTR * ) ;
int  browse4_scrollright          ( B4BROWSE S4PTR * ) ;
void browse4_scrollset            ( B4BROWSE S4PTR * ) ;
void browse4_select               ( B4BROWSE S4PTR * ) ;
int  browse4_skip                 ( B4BROWSE S4PTR *, int ) ;

int S4CALL button4_cancelproc      ( G4BUTTON S4PTR * ) ;
void       button4_construct       ( G4BUTTON S4PTR *, W4ENTRY S4PTR * ) ;
G4BUTTON S4PTR *button4_create     ( W4ENTRY S4PTR *, C4CONTROLITEM S4PTR *, char S4PTR *, int, int ) ;
void       button4_free            ( G4BUTTON S4PTR * ) ;
int S4CALL button4_okproc          ( G4BUTTON S4PTR * ) ;
int        button4_process_message ( G4BUTTON S4PTR *, WORD, WORD, LONG ) ;
int        button4_save            ( G4BUTTON S4PTR * ) ;

void       combo4_buttondrawbitmap ( HDC, HBITMAP, short, short ) ;
void       combo4_buttondraw       ( LPDRAWITEMSTRUCT, W4ENTRY S4PTR * ) ;
int S4CALL combo4_buttonproc       ( G4BUTTON S4PTR * ) ;
int        combo4_combosize        ( W4ENTRY S4PTR * ) ;
G4COMBO S4PTR *combo4_create       ( W4ENTRY S4PTR *, C4CONTROLITEM S4PTR *, char S4PTR *, unsigned, int, int ) ;
void       combo4_free             ( G4COMBO S4PTR *  ) ;
int        combo4_process_message  ( G4COMBO S4PTR *, WORD, WORD, LONG ) ;
int        combo4_selected         ( G4COMBO S4PTR *, int ) ;
void       combo4_setdrop          ( G4COMBO S4PTR *, int ) ;
void       combo4_setselection     ( HWND, W4ENTRY S4PTR * ) ;

int control4_construct       ( G4CONTROL S4PTR *, W4ENTRY S4PTR * ) ;
int control4_process_message ( G4CONTROL S4PTR *, WORD, WORD, LONG ) ;

G4DISPLAY *display4_construct ( W4ENTRY S4PTR * ) ;
G4DISPLAY *display4_create    ( W4ENTRY S4PTR *, C4CONTROLITEM S4PTR *, char S4PTR *, int, int, int ) ;

int        edit4_calcpos          ( G4EDIT S4PTR *, int ) ;
void       edit4_caretcreate      ( G4EDIT S4PTR * ) ;
void       edit4_caretmove        ( G4EDIT S4PTR * ) ;
G4EDIT S4PTR *edit4_construct     ( W4ENTRY S4PTR * ) ;
G4EDIT S4PTR *edit4_create        ( W4ENTRY S4PTR *, C4CONTROLITEM S4PTR *, char S4PTR *, int, int ) ;
int        edit4_datecheck        ( G4EDIT S4PTR * ) ;   
int        edit4_delete           ( G4EDIT S4PTR *, int, int ) ;
void       edit4_free             ( G4EDIT S4PTR *   ) ;
void       edit4_init             ( G4EDIT S4PTR *, int, int, char S4PTR *, int, int, 
                                                                   int ) ;
int        edit4_insertchar       ( G4EDIT S4PTR *, char, int ) ;
int        edit4_insertstr        ( G4EDIT S4PTR *, char S4PTR *, int ) ;
int        edit4_insertsaved      ( G4EDIT S4PTR * ) ;
void       edit4_make             ( W4ENTRY S4PTR *, G4EDIT S4PTR * ) ;
void       edit4_make2            ( W4ENTRY S4PTR *, G4EDIT S4PTR *, int, int, char S4PTR *,
                                                     int, int, int, int ) ;
G4CONTROL S4PTR *edit4_next       ( L4LIST S4PTR *, void S4PTR * ) ;
int        edit4_nonpictchars     ( char S4PTR * ) ;
void       edit4_packnumeric      ( G4EDIT S4PTR * ) ;
int        edit4_paint            ( G4EDIT S4PTR * ) ;
int        edit4_pictureapply     ( G4EDIT S4PTR * ) ;
int        edit4_picturecheck     ( G4EDIT S4PTR *, int, char S4PTR * ) ;
int        edit4_pictureverify    ( G4EDIT S4PTR * ) ;
int        edit4_posget           ( G4EDIT S4PTR * ) ;
void       edit4_posset           ( G4EDIT S4PTR *, int, int ) ;
G4CONTROL S4PTR *edit4_prev       ( L4LIST S4PTR *, void S4PTR * ) ;
int        edit4_process_message  ( G4EDIT S4PTR *, WORD, WORD, LONG ) ;
int        edit4_refresh          ( G4EDIT S4PTR * ) ;
int        edit4_replace          ( G4EDIT S4PTR *, int, int ) ;
int        edit4_save             ( G4EDIT S4PTR * ) ;
void       edit4_selectall        ( G4EDIT S4PTR * ) ;
int        edit4_selectblanks     ( G4EDIT S4PTR *, int, int ) ;
int        edit4_selectdelete     ( G4EDIT S4PTR * ) ;
int        edit4_selectextraword  ( G4EDIT S4PTR *, int ) ;
int        edit4_selectnone       ( G4EDIT S4PTR * ) ;
int        edit4_selectposlow     ( G4EDIT S4PTR * ) ;
int        edit4_selectlen        ( G4EDIT S4PTR * ) ;
int        edit4_selectsave       ( G4EDIT S4PTR * ) ;
int        edit4_selectto         ( G4EDIT S4PTR *, int ) ;
int        edit4_selectusingmouse ( G4EDIT S4PTR *, int ) ;
void       edit4_setmodify        ( G4EDIT S4PTR *, int ) ;
void       edit4_textout          ( G4EDIT S4PTR *, HDC, int, int, char S4PTR *, int ) ;
int        edit4_typeoverflip     ( G4EDIT S4PTR * ) ;
int        edit4_undo             ( G4EDIT S4PTR * ) ;

int     entry4_construct        ( W4ENTRY S4PTR * ) ;
int     entry4_controlsrefresh  ( W4ENTRY S4PTR * ) ;
int     entry4_controlssave     ( W4ENTRY S4PTR * ) ;
void    entry4_free             ( W4ENTRY S4PTR *  ) ;
G4EDIT S4PTR *entry4_get_create ( W4ENTRY S4PTR *, int, int, char S4PTR *, int, int, int, int ) ;
int     entry4_init             ( W4ENTRY S4PTR *, C4WINDOWS S4PTR *, char S4PTR *, char S4PTR *,
                                  DWORD, int, int, int, int, HWND, HMENU ) ;
void S4PTR *entry4_init_dialog  ( C4WINDOWS S4PTR *, HWND, char S4PTR *, int, int ) ;
int     entry4_nextid           ( W4ENTRY S4PTR * ) ;
int     entry4_process_message  ( W4ENTRY S4PTR *, WORD, WORD, LONG ) ;
int     entry4_readstring       ( char S4PTR *, H4SEQ_READ S4PTR *, int ) ;
int     entry4_resizeedits      ( W4ENTRY S4PTR * ) ;

W4GENERAL S4PTR *general4_create ( C4WINDOWS S4PTR *, char S4PTR *, char S4PTR *, DWORD, int, int, int,
				   	              int, HWND, HMENU ) ;
void       general4_init   ( W4GENERAL S4PTR *, C4WINDOWS S4PTR *, char S4PTR *, char S4PTR *,
                             DWORD, int, int, int, int, HWND, HMENU ) ;
void       general4_read   ( W4GENERAL S4PTR *, HWND ) ;

G4LIST S4PTR *list4_create    ( W4ENTRY S4PTR *, C4CONTROLITEM S4PTR *, char S4PTR *, int, int ) ;
void    list4_free            ( G4LIST S4PTR *   ) ;
int     list4_process_message ( G4LIST S4PTR *, WORD, WORD, LONG ) ;
int     list4_save            ( G4LIST S4PTR * ) ;
int     list4_setselect       ( W4ENTRY S4PTR * ) ;
void    list4_setselection    ( HWND, W4ENTRY S4PTR * ) ;

W4ENTRY S4PTR *S4FUNCTION b4create          ( C4WINDOWS S4PTR *, char S4PTR *, DWORD, int, int, int, int,
					      HWND, D4DATA S4PTR *, int ) ;
W4ENTRY S4PTR *S4FUNCTION b4create_dialog   ( C4WINDOWS S4PTR *, HWND, char S4PTR *, D4DATA S4PTR *, int ) ;
void     S4FUNCTION b4delete_function_work  ( W4ENTRY S4PTR *, S4DELETE *, void S4PTR * ) ;
void     S4FUNCTION b4record_function_work  ( W4ENTRY S4PTR *, S4RECORD *, void S4PTR * ) ;

G4BUTTON  S4PTR *S4FUNCTION g4button_work   ( W4ENTRY S4PTR *, long, int, int, int, int, char S4PTR *,
						     S4BUTTON *, void S4PTR * ) ;
G4BUTTON  S4PTR *S4FUNCTION g4button_cancel ( W4ENTRY S4PTR *, int, int ) ;
void S4FUNCTION g4button_checked            ( G4BUTTON S4PTR *, char S4PTR * ) ;
void      S4PTR *S4FUNCTION g4button_data   ( G4BUTTON S4PTR * ) ;
void      S4FUNCTION g4button_function_work ( G4BUTTON S4PTR *, S4BUTTON *, void S4PTR * ) ;
G4BUTTON  S4PTR *S4FUNCTION g4button_ok     ( W4ENTRY S4PTR *, int, int ) ;
C4CODE    S4PTR *S4FUNCTION g4cb            ( void S4PTR * ) ;
G4COMBO   S4PTR *S4FUNCTION g4combo         ( W4ENTRY S4PTR *, int, int, int, int, char S4PTR *,
					                       unsigned, int ) ;
void       S4FUNCTION g4combo_add         ( G4COMBO S4PTR *, char S4PTR *, int ) ;
void       S4FUNCTION g4combo_simple      ( G4COMBO S4PTR * ) ;
G4EDIT    S4PTR *S4FUNCTION g4date        ( W4ENTRY S4PTR *, int, int, char S4PTR * ) ;
G4DISPLAY S4PTR *S4FUNCTION g4display     ( W4ENTRY S4PTR *, int, int, char S4PTR * ) ;
void       S4FUNCTION g4display_buffer    ( G4DISPLAY S4PTR *, char S4PTR *, unsigned ) ;
G4DISPLAY S4PTR *S4FUNCTION g4display_len ( W4ENTRY S4PTR *, int, int, char S4PTR *, int ) ;
void       S4FUNCTION g4display_update    ( G4DISPLAY S4PTR * ) ;
G4EDIT    S4PTR *S4FUNCTION g4field       ( W4ENTRY S4PTR *, int, int, F4FIELD S4PTR * ) ;
void       S4FUNCTION g4field_id          ( W4ENTRY S4PTR *ew, int id, F4FIELD S4PTR *fld ) ;
long       S4FUNCTION g4get_style         ( void S4PTR * ) ;
HWND       S4FUNCTION g4handle            ( void S4PTR * ) ;
HANDLE     S4FUNCTION g4hInst             ( void S4PTR * ) ;
int        S4FUNCTION g4id                ( void S4PTR * ) ;
G4LIST    S4PTR *S4FUNCTION g4list        ( W4ENTRY S4PTR *, int, int, int, int, char S4PTR *, unsigned,
					    		               int ) ;
void       S4FUNCTION g4list_add          ( G4LIST S4PTR *, char S4PTR *, int ) ;
void       S4FUNCTION g4list_buffer       ( G4LIST S4PTR *,  char S4PTR *, unsigned ) ;
G4EDIT    S4PTR *S4FUNCTION g4logical         ( W4ENTRY S4PTR *, int, int, char S4PTR * ) ;
G4CONTROL S4PTR *S4FUNCTION g4lookup          ( W4ENTRY S4PTR *, int ) ;
G4BUTTON  S4PTR *S4FUNCTION g4lookup_button   ( W4ENTRY S4PTR *, int ) ;
G4COMBO   S4PTR *S4FUNCTION g4lookup_combo    ( W4ENTRY S4PTR *, int ) ;
G4DISPLAY S4PTR *S4FUNCTION g4lookup_display  ( W4ENTRY S4PTR *, int ) ;
G4EDIT    S4PTR *S4FUNCTION g4lookup_edit     ( W4ENTRY S4PTR *, int ) ;
G4LIST    S4PTR *S4FUNCTION g4lookup_list     ( W4ENTRY S4PTR *, int ) ;
void       S4FUNCTION g4modify          ( G4EDIT S4PTR *, int ) ;
G4EDIT    S4PTR *S4FUNCTION g4numeric   ( W4ENTRY S4PTR *, int, int, char S4PTR *, int, int ) ;
void       S4FUNCTION g4picture         ( G4EDIT S4PTR *, char S4PTR * ) ;
int        S4FUNCTION g4read            ( W4ENTRY S4PTR *, HWND ) ;
void       S4FUNCTION g4set_buffer      ( G4EDIT S4PTR *, char S4PTR *, unsigned ) ;
void       S4FUNCTION g4set_field       ( G4EDIT S4PTR *, F4FIELD S4PTR * ) ;
void       S4FUNCTION g4set_style       ( void S4PTR *, long ) ;
G4EDIT    S4PTR *S4FUNCTION g4text      ( W4ENTRY S4PTR *, int, int, char S4PTR *, unsigned ) ;
void       S4FUNCTION g4upper           ( G4EDIT S4PTR * ) ;
void       S4FUNCTION g4valid_work      ( G4EDIT S4PTR *, S4VALID *, void S4PTR * ) ;
int        S4FUNCTION g4width           ( G4EDIT S4PTR *, int ) ;

void  S4FUNCTION p4buffer   ( P4PRINT S4PTR *, int, int, char S4PTR *, int ) ;
void  S4FUNCTION p4eject    ( P4PRINT S4PTR * ) ;
int   S4FUNCTION p4font_set ( P4PRINT S4PTR *, HFONT ) ;
int   S4FUNCTION p4free     ( P4PRINT S4PTR * ) ;
int   S4FUNCTION p4get_dc   ( P4PRINT S4PTR * ) ;
int   S4FUNCTION p4init     ( P4PRINT S4PTR *, C4WINDOWS S4PTR *, char S4PTR * ) ;
int   S4FUNCTION p4lines    ( P4PRINT S4PTR * ) ;
void  S4FUNCTION p4ptr      ( P4PRINT S4PTR *, int, int, char S4PTR * ) ;
void  S4FUNCTION p4tty      ( P4PRINT S4PTR * ) ;

C4CODE    S4PTR *S4FUNCTION w4cb      ( W4ENTRY S4PTR * ) ;
int        S4FUNCTION w4char_height   ( W4ENTRY S4PTR * ) ;
int        S4FUNCTION w4char_width    ( W4ENTRY S4PTR * ) ;
W4ENTRY   S4PTR *S4FUNCTION w4create  ( C4WINDOWS S4PTR *, char S4PTR *, DWORD, int, int,
	 		                int, int, HWND, HMENU ) ;
W4ENTRY   S4PTR *S4FUNCTION w4create_dialog ( C4WINDOWS S4PTR *, HWND, char S4PTR * ) ;
W4ENTRY   S4PTR *S4FUNCTION w4entry   ( void S4PTR * ) ;
HWND       S4FUNCTION w4handle        ( W4ENTRY S4PTR * ) ;
HANDLE     S4FUNCTION w4hInst         ( void S4PTR * ) ;
C4WINDOWS S4PTR *S4FUNCTION w4init    ( HANDLE, HANDLE ) ;
void       S4FUNCTION w4init_undo     ( C4WINDOWS S4PTR * ) ;
