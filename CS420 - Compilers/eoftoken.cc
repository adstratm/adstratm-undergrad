/** eoftoken.cc
  * An implementation of eoftoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "eoftoken.h"

EofToken::EofToken()
{
    this->set_token_type( TOKEN_EOF );
}

EofToken::~EofToken()
{
    this->~Token();
}

string * EofToken::to_string() const
{
    string *output;
    output = new string("END OF FILE TOKEN");
    return output;
}
