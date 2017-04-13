#include "PrtGraph.h"

extern "C" {
   #include <stdio.h>
   #include <string.h>
   #include <conio.h>
   #include <dos.h>
   #include <Ctype.h>
   #include <graphics.h>
}


#ifndef  _HARDERR_IGNORE
   #define _HARDERR_IGNORE  0
   #define _HARDERR_RETRY   1
   #define _HARDERR_ABORT   2
   #define _HARDERR_FAIL    3
#endif

 UserResponce(const char *msg1, const char *msg2)
 {
    int retval;
    int c;
    do
    {
	    c = toupper(getch());
       if ( c!=0 )
          switch (c)
          {
             case 'A':  retval = _HARDERR_ABORT; break;
             case 'R':  retval = _HARDERR_RETRY; break;
             case 'I':  retval = _HARDERR_IGNORE; break;
             default: c=0;
          }
       else getch();
    } while ( c==0 );
    return(retval);
 }


 #pragma warn -par
 int harderr_handler(int errval,int ax,int bp,int si)
 {
    char msg1[20];
    char msg2[]="A)bort, R)etry, F)ail";
    int drive;
    int rc;
    if (ax < 0) {
       // not a disk
       _fstrcpy(msg1,"Error on ");
       _fstrncat(msg1,(const char far*)MK_FP(bp,si+0xa),8);
    }
    else
    {
       /* otherwise it was a disk error */
          drive = ax & 0x00FF;
          //errorno = di & 0x00FF;
          _fstrcpy(msg1,"Error on drive x:");
	       msg1[15] = 'A' + drive;
    }
    rc = UserResponce(msg1,msg2);
    if (rc==_HARDERR_IGNORE) hardretn(rc);
    if (rc==_HARDERR_ABORT)
    {
       if ( BGIactive(0) ) closegraph();
    }
    hardresume(rc);
    return _HARDERR_ABORT;
 }
 #pragma warn +par


#pragma option -r-
#pragma option -N-
#ifdef __cplusplus
   void interrupt CtrlBreak_handler(...)
#else
   void interrupt CtrlBreak_handler(void)
#endif
{
   PRT_HaltPrinting=1;
   return;
}

