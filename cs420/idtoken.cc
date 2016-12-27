/** idtoken.cc
  * An implementation of idtoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "idtoken.h"

IdToken::IdToken()
{
    this->set_token_type( TOKEN_ID );
}

IdToken::IdToken( const string& attr )
{
    this->set_token_type( TOKEN_ID );
    //must allocate memory to fit the current size
    attribute = new string(attr);
}

IdToken::~IdToken()
{
    this->~Token();
}

string * IdToken::get_attribute()
{
    return attribute;
}

void IdToken::set_attribute( const string& attr )
{
    (*attribute) = attr;
}

string * IdToken::to_string() const
{
    string *output;
    //must reallocate memory to hold larger string
    output = new string("IDENTIFIER: " + *attribute);
    return output;
}
