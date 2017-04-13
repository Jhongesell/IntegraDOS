#ifndef __BGI2_H
#define __BGI2_H

#include <DIR.H>



#define  PRT_DATA            /* far */
#define  PRT__FAR_PTR        far
#define  PRT_FUNC            huge pascal
#if ForPascal
    #define  PRT_CFUNC        far pascal
    #define  PRT__HUGE_PROC   far
#else
   #if __HUGE__ || PRT_building_library
      #define  PRT_CFUNC        huge
      #define  PRT__HUGE_PROC   huge
   #else
      #define  PRT_CFUNC        far
      #define  PRT__HUGE_PROC   huge
   #endif
#endif



#ifndef _PRT__pascal
   #define   _PRT__pascal
#endif
#define MaxBGIDriver  16 // <<=== must be the same value as in GLOBAL.pas file }
typedef void pascal (far *BGIaddressT)(void);
typedef struct {
					  unsigned userRegistered; }
        BGIstatusT;
#define Scrn_BGIgroup 1
#define Prt_BGIgroup  2

#ifdef __cplusplus
extern  "C" {
#endif
   void far* PRT_FUNC BGI_ReadDriver( const char far*name, const char far* bgiPath );
   void far* PRT_FUNC BGI_LoadDriver( int driverNo, const char far* bgiPath );
   int PRT_FUNC BGI_installuserdriver( const char far *filename,
                                         int huge (*detect)(void),
                                         int BGIgroup );
   //int PRT_FUNC BGI_suspend(int far *BDriver);
   //int PRT_FUNC BGI_Resume(int far *BDriver);
   int PRT_FUNC BGI_ActiveDriverNo(void);
   #define Scrn_BGIgroup 1
   #define Prt_BGIgroup  2
   int PRT_FUNC BGI_initgraph( int far *BDriver, int far *aMode,
                                 const char far *aBGIPath, int BGIgroup );
   int PRT_FUNC BGI_closegraph(void);
   int PRT_FUNC BGI_registerfarbgidriver(void far pascal (*driver)(void) );
   int PRT_FUNC BGI_getgraphmode( int BGIgroup );
   void PRT_FUNC BGI_setgraphmode(int mode);
	const char far* PRT_FUNC BGI_getmodename(int mode);
   int PRT_FUNC BGI_getresolution(int far *x, int far*y);

#ifdef __cplusplus
}
#endif

#endif // __BGI2_H
