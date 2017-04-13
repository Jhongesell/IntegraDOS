/*
   Printer BGI driver C interface
   Copyright (c) 1991 A. Resztak
 */
 /*   Header file */

#ifndef __PRTGRAPH_H__
#define __PRTGRAPH_H__

#if   !defined(__LARGE__) && !defined(__HUGE__)
   #error PRTgraph library will run only in LARGE memory model.
#endif


#if   !defined(_PRT__pascal)
      #define  _PRT__pascal
#endif


 #define PRT_NO_MEMORY           1
 #define PRT_WRONG_PARAMETERS    2
 #define BGI_WRONG_PARAMETERS    2
 #define PRT_NOT_INITIALIZED     3
 #define PRT_IO_ERROR            4
 #define PRT_ERROR               5
 #define PRT_NO_FILE             6
 #define PRT_USER_INTERRUPT      7


 /*   drivers  */

 #define STAR_SR9    1     /* Epson ????, Star SR  */
 #define IBM9        2     /* IBM GraphPrinter, ProPrinter  */
 #define EPSON9      2     /* EPSON,CITIZEN ( graphic mode: Esc,'K'/'L'/'Y'/'Z' ) */
 #define EPSON9II    3     /* EPSON,CITIZEN ( graphic mode: Esc,'*',n )     */
 #define PANASONIC9  4     /* Panasonic KX-P1124 - not tested */
 #define IBM9c       5     /* IBM ProPrinter (color) */
 #define EPSON9c     5     /* EPSON (color) (graphic mode: Esc,'K'/'L'/'Y'/'Z') */
 #define EPSON9IIc   6     /* EPSON (color) - (graphic mode: Esc,'*',n) */
 #define EPSON24     7     /* 24 pins EPSON compatible printer */
 #define IBM24       8     /* 24 pins IBM compatible printer */
 #define EPSON24c    9     /* 24 pins EPSON compatible color printer */
                           /*  not tested */
 #define IBM24c     10     /* 24 pins IBM compatible color printer */
                           /*  not tested */
 #define PaintJet    11    /* general Paint Jet  */
 #define HPPaintJet  12    /* HP Paint Jet */
 #define HPLJ        13    /* Laser Jet    */
 #define HPLJII      14    /* HP Laser Jet */
 /* #define HPLJIII     12  */
 /* #define POSTSCRIPT  13  */
 /* #define ROLANDP  14     */

 /* modes for particular drivers */
   /* EPSON9   */
      #define  EPSON9_60x72      0
      #define  EPSON9_120x72     1
      #define  EPSON9_120x216    2
      #define  EPSON9_240x216    3
   /* STAR_SR9    */
      #define  STAR_SR9_60x72        0
      #define  STAR_SR9_120x72       1
      #define  STAR_SR9_120x144      2
      #define  STAR_SR9_240x144      3
   /* IBM9     */
      #define  IBM9_60x72        0
      #define  IBM9_120x72       1
      #define  IBM9_120x216      2
      #define  IBM9_240x216      3
   /* EPSON24  */
      #define  EPSON24_60x180     0
      #define  EPSON24_120x180    1
      #define  EPSON24_180x180    2
      #define  EPSON24_360x180    3
      #define  EPSON24_360x360    4
   /* IBM24    */
      #define  IBM24_60x180     0
      #define  IBM24_120x180    1
      #define  IBM24_180x180    2
      #define  IBM24_360x180    3
   /* PaintJet   */
      #define  PaintJet_90x90        0
      #define  PaintJet_180x180      1
      #define  PaintJet_90x90c       2
      #define  PaintJet_180x180c     3
   /* HPPJ   */
      #define  HPPJ_90x90        0
      #define  HPPJ_180x180      1
      #define  HPPJ_90x90c       2
      #define  HPPJ_180x180c     3
   /* HP LaserJet   */
      #define  HPLJ_75x75        0
      #define  HPLJ_100x100      1
      #define  HPLJ_150x150      2
      #define  HPLJ_300x300      3
   /* HPLJII   */
      #define  HPLJII_75x75        0
      #define  HPLJII_100x100      1
      #define  HPLJII_150x150      2
      #define  HPLJII_300x300      3



#include "BGI2.h"


typedef  unsigned char  byte;


#define PRT_HaltValue 100
extern volatile byte PRT_DATA pascal PRT_HaltPrinting;


 /*   F U N C T I O N S    P R O T O T Y P E S  */

#ifdef __cplusplus
extern "C" {
#endif

int  PRT_FUNC PRT_LinkDrivers(void);
int  PRT_FUNC PRT_ReadDrivers
     ( const char PRT__FAR_PTR* path, const char PRT__FAR_PTR* filename );
int PRT_FUNC PRT_End(void);

int PRT__HUGE_PROC pascal PRT_SetPrinterDrv ( unsigned drv, unsigned mode );
int PRT__HUGE_PROC pascal PRT_SetPictureInch ( unsigned width, unsigned height,
                                           unsigned options );
int PRT__HUGE_PROC pascal PRT_SetPicturePix ( unsigned hPix, unsigned vPix,
                                              unsigned options );
int PRT_FUNC PRT_SetDriver ( unsigned drv, unsigned mode,
                            unsigned width, unsigned height,
                            unsigned options );
   #define  PRT_NORMAL     0
   #define  PRT_ROTATE     1
   #define  PRT_INVERSE    2
   #define  PRT_REVERSE    2

int PRT_FUNC PRT_SetDriverFName ( const char PRT__FAR_PTR* fname );

int PRT_FUNC  PRT_FormFeedNeeded ( int ffneeded );

// #if ForPascal
   typedef int far *PRT__handleT; /* pointer to Pascal File */
// #else
//   typedef int   PRT__handleT;
// #endif

unsigned PRT_FUNC PRT_Version(void);


 /* Procedures which may be called before initializing PBGI */
int  PRT_FUNC PRT_Buffer ( void far *address, long size, int usable );
int  PRT_FUNC PRT_EMSBuffer ( int EMShandle, long size, int usable );
int  PRT_FUNC PRT_XMSBuffer ( int XMShandle, long size, int usable );
int   PRT_FUNC   PRT_SetBuffer ( long size, unsigned BufOpt );
      #define  NotUseEMS   1
      #define  NotUseXMS   2
 // int PRT_FUNC PRT_DefineWorld ( unsigned MaxX, unsigned MaxY );
 int  PRT_FUNC   PRT_SetOutName ( const char PRT__FAR_PTR * DeviceName );
 const char PRT__FAR_PTR *   PRT_FUNC   PRT_GetOutName ( void );
 int  PRT_FUNC   PRT_SetMargins ( int left, int top );
 int  PRT_FUNC PRT_RescaleFillPattern ( int r );
                        /* 0 = never, 1 = always , -1 = at high densities only */

 /* Procedure allocates PRT buffer, initializes PBGI, calls Draw(),
    closes PBGI, prints picture, and frees the buffer.
    All above is done neccesary number of times to print
    entire picture. */
int PRT_FUNC PRT_PrintBGI ( int far * graphdriver, int far *graphmode,
			                   const char far *pathtodriver,
                            int ( PRT_CFUNC *Draw)
                                  (void PRT__FAR_PTR * UserPointer),
                            void PRT__FAR_PTR* UserPointer );

       /* Various informing procedures */
   /* May be allways called   */
 int  PRT_FUNC   PRT_MaxDriver ( void );
 int  PRT_FUNC   PRT_DriverName ( unsigned driverno, const char PRT__FAR_PTR * PRT__FAR_PTR * name_ptr  );
 int  PRT_FUNC   PRT_MaxMode ( unsigned driverno, int PRT__FAR_PTR *maxmode );
 int  PRT_FUNC   PRT_ModeName ( unsigned driverno, int modeno,
                              const char PRT__FAR_PTR * PRT__FAR_PTR * name_ptr );
 int PRT_FUNC PRT_ModeParms ( unsigned drv, unsigned mode,
                              unsigned far *Xres, unsigned far *Yres,
                              unsigned far *colors );
 int  PRT_FUNC   PRT_DriverNo ( void );
 int  PRT_FUNC   PRT_ModeNo ( void );
 char far * PRT_FUNC   PRT_grapherrormsg ( int errorcode );

	/*  May be called only after PRT_SetDriver */
 long PRT_FUNC   PRT_BufferNeeded ( int x1, int y1, int x2, int y2 );
 long PRT_FUNC   PRT_EMSBufferNeeded ( int x1, int y1, int x2, int y2 );
 long PRT_FUNC   PRT_XMSBufferNeeded ( int x1, int y1, int x2, int y2 );
 int  PRT_FUNC   PRT_MaxFitArea ( int far *dxPtr, int far *dyPtr,
                                int far *adj_xPtr, int far * adj_yPtr );

 int  PRT_FUNC   PRT_Resolution ( int PRT__FAR_PTR *Xres, int PRT__FAR_PTR *Yres );


 /* Procedure initializes PBGI, calls Draw(), closes PBGI,
    DOES NOT print picture, and DOES NOT allocate or free the buffer */
int PRT_FUNC  PRT_BuildBitMap (
                              int far * graphdriver, int far *graphmode,
										const char far *pathtodriver,
                              int x1,int y1, int x2,int y2,
                              int ( PRT_CFUNC *Draw)
                                  (void PRT__FAR_PTR * UserPointer),
                              void PRT__FAR_PTR* UserPointer );

#if ForPascal
   int PRT_FUNC PRT_Send( const char far *s);
#else
   int PRT_FUNC PRT_Send( const char far *s, unsigned slen);
#endif

/* Procedures which may be called before freeing the buffer */
int PRT_FUNC PRT_InitPrt ( PRT__handleT handle );
int PRT_FUNC PRT_EndPrt ( PRT__handleT handle );
int PRT_FUNC  PRT_FirstPicPart ( void );
int PRT_FUNC  PRT_LastPicPart ( void );
int PRT_FUNC PRT_PrintBuffer (PRT__handleT *handlePPtr);
int PRT_FUNC   PRT_WritePCX ( PRT__handleT *handlePPtr );

unsigned PRT_FUNC  PRT_getpixel ( int x, int y );
void far * PRT_FUNC  PRT_getpixeladdress( int far* bitno, int Bufx, int Bufy,
                                          int planeno );
void  PRT_FUNC   setfillpattern16 ( char PRT__FAR_PTR * upattern, int color );
void  PRT_FUNC   getfillpattern16 ( char PRT__FAR_PTR * upattern );
void  PRT_FUNC   setcharsize_Inch (int width, int height);
      /* Sets char size in 1/1000 inch but is version dependent.
         Might cause link errors in versions other than BC++ 2.0 or 3.0 */
void  PRT_FUNC   setcharsize_Pix (int width, int height);

int   PRT_FUNC   PRT_AllocateBuffer ( void );
int   PRT_FUNC   PRT_FreeBuffer (void);
void  PRT_FUNC   PRT_SetHaltVariable(const byte far *haltVariable);


	int PRT_FUNC PRT_installuserdriver ( const char far * name, int huge (*detect)(void) );
   int PRT_FUNC PRT_registerfarbgidriver ( void far pascal (*driver)(void) );
   int PRT_FUNC PRT_Unregisterfarbgidriver ( void far pascal(*driver)(void) );
   int PRT_FUNC PRT_SetBuildSize( int x1, int y1, int x2, int y2 );
   int PRT_FUNC PRT_initgraph( int far * graphdriver, int far *graphmode,
													const char far *pathtodriver );
   int PRT_FUNC PRT_closegraph(void); /* DOES NOT free the buffer   */

   typedef int PRT_CFUNC ( * pascal PRT_ErrorFuncP) ( PRT__handleT handle );
   typedef PRT__handleT PRT_CFUNC ( * pascal PRT_OpenFuncP)
                                  ( const char far* outname, int omode );
   typedef int PRT_CFUNC ( * pascal PRT_CloseFuncP)( PRT__handleT handle );
   typedef int PRT_CFUNC ( * pascal PRT_WriteFuncP)( PRT__handleT handle, const void PRT__FAR_PTR * b,
                                                           unsigned len );
PRT_ErrorFuncP PRT_FUNC PRT_SetErrorFunc ( int           PRT_CFUNC WrErrFunc(PRT__handleT handle) );
PRT_OpenFuncP  PRT_FUNC PRT_SetOpenFunc  ( PRT__handleT  PRT_CFUNC
                                           f(const char far* outname,int omode) );
PRT_CloseFuncP PRT_FUNC PRT_SetCloseFunc ( int 		 PRT_CFUNC f(PRT__handleT handle) );
PRT_WriteFuncP PRT_FUNC PRT_SetWriteFunc ( int PRT_CFUNC
						                         f(PRT__handleT handle,
						                           const void PRT__FAR_PTR * b,
						                           unsigned len) );

typedef int _PRT__pascal PRT_UserPrintFuncT( void far* UserPtr,
					                              PRT__handleT   *handlePPtr,
                                             const char far* BGIpath);
PRT_UserPrintFuncT far * PRT_FUNC
	 PRT_SetUserPrintFunc(PRT_UserPrintFuncT far* func);

typedef int _PRT__pascal PRT_UserBeforeBuildFuncT( void far* UserPtr );
PRT_UserBeforeBuildFuncT* PRT_FUNC
        PRT_SetUserBeforeBuildFunc(PRT_UserBeforeBuildFuncT far* func);

int PRT_FUNC PRT_PreviewPossible(int driver);


int PRT_FUNC BGIactive ( int BGIgroup );
int PRT_FUNC CloseCallerBGI ( void );
int PRT_FUNC RestoreCallerBGI ( int far * graphdriver, int far *graphmode,
                              const char far *pathtodriver );


extern void far pascal BitImage(void);  /* link our bit image BGI driver */

#ifdef __cplusplus
   }
#endif


#ifdef __cplusplus
extern "C" {
#endif
int PRT_FUNC MoveToScrn ( int graphdriver, int graphmode,
                            int BufLeft, int BufTop,
                            int ScrnLeft, int ScrnTop,
                            int width, int height );
int PRT_FUNC PRT_MoveToScrn ( int BufLeft, int BufTop,
                              int ScrnLeft, int ScrnTop,
                              int width, int height );
int PRT_FUNC PRT_ScrollScrn
    (int *dxPtr, int *dyPtr );
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
      PRT__handleT PRT_CFUNC __PRT_Open(const char far* outname,int omode);
      int          PRT_CFUNC __PRT_Close( PRT__handleT handle );
      int          PRT_CFUNC __PRT_Write( PRT__handleT handle, const void PRT__FAR_PTR * b, unsigned len );
#ifdef __cplusplus
           }
#endif

#endif  // __PRTGRAPH_H__

   /* end of PRTGRAPH.H  */
