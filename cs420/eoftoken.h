/** eoftoken.h
  * A class for TruPL end-of-file tokens
  * @author Dr. Bob Matthews
  * @author Andrew Stratmann
  * @version 16 December 2016
  */
#ifndef TOKENEOF_H
#define TOKENEOF_H

using namespace std;

#include "token.h"

// The EofToken has no attributes.

class EofToken : public Token
{
 public:
  EofToken ();
  ~EofToken ();

  string *to_string() const;

};

#endif
