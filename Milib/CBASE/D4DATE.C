/* d4date.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#include <time.h>

#define  JULIAN_ADJUSTMENT    1721425L

static int  month_tot[]=
    { 0, 0,  31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 } ;
	 /* Jan Feb Mar  Apr  May  Jun	Jul  Aug  Sep  Oct  Nov  Dec
	     31  28  31   30   31   30	 31   31   30	31   30   31
	 */

static int c4julian( int year, int month, int day )
{
/*  Returns */
/*     >0   The day of the year starting from 1 */
/*          Ex.    Jan 1, returns  1 */
/*     -1   Illegal Date */
   int is_leap, month_days ;

   is_leap =   ( year%4 == 0 && year%100 != 0 || year%400 == 0 ) ?  1 : 0 ;

   month_days =  month_tot[ month+1 ] -  month_tot[ month] ;
   if ( month == 2 )  month_days += is_leap ;

   if ( year  < 0  ||
	month < 1  ||  month > 12  ||
	day   < 1  ||  day   > month_days )
	return( -1 ) ;	/* Illegal Date */

   if ( month <= 2 )  is_leap = 0 ;

   return(  month_tot[month] + day + is_leap ) ;
}

static long c4ytoj( int yr )
{
/*  Calculates the number of days to the year */

/*  This calculation takes into account the fact that */
/*     1)  Years divisible by 400 are always leap years. */
/*     2)  Years divisible by 100 but not 400 are not leap years. */
/*     3)  Otherwise, years divisible by four are leap years. */

/*  Since we do not want to consider the current year, we will */
/*  subtract the year by 1 before doing the calculation. */

   yr-- ;
   return( yr*365L +  yr/4L - yr/100L + yr/400L ) ;
}

long S4FUNCTION a4long( char *date_ptr )
{
/*  Returns: */
/*    >0  -  Julian day */
/*           That is the number of days since the date  Jan 1, 4713 BC */
/*           Ex.  Jan 1, 1981 is  2444606 */
/*     0  -  NULL Date (dbf_date is all blank) */
/*    -1  -  Illegal Date */
   int  year, month, day, day_year ;

   year =  c4atoi( date_ptr, 4) ;
   if ( year == 0)
      if ( memcmp( date_ptr, "        ", 8 ) == 0)
	 return  0 ;

   month =  c4atoi( date_ptr+4, 2) ;
   day   =  c4atoi( date_ptr+6, 2) ;
   day_year =  c4julian( year, month, day) ;
   if (day_year < 1)  return -1L ;  /* Illegal Date */

   return (c4ytoj(year) + day_year + JULIAN_ADJUSTMENT) ;
}

typedef struct
{
   char cdow[10] ;
}  DOW ;

static DOW day_of_week[] =
{
   { "\0        " },
   { "Sunday\0  " },
   { "Monday\0  " },
   { "Tuesday\0 " },
   { "Wednesday" },
   { "Thursday\0" },
   { "Friday\0  " },
   { "Saturday\0" },
} ;

char *S4FUNCTION a4cdow( char *date_ptr )
{
   return  day_of_week[a4dow(date_ptr)].cdow ;
}

typedef struct
{
   char cmonth[10] ;
} MONTH ;

static MONTH month_of_year[] =
#ifdef LANGUAGE
{
   { "\0        " },
   { "Januar   " },
   { "Februar  " },
   { "M„rz     " },
   { "April    " },
   { "Mai      " },
   { "Juni     " },
   { "Juli     " },
   { "August   " },
   { "September" },
   { "Oktober  " },
   { "November " },
   { "Dezember " },
} ;
#else
{
   { "\0        " },
   { "January\0 " },
   { "February\0" },
   { "March\0   " },
   { "April\0   " },
   { "May\0     " },
   { "June\0    " },
   { "July\0    " },
   { "August\0  " },
   { "September"  },
   { "October\0 " },
   { "November\0" },
   { "December\0" },
} ;
#endif

char *S4FUNCTION a4cmonth( char *date_ptr )
{
   return month_of_year[ a4month(date_ptr) ].cmonth ;
}

int S4FUNCTION a4day( char *date_ptr )
{
   return  (int) c4atol( date_ptr+6, 2 ) ;
}

int S4FUNCTION a4dow( char *date_ptr )
{
   long date ;
   date = a4long(date_ptr) ;
   if (date < 0) return 0 ;
   return (int) ((date+1) % 7) + 1 ;
}

void S4FUNCTION a4format( char *date_ptr, char *result, char *picture )
{
   int   result_len, rest, m_num, length ;
   char *ptr_end, *month_ptr ;

   result_len =  strlen(picture) ;
   memset( result, ' ', result_len ) ;

   c4encode( result, date_ptr, picture, "CCYYMMDD") ;

   ptr_end  =  strchr( picture, 'M' ) ;
   if ( ptr_end  !=  (char *) 0 )
   {
      month_ptr =  result+  (int) (ptr_end-picture) ;
      length =  0 ;
      while ( *(ptr_end++) == 'M' )  length++ ;

      if ( length > 2)
      {
	 /* Convert from a numeric form to character format for month */
	 m_num =  c4atoi( date_ptr+4, 2) ;
	 if ( m_num < 1)  m_num =  1 ;
	 if ( m_num > 12) m_num = 12 ;

	 rest =  length - 9 ;
	 if (length > 9) length = 9 ;

	 memcpy( month_ptr, month_of_year[m_num].cmonth, (size_t) length ) ;
	 if (rest > 0)	memset( month_ptr+length, (int) ' ', (size_t) rest ) ;
      }
   }
}

int S4FUNCTION a4format_mdx2( char *date_ptr, double *result )
{
   *result = a4format_mdx( date_ptr );
   if (*result == 1.0E100)
      return -1;
   return 0;
}

double S4FUNCTION a4format_mdx( char *date_ptr )
{
   long  ldate ;
   ldate =  a4long(date_ptr) ;
   if ( ldate == 0 )  return 1.0E100 ;  /* Blank or Null date */ 
   return (double) ldate ;
}

int S4FUNCTION a4month( char *date_ptr )
{
   return  (int) c4atol( date_ptr+4, 2 ) ;
}

int S4FUNCTION a4year( char *year_ptr )
{
   return  (int) c4atol( year_ptr, 4 ) ;
}

static int  c4mon_dy( int year, int days,  int *month_ptr,  int *day_ptr )
{
/*  Given the year and the day of the year, returns the month and day of month. */
   int is_leap, i ;

   is_leap =  ( year%4 == 0 && year%100 != 0 || year%400 == 0 ) ?  1 : 0 ;
   if ( days <= 59 )  is_leap = 0 ;

   for( i = 2; i <= 13; i++)
   {
      if ( days <=  month_tot[i] + is_leap )
      {
	 *month_ptr =  --i ;
	 if ( i <= 2) is_leap = 0 ;

	 *day_ptr   =  days - month_tot[ i] - is_leap ;
	 return( 0) ;
      }
   }
   *day_ptr   =  0 ;
   *month_ptr =  0 ;

   return( -1 ) ;
}

void S4FUNCTION a4assign( char *date_ptr, long ldate )
{
/*  Converts from a julian day to the dbf file date format. */
   long tot_days ;
   int  i_temp, year, n_days ;
   int max_days ;
   int  month, day ;

   if ( ldate <= 0 )
   {
      memset( date_ptr, ' ',  8 ) ;
      return ;
   }

   tot_days =  ldate - JULIAN_ADJUSTMENT ;
   i_temp   =  (int) ((double) tot_days / 365.2425) ;
   year     =  i_temp + 1 ;
   n_days   =  (int) (tot_days -  c4ytoj(year)) ;
   if ( n_days <= 0 )
   {
      year-- ;
      n_days   =  (int) (tot_days - c4ytoj(year)) ;
   }

   if (year%4 == 0 && year%100 != 0 || year%400 == 0)
      max_days =  366 ;
   else
      max_days =  365 ;

   if ( n_days > max_days )
   {
      year++ ;
      n_days -= max_days ;
   }

   #ifdef S4DEBUG
      if ( c4mon_dy( year, n_days, &month, &day ) < 0 )
	 e4severe( e4result, "a4assign()", (char *) 0 ) ;
   #else
      c4mon_dy( year, n_days, &month, &day) ;
   #endif

   c4ltoa45( (long) year,  date_ptr,   -4 ) ;
   c4ltoa45( (long) month, date_ptr+4, -2 ) ;
   c4ltoa45( (long) day,	 date_ptr+6, -2 ) ;
}

void S4FUNCTION a4init( char *date_ptr, char *date_data, char *picture )
{
   char  *month_start, month_data[10], buf[2] ;
   int	 year_count, month_count, day_count, century_count, i, length ;

   day_count	=  5 ;
   month_count	=  3 ;
   year_count	=  1 ;
   century_count= -1 ;

   memset( date_ptr, ' ', 8 ) ;

   for ( i=0; picture[i] != '\0'; i++ )
   {
      switch( picture[i] )
      {
	 case 'D':
	    if ( ++day_count >= 8) break ;
	    date_ptr[day_count] =  date_data[i] ;
	    break ;

	 case 'M':
	    if ( ++month_count >=6) break ;
	    date_ptr[month_count] =  date_data[i] ;
	    break ;

	 case 'Y':
	    if ( ++year_count >= 4) break ;
	    date_ptr[year_count] =  date_data[i] ;
	    break ;

	 case 'C':
	    if ( ++century_count >= 2) break ;
	    date_ptr[century_count] =  date_data[i] ;
	    break ;
      }
   }

   if ( strcmp( date_ptr, "        " ) == 0 )  return ;

   if ( century_count ==  -1 )	memcpy( date_ptr,   "19", (size_t) 2 ) ;
   if ( year_count    ==   1 )	memcpy( date_ptr+2, "01", (size_t) 2 ) ;
   if ( month_count   ==   3 )	memcpy( date_ptr+4, "01", (size_t) 2 ) ;
   if ( day_count     ==   5 )	memcpy( date_ptr+6, "01", (size_t) 2 ) ;

   if ( month_count >= 6 )
   {
      /* Convert the Month from Character Form to Date Format */
      month_start =  strchr( picture, 'M' ) ;

      length =  month_count - 3 ;	/* Number of 'M' characters in picture */

      memcpy( date_ptr+4, "  ", (size_t) 2 ) ;

      if ( length > 3 ) length = 3 ;
      memcpy( month_data, date_data+ (int) (month_start-picture), (size_t) length) ;
      while ( length > 0 )
	 if ( month_data[length-1] == ' ' )
	    length-- ;
	 else
	    break ;

      month_data[length] =  '\0' ;

      c4lower( month_data ) ;
      buf[0] =  month_data[0] ;
      buf[1] =  0 ;
      c4upper(buf) ;
      month_data[0] =  buf[0] ;

      if ( length > 0 )
	 for( i=1; i<= 12; i++ )
	 {
	    if ( memcmp( month_of_year[i].cmonth, month_data, (size_t) length) == 0 )
	    {
	       c4ltoa45( (long) i, date_ptr+4, 2 ) ;  /* Found Month Match */
	       break ;
	    }
	 }
   }

   for ( i=0; i< 8; i++ )
      if ( date_ptr[i] == ' ' )  date_ptr[i] =  '0' ;
}

void S4FUNCTION a4today( char *date_ptr )
{
   long time_val ;
   struct tm *tm_ptr ;

   time( (time_t *) &time_val) ;
   tm_ptr =  localtime( (time_t *) &time_val) ;

   c4ltoa45( 1900L+ tm_ptr->tm_year, date_ptr, -4 ) ;
   c4ltoa45( (long) tm_ptr->tm_mon+1, date_ptr+4, -2 ) ;
   c4ltoa45( (long) tm_ptr->tm_mday, date_ptr+6, -2 ) ;
}

void S4FUNCTION a4time_now( char *time_data )
{
   long time_val ;
   struct tm *tm_ptr ;

   time( &time_val) ;
   tm_ptr =  localtime( (time_t *) &time_val) ;

   c4ltoa45( (long) tm_ptr->tm_hour, time_data, -2) ;
   time_data[2] = ':' ;
   c4ltoa45( (long) tm_ptr->tm_min, time_data+3, -2) ;
   time_data[5] = ':' ;
   c4ltoa45( (long) tm_ptr->tm_sec, time_data+6, -2) ;
}
