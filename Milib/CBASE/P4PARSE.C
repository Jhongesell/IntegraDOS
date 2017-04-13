/* p4parse.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

extern unsigned e4buf_len ;
extern char  *e4buf ;

E4EXPR *S4FUNCTION e4parse( D4DATA *d4, char *expr_ptr )
{
   E4PARSE parse ;
   char    ops[128] ;
   int     rc ;
   E4EXPR *e4 ;

   memset( (void *)&parse, 0, sizeof(E4PARSE) ) ;
   memset( ops, 0, sizeof(ops));

   parse.expr.data   =  d4 ;
   parse.code_base   =  d4->code_base ;

   parse.op.ptr =  ops ;
   parse.op.len =  sizeof(ops) ;
   parse.op.code_base =  d4->code_base ;

   parse.result.ptr =  e4buf ;
   parse.result.len =  e4buf_len ;
   parse.result.do_extend =  1 ;
   parse.result.code_base =  d4->code_base ;

   s4scan_init( &parse.scan, expr_ptr ) ;

   rc = e4parse_expr( &parse ) ;
   e4buf =  parse.result.ptr ;
   e4buf_len =  parse.result.len ;
   if ( rc < 0 )  return 0 ;

   if ( s4stack_cur( &parse.op ) != Q4NO_FUNCTION )
   {
      e4error( parse.code_base, e4complete, expr_ptr, (char *) 0 ) ;
      return 0 ;
   }

   s4stack_push_int( &parse.result, -1 ) ;

   if ( e4type_check( &parse ) < 0 )   return 0 ;

   e4 =  (E4EXPR *) u4alloc( sizeof(E4EXPR) + parse.result.len + parse.scan.len + 1 ) ;
   if ( e4 == 0 )  return 0 ;

   e4->data =  d4 ;
   e4->parsed =  (char *) (e4+1) ;
   e4->source =  e4->parsed + parse.result.len ;

   memcpy( e4->parsed, parse.result.ptr, parse.result.len ) ;
   strcpy( e4->source, expr_ptr ) ;

   return e4 ;
}

int  e4field_function( F4FIELD *f4 )
{
   switch( f4->type )
   {
      case (int) 'C':
      case (int) 'M':
         return  I4FIELD_STR ;

      case (int) 'N':
      case (int) 'F':
         return  I4FIELD_NUM_S ;

      case (int) 'D':
         return  I4FIELD_DATE_S ;

      case (int) 'L':
         return  I4FIELD_LOG ;
   }
   return -1 ;
}

int  e4get_f_code( E4PARSE *p4, char *f_code_ptr, E4PARSE_PARM *parms )
{
   int  i, p_no, flg, i_functions ;

   /* flg Values
     -1 -  Error
      0 -  OK
      1 -  Conversion Required
   */

   memcpy( (void *)&i_functions, f_code_ptr, sizeof(int)) ;

   for ( i= i_functions; ; i++ )
   {
      if ( v4functions[i].code == 0 ||
      v4functions[i].code != v4functions[i_functions].code )
         break ;

      /* Check the Type Match */
      for ( p_no = 0, flg = 0; p_no < v4functions[i].num_parms; p_no++ )
      {
         int  i_temp ;

         if ( v4functions[i].type[p_no] == parms[p_no].type )   continue ;
     
         memcpy( (void *)&i_temp, parms[p_no].ptr, sizeof(int) ) ;

         if ( v4functions[i].type[p_no] == t4num_doub )
         {
            if ( i_temp == I4FIELD_NUM_S )
            {
               flg =  1 ;
               continue ;
            }
         }

         if ( v4functions[i].type[p_no] == t4date_doub )
         {
            if ( i_temp == I4FIELD_DATE_S ) 
            {
               flg =  1 ;
               continue ;
            }
         }

         flg =  -1 ;  /* No Match */
         break ;
      }

      if ( flg >= 0 )
      {
          /* Success */
         if ( flg > 0 )
         {
            /* Parameter Change Required First */
            for ( p_no = 0, flg = 0; p_no < v4functions[i].num_parms; p_no++) 
            {
               int  i_temp ;

               if ( v4functions[i].type[p_no] == parms[p_no].type )   continue ;

               memcpy( (void *)&i_temp, parms[p_no].ptr, sizeof(int) ) ;
               if ( i_temp == I4FIELD_DATE_S )
                  i_temp = I4FIELD_DATE_D ;
               else
               {
                  if ( i_temp == I4FIELD_NUM_S )
                     i_temp = I4FIELD_NUM_D ;
               }
               memcpy( parms[p_no].ptr, (void *)&i_temp, sizeof(int) ) ;
            }
         }

         memcpy( f_code_ptr, (void *)&i, sizeof(int) ) ;
         f_code_ptr +=  sizeof(int) ;

         parms[0].type =  v4functions[i].return_type ;
         parms[0].ptr  =  f_code_ptr ;

         return( 0 ) ;
      }
   }

   e4error( p4->code_base, e4type_sub, E4_TYPE_SUB, p4->scan.ptr, "",
          "Function:", v4functions[i_functions].name, (char *) 0 ) ;
   return( -1 ) ;
}

/*    Looks at the input string and returns and puts a character code on the
   result stack corresponding to the next operator.  The operators all operate
   on two operands.  Ex. +,-,*,/, >=, <, .AND., ...

    If the operator is ambiguous, return the arithmetic choice.

   Returns -2 (Done), 0, -1 (Error)
*/

int  e4get_operator( E4PARSE *p4, int *op_return)
{
   char ch ;
   int  op ;

   s4scan_range( &p4->scan, 1, ' ' ) ;
   ch =  s4scan_char(&p4->scan) ;
   if ( ch==0 || ch==')' || ch==',')
   {
      *op_return =  -2 ;
      return(0) ; /* Done */
   }

   op  =  e4lookup( p4->scan.ptr+p4->scan.pos, -1, E4FIRST_OPERATOR, E4LAST_OPERATOR ) ;
   if ( op < 0 )
      return e4error( p4->code_base, e4unrec_operator, p4->scan.ptr, (char *) 0 ) ;

   p4->scan.pos +=  v4functions[op].name_len ;
   *op_return = op ;

   return 0 ;
}

/* e4lookup, searches 'v4functions' for an operator or function.

       str - the function name
       len - the number of characters in the function name

   len <= 0

      Needs Exact Lookup.  All of the characters in 'v4functions'
      must be present in 'str'.

   len > 0 

      Only examines 'len' characters in 'str'.  It needs an exact
      match on 'len' characters.  If 'len <= 3', there cannot
      be any extra characters in 'v4functions'.
   Returns:  
       >= 0   The index into v4functions.
         -1   Not Located
*/

int  e4lookup( char *str, int len, int start_i, int end_i )
{
   char u_str[20] ;
   int  i, exact_lookup ;

   if ( len <= 0 )
   {
      exact_lookup =  1 ;

      /* Determine 'len' */
      for ( len=0; str[len] != ' ' && str[len] != '\0'; len++ ) ;
   }
   else
      exact_lookup =  0 ;

   u4ncpy( u_str, str, sizeof(u_str) ) ;
   c4upper( u_str ) ;

   for( i=start_i; i<= end_i; i++)
   {
      if ( v4functions[i].code < 0 )  break ;

      if ( v4functions[i].name[0] == u_str[0] )
      {
         if ( exact_lookup )
         {
            if ( v4functions[i].name_len <= len && v4functions[i].name_len > 0 )
               if (memcmp(u_str, v4functions[i].name, (size_t) v4functions[i].name_len) == 0)
                  return( i ) ;
         }
         else
            if ( memcmp(u_str, v4functions[i].name, (size_t) len) == 0)
            {
               if ( len >= 4 )  return( i ) ;
               if ( v4functions[i].name_len == len )  return(i) ;
            }
      }
   }
   return -1 ;
}

/*
     Parses an expression consisting of value [[operator value] ...]
   The expression is ended by a ')', a ',' or a '\0'.
   Operators are only popped until a '(', a ',' or the start of the stack.
   Left to right evaluation for operators of equal priority.

      An ambiguous operator is one which can be interpreted differently
   depending on its operands.  However, its operands depend on the
   priority of the operators and the evaluation order. Fortunately, the
   priority of an ambigous operator is constant regardless of its
   interpretation.  Consequently, the evaluation order is determined first.
   Then ambiguous operators can be exactly determined.

   Ambigous operators:+, -,  >, <, <=, >=, =, <>, #

   Return

       0  Normal
      -1  Error
*/

int  e4parse_expr( E4PARSE *p4 )
{
   if ( e4parse_value(p4) < 0 )  return -1 ;

   for(;;)
   {
      int  op_value ;

      if ( e4get_operator(p4, &op_value) < 0 )  return -1 ;
      if (op_value  == -2)
      {
        /* Done */

         while( s4stack_cur(&p4->op) != Q4L_BRACKET
            && s4stack_cur(&p4->op) != Q4COMMA
            && s4stack_cur(&p4->op) != Q4NO_FUNCTION )
               if (s4stack_push_int( &p4->result, s4stack_pop(&p4->op)) < 0) return -1;
               return( 0) ;
      }

         /* Everything with a higher or equal priority than 'op_value' must be
            executed first. (equal because of left to right evaluation order)
            Consequently, all high priority operators are sent to the result
            stack.
         */
      while ( s4stack_cur(&p4->op) >= 0 )
      {
         if ( v4functions[op_value].priority <=
              v4functions[s4stack_cur(&p4->op)].priority)
         {
            if (s4stack_push_int( &p4->result, s4stack_pop(&p4->op)) < 0) return -1;
         }
         else
            break ;
      }

      s4stack_push_int( &p4->op, op_value) ;

      if ( e4parse_value(p4) < 0 )  return -1 ;
   }
}

int  e4parse_function( E4PARSE *p4, char *start_ptr, int f_len )
{
   int f_num, num_parms ;
   char ch ;
   double d ;

   if ( p4->code_base->error_code < 0 )  return -1 ;

   f_num =  e4lookup( start_ptr, f_len, E4FIRST_FUNCTION, 0x7FFF) ;
   if (f_num== Q4NO_FUNCTION)
      return e4error( p4->code_base, e4unrec_function, p4->scan.ptr, (char *) 0 ) ;

   s4stack_push_int( &p4->op, Q4L_BRACKET ) ;
   p4->scan.pos++ ;

   num_parms = 0 ;
   for(;;)
   {
      ch =  s4scan_char( &p4->scan ) ;
      if ( ch == 0 ) return e4error( p4->code_base, e4right_missing, p4->scan.ptr, (char *) 0 ) ;
      if ( ch == ')')
      {
      p4->scan.pos++ ;
      break ;
      }

      if ( e4parse_expr(p4) < 0 )  return -1 ;
      num_parms++ ;

      while( s4scan_char( &p4->scan ) <= ' ' &&
             s4scan_char( &p4->scan ) >='\1')  p4->scan.pos++ ;

      if ( s4scan_char( &p4->scan ) == ')')
      {
         p4->scan.pos++ ;
         break ;
      }
      if ( s4scan_char( &p4->scan ) != ',')
         return e4error( p4->code_base, e4comma_expected, p4->scan.ptr, (char *) 0) ;
      p4->scan.pos++ ;
   }

   s4stack_pop( &p4->op ) ;  /* pop the left bracket */

   if ( num_parms != v4functions[f_num].num_parms )
   {
      if ( strcmp( v4functions[f_num].name, "STR" ) == 0 )
      {
         if ( num_parms == 1 )
         {
            d = 10.0 ;
            s4stack_push_int( &p4->result, I4DOUBLE ) ;
            s4stack_push_str( &p4->result, &d, sizeof(double) ) ;
            num_parms++ ;
         }
         if ( num_parms == 2 )
         {
            d = 0.0 ;
            s4stack_push_int( &p4->result, I4DOUBLE ) ;
            s4stack_push_str( &p4->result, &d, sizeof(double) ) ;
            num_parms++ ;
         }
      }
      if ( strcmp( v4functions[f_num].name, "SUBSTR" ) == 0 )
      {
         if ( num_parms == 2 )
         {       
            d = (double) 0x7FFF ;
            s4stack_push_int( &p4->result, I4DOUBLE ) ;
            s4stack_push_str( &p4->result, &d, sizeof(double) ) ;
            num_parms++ ;
         }
      }     
   }        
            
   if ( p4->code_base->error_code < 0 )  return -1 ;

   if ( num_parms != v4functions[f_num].num_parms )
      return e4error( p4->code_base, e4num_parms, p4->scan.ptr, E4_NUM_PARMS, v4functions[f_num].name, (char *) 0 ) ;
            
   return s4stack_push_int( &p4->result, f_num ) ;
}           
            
int  e4parse_value( E4PARSE *p4 )
{
   F4FIELD * field_ptr ;
   char ch, *start_ptr, search_char ;
   int  i_functions, len ;
   double d ;
            
      if ( p4->code_base->error_code < 0 )  return -1 ;
             
      s4scan_range( &p4->scan, ' ', ' ' ) ;
             
      /* expression */
             
      if ( s4scan_char( &p4->scan ) == '(')
      {      
         p4->scan.pos++ ;

         s4stack_push_int( &p4->op, Q4L_BRACKET) ;
         if ( e4parse_expr(p4) < 0 )  return( -1 ) ;
               
         while ( s4scan_char( &p4->scan ) <= ' ' &&
            s4scan_char( &p4->scan ) != 0)   p4->scan.pos++ ;

         if ( s4scan_char( &p4->scan ) != ')' )
            return e4error( p4->code_base, e4right_missing, p4->scan.ptr, (char *) 0 ) ;
         p4->scan.pos++ ;
         s4stack_pop( &p4->op ) ;
         return( 0 ) ;
      }

    /* logical */
      if ( s4scan_char( &p4->scan ) == '.' )
      {
         i_functions =  e4lookup( p4->scan.ptr+p4->scan.pos, -1, E4FIRST_LOG, E4LAST_LOG ) ;
         if ( i_functions >= 0 )
         {
            p4->scan.pos +=  v4functions[i_functions].name_len ;

            if ( strcmp( v4functions[i_functions].name, ".NOT." ) == 0 )
               if ( e4parse_expr(p4) < 0 )  return( -1 ) ; /* One operand operation */

            return s4stack_push_int( &p4->result, i_functions ) ;
         }
      }

    /* real */
      ch =  s4scan_char( &p4->scan ) ;
      if ( ch >='0' && ch <='9' || ch == '-' || ch == '+' || ch == '.' )
      {
         start_ptr =  p4->scan.ptr+p4->scan.pos ;
         p4->scan.pos++ ;
         len = 1 ;

         while( s4scan_char( &p4->scan ) >= '0' &&
            s4scan_char( &p4->scan ) <= '9' ||
            s4scan_char( &p4->scan ) == '.' )
         {
            if ( s4scan_char( &p4->scan ) == '.' )
               if ( strnicmp( p4->scan.ptr+p4->scan.pos, ".AND.",5) == 0 ||
                    strnicmp( p4->scan.ptr+p4->scan.pos, ".OR.",4) == 0 ||
                    strnicmp( p4->scan.ptr+p4->scan.pos, ".NOT.",5) == 0 )
                  break ;
               len++ ;
               p4->scan.pos++ ;
         }

         d =  c4atod( start_ptr, len ) ;
         s4stack_push_int( &p4->result, I4DOUBLE ) ;
         s4stack_push_str( &p4->result, &d, sizeof(d) ) ;
         return( 0 );
      }

    /* string */
      ch =  s4scan_char( &p4->scan ) ;
      if ( ch == '\'' || ch == '\"')
      {
         search_char =  s4scan_char( &p4->scan ) ;

         p4->scan.pos++ ;
         start_ptr =  p4->scan.ptr + p4->scan.pos ;

         len =  s4scan_search( &p4->scan, search_char ) ;
         if ( s4scan_char( &p4->scan ) != search_char )
            if ( len < 0 )
               return e4error( p4->code_base, e4unterminated, p4->scan.ptr, (char *) 0 ) ;

         p4->scan.pos++ ;
         s4stack_push_int( &p4->result, I4STRING ) ;
         s4stack_push_int( &p4->result, len ) ;
         return s4stack_push_str( &p4->result, start_ptr, len ) ;
      }

    /* function or field */
      if (u4name_char(s4scan_char( &p4->scan )) )
      {
         D4DATA *base_ptr ;
         char b_name[11], f_name[11] ;

         start_ptr =  p4->scan.ptr + p4->scan.pos ;

         for( len=0; u4name_char(s4scan_char( &p4->scan )); len++ )
            p4->scan.pos++ ;

         s4scan_range( &p4->scan, (char) 0, ' ' ) ;

         if ( s4scan_char( &p4->scan ) == '(' )
            return e4parse_function( p4, start_ptr, len ) ;


         base_ptr = 0 ;
         if ( s4scan_char( &p4->scan ) == '-')
            if ( p4->scan.ptr[p4->scan.pos+1] == '>')
            {                                    
               if ( len > 10 )  len =  10 ;
               memmove( b_name, start_ptr, (size_t) len ) ;
               b_name[len] = '\0' ;
            
               base_ptr =  d4data( p4->code_base, b_name) ;
            
               if ( base_ptr == 0 )
                  return e4error( p4->code_base, e4data_name, b_name, p4->scan.ptr, (char *) 0 ) ;
               p4->scan.pos++ ;
               p4->scan.pos++ ;
            
               start_ptr =  p4->scan.ptr + p4->scan.pos ;
               for( len=0; u4name_char(s4scan_char( &p4->scan )); len++ )
                  p4->scan.pos++ ;
            }
         
         if ( base_ptr == 0 )
            base_ptr =  (D4DATA *) p4->expr.data ;
         
         if ( len <= 10)
         {
            memmove( f_name, start_ptr, (size_t) len ) ;
            f_name[len] =  0 ;
            field_ptr =  d4field( base_ptr, f_name ) ;
         
            if ( field_ptr == 0 )  return -1 ;
         
            s4stack_push_int( &p4->result, e4field_function( field_ptr ) ) ;
            return s4stack_push_str( &p4->result, &field_ptr, sizeof(field_ptr)) ;
         }
      }     
      return e4error( p4->code_base, e4unrec_value, p4->scan.ptr, (char *) 0 ) ;
}

int  e4type_check( E4PARSE *p4 )
{
   int      f_code ;
   char    *f_code_ptr, *compile_ptr ;
   int      n_parms, len ;

   E4PARSE_PARM  parms[E4PARM_STACK_SIZE] ;

   if ( p4->code_base->error_code < 0 )  return -1 ;

   parms[0].type =  0 ;
   compile_ptr =  p4->result.ptr ;

   for ( n_parms= 0;; )
   {
      f_code_ptr =  compile_ptr ;
      memcpy( (void *)&f_code, f_code_ptr, sizeof(int) ) ;

      if ( f_code == -1 )
      {
         #ifdef S4DEBUG
            if (n_parms != 1)  e4severe( e4info, "e4type_check() 1", p4->scan.ptr, (char *) 0 ) ;
         #endif
         return  parms[0].type ;
      }

      if ( f_code <= I4LAST_IMMEDIATE )
      {
         parms[n_parms].type =  v4functions[f_code].return_type ;
         parms[n_parms].ptr  =  compile_ptr ;
         compile_ptr +=  sizeof(int) ;

         if ( f_code <= LAST_I4FIELD )
            compile_ptr = compile_ptr + sizeof(F4FIELD *) ;
         else
         {
            if ( f_code == I4DOUBLE )
               len = sizeof(double) ;
            else
            {
               memcpy( (void *)&len, compile_ptr, sizeof(int) ) ;
               compile_ptr += sizeof(int) ;
            }
            compile_ptr +=  len ;
         }
      }
      else
      {
         n_parms -=  v4functions[f_code].num_parms ;
         #ifdef S4DEBUG
            if ( n_parms < 0 )
               e4severe( e4info, "e4type_check() 2", p4->scan.ptr, (char *) 0 ) ;
         #endif

         if ( e4get_f_code( p4, f_code_ptr, parms+ n_parms ) < 0 )  return -1 ;
         compile_ptr +=  sizeof(int) ;
      }
      n_parms++ ;

      if ( n_parms+1 >= E4PARM_STACK_SIZE )
         return e4error( p4->code_base, e4overflow, p4->scan.ptr, (char *) 0 ) ;
   }
}

int s4stack_pop( S4STACK *s4 )
{
   int ret_value ;

   ret_value =  s4stack_cur(s4) ;

   if ( s4->pos >= sizeof(int) )
      s4->pos -= sizeof(int) ;
   return ret_value ;
}

int s4stack_cur( S4STACK *s4 )
{
   int   pos, cur_data ;

   if ( s4->pos < sizeof(int) )  return Q4NO_FUNCTION ;
   pos =  s4->pos - sizeof(int) ;
   memcpy( (void *)&cur_data, s4->ptr+pos, sizeof(int) ) ;
   return cur_data ;
}

int s4stack_push_int( S4STACK *s4, int i )
{
   return s4stack_push_str( s4, &i, sizeof(i)) ;
}

int s4stack_push_str( S4STACK *s4, void *p, int len )
{
   char *old_ptr ;

   if ( s4->code_base->error_code < 0 )  return -1 ;

   if ( s4->pos+len > s4->len )
   {
      old_ptr =  s4->ptr ;
      if ( ! s4->do_extend )
         s4->ptr =  0 ;
      else
         s4->ptr =  (char *) u4alloc( s4->len + 256 ) ;
      if ( s4->ptr == 0 )
      {
         s4->ptr =  old_ptr ;
         e4error( s4->code_base, e4memory, (char *) 0 ) ;
         return -1 ;
      }
      memcpy( s4->ptr, old_ptr, s4->len ) ;
      u4free( old_ptr ) ;
      s4->len +=  256 ;

      return  s4stack_push_str( s4, p, len ) ;
   }
   else
   {
      memcpy( s4->ptr+s4->pos, p, len ) ;
      s4->pos +=  len ;
   }
   return 0 ;
}


char s4scan_char( S4SCAN *s4 )
{
   if ( s4->pos >= s4->len )  return 0 ;
   return s4->ptr[s4->pos] ;
}

void s4scan_init( S4SCAN *s4, char *p )
{
   s4->ptr =  p ;
   s4->pos =  0 ;
   s4->len =  strlen(p) ;
}

int s4scan_range( S4SCAN *s4, char start_char, char end_char )
{
   int count ;

   for ( count = 0; s4->pos < s4->len; s4->pos++, count++ )
      if ( s4->ptr[s4->pos] < start_char || s4->ptr[s4->pos] > end_char )
         return count ;
   return count ;
}

int s4scan_search( S4SCAN *s4, char search_char )
{
   int count ;

   for ( count = 0; s4->pos < s4->len; s4->pos++, count++ )
      if ( s4->ptr[s4->pos] == search_char )  return count ;
   return count ;
}
