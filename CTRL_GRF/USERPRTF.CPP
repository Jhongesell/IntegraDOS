#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

#include "scroll.h"
#include "PRTgraph.h"


extern int ScreenPreview;
extern int PCXmode;

int _PRT__pascal StdUserPrtFunc( void far* UserPtr, PRT__handleT   *handlePPtr, const char far* BGIpath )
{  int  graphdriver=DETECT,graphmode=0;
   int  Scrn_x1,Scrn_y1,dx,dy,buf_x,buf_y,Scrn_width,Scrn_height;
   int  deltaX,deltaY;
   int  c;
   int  ReturnCode;
   struct viewporttype ts;
   int    BGI_inited=0;

   if(ScreenPreview && PRT_PreviewPossible(0) ) {
      if(!BGIactive(Scrn_BGIgroup) ) {
          BGI_initgraph(&graphdriver,&graphmode,BGIpath,Scrn_BGIgroup);
          if ( graphresult() ) exit(16);
           else BGI_inited=1;
      }

      Scrn_x1=0; Scrn_y1=0;
      Scrn_width=getmaxx()+1; Scrn_height=getmaxy()+1;
      getviewsettings(&ts);
      setviewport ( Scrn_x1,Scrn_y1,Scrn_x1+Scrn_width-1,Scrn_y1+Scrn_height-1,1);
      clearviewport();

      Scrn_x1=(Scrn_x1+15)&(-16);    Scrn_y1=(Scrn_y1+15)&(-16);
      if ( Scrn_x1+Scrn_width  > getmaxx()+1 ) Scrn_width= (getmaxx()-Scrn_x1+1);
      if ( Scrn_y1+Scrn_height > getmaxy()+1 ) Scrn_height=(getmaxy()-Scrn_y1+1);
      Scrn_width &= (-16);
      deltaX = (Scrn_width/5+15) & (-16);
      deltaY = (Scrn_height/5+15) & (-16);
      buf_x=buf_y=0;
      PRT_MoveToScrn (buf_x,buf_y, Scrn_x1,Scrn_y1,Scrn_width,Scrn_height);
      do {
         if (!kbhit()) { sound(650);delay(5);nosound(); }
         dx=dy=0;
         if ( (c=getch())==0)
         {
            switch (c=getch())
            {
               case 0x4b: dx=-deltaX; c=0;  break;  // left
               case 0x4d: dx=deltaX;  c=0;  break;  // right
               case 0x50: dy=deltaY;  c=0;  break;  // down
               case 0x48: dy=-deltaY; c=0;  break;  // up
               default: ;
            }
         }
         PRT_ScrollScrn ( &dx, &dy );
      } while (c==0);
   }

   if(c!=0x1b ) {
      if(PCXmode) ReturnCode =  PRT_WritePCX(handlePPtr);
       else ReturnCode =  PRT_PrintBuffer(handlePPtr);
   } else ReturnCode=PRT_USER_INTERRUPT;

   setviewport(ts.left,ts.top, ts.right,ts.bottom, ts.clip);

   if(BGI_inited) BGI_closegraph();
   return ReturnCode;
}


