/** An implementation of parser.h
  * @author Andrew Stratmann
  * @version 15 December 2016
  */
#include "parser.h"

Parser::Parser(Scanner *the_scanner)
{
  /* Initialize the parser. */
  lex = the_scanner;
  word = lex->next_token();

  //Semantic Initializations
  current_env = main_env = procedure_name = NULL;
  actual_parm_position = formal_parm_position = -1;
  parsing_formal_parm_list = false;

  //IR initializations
  e = new Emitter();
  allocator = new Register_Allocator();
}

Parser::~Parser()
{
  /* Delete the parser. */
  if (lex != NULL) {
    delete lex;
  }
  if (word != NULL) {
    delete word;
  }
  if(e != NULL) {
    delete e;
  }
  if(allocator != NULL) {
    delete allocator;
  }
}

void Parser::advance()
{
    delete word;
    word = lex->next_token();
}

// If we have parsed the entire program, then word
// should be the EOF Token.  This function tests
// that condition.
bool Parser::done_with_input()
{
  return word->get_token_type() == TOKEN_EOF;
}

void Parser::multiply_defined_identifier(string *id) const
{
    cerr << "The identifier " << *id << " has already been declared. " << endl;
    exit(-1);
}

void Parser::type_error (const expr_type expected,
			 const expr_type found) const
{
  cerr << "Type error: expected " << *(stab.type_to_string (expected))
       << " found " << *(stab.type_to_string(found)) << "." << endl;
  exit (-1);
}


void Parser::type_error (const expr_type expected1,
			 const expr_type expected2,
			 const expr_type found) const
{
  cerr << "Type error: expected " << *(stab.type_to_string (expected1))
       << " or " << *(stab.type_to_string (expected2))
       << ", found " << *(stab.type_to_string(found)) << "." << endl;
  exit (-1);
}


bool Parser::parse_program()
{
   // PROGRAM -> program identifier ; DECL_LIST BLOCK ;
   // Predict (program identifier ; DECL_LIST BLOCK ;) == {program}

   // Match keyword program, first symbol on RHS
   if (word->get_token_type() == TOKEN_KEYWORD
       && static_cast<KeywordToken *>(word)->get_attribute() == KW_PROGRAM) {

     /* ADVANCE - Notice that we only delete a token on an ADVANCE,
       and, if we ADVANCE, it is the ADVANCE code that is responsible
       for getting the next token.
     */
     delete word;
     word = lex->next_token();

     // Match identifier, 2nd symbol on RHS
     if (word->get_token_type() == TOKEN_ID) {

        //get the name of the identifier and install it
        //in the symbol table
        string *id_name = static_cast<IdToken *>(word)->get_attribute();
        //emit the name of the program
        e->emit_label(id_name);
        string *global_env_name = new string ("_EXTERNAL");
        stab.install(id_name, global_env_name, PROGRAM_T);
        current_env = new string(*id_name);
        main_env = new string(*id_name);
        delete global_env_name;
        delete id_name;

        // ADVANCE
        delete word;
        word = lex->next_token();

	// Match semicolon(;), 3rd symbol on RHS
	if (word->get_token_type() == TOKEN_PUNC
	    && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
	  // ADVANCE
	  delete word;
	  word = lex->next_token();

	  /* Match DECL_LIST, 4th bymbol on RHS.  This is an ACTION,
	     not an advance, so we don't grab another token if we
	     succeed.  We are just growing the parse tree at this
	     point.
	  */
	  if (parse_decl_list()) {

	    // Match BLOCK, 5th on RHS - ACTION
	    if (parse_block()) {

	      // Match semicolon(;), 6th and last on RHS -
	      if (word->get_token_type() == TOKEN_PUNC
		  && static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI) {
		// ADVANCE
		// Since we advanced, we matched a token so we get the next one.
		delete word;
		word = lex->next_token();

		// Parse_program succeeded.
        //cout << "Parse of program succeeds" << endl;

        //emit a halt statement
        e->emit_halt();
        e->emit_comment("Data Directives");
        //emit the data directives for the program
        for(int i = 0; i < stab.get_size(); i++)
        {
            if(stab.is_intbool_at_position(i))
            {
                e->emit_data_directive(stab.get_id_at_position(i), 1);
            }
        }

		return true;

		// We failed to match the second semicolon
	      } else {
		string *expected = new string ("';'");
		// Variable expected should be deleted in parse_error()
		parse_error (expected, word);
    //cout << "Parse of program fails" << endl;
		return false;
	      }

	      /* We failed to parse BLOCK.  Don't print an error here;
		 instead, print the error when you discover it,
		 i.e. when you are trying to advance. */
	    } else {
        //cout << "Parse of program fails" << endl;
	      return false;
	    }

	    // We failed to parse DECL_LIST
	  } else {
      //cout << "Parse of program fails" << endl;
	    return false;
	  }

	  // We failed to match the first semicolon
	} else {
	  string *expected = new string ("';'");
	  parse_error (expected, word);
    //cout << "Parse of program fails" << endl;
	  return false;
	}

	// We failed to match an identifier
     } else {
       string *expected = new string ("identifier");
       parse_error (expected, word);
       //cout << "Parse of program fails" << endl;
       return false;
     }

     // We failed to match the keyword program
   } else {
     string *expected = new string ("keyword program");
     parse_error (expected, word);
     //cout << "Parse of program fails" << endl;
     return false;
   }

   /* We shouldn't reach this statement, but it is here as a defensive
      programming measure. */
   //cout << "Parse of program fails" << endl;
   return false;
}


bool Parser::parse_decl_list()
{
   /* DECL_LIST -> VARIABLE_DECL_LIST PROCEDURE_DECL_LIST

      Predict(VARIABLE_DECL_LIST PROCEDURE_DECL_LIST)
        = First(VARIABLE_DECL_LIST)
       union First (PROCEDURE_DECL_LIST)
       union Follow (DECL_LIST) = {identifier, procedure, begin}

       Note that we don't actually need to check the predict set
       here.  The predict set is used to choose the correct
       production, but there isn't a choice here.

       In addition, we take advantage of C++'s short circuit
       evaluation of Boolean expressions. */

  return parse_variable_decl_list() && parse_procedure_decl_list();
}

bool Parser::parse_variable_decl_list()
{
    //The predict set contains identifier and lambda
    //first check for an identifier, and if not,
    //take lambda transition and return true.

    if(word->get_token_type() == TOKEN_ID)
    {
        //take the first transition.
        if(parse_variable_decl())
        {
            //first part of the transition succeded, continue
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
            {
                delete word;
                word = lex->next_token();
                //found the semicolon
                if(parse_variable_decl_list())
                {
                    //parse completes
                    //cout << "Parse of variabledecllist succeeds" << endl;
                    return true;
                }
                else
                {
                    //parse of variable_decl_list failed
                    //cout << "Parse of variabledecllist fails" << endl;
                    return false;
                }
            }
            else
            {
                //didn't find the semicolon
                string *expected = new string ("';'");
                parse_error(expected, word);
                //cout << "Parse of variabledecllist fails" << endl;
                return false;
            }
        }
        else
        {
            //variable_decl parse failed
            //cout << "Parse of variabledecllist fails" << endl;
            return false;
        }
    }
    else
    {
        //take the lambda transition
        //cout << "Parse of variabledecllist succeeds" << endl;
        return true;
    }
    //defensive
    //cout << "Parse of variabledecllist fails" << endl;
    return false;
}

bool Parser::parse_variable_decl()
{
    //identifier is the only token in the predict set

    expr_type std_type_type = GARBAGE_T;

    if(word->get_token_type() == TOKEN_ID)
    {
        if(parse_identifier_list())
        {
            //parse of identifier list succeeds, get next token and continue
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON)
            {
                //found the colon, get next word
                delete word;
                word = lex->next_token();
                if(parse_standard_type(std_type_type))
                {
                    //update type of all declared variables, function iterates
                    stab.update_type(std_type_type);

                    //parse completes
                    //cout << "Parse of variabledecl succeeds" << endl;
                    return true;
                }
                else
                {
                    //parse of standard_type fails
                    //cout << "Parse of variabledecl fails" << endl;
                    return false;
                }
            }
            else
            {
                //expected :
                string *expected = new string("':'");
                parse_error(expected, word);
                //cout << "Parse of variabledecl fails" << endl;
                return false;
            }
        }
        else
        {
            //parse of id list fails
            //cout << "Parse of variabledecl fails" << endl;
            return false;
        }
    }
    else
    {
        //parse error - expected id
        string *expected = new string ("TOKEN_ID");
        parse_error(expected, word);
        //cout << "Parse of variabledecl fails" << endl;
        return false;
    }
    //cout << "Parse of variabledecl fails" << endl;
    return false;
}

bool Parser::parse_procedure_decl_list()
{
    //Predict is procedure and lambda

    if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE)
    {
        if(parse_procedure_decl())
        {
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
            {
                //continue, get next word
                delete word;
                word = lex->next_token();
                if(parse_procedure_decl_list())
                {
                    //parse succeeds
                    //cout << "Parse of proceduredecllist succeeds" << endl;
                    return true;
                }
                else
                {
                    //parse of proceduredecl fails
                    //cout << "Parse of proceduredecllist fails" << endl;
                    return false;
                }
            }
            else
            {
                //expected ';'
                string *expected = new string("';'");
                parse_error(expected, word);
                //cout << "Parse of proceduredecllist fails" << endl;
                return false;
            }
        }
        else
        {
            //parse of procedure decl failed
            //cout << "Parse of proceduredecllist fails" << endl;
            return false;
        }
    }
    else
    {
        //take the lambda transition
        //cout << "Parse of proceduredecllist succeeds" << endl;
        return true;
    }
    //cout << "Parse of proceduredecllist fails" << endl;
    return false;
}

bool Parser::parse_identifier_list()
{
    //predict is only identifier

    if(word->get_token_type() == TOKEN_ID)
    {
        //Perform the semantic analysis
        string *id_name = static_cast<IdToken *>(word)->get_attribute();
        if(stab.is_decl(id_name, current_env))
        {
            multiply_defined_identifier(id_name);
        }
        else
        {
            stab.install(id_name, current_env, UNKNOWN_T);
        }


        //get next token and continue
        delete word;
        word = lex->next_token();
        if(parse_identifier_list_prm())
        {
            //cout << "Parse of idlist succeeds" << endl;
            return true;
        }
        else
        {
            //cout << "Parse of idlist fails" << endl;
            //parse of idlistprm failed
            return false;
        }
    }
    else
    {
        //parse failed
        string *expected = new string("TOKEN_ID");
        parse_error(expected, word);
        //cout << "Parse of idlist fails" << endl;
        return false;
    }
    //cout << "Parse of idlist fails" << endl;
    return false;
}

bool Parser::parse_standard_type(expr_type &standard_type_type)
{
    //must see int or bool for a successful parse

    if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_INT)
    {
        // return int type
        standard_type_type = INT_T;


        delete word;
        word = lex->next_token();
        //cout << "Parse of stdtype succeeds" << endl;
        return true;
    }
    else if(word->get_token_type() == TOKEN_KEYWORD &&
            static_cast<KeywordToken *>(word)->get_attribute() == KW_BOOL)
    {
        //return bool type
        standard_type_type = BOOL_T;

        delete word;
        word = lex->next_token();
        //cout << "parse of stdtype succeeds" << endl;
        return true;
    }
    else
    {

        //parse error, expected int or bool
        string *expected = new string("KW_INT or KW_BOOL");
        parse_error(expected, word);
        //cout << "Parse of stdtype fails" << endl;
        return false;
    }
    //cout << "Parse of stdtype fails" << endl;
    return false;
}

bool Parser::parse_identifier_list_prm()
{
    //predict consists of , and lambda

    if(word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
    {
        //continue, get the next word
        delete word;
        word = lex->next_token();
        if(word->get_token_type() == TOKEN_ID)
        {
            //Perform the semantic analysis
            string *id_name = static_cast<IdToken *>(word)->get_attribute();
            if(stab.is_decl(id_name, current_env))
            {
                multiply_defined_identifier(id_name);
            }
            else if(parsing_formal_parm_list)
            {
                stab.install(id_name, current_env, UNKNOWN_T, formal_parm_position);
                formal_parm_position++;
            }
            else
            {
                stab.install(id_name, current_env, UNKNOWN_T);
            }

            //continue, get next word
            delete word;
            word = lex->next_token();
            if(parse_identifier_list_prm())
            {
                //cout << "Parse of idlistprm succeeds" << endl;
                return true;
            }
            else
            {
                //cout << "Parse of idlistprm fails" << endl;
                return false;
            }
        }
        else
        {
            //expected id
            string *expected = new string("TOKEN_ID");
            parse_error(expected, word);
            //cout << "Parse of idlistprm fails" << endl;
            return false;
        }
    }
    else
    {
        //lambda transition
        //cout << "Parse of idlistprm succeeds" << endl;
        return true;
    }
    //cout << "Parse of idlistprm fails" << endl;
    return false;
}

bool Parser::parse_block()
{
    // the only token in the predict set is begin

    if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_BEGIN)
    {
        //consume token and continue
        delete word;
        word = lex->next_token();
        if(parse_stmt_list())
        {
            //parse of statement list succeds, continue
            if(word->get_token_type() == TOKEN_KEYWORD &&
                static_cast<KeywordToken *>(word)->get_attribute() == KW_END)
            {
                //parse succeeds, consume token and return true
                delete word;
                word = lex->next_token();
                //cout << "Parse of block succeeds" << endl;
                return true;
            }
            else
            {
                //expected end
                string *expected = new string("END");
                parse_error(expected, word);
                //cout << "Parse of block fails" << endl;
                return false;
            }
        }
        else
        {
            //parse of stmt_list fails
            //cout << "Parse of block fails" << endl;
            return false;
        }
    }
    else
    {
        //expected begin found word
        string *expected = new string("BEGIN");
        parse_error(expected, word);
        //cout << "Parse of block fails" << endl;
        return false;
    }
}

bool Parser::parse_procedure_decl()
{
    //procedure is the only thing in the predict set

    if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_PROCEDURE)
    {
        //found procedure, next token and continue
        delete word;
        word = lex->next_token();
        if(word->get_token_type() == TOKEN_ID)
        {
            /* Semantic Analysis */
            string *id_name = static_cast<IdToken *>(word)->get_attribute();
            if(stab.is_decl(id_name, current_env))
            {
                multiply_defined_identifier(id_name);
            }
            else
            {
                stab.install(id_name, current_env, PROCEDURE_T);
                current_env = id_name;
                formal_parm_position = 0;
            }

            //next token and continue
            delete word;
            word = lex->next_token();
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
            {
                //found (, next token and continue
                delete word;
                word = lex->next_token();
                if(parse_procedure_args())
                {
                    //continue, next token should be )
                    if(word->get_token_type() == TOKEN_PUNC &&
                        static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
                    {
                        //found ), get next word
                        delete word;
                        word = lex->next_token();
                        if(parse_variable_decl_list())
                        {
                            if(parse_block())
                            {
                                /* Semantic Analysis */
                                current_env = main_env;

                                //parse succeeds
                                //cout << "Parse of proceduredecl succeeds" << endl;
                                return true;
                            }
                            else
                            {
                                //parse of block fails
                                //cout << "Parse of proceduredecl" << endl;
                                return false;
                            }
                        }
                        else
                        {
                            //parse of variable decl list fails
                            //cout << "Parse of proceduredecl fails" << endl;
                            return false;
                        }
                    }
                    else
                    {
                        //expected )
                        string *expected = new string(")");
                        parse_error(expected, word);
                        //cout << "Parse of proceduredecl fails" << endl;
                        return false;
                    }

                }
                else
                {
                    //parse of procedureargs fails
                    //cout << "Parse of proceduredecl fails" << endl;
                    return false;
                }
            }
            else
            {
                //expected (, parse fails
                string *expected = new string("(");
                parse_error(expected, word);
                //cout << "Parse of proceduredecl fails" << endl;
                return false;
            }
        }
        else
        {
            //expected id, parse fails
            string *expected = new string("IDENTIFIER");
            parse_error(expected, word);
            //cout << "Parse of proceduredecl fails" << endl;
            return false;
        }
    }
    else
    {
        //expected procedure, parse fails
        string *expected = new string("PROCEDURE");
        parse_error(expected, word);
        //cout << "Parse of proceduredecl fails" << endl;
        return false;
    }
}

bool Parser::parse_procedure_args()
{
    //formal_parm_list and lambda make up the predict set
    if(word->get_token_type() == TOKEN_ID)
    {
        //Semantic
        parsing_formal_parm_list = true;
        if(parse_formal_parm_list())
        {
            //semantic
            parsing_formal_parm_list = false;
            //cout << "Parse of procedure args succeeds" << endl;
            return true;
        }
        else
        {
            //cout << "Parse of procedureargs fails" << endl;
            return false;
        }
    }
    else
    {
        //take the lambda transition
        //cout << "Parse of procedureargs succeeds" << endl;
        return true;
    }
}

bool Parser::parse_formal_parm_list()
{
    //identifier is the only thing in the predict set

    if(word->get_token_type() == TOKEN_ID)
    {
        /* Semantic Analysis */
        string *id_name = static_cast<IdToken *>(word)->get_attribute();
        if(stab.is_decl(id_name, current_env))
        {
            multiply_defined_identifier(id_name);
        }
        else
        {
            stab.install(id_name, current_env, UNKNOWN_T, formal_parm_position);
            formal_parm_position++;
        }

        //consume a token
        delete word;
        word = lex->next_token();
        if(parse_identifier_list_prm())
        {
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_COLON)
            {
                //consume token
                delete word;
                word = lex->next_token();
                //create variable to get output from standard type
                expr_type std_type_type = GARBAGE_T;
                if(parse_standard_type(std_type_type))
                {
                    /* Semantic Analysis */
                    stab.update_type(std_type_type);

                    if(parse_formal_parm_list_hat())
                    {
                        //parse succeeds
                        //cout << "Parse of formalparmlist succeeds" << endl;
                        return true;
                    }
                    else
                    {
                        //parse of formalparmlisthat failed
                        //cout << "Parse of formalparmlist fails" << endl;
                        return false;
                    }
                }
                else
                {
                    //parse of standard type failed
                    //cout << "Parse of formalparmlist fails" << endl;
                    return false;
                }
            }
            else
            {
                //expected :
                string *expected = new string(":");
                parse_error(expected, word);
                //cout << "Parse of formalparmlist fails" << endl;
                return false;
            }
        }
        else
        {
            //parse of idlistprm failed
            //cout << "Parse of formalparmlist fails" << endl;
            return false;
        }
    }
    else
    {
        //expected identifier
        string *expected = new string("IDENTIFIER");
        parse_error(expected, word);
        //cout << "Parse of formalparmlist fails" << endl;
        return false;
    }
}

bool Parser::parse_formal_parm_list_hat()
{
    //predict set contains ; and lambda

    if(word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
    {
        //found the ;, consume a token
        delete word;
        word = lex->next_token();
        if(parse_formal_parm_list())
        {
            //parse succeeds
            //cout << "Parse of formalparmlisthat succeeds" << endl;
            return true;
        }
        else
        {
            //parse of formal parm list failed
            //cout << "Parse of formalparmlisthat fails" << endl;
            return false;
        }
    }
    else
    {
        //take the lambda transition
        //cout << "Parse of formalparmlisthat succeeds" << endl;
        return true;
    }
}

bool Parser::parse_stmt_list()
{
    //predict set contains id, if, while, print | ;

    if((word->get_token_type() == TOKEN_KEYWORD &&
        (static_cast<KeywordToken *>(word)->get_attribute() == KW_IF ||
        static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE ||
        static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)) ||
        word->get_token_type() == TOKEN_ID)
    {
        if(parse_stmt())
        {
            //check for semicolon
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
            {
                //found ; consume token
                delete word;
                word = lex->next_token();
                if(parse_stmt_list_prm())
                {
                    //parse succeeds
                    //cout << "Parse of stmtlist succeeds" << endl;
                    return true;
                }
                else
                {
                    //parse of stmt list prm failed
                    //cout << "Parse of stmtlist fails" << endl;
                    return false;
                }
            }
            else
            {
                //expected;
                string *expected = new string(";");
                parse_error(expected, word);
                //cout << "Parse of stmtlist fails" << endl;
                return false;
            }
        }
        else
        {
            //parse of stmt failed
            //cout << "Parse of stmt list fails" << endl;
            return false;
        }
    }
    else if(word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
    {
        //found the semi, consume token
        delete word;
        word = lex->next_token();
        if(parse_stmt_list_prm())
        {
            //parse succeeds
            //cout << "Parse of stmtlist succeeds" << endl;
            return true;
        }
        else
        {
            //parse of stmt list prm failed
            //cout << "Parse of stmtlist fails" << endl;
            return false;
        }
    }
    else
    {
        //parse error, expected id, if, while, print, ;
        string *expected = new string("IDENTIFIER, IF, WHILE, PRINT, ;");
        parse_error(expected, word);
        //cout << "Parse of stmtlist fails" << endl;
        return false;
    }
}

bool Parser::parse_stmt_list_prm()
{
    //predict set is id, if, while, print, lambda
    if((word->get_token_type() == TOKEN_KEYWORD &&
        (static_cast<KeywordToken *>(word)->get_attribute() == KW_IF ||
        static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE ||
        static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)) ||
        word->get_token_type() == TOKEN_ID)
    {
        if(parse_stmt())
        {
            //check for semi
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_SEMI)
            {
                //consume token and continue
                delete word;
                word = lex->next_token();
                if(parse_stmt_list_prm())
                {
                    //parse succeeds
                    //cout << "Parse of stmtlistprm succeeded" << endl;
                    return true;
                }
                else
                {
                    //parse of stmtlistprime failed
                    //cout << "Parse of stmtlistprm fails" << endl;
                    return false;
                }
            }
            else
            {
                //expected semi
                string *expected = new string(";");
                parse_error(expected, word);
                //cout << "Parse of stmtlistprm failed" << endl;
                return false;
            }
        }
        else
        {
            //parse of stmt failed
            //cout << "Parse of stmtlistprm failed" << endl;
            return false;
        }

    }
    else
    {
        //take the lambda transition
        //cout << "Parse of stmtlistprm succeeded" << endl;
        return true;
    }
}

bool Parser::parse_stmt()
{
    //predict set is if, while print, identifier
    if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
    {
        if(parse_if_stmt())
        {
            //parse succeeds
            //cout << "Parse of stmt succeeded" << endl;
            return true;
        }
        else
        {
            //parse of ifstmt failed
            //cout << "Parse of stmt failed" << endl;
            return false;
        }
    }
    else if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
    {
        if(parse_while_stmt())
        {
            //parse succeeds
            //cout << "Parse of stmt succeeded" << endl;
            return true;
        }
        else
        {
            //parse of whilestmt failed
            //cout << "Parse of stmt failed" << endl;
            return false;
        }
    }
    else if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)
    {
        if(parse_print_stmt())
        {
            //parse succeeds
            //cout << "Parse of stmt succeeded" << endl;
            return true;
        }
        else
        {
            //parse of printstmt failed
            //cout << "Parse of stmt failed" << endl;
            return false;
        }
    }
    else if(word->get_token_type() == TOKEN_ID)
    {
        /* Semantic Analysis */
        string *id_name = static_cast<IdToken *>(word)->get_attribute();
        if(!stab.is_decl(id_name, current_env))
        {
            undeclared_identifier(id_name);
        }
        else
        {
            procedure_name = id_name;
        }

        //consume a token and continue
        delete word;
        word = lex->next_token();
        //get the output of the next parse
        expr_type ahpctail_type = GARBAGE_T;
        if(parse_ad_hoc_as_pc_tail(ahpctail_type))
        {
            /* Semantic Analysis*/
            if(ahpctail_type != stab.get_type(id_name, current_env))
            {
                //type error
                type_error(stab.get_type(id_name, current_env), ahpctail_type);
            }

            //parse succeeds
            //cout << "Parse of stmt succeeded" << endl;
            return true;
        }
        else
        {
            //parse of adhocaspctail failed
            //cout << "Parse of stmt failed" << endl;
            return false;
        }
    }
    else
    {
        //expected if, while, print, id
        string *expected = new string("IF, WHILE, PRINT, or IDENTIFIER");
        parse_error(expected, word);
        //cout << "Parse of stmt failed" << endl;
        return false;
    }
}

bool Parser::parse_ad_hoc_as_pc_tail(expr_type &ad_hoc_as_pc_tail_type)
{
    //predict set contains assignment and left paren

    if(word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_ASSIGN)
    {
        //found assignment, consume token
        delete word;
        word = lex->next_token();
        //get the result of parse_expr
        expr_type expr_type_res = GARBAGE_T;
        //get the value of the expression
        Operand *expression;
        if(parse_expr(expr_type_res, expression))
        {
            /* Semantic Analysis*/
            ad_hoc_as_pc_tail_type = expr_type_res;

            /* IR generation */
            //load the value of the expression into a register
            Register *expression_register = NULL;
            if (expression->get_type() == OPTYPE_REGISTER)
            {
                expression_register = expression->get_r_value();
            }
            else
            {
	        /* Allocate a register for the expression value and move it
	        there. */
	        expression_register = allocator->allocate_register();
	        if (expression->get_type() == OPTYPE_IMMEDIATE)
                {
    	            e->emit_move(expression_register, expression->get_i_value());
	        }
                else if (expression->get_type() == OPTYPE_MEMORY)
                {
                    e->emit_move(expression_register, expression->get_m_value());
                }
            }

            //move the register value to procedure_name
            e->emit_move(procedure_name, expression_register);

            //release the register to the pool
            allocator->deallocate_register(expression_register);
            delete expression;


            //parse succeeds
            //cout << "Parse of adhocaspctail succeeded" << endl;
            return true;
        }
        else
        {
            //parse of expr failed
            //cout << "Parse of adhocaspctail failed" << endl;
            return false;
        }
    }
    else if(word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
    {
        /* Semantic Analysis */
        if(stab.get_type(procedure_name, main_env) != PROCEDURE_T)
        {
            type_error(PROCEDURE_T, stab.get_type(procedure_name, main_env));
        }
        actual_parm_position = 0;

        //found '(', consume token and continue
        delete word;
        word = lex->next_token();
        if(parse_expr_list())
        {
            //check for ')'
            if(word->get_token_type() == TOKEN_PUNC &&
                static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
            {
                /* Semantic Analysis */
                ad_hoc_as_pc_tail_type = PROCEDURE_T;

                //consume a token and succeed
                delete word;
                word = lex->next_token();
                //cout << "Parse of adhocaspctail succeeded" << endl;
                return true;
            }
            else
            {
                //expected )
                string *expected = new string(")");
                parse_error(expected, word);
                //cout << "Parse of adhocaspctail failed" << endl;
                return false;
            }
        }
        else
        {
            //parse of expr list failed
            //cout << "Parse of adhocaspctail failed" << endl;
            return false;
        }
    }
    else
    {
        //parse error, expected := or (
        string *expected = new string(":= or (");
        parse_error(expected, word);
        //cout << "Parse of adhocaspctail failed" << endl;
        return false;
    }
}

bool Parser::parse_if_stmt()
{
  if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_IF)
  {

    advance();
    expr_type expr_type_value = GARBAGE_T;
    Operand *expression;
    if (parse_expr(expr_type_value, expression))
    {
      /* Semantic Analysis */
      if (expr_type_value != BOOL_T)
      {
        type_error (BOOL_T, expr_type_value);
      }

      /* IR actions

	    Make sure operand that holds the expr value is in a
	    register.*/
      Register *expression_register = NULL;
      if (expression->get_type() == OPTYPE_REGISTER)
      {
        expression_register = expression->get_r_value();
      }
      else
      {
	      /* Allocate a register for the expression value and move it
	      there. */
	      expression_register = allocator->allocate_register();
	      if (expression->get_type() == OPTYPE_IMMEDIATE)
        {
    	    e->emit_move (expression_register, expression->get_i_value());
	      }
        else if (expression->get_type() == OPTYPE_MEMORY)
        {
          e->emit_move (expression_register, expression->get_m_value());
        }
      }

      /* Generate labels of else part (even if it doesn't exist) and
	    the next statement after the if. */
      string *else_part = e->get_new_label ("else");
      string *if_done = e->get_new_label ("if_done");

      /* Test the register that holds the value of the expression.  If
	    it is false, jump to the else part. */
      e->emit_branch (INST_BREZ, expression_register, else_part);

      /* We are done with the expresion operand and the register in
	    which it resides.  Deallocate both. */
      allocator->deallocate_register (expression_register);
      delete expression;


      if (word->get_token_type() == TOKEN_KEYWORD
	  && static_cast<KeywordToken *>(word)->get_attribute() == KW_THEN)
      {

	      advance();
	      // IR code for then part generated by parse_block().
	      if (parse_block())
        {

          // IR - Skip over else part
          e->emit_branch (if_done);
          e->emit_label (else_part);

          /* IR - if there is an else part to the if statment, the
          code for the else part code will be generated by
          parse_if_stmt_hat(). */
          if (parse_if_stmt_hat())
          {
            // IR - emit label for statement following if statment
            e->emit_label (if_done);
            return true;
          }
          else
          {
            return false;
          }
        }
        else
        {
	        return false;
	      }
      }
      else
      {
        string *expected = new string("THEN");
        parse_error(expected, word);
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    string *expected = new string("IF");
    parse_error(expected, word);
    return false;
  }
}

bool Parser::parse_if_stmt_hat()
{
  if (word->get_token_type() == TOKEN_KEYWORD
      && static_cast<KeywordToken *>(word)->get_attribute() == KW_ELSE)
  {

    advance();

    // IR - code for else part is generated here
    if (parse_block())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return true;
  }
}

bool Parser::parse_while_stmt()
{
    //predict set contains only while
    if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_WHILE)
    {
        //consume token and continue
        delete word;
        word = lex->next_token();
        //get the result of parse_expr
        expr_type expr_type_res = GARBAGE_T;
        //generate the labels for the top of the while and the bottom
        string *while_top = e->get_new_label("beginwhile");
        string *while_bottom = e->get_new_label("endwhile");
        //emit the top label
        e->emit_label(while_top);
        //get the value of the expression
        Operand *expression;
        if(parse_expr(expr_type_res, expression))
        {
            /* Semantic Analysis */
            if(expr_type_res != BOOL_T)
            {
                type_error(BOOL_T, expr_type_res);
            }

            //look for loop
            if(word->get_token_type() == TOKEN_KEYWORD &&
                static_cast<KeywordToken *>(word)->get_attribute() == KW_LOOP)
            {


                //load the value of the expression into a register
                Register *expression_register = NULL;
                if(expression->get_type() == OPTYPE_REGISTER)
                {
                    expression_register = expression->get_r_value();
                }
                else
                {
                    //allocate a register for the expression
                    expression_register = allocator->allocate_register();
                    if(expression->get_type() == OPTYPE_IMMEDIATE)
                    {
                        e->emit_move(expression_register, expression->get_i_value());
                    }
                    else if(expression->get_type() == OPTYPE_MEMORY)
                    {
                        e->emit_move(expression_register, expression->get_m_value());
                    }
                }

                //branch to the end of the loop if the expression is false
                e->emit_branch(INST_BREZ, expression_register, while_bottom);

                //return the register to the pool
                allocator->deallocate_register(expression_register);



                //consume token and continue
                delete word;
                word = lex->next_token();
                if(parse_block())
                {
                    /*IR generation*/
                    //unconditional branch to the top of the loop
                    e->emit_branch(while_top);

                    //emit the end label
                    e->emit_label(while_bottom);

                    //return the register to the pool
                    if(expression_register->is_inuse())
                    {
                        allocator->deallocate_register(expression_register);
                    }
                    delete expression;

                    //parse succeeds
                    //cout << "Parse of whilstmt succeeded" <<  endl;
                    return true;
                }
                else
                {
                    //parse of block failed
                    //cout << "Parse of whilstmt failed" <<  endl;
                    return false;
                }
            }
            else
            {
                //expected loop
                string *expected = new string("LOOP");
                parse_error(expected, word);
                //cout << "Parse of whilstmt failed" <<  endl;
                return false;
            }
        }
        else
        {
            //parse of expr failed
            //cout << "Parse of whilstmt failed" <<  endl;
            return false;
        }
    }
    else
    {
        //expected while
        string *expected = new string("WHILE");
        parse_error(expected, word);
        //cout << "Parse of whilstmt failed" <<  endl;
        return false;
    }
}

bool Parser::parse_print_stmt()
{
    //predict contains only print
    if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_PRINT)
    {
        //consume token and continue
        delete word;
        word = lex->next_token();
        Operand *expression;
        expr_type expr_type_res = GARBAGE_T;
        if(parse_expr(expr_type_res, expression))
        {
            /* Semantic analysis */
            if(expr_type_res != INT_T && expr_type_res != BOOL_T)
            {
                type_error(INT_T, BOOL_T, expr_type_res);
            }

            /* IR Generation */
            Register *expression_register = NULL;
            if(expression->get_type() == OPTYPE_REGISTER)
            {
                expression_register = expression->get_r_value();
            }
            else
            {
              //allocate a register
                expression_register = allocator->allocate_register();
                if(expression->get_type() == OPTYPE_IMMEDIATE)
                {
                    e->emit_move(expression_register, expression->get_i_value());
                }
                else if(expression->get_type() == OPTYPE_MEMORY)
                {
                    e->emit_move(expression_register, expression->get_m_value());
                }
            }

            //now the value to be printed is in expression_register
            //emit the print statement
            e->emit_1addr(INST_OUTB, expression_register);

            //return the register to the pool
            allocator->deallocate_register(expression_register);
            delete expression;

            //cout << "Parse of printstmt succeeded" <<  endl;
            return true;
        }
        else
        {
            //parse of expr failed
            //cout << "Parse of printstmt failed" <<  endl;
            return false;
        }
    }
    else
    {
        //expected print
        string *expected = new string("PRINT");
        parse_error(expected, word);
        //cout << "Parse of printstmt failed" <<  endl;
        return false;
    }
}


bool Parser::parse_expr_list()
{
    //predict set is id, num, (, +, -, not, lambda
    if(word->get_token_type() == TOKEN_ID ||
        word->get_token_type() == TOKEN_NUM ||
        (word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) ||
        (word->get_token_type() == TOKEN_ADDOP &&
        (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD ||
        static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) ||
        (word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
    {
        if(parse_actual_parm_list())
        {
            //parse succeeds
            //cout << "Parse of exprlist succeeded" <<  endl;
            return true;
        }
        else
        {
            //parse of actual parm list failed
            //cout << "Parse of exprlist failed" <<  endl;
            return false;
        }
    }
    else
    {
        //take the lambda transition
        //cout << "Parse of exprlist succeeded" <<  endl;
        return true;
    }

}

bool Parser::parse_actual_parm_list()
{
    //predict set is id, num, (, +, -, not

    if(word->get_token_type() == TOKEN_ID ||
        word->get_token_type() == TOKEN_NUM ||
        (word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) ||
        (word->get_token_type() == TOKEN_ADDOP &&
        (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD ||
        static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) ||
        (word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
    {
        expr_type expr_type_res = GARBAGE_T;
        // just to satisfy the compiler, no
        Operand *expression;
        if(parse_expr(expr_type_res, expression))
        {
            delete expression;
            /* Semantic Analysis */
            if(stab.get_type(procedure_name, actual_parm_position) != expr_type_res)
            {
                type_error(stab.get_type(procedure_name, actual_parm_position),
                    expr_type_res);
            }
            actual_parm_position++;

            if(parse_actual_parm_list_hat())
            {
                //parse succeeds
                //cout << "Parse of actualparmlist succeeded" << endl;
                return true;
            }
            else
            {
                //parse of actualparmlisthat failed
                //cout << "Parse of actualparmlist failed" << endl;
                return false;
            }
        }
        else
        {
            //parse of expr failed
            //cout << "Parse of actualparmlist failed" << endl;
            return false;
        }
    }
    else
    {
        //parse failed
        string *expected = new string("TOKEN ID or TOKEN NUM or ( or + or - or NOT");
        parse_error(expected, word);
        //cout << "Parse of actualparmlist failed" << endl;
        return false;
    }
}

bool Parser::parse_actual_parm_list_hat()
{
    //predict set is , and lambda

    if(word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_COMMA)
    {
        //consume a token and continue
        delete word;
        word = lex->next_token();
        if(parse_actual_parm_list())
        {
            //parse succeeds
            //cout << "Parse of actualparmlisthat succeeded" << endl;
            return true;
        }
        else
        {
            //parse of actual parm list failed
            //cout << "Parse of actualparmlisthat failed" << endl;
            return false;
        }
    }
    else
    {
        //take the lambda transition
        //cout << "Parse of actualparmlisthat succeeded" << endl;
        return true;
    }
}

bool Parser::parse_expr(expr_type &expr_type_result, Operand *&op)
{
    //predict is id, num, (, +, -, not
    if(word->get_token_type() == TOKEN_ID ||
        word->get_token_type() == TOKEN_NUM ||
        (word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) ||
        (word->get_token_type() == TOKEN_ADDOP &&
        (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD ||
        static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) ||
        (word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
    {
        expr_type simple_expr_type = GARBAGE_T;
        expr_type expr_hat_type = GARBAGE_T;
        if(parse_simple_expr(simple_expr_type, op))
        {
            if(parse_expr_hat(expr_hat_type, op))
            {
                /* Semantic analysis */
                if(expr_hat_type == NO_T)
                {
                    expr_type_result = simple_expr_type;
                }
                else if(simple_expr_type == INT_T && expr_hat_type == INT_T)
                {
                    expr_type_result = BOOL_T;
                }
                else
                {
                    type_error(NO_T, INT_T, simple_expr_type);
                }

                //parse succeeded
                //cout << "Parse of expr succeeded" << endl;
                return true;
            }
            else
            {
                //parse of exprhat failed
                //cout << "Parse of expr failed" << endl;
                return false;
            }
        }
        else
        {
            //parse of simpleexpr failed
            //cout << "Parse of expr failed" << endl;
            return false;
        }
    }
    else
    {
        //parse error
        string *expected = new string("TOKEN ID or TOKEN NUM or ( or + or - or NOT");
        parse_error(expected, word);
        //cout << "Parse of expr failed" << endl;
        return false;
    }


}

bool Parser::parse_expr_hat(expr_type &expr_hat_type, Operand *&op)
{
    //predict set is relop and lambda
    if(word->get_token_type() == TOKEN_RELOP)
    {
        relop_attr_type relop_token_type;
        relop_token_type = static_cast<RelopToken *>(word)->get_attribute();
        //consume token and continue
        delete word;
        word = lex->next_token();
        //get the result of parse_simple_expr
        expr_type simple_expr_type = GARBAGE_T;

        /* IR generation */
        Operand *right_op = NULL;

        if(parse_simple_expr(simple_expr_type, right_op))
        {
            /* Semantic Analysis*/
            if(simple_expr_type == INT_T)
            {
                expr_hat_type = INT_T;
            }
            else
            {
                type_error(INT_T, simple_expr_type);
            }

            /* Continue IR Generation */
            Register *left_op_reg;
            if(op->get_type() == OPTYPE_REGISTER)
            {
                left_op_reg = op->get_r_value();
            }
            else
            {
                left_op_reg = allocator->allocate_register();
                if(op->get_type() == OPTYPE_IMMEDIATE)
                {
                    e->emit_move(left_op_reg, op->get_i_value());
                }
                else
                {
                    e->emit_move(left_op_reg, op->get_m_value());
                }
            }

            //the left hand value is in left_op_reg
            delete op;
            op = new Operand(OPTYPE_REGISTER, left_op_reg);

            //generate labels for the true and false cases
            string *expr_true = e->get_new_label("true");
            string *expr_false = e->get_new_label("false");
            string *expr_end = e->get_new_label("end");

            //perform the correct operation depending on the relational operator
            //first calculate the value left op - right op
            switch(right_op->get_type())
            {
                case OPTYPE_REGISTER:
                {
                    e->emit_2addr(INST_SUB, left_op_reg, right_op->get_r_value());
                    break;
                }
                case OPTYPE_IMMEDIATE:
                {
                    e->emit_2addr(INST_SUB, left_op_reg, right_op->get_i_value());
                    break;
                }
                case OPTYPE_MEMORY:
                {
                    e->emit_2addr(INST_SUB, left_op_reg, right_op->get_m_value());
                    break;
                }
                default: break;
            }

            //we are done with the right op
            if (right_op->get_type() == OPTYPE_REGISTER)
            {
	            // IMPORTANT: don't forget to deallocate the register if
	            // you are done with an operand that resides in a
	            // register.  We only have 3 usuable ones.
	            allocator->deallocate_register(right_op->get_r_value());
            }
            delete right_op;
            //the value of left_op - right_op is in left_op (branch on the false condition)
            if(relop_token_type == RELOP_EQ)
            {
                //branch on positive or negative
                e->emit_branch(INST_BRNE, left_op_reg, expr_false);
                e->emit_branch(INST_BRPO, left_op_reg, expr_false);
            }
            else if(relop_token_type == RELOP_NE)
            {
                //branch on zero
                e->emit_branch(INST_BREZ, left_op_reg, expr_false);
            }
            else if(relop_token_type == RELOP_GT)
            {
                //branch on negative or zero
                e->emit_branch(INST_BRNE, left_op_reg, expr_false);
                e->emit_branch(INST_BREZ, left_op_reg, expr_false);
            }
            else if(relop_token_type == RELOP_GE)
            {
                //branch on negative
                e->emit_branch(INST_BRNE, left_op_reg, expr_false);
            }
            else if(relop_token_type == RELOP_LT)
            {
                //branch on positive or zero
                e->emit_branch(INST_BRPO, left_op_reg, expr_false);
                e->emit_branch(INST_BREZ, left_op_reg, expr_false);
            }
            else if(relop_token_type == RELOP_LE)
            {
                //branch on positive
                e->emit_branch(INST_BRPO, left_op_reg, expr_false);
            }

            //emit the label and code for the true part (include branch to pass false part)
            e->emit_label(expr_true); // unnecessary
            e->emit_move(left_op_reg, 1); // expr is true
            e->emit_branch(expr_end); // jump past the false part


            //emit the code for the false part
            e->emit_label(expr_false);
            e->emit_move(left_op_reg, 0); // expr is false


            //emit the end label
            e->emit_label(expr_end);


            //parse succeeded
            //cout << "Parse of exprhat succeeded" << endl;
            return true;
        }
        else
        {
            //parse of simpleexpr failed
            //cout << "Parse of exprhat failed" << endl;
            return false;
        }
    }
    else
    {
        /* Semantic Analysis*/
        expr_hat_type = NO_T;

        //take the lambda transition
        //cout << "Parse of exprhat succeded" << endl;
        return true;
    }
}

bool Parser::parse_simple_expr(expr_type &simple_expr_type, Operand *&op)
{
    //predict set is id, num, (, +, -, not
    if(word->get_token_type() == TOKEN_ID ||
        word->get_token_type() == TOKEN_NUM ||
        (word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) ||
        (word->get_token_type() == TOKEN_ADDOP &&
        (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD ||
        static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) ||
        (word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
    {
        expr_type term_type = GARBAGE_T;
        expr_type simple_expr_prm_type = GARBAGE_T;
        if(parse_term(term_type, op))
        {
            if(parse_simple_expr_prm(simple_expr_prm_type, op))
            {
                /* Semantic Analysis*/
                if(simple_expr_prm_type == NO_T)
                {
                    simple_expr_type = term_type;
                }
                else if(term_type == simple_expr_prm_type)
                {
                    simple_expr_type = term_type;
                }
                else
                {
                    //ATTN: this could be wrong
                    type_error(NO_T, simple_expr_prm_type, term_type);
                }

                //parse succeeds
                //cout << "Parse of simpleexpr succeeded" << endl;
                return true;
            }
            else
            {
                //parse of simpleexprprime failed
                //cout << "Parse of simpleexpr failed" << endl;
                return false;
            }
        }
        else
        {
            //parse of term failed
            //cout << "Parse of simpleexpr failed" << endl;
            return false;
        }
    }
    else
    {
        //parse error
        string *expected = new string("TOKEN ID or TOKEN NUM or ( or + or - or NOT");
        parse_error(expected, word);
        //cout << "Parse of simpleexpr failed" << endl;
        return false;
    }
}

bool Parser::parse_simple_expr_prm(expr_type &simple_expr_prm_type, Operand *&op)
{
    //predict set is addop and lambda
    if(word->get_token_type() == TOKEN_ADDOP)
    {
        expr_type addop_type = GARBAGE_T;
        //get the type of the addop
        if((static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB) ||
            (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD))
        {
            addop_type = INT_T;
        }
        else if(static_cast<AddopToken *>(word)->get_attribute() == ADDOP_OR)
        {
            addop_type = BOOL_T;
        }

        //determine the instruction
        inst_type instruction;
        if(static_cast<AddopToken *>(word)->get_attribute() == ADDOP_OR ||
            static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
        {
            instruction = INST_ADD;
        }
        else if(static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
        {
            instruction = INST_SUB;
        }

        advance();

        expr_type term_type = GARBAGE_T;
        expr_type simple_expr_prm_1_type = GARBAGE_T;
        Operand *right_op = NULL;
        if(parse_term(term_type, right_op))
        {

            /* IR Generation */
            //Move the left operand into a register
            Register *left_op_reg;
            if(op->get_type() == OPTYPE_REGISTER)
            {
                left_op_reg = op->get_r_value();
            }
            else
            {
                left_op_reg = allocator->allocate_register();
                if(op->get_type() == OPTYPE_IMMEDIATE)
                {
                    e->emit_move(left_op_reg, op->get_i_value());
                }
                else
                {
                    e->emit_move(left_op_reg, op->get_m_value());
                }
            }
            //the value of the left operand is in left_op_reg
            delete op;
            op = new Operand(OPTYPE_REGISTER, left_op_reg);



            //output the appropriate instruction
            switch(right_op->get_type())
            {
                case OPTYPE_REGISTER:
                {
                    e->emit_2addr(instruction, op->get_r_value(), right_op->get_r_value());
                    break;
                }
                case OPTYPE_IMMEDIATE:
                {
                    e->emit_2addr(instruction, op->get_r_value(), right_op->get_i_value());
                    break;
                }
                case OPTYPE_MEMORY:
                {
                    e->emit_2addr(instruction, op->get_r_value(), right_op->get_m_value());
                    break;
                }
                default: break;
            }

            //we are now done with the right operand object
            if(right_op->get_type() == OPTYPE_REGISTER)
            {
                allocator->deallocate_register(right_op->get_r_value());
            }
            delete right_op;


            if(parse_simple_expr_prm(simple_expr_prm_1_type, op))
            {
                /* Semantic Analysis*/
                if(simple_expr_prm_1_type == NO_T)
                {
                    if(addop_type == term_type)
                    {
                        simple_expr_prm_type = addop_type;
                    }
                    else
                    {
                        type_error(addop_type, term_type);
                    }
                }
                else if(addop_type == term_type && term_type == simple_expr_prm_1_type)
                {
                    simple_expr_prm_type = addop_type;
                }
                else
                {
                    type_error(addop_type, term_type);
                }

                //cout << "Parse of simpleexprprm succeeded" << endl;
                return true;
            }
            else
            {
                //parse of simpleexprprm failed
                //cout << "Parse of simpleexprprm failed" << endl;
                return false;
            }
        }
        else
        {
            //parse of term failed
            //cout << "Parse of simpleexprprm failed" << endl;
            return false;
        }
    }
    else
    {
        /* Semantic Analysis*/
        simple_expr_prm_type = NO_T;

        //take the lambda transition
        //cout << "Parse of simpleexprprm succeeded" << endl;
        return true;
    }
}

bool Parser::parse_term(expr_type &term_type, Operand *&op)
{
    //predict set is id, num, (, +, -, not
    if(word->get_token_type() == TOKEN_ID ||
        word->get_token_type() == TOKEN_NUM ||
        (word->get_token_type() == TOKEN_PUNC &&
        static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN) ||
        (word->get_token_type() == TOKEN_ADDOP &&
        (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD ||
        static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)) ||
        (word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
    {
        expr_type factor_type = GARBAGE_T;
        expr_type term_prm_type = GARBAGE_T;
        //get the operand value from factor
        if(parse_factor(factor_type, op))
        {
            //send and get the operand value from factor
            if(parse_term_prm(term_prm_type, op))
            {
                /* Semantic Analysis */
                if(term_prm_type == NO_T)
                {
                    term_type = factor_type;
                }
                else if(factor_type == term_prm_type)
                {
                    term_type = factor_type;
                }
                else
                {
                    type_error(factor_type, term_type);
                }

                //cout << "Parse of term succeeded" << endl;
                return true;
            }
            else
            {
                //parse of termprm failed
                //cout << "Parse of term failed" << endl;
                return false;
            }
        }
        else
        {
            //parse of factor failed
            //cout << "Parse of term failed" << endl;
            return false;
        }
    }
    else
    {
        //parse error
        string *expected = new string("TOKEN ID or TOKEN NUM or ( or + or - or NOT");
        parse_error(expected, word);
        //cout << "Parse of term failed" << endl;
        return false;
    }

}

bool Parser::parse_term_prm(expr_type &term_prm0_type, Operand *&left_op)
{
  expr_type factor_type = GARBAGE_T,
  term_prm1_type = GARBAGE_T;

  Operand *right_op = NULL;

  if (word->get_token_type() == TOKEN_MULOP)
  {

    /* Semantic Analysis */
    expr_type mulop_type = GARBAGE_T;
    mulop_attr_type mulop_attr = MULOP_NO_ATTR;
    mulop_attr = static_cast<MulopToken *>(word)->get_attribute();
    if (mulop_attr == MULOP_MUL || mulop_attr == MULOP_DIV)
    {
      mulop_type = INT_T;
    }
    else
    {
      mulop_type = BOOL_T;
    }

    advance();

    if (parse_factor(factor_type, right_op))
    {

      /* At this point, we can generate code for

	 "right_op operation left_op".

	 First we need to make sure the left operand (which
	 was passed to us as a parm) is in a register.  If the left op
	 is in a register, grab the register.  If the left operand is
	 not in a register, allocate a register and move the operand
	 there.
      */
      Register *left_op_reg;
      if (left_op->get_type() == OPTYPE_REGISTER)
      {
	    // The left operand is already in a register.
	    left_op_reg = left_op->get_r_value();
      }
      else
      {
	    // Allocate a new register and move the left op into it.
	    left_op_reg = allocator->allocate_register();
	    if (left_op->get_type() == OPTYPE_IMMEDIATE)
        {
	      e->emit_move (left_op_reg, left_op->get_i_value());
	    }
        else
        { // left factor op is in memory
	      e->emit_move (left_op_reg, left_op->get_m_value());
	    }
	    // We are done with the Operand object that our parent node
	    // gave us, and it no longer describes the left operand.
	    // Delete it and make a new Operand object representing the
	    // current state of the left operand, which is now in a
	    // register.
	    delete left_op;
	    left_op = new Operand (OPTYPE_REGISTER, left_op_reg);
      }

      /* Output IR to perform the operation.

	  First, determine which mulop we the program has called for.
      */
      inst_type instruction;
      if (mulop_attr == MULOP_MUL || mulop_attr == MULOP_AND)
      {
	    instruction = INST_MUL;
      }
      else
      {
	    instruction = INST_DIV;
      }

      /* Now, output the appropriate instruction, depending on the
	  location (immediate, register, memory) of the right hand op.
       */
      switch (right_op->get_type())
      {
        case OPTYPE_REGISTER:
	      e->emit_2addr (instruction, left_op->get_r_value(),
		       right_op->get_r_value());
	      break;
        case OPTYPE_IMMEDIATE:
	      e->emit_2addr (instruction, left_op->get_r_value(),
		       right_op->get_i_value());
	      break;
        case OPTYPE_MEMORY:
	      e->emit_2addr (instruction, left_op->get_r_value(),
		       right_op->get_m_value());
	      break;
        default:
	      break;
      }
      /* Clean up.

	  We are done with the right Operand object.
      */
      if (right_op->get_type() == OPTYPE_REGISTER)
      {
	    // IMPORTANT: don't forget to deallocate the register if
	    // you are done with an operand that resides in a
	    // register.  We only have 3 usuable ones.
	    allocator->deallocate_register(right_op->get_r_value());
      }
      delete right_op;

      // Send left_op to next step in expression parse.
      if (parse_term_prm(term_prm1_type, left_op))
      {

	    /* Semantic Analysis cont. */
	    if (term_prm1_type == NO_T && mulop_type == factor_type)
        {
	      term_prm0_type = mulop_type;
	    }
        else if (mulop_type == factor_type && factor_type == term_prm1_type)
        {
	      term_prm0_type = mulop_type;
	    }
        else
        {
	      type_error (mulop_type, factor_type, term_prm1_type);
	    }

	    return true;
      }
      else
      {
	    return false;
      }
    }
    else
    {
      return false;
    }

  }
  else
  { // -> \lambda

    /* Semantic action */
    term_prm0_type = NO_T;

    return true;
  }
}

bool Parser::parse_factor(expr_type &factor0_type, Operand *&op)
{
  if (word->get_token_type() == TOKEN_ID)
  {

    /* Semantic Analysis */
    string *id_name = new string(*static_cast<IdToken *>(word)->get_attribute());
    expr_type id_type = GARBAGE_T;
    if (stab.is_decl (id_name, current_env))
    {
      id_type = stab.get_type (id_name, current_env);
    }
    else
    {
      undeclared_identifier (id_name);
    }
    // semantic action
    factor0_type = id_type;
    // ir action
    op = new Operand (OPTYPE_MEMORY, id_name);
    delete id_name;

    advance();
    return true;

  }
  else if (word->get_token_type() == TOKEN_NUM)
  {

    // semantic action
    factor0_type = INT_T;
    /* IR action

       Make a new Operand object to represent the literal we just
       found.

       There is a slight complication here.  NumToken attributes are
       stored as strings in the token, but we want them as ints in the
       operand.  We do the conversion here. */
    stringstream ss (*(static_cast<NumToken *>(word)->get_attribute()));
    int op_val;
    ss >> op_val;
    op = new Operand (OPTYPE_IMMEDIATE, op_val);

    advance();
    return true;

  }
  else if (word->get_token_type() == TOKEN_PUNC
	     && static_cast<PuncToken *>(word)->get_attribute() == PUNC_OPEN)
  {
    advance();

    // Parse the expression between the (), discover the expression
    // type, and create an Operand object for it.
    expr_type expr_type_result = GARBAGE_T;
    if (parse_expr(expr_type_result, op))
    {
      if (word->get_token_type() == TOKEN_PUNC
	     && static_cast<PuncToken *>(word)->get_attribute() == PUNC_CLOSE)
      {
	    advance();

	    factor0_type = expr_type_result;

	    return true;

      }
      else
      {
        string * expected = new string(")");
	    parse_error (expected, word);
	    return false;
      }
    }
    else
    {
      return false;
    }

  }
  else if ((word->get_token_type() == TOKEN_ADDOP
	     && (static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD
		 || static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB))
	     || (word->get_token_type() == TOKEN_KEYWORD
		 && static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT))
  {

    // Hack here.  We need to save the unary operator for IR code
    // generation, later on in parse_factor().  We will
    // use an int flag to do it.  0 == '+', 1 == '-', 2 == 'not'.
    int sign_operation;
    if (word->get_token_type() == TOKEN_ADDOP
	&& static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
    {
      sign_operation = 0;
    }
    else if (word->get_token_type() == TOKEN_ADDOP
	       && static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
    {
      sign_operation = 1;
    }
    else
    {
      sign_operation = 2;
    }

    // semantic action
    expr_type sign_type = GARBAGE_T, factor1_type = GARBAGE_T;

    if (parse_sign(sign_type))
    {
      if (parse_factor(factor1_type, op))
      {

	    /* semantic action */
	    if (sign_type != factor1_type)
        {
	      type_error (sign_type, factor1_type);
	    }
	    factor0_type = factor1_type;

	    /* At this point, we need to generate code to apply the sign
	    operation to the operand.  If the sign is a "+", that's a
	    no-op.  If it's a "-" or NOT, we need to move the operand
	    into a register and then generate the instruction to peform
	    the appropriate operation. */
	    if (sign_operation == 0)
        { // sign is '+'
	      // do nothing
	    }
        else
        {

	      // make sure operand is in a register
	      Register *op_register;
	      if (op->get_type() == OPTYPE_REGISTER)
              {
	      op_register = op->get_r_value();
	      }
          else
          {

	        op_register = allocator->allocate_register();

	        // Emit instruction to move operand into register
	        if (op->get_type() == OPTYPE_IMMEDIATE)
            {
	            // move Rn, #immediate_value
	            e->emit_move(op_register, op->get_i_value());
	        }
            else if (op->get_type() == OPTYPE_MEMORY)
            {
	            // move RN, #memory_location_name
	            e->emit_move (op_register, op->get_m_value());
	        }

	        // The operand is now in register op_register.  Make a new
	        // operand object to represent it.
	        delete op;
	        op = new Operand (OPTYPE_REGISTER, op_register);
          } // op not in a register

	      // Finally, emit the instruction to perform the SIGN
	      // operation ('+' or "or" only).
	      if (sign_operation == 1)
          { // sign is '-'
	        e->emit_1addr (INST_NEG, op->get_r_value());
	      }
          else if (sign_operation == 2)
          {  // sign is 'not'
	        e->emit_1addr (INST_NOT, op->get_r_value());
	      }

        } // operation is "-" or "NOT"

	    return true;
      }
      else
      {
	    return false;
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool Parser::parse_sign(expr_type &sign_type)
{
    //predict is +, -, not
    if(word->get_token_type() == TOKEN_ADDOP &&
        static_cast<AddopToken *>(word)->get_attribute() == ADDOP_ADD)
    {
        /* Semantic Analysis*/
        sign_type = INT_T;

        //consume token and return true
        delete word;
        word = lex->next_token();
        return true;
    }
    else if(word->get_token_type() == TOKEN_ADDOP &&
            static_cast<AddopToken *>(word)->get_attribute() == ADDOP_SUB)
    {
        /* Semantic Analysis*/
        sign_type = INT_T;

        //consume and return true
        delete  word;
        word = lex->next_token();
        return true;
    }
    else if(word->get_token_type() == TOKEN_KEYWORD &&
        static_cast<KeywordToken *>(word)->get_attribute() == KW_NOT)
    {
        /* Semantic Analysis*/
        sign_type = BOOL_T;

        //consume and return true
        delete word;
        word = lex->next_token();
        return true;
    }
    else
    {
        //parse error
        string *expected = new string("+ or - or NOT");
        parse_error(expected, word);
        //cout << "Parse of sign failed" << endl;
        return false;
    }
}

void Parser::parse_error(string *expected, Token *found)
{
    cerr << "Found " << *(found->to_string()) << " Expected: " << *expected << endl;
    delete expected;
}

void Parser::undeclared_identifier(string *id)const
{
    cerr << "The identifier " << *id << " has not been declared. " << endl;
    exit (-1);
}
