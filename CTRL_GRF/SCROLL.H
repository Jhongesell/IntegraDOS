#ifdef ForPascal
   #include "global.h"
#endif


typedef struct _Rectangle
      {
         void far *addr;
         int       width,  /* in bytes */
                   height;
         //
         void far* (huge pascal *XYoffsetProc) ( struct _Rectangle  *R_ptr, int x, int y );
      } Rectangle;


#ifdef __cplusplus
extern "C" {
#endif

int far pascal ScrollGraph( int graphdriver, int graphmode,
                            int x1,int y1, int x2,int y2, int w,int h);
void far pascal MoveRectangle (
                                Rectangle *R1, int x1, int y1,
                                Rectangle *R2, int x2, int y2,
                                int w, int h );
int far pascal SetRvalues( int graphdriver, int graphmode,
                           Rectangle *R,short *PixelsInByte);


void far* huge pascal XY_Hercules ( Rectangle *R, int x, int y );
void far* huge pascal XY_CGA ( Rectangle *R, int x, int y );
void far* huge pascal XY_EGA ( Rectangle *R, int x, int y );
void far* huge pascal XY_bitLinear ( Rectangle *R, int x, int y );

#ifdef __cplusplus
   }
#endif
