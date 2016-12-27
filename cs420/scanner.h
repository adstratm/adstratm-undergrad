#ifndef SCANNER_H
#define SCANNER_H

/** Scanner.h
  * A lexical analyzer class for recognizing TruPL tokens
  * @author Andrew Stratmann
  * @version 3 October 2016
  */

#include <iostream>
#include <stdlib.h>

// The scanner reads from the buffer.
#include "buffer.h"

// The scanner returns objects from the Token class when 
// next_token() is called.
#include "token.h"
#include "keywordtoken.h"
#include "punctoken.h"
#include "reloptoken.h"
#include "addoptoken.h"
#include "muloptoken.h"
#include "idtoken.h"
#include "numtoken.h"
#include "eoftoken.h"

using namespace std;

class Scanner
{
 public:
  /** Construct a scanner object
    * @param filename The filepath of the source code
    */
  Scanner (char *filename);

  /** Removes a scanner object
    */
  ~Scanner();

  /** Get the next token in the source file
    * @return The next token
    */
  Token *next_token();

 private:

  //The buffer that stores data from the source file
  Buffer *buf;

  /** Determines if a character is alphabetic
    * @param c The character in question
    * @return The alphabetic status of c
    */
  inline bool is_alpha (char c) const 
  {
    return c >= 'a' && c <= 'z';
  }

  /** Determines if a character is a digit
    * @param c The character in question
    * @return The digit status of c
    */
  inline bool is_digit (char c) const
  {
    return c >= '0' && c <= '9';
  }

  /** Determines if a character is alphanumeric
    * @param c The character in question
    * @return The alphanumeric status of c
    */
  inline bool is_alphanum (char c) const
  {
    return is_alpha(c) || is_digit(c);
  }

  /** Determines if a character is a space
    * @param c The character in question
    * @return The space status of c
    */
  inline bool is_space(char c) const
  {
    return c == ' ';
  }

  /** Prints an error message and terminates the program
    * Used when invalid characters are encountered
    * @param mesg The error message to be printed
    */
  void scanner_fatal_error(const string& mesg);

};
#endif
