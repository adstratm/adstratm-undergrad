/** parser.h
  * Parses the TruPL language
  * TruPL author: Dr. Robert Matthews
  * @author Andrew Stratmann
  * @version 14 November 2016
  */
#ifndef PARSER_H
#define PARSER_H

// The parser needs to access token attributes.
#include "token.h"
#include "keywordtoken.h"
#include "punctoken.h"
#include "reloptoken.h"
#include "addoptoken.h"
#include "muloptoken.h"
#include "idtoken.h"
#include "numtoken.h"
#include "eoftoken.h"
#include "scanner.h"
#include "symbol_table.h"
//code generation files
#include "register.h"
#include "register_allocator.h"
#include "emitter.h"
#include "operand.h"

// To print error messages.
#include <iostream>

using namespace std;

class Parser {
   public:

      /** Constructs a new parser object.
        * @param the_scanner The scanner object to retrieve tokens from the source file
        */
      Parser (Scanner *the_scanner);

      /** Destructs a parser
        */
      ~Parser();
  
      /** Parses a program
        * @return The parse status of the program
        */
      bool parse_program();

      /** Determines is the input is exhausted
        * @return The status of remaining tokens
        */
      bool done_with_input();

  
   private:
      /** Discards the current token and retrieves the next
        */
      void advance();

      /** Parses a declaration list
        * @return The success of the parse
        */
      bool parse_decl_list();

      /** Parses a variable declaration list
        * @return The success of the parse
        */
      bool parse_variable_decl_list();

      /** Parses a block
        * @return The success of the parse
        */
      bool parse_block();

      /** Parses a procedure declaration list
        * @return The success of the parse
        */
      bool parse_procedure_decl_list();
      
      /** Parses a variable declaration
        * @return The success of the parse
        */
      bool parse_variable_decl();

      /** Parses a standard type
        * @param standard_type_type The output type
        * @return The success of the parse
        */
      bool parse_standard_type(expr_type &standard_type_type);
      
      /** Parses an identifier list
        * @return The success of the parse
        */
      bool parse_identifier_list();

      /** Parses a procedure declaration
        * @return The success of the parse
        */
      bool parse_procedure_decl();

      /** Parses identifier list prime
        * @return The success of the parse
        */
      bool parse_identifier_list_prm();

      /** Parses a statement list
        * @return The success of the parse
        */
      bool parse_stmt_list();

      /** Parses procedure arguments
        * @return The success of the parse
        */
      bool parse_procedure_args();

      /** Parses a formal parameter list
        * @return The success of the parse
        */
      bool parse_formal_parm_list();

      /** Parses formal parameter list hat
        * @return The success of the parse
        */
      bool parse_formal_parm_list_hat();

      /** Parses statement list prime
        * @return The status of the parse
        */
      bool parse_stmt_list_prm();

      /** Parses a statement
        * @return The success of the parse
        */
      bool parse_stmt();

      /** Parses an if statement
        * @return The success of the parse
        */
      bool parse_if_stmt();

      /** Parses a while statement
        * @return The success of the parse
        */
      bool parse_while_stmt();

      /** Parses a print statement
        * @return The success of the parse
        */
      bool parse_print_stmt();

      /** Parses ad hoc as pc tail
        * @param ad_hoc_as_pc_tail_type The output type
        * @return The success of the parse
        */
      bool parse_ad_hoc_as_pc_tail(expr_type &ad_hoc_as_pc_tail_type);

      /** Parses an expression
        * @param expr_type_result The expression output type
        * @return The success of the parse
        */
      bool parse_expr(expr_type &expr_type_result, Operand *&op);

      /** Parses if statement hat
        * @return The success of the parse
        */
      bool parse_if_stmt_hat();

      /** Parses an expression list
        * @return The success of the parse
        */
      bool parse_expr_list();

      /** Parses an actual parameter list
        * @return The success of the parse
        */
      bool parse_actual_parm_list();

      /** Parses actual parameter list hat
        * @return The success of the parse
        */
      bool parse_actual_parm_list_hat();

      /** Parses expression hat
        * @param expr_hat_type The output type
        * @return The success of the parse
        */
      bool parse_expr_hat(expr_type &expr_hat_type, Operand *&op);

      /** Parses a simple expression
        * @param simple_expr_type The output type
        * @return The success of the parse
        */
      bool parse_simple_expr(expr_type &simple_expr_type, Operand *&op);

      /** Parses simple expression prime
        * @param simple_expr_prime_type The output type
        * @return The success of the parse
        */
      bool parse_simple_expr_prm(expr_type &simple_expr_prm_type, Operand *&op);

      /** Parses a term
        * @param term_type The output type
        * @return The success of the parse
        */
      bool parse_term(expr_type &term_type, Operand *&op);

      /** Parses term prime
        * @param term_prm_type The output type
        * @return The success of the parse
        */
      bool parse_term_prm(expr_type &term_prm_type, Operand *&left_op);

      /** Parses a factor
        * @param factor0_type The output type
        * @return The success of the parse
        */
      bool parse_factor(expr_type &factor0_type, Operand *&op);

      /** Parses a sign
        * @param sign_type The output type
        * @return The success of the parse
        */
      bool parse_sign(expr_type &sign_type);

      /** Outputs a parse error and exits the program
        * @param expected The token that was expected
        * @param found The token that was found
        */
      void parse_error (string *expected, Token *found);

      /** Outputs a multiply defined error and exits the program
        * @param The identifier that was defined multiple times
        */
      void multiply_defined_identifier (string *id) const;

      /** Outputs an undeclared identifier error and exits the program
        * @param id The identifier that was not declared
        */
      void undeclared_identifier (string *id) const;

      /** Outputs a type error and exits the program
        * @param expected The type expected
        * @param found The type found
        */
      void type_error (const expr_type expected, 
		       const expr_type found) const;

      /** Outputs a type error and exits the program
        * @param expected1 The first expected type
        * @param expected2 The second expected type
        * @param found The type that was found
        */
      void type_error (const expr_type expected1,
			const expr_type expected2,
			const expr_type found) const;
  
      
      /*********** Parser Things ***********/

      // The lexical analyzer
      Scanner *lex;
      // The current token the parser is looking at
      Token *word;


      /*********** Semantial Analysis stuff **********/
      /* See grammar 3.0 for an explanation of these. */
      string *current_env, *main_env, *procedure_name;
      int actual_parm_position, formal_parm_position;

      bool parsing_formal_parm_list;

      //The symbol table
      Symbol_Table stab; 

      /**************** IR stuff *******************/
      Register_Allocator *allocator;
      Emitter *e;
};

#endif
