/** scanner.cc
  * An implementation of scanner.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "scanner.h"

#define EXIT_STATE 999

Scanner::Scanner (char *filename)
{
  // Create and fill the buffer.
  buf = new Buffer (filename);
}

Scanner::~Scanner ()
{
  delete buf;
}

Token *Scanner::next_token()
{
  Token *lexeme = NULL;
  char c;
  int state = 0;
  std::string currentId = "";

  //get the first char
  while(state != EXIT_STATE)
  {
    switch(state)
    {
      case 0:
      {
        // we know nothing, get a char
        c = buf->next_char();
        //easy ones first:
        if((c == ' ')|(c == 0))
        {
          state = 0;
          break;
        }
        else if(c == ':')
        {
          state = 1;
          break;
        }
        else if((c == ';')|(c == ',')|(c == '(')|(c == ')')|
                (c == '=')|(c == '/')|(c == '*')|(c == '+')|
                (c == '-'))
        {
          if(c == ';')
          {
            lexeme = new PuncToken(PUNC_SEMI);
            state = EXIT_STATE;
            break;
          }
          else if(c == ',')
          {
            lexeme = new PuncToken(PUNC_COMMA);
            state = EXIT_STATE;
            break;
          }
          else if(c == '(')
          {
            lexeme = new PuncToken(PUNC_OPEN);
            state = EXIT_STATE;
            break;
          }
          else if(c == ')')
          {
            lexeme = new PuncToken(PUNC_CLOSE);
            state = EXIT_STATE;
            break;
          }
          else if(c == '=')
          {
            lexeme = new RelopToken(RELOP_EQ);
            state = EXIT_STATE;
            break;
          }
          else if(c == '/')
          {
            lexeme = new MulopToken(MULOP_DIV);
            state = EXIT_STATE;
            break;
          }
          else if(c == '*')
          {
            lexeme = new MulopToken(MULOP_MUL);
            state = EXIT_STATE;
            break;
          }
          else if(c == '+')
          {
            lexeme = new AddopToken(ADDOP_ADD);
            state = EXIT_STATE;
            break;
          }
          else if(c == '-')
          {
            lexeme = new AddopToken(ADDOP_SUB);
            state = EXIT_STATE;
            break;
          }
        }
        else if(c == '<')
        {
          state = 3;
          break;
        }
        else if(c == '>')
        {
          state = 4;
          break;
        }
        else if(c == 'a')
        {
          state = 5;
          break;
        }
        else if(c == 'l')
        {
          state = 6;
          break;
        }
        else if(c == 'o')
        {
          state = 7;
          break;
        }
        else if(c == 'n')
        {
          state = 8;
          break;
        }
        else if(c == 'b')
        {
          state = 9;
          break;
        }
        else if(is_digit(c))
        {
          //we have seen a digit
          currentId += c;
          state = 10;
          break;
        }
        else if(c == 'w')
        {
          state = 11;
          break;
        }
        else if(c == 't')
        {
          state = 12;
          break;
        }
        else if(c == 'e')
        {
          state = 13;
          break;
        }
        else if(c == 'i')
        {
          state = 14;
          break;
        }
        else if(c == 'p')
        {
          state = 15;
          break;
        }
        else if(is_alpha(c))
        {
          state = 2;
          currentId += c;
          break;
        }
        else if((c == EOF_MARKER)|(c == '$'))
        {
          state = 17;
          break;
        }
        else if(is_space(c))
        {
          state = 0;
          //redundant
          break;
        }
        else
        {
          //invalid character
          scanner_fatal_error("INVALID CHARACTER");
          break;
        }
      }
      case 1:
      {
        //we have seen a ':'
        c = buf->next_char();
        if(c == '=')
        {
          state = 18;
          break;
        }
        else
        {
          buf->unread_char(c);
          lexeme = new PuncToken(PUNC_COLON);
          state = EXIT_STATE;
          break;
        }
      }
      case 2:
      {
	//case for dealing with identifiers
        c = buf->next_char();
        if((c == ' ')|(c == ';')|(c == ',')|(c == '(')|(c == ')')|
                (c == '=')|(c == '/')|(c == '*')|(c == '+')|
                (c == '-')|(c == ':')|(c == '<')|(c == '>'))
        {
          buf->unread_char(c);
          lexeme = new IdToken(currentId);
          state = EXIT_STATE;
          break;
        }
        else if(is_alphanum(c))
        {
          currentId += c;
          state = 2;
          break;
        }
        else
        {
          //non-allowed character - error message
          //cout << "The problematic character: " << (int)c << endl;
          scanner_fatal_error("NONVALID CHARACTER");
          break;
        }
      }
      case 3:
      {
        //we have seen a '<'
        c = buf->next_char();
        if(c == '=')
        {
          state = 19;
          break;
        }
        else if(c == '>')
        {
          state = 20;
          break;
        }
        else
        {
          buf->unread_char(c);
          lexeme = new RelopToken(RELOP_LT);
          state = EXIT_STATE;
          break;
        }
      }
      case 4:
      {
        //we have seen a '>'
        c = buf->next_char();
        if(c == '=')
        {
          state = 21;
          break;
        }
        else
        {
          buf->unread_char(c);
          lexeme = new RelopToken(RELOP_GT);
          state = EXIT_STATE;
          break;
        }
      }
      case 5:
      {
        //we have seen an 'a'
        c = buf->next_char();
        if(c == 'n')
        {
          state = 22;
          break;
        }
        else
        {
          //create an identifier with attribute 'a' and move to state for ids
          buf->unread_char(c);
          currentId += 'a';
          state = 2;
          break;
        }
      }
      case 6:
      {
        //we have seen an 'l'
        c = buf->next_char();
        if(c == 'o')
        {
          state = 23;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += 'l';
          state = 2;
          break;
        }
      }
      case 7:
      {
        //we have seen an 'o'
        c = buf->next_char();
        if(c == 'r')
        {
          state = 24;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += 'o';
          state = 2;
          break;
        }
      }
      case 8:
      {
        //we have seen an 'n'
        c = buf->next_char();
        if(c == 'o')
        {
          state = 25;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += 'n';
          state = 2;
          break;
        }
      }
      case 9:
      {
        //we have seen a 'b'
        c = buf->next_char();
        if(c == 'e')
        {
          state = 26;
          break;
        }
        else if(c == 'o')
        {
          state = 27;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += 'b';
          state = 2;
          break;
        }
      }
      case 10:
      {
        //we have started a number
        c = buf->next_char();
        if(is_alpha(c))
        {
          scanner_fatal_error("IDENTIFIERS CANNOT START WITH DIGITS");
          break;
        }
        else if(is_digit(c))
        {
          currentId += c;
          state = 10;
          break;
        }
        else
        {
          buf->unread_char(c);
          lexeme = new NumToken(currentId);
          state = EXIT_STATE;
          break;
        }
      
      }
      case 11:
      {
        //we have seem a 'w'
        c = buf->next_char();
        if(c == 'h')
        {
          state = 28;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "w";
          state = 2;
          break;
        }
      }
      case 12:
      {
        //we have seen a 't'
        c = buf->next_char();
        if(c == 'h')
        {
          state = 29;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "t";
          state = 2;
          break;
        }
      }
      case 13:
      {
        //we have seen an 'e'
        c = buf->next_char();
        if(c == 'l')
        {
          state = 30;
          break;
        }
        else if(c == 'n')
        {
          state = 31;
          break;
        }
        else
        {
          //identifier with an 'e'
          buf->unread_char(c);
          state = 2;
          currentId += "e";
          break;
      }
      case 14:
      {
        //we have seen an 'i'
        c = buf->next_char();
        if(c == 'f')
        {
          state = 32;
          break;
        }
        else if(c == 'n')
        {
          state = 33;
          break;
        }
        else
        {
          buf->unread_char(c);
          state = 2;
          currentId += "i";
          break;
        }
      }
      case 15:
      {
        //we have seen a 'p'
        c = buf->next_char();
        if(c == 'r')
        {
          state = 34;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "p";
          state = 2;
          break;
        }
      }
      case 16:
      {
        //currently unused
      }
      case 17:
      {
        //create eof token
        lexeme = new EofToken();
        state = EXIT_STATE;
        break;
      }
      case 18:
      {
        //we have seen ':='
        lexeme = new PuncToken(PUNC_ASSIGN);
        state = EXIT_STATE;
        break;
      }
      case 19:
      {
        //we have seen '<='
        lexeme = new RelopToken(RELOP_LE);
        state = EXIT_STATE;
        break;
      }
      case 20:
      {
        //we have seen '<>'
        lexeme = new RelopToken(RELOP_NE);
        state = EXIT_STATE;
        break;
      }
      case 21:
      {
        //we have seen '>='
        lexeme = new RelopToken(RELOP_GE);
        state = EXIT_STATE;
        break;
      }
      case 22:
      {
        //we have seen 'an'
        c = buf->next_char();
        if(c == 'd')
        {
          state = 35;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "an";
          state = 2;
          break;
        }
      }
      case 23:
      {
        //we have seen 'lo'
        c = buf->next_char();
        if(c == 'o')
        {
          state = 36;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "lo";
          state = 2;
          break;
        }
      }
      case 24:
      {
        //we have seen 'or'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new AddopToken(ADDOP_OR);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "or";
          state = 2;
          break;
        }
      }
      case 25:
      {
        //we have seen 'no'
        c = buf->next_char();
        if(c == 't')
        {
          state = 37;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "no";
          state = 2;
          break;
        }
      }
      case 26:
      {
        //we have seen 'be'
        c = buf->next_char();
        if(c == 'g')
        {
          state = 38;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "be";
          state = 2;
          break;
        }
      }
      case 27:
      {
        //we have seen 'bo'
        c = buf->next_char();
        if(c == 'o')
        {
          state = 39;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "bo";
          state = 2;
          break;
        }
      }
      case 28:
      {
        //we have seen 'wh'
        c = buf->next_char();
        if(c == 'i')
        {
          state = 40;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "wh";
          state = 2;
          break;
        }
      }
      case 29:
      {
        //we have seen 'th'
        c = buf->next_char();
        if(c == 'e')
        {
          state = 41;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "th";
          state = 2;
          break;
        }
      }
      case 30:
      {
        //we have seen 'el'
        c = buf->next_char();
        if(c == 's')
        {
          state = 42;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "el";
          state = 2;
          break;
        }
      }
      case 31:
      {
        //we have seen 'en'
        c = buf->next_char();
        if(c == 'd')
        {
          state = 43;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "en";
          state = 2;
          break;
        }
      }
      case 32:
      {
        //we have seen 'if'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_IF);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "if";
          state = 2;
          break;
        }
      }
      case 33:
      {
        //we have seen 'in'
        c = buf->next_char();
        if(c == 't')
        {
          state = 44;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "in";
          state = 2;
          break;
        }
      }
      case 34:
      {
        //we have seen 'pr'
        c = buf->next_char();
        if(c == 'i')
        {
          state = 45;
          break;
        }
        else if(c == 'o')
        {
          state = 46;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "pr";
          state = 2;
          break;
        }
      }
      case 35:
      {
        //we have seen 'and'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new MulopToken(MULOP_AND);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "and";
          state = 2;
          break;
        }
      }
      case 36:
      {
        //we have seen 'loo'
        c = buf->next_char();
        if(c == 'p')
        {
          state = 47;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "loo";
          state = 2;
          break;
        }
      }
      case 37:
      {
        //we have seen 'not'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_NOT);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "not";
          state = 2;
          break;
        }
      }
      case 38:
      {
        //we have seen 'beg'
        c = buf->next_char();
        if(c == 'i')
        {
          state = 48;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "beg";
          state = 2;
          break;
        }
      }
      case 39:
      {
        //we have seen 'boo'
        c = buf->next_char();
        if(c == 'l')
        {
          state = 49;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "boo";
          state = 2;
          break;
        }
      }
      case 40:
      {
        //we have seen 'whi'
        c = buf->next_char();
        if(c == 'l')
        {
          state = 50;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "whi";
          state = 2;
          break;
        }
      }
      case 41:
      {
        //we have seen 'the'
        c = buf->next_char();
        if(c == 'n')
        {
          state = 51;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "the";
          state = 2;
          break;
        }
      }
      case 42:
      {
        //we have seen 'els'
        c = buf->next_char();
        if(c == 'e')
        {
          state = 52;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "els";
          state = 2;
          break;
        }
      }
      case 43:
      {
        //we have seen 'end'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_END);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "end";
          state = 2;
          break;
        }
      }
      case 44:
      {
        //we have seen 'int'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_INT);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "int";
          state = 2;
          break;
        }
      }
      case 45:
      {
        //we have seen 'pri'
        c = buf->next_char();
        if(c == 'n')
        {
          state = 53;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "pri";
          state = 2;
          break;
        }
      }
      case 46:
      {
        //we have seen 'pro'
        c = buf->next_char();
        if(c == 'c')
        {
          state = 54;
          break;
        }
        else if(c == 'g')
        {
          state = 55;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "pro";
          state = 2;
          break;
        }
      }
      case 47:
      {
        //we have seen 'loop'
        c = buf->next_char();
        if (!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_LOOP);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "loop";
          state = 2;
          break;
        }
      }
      case 48:
      {
        //we have seen 'begi'
        c = buf->next_char();
        if(c == 'n')
        {
          state = 56;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "begi";
          state = 2;
          break;
        }
      }
      case 49:
      {
        //we have seen 'bool'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_BOOL);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "bool";
          state = 2;
          break;
        }
      }
      case 50:
      {
        //we have seen 'whil'
        c = buf->next_char();
        if(c == 'e')
        {
          state = 57;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "whil";
          state = 2;
          break;
        }
      }
      case 51:
      {
        //we have seen 'then'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_THEN);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "then";
          state = 2;
          break;
        }
      }
      case 52:
      {
        //we have seen 'else'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_ELSE);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "else";
          state = 2;
          break;
        }
      }
      case 53:
      {
        //we have seen 'prin'
        c = buf->next_char();
        if(c == 't')
        {
          state = 58;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "prin";
          state = 2;
          break;
        }
      }
      case 54:
      {
        //we have seen 'proc'
        c = buf->next_char();
        if(c == 'e')
        {
          state = 59;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "proc";
          state = 2;
          break;
        }
      }
      case 55:
      {
        //we have seen 'prog'
        c = buf->next_char();
        if(c == 'r')
        {
          state = 60;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "prog";
          state = 2;
          break;
        }
      }
      case 56:
      {
        //we have seen 'begin'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_BEGIN);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "begin";
          state = 2;
          break;
        }
      }
      case 57:
      {
        //we have seen 'while'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_WHILE);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "while";
          state = 2;
          break;
        }
      }
      case 58:
      {
        //we have seen 'print'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_PRINT);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "print";
          state = 2;
          break;
        }
      }
      case 59:
      {
        //we have seen 'proce'
        c = buf->next_char();
        if(c == 'd')
        {
          state = 61;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "proce";
          state = 2;
          break;
        }
      }
      case 60:
      {
        //we have sen 'progr'
        c = buf->next_char();
        if(c == 'a')
        {
          state = 62;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "progr";
          state = 2;
          break;
        }
      }
      case 61:
      {
        //we have seen 'proced'
        c = buf->next_char();
        if(c == 'u')
        {
          state = 63;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "proced";
          state = 2;
          break;
        }
      }
      case 62:
      {
        //we have seen 'progra'
        c = buf->next_char();
        if(c == 'm')
        {
          state = 64;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "progra";
          state = 2;
          break;
        }
      }
      case 63:
      {
        //we have seen 'procedu'
        c = buf->next_char();
        if(c == 'r')
        {
          state = 65;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "procedu";
          state = 2;
          break;
        }
      }
      case 64:
      {
        //we have seen 'program'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          lexeme = new KeywordToken(KW_PROGRAM);
          buf->unread_char(c);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "program";
          state = 2;
          break;
        }
      }
      case 65:
      {
        //we have seen 'procedur'
        c = buf->next_char();
        if(c == 'e')
        {
          state = 66;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "procedur";
          state = 2;
          break;
        }
      }
      case 66:
      {
        //we have seen 'procedure'
        c = buf->next_char();
        if(!is_alphanum(c))
        {
          buf->unread_char(c);
          lexeme = new KeywordToken(KW_PROCEDURE);
          state = EXIT_STATE;
          break;
        }
        else
        {
          buf->unread_char(c);
          currentId += "procedure";
          state = 2;
          break;
        }
      }
    }
  }
}
  //return the newly created lexeme
  return lexeme;
}

void Scanner::scanner_fatal_error(const string& message)
{
  cerr << "Exiting on Scanner Fatal Error: " << message << endl;
  exit (-1);
}
