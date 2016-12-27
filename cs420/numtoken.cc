/** numtoken.cc
  * An implementation of numtoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "numtoken.h"

NumToken::NumToken()
{
    this->set_token_type( TOKEN_NUM );
}

NumToken::NumToken( const string& attr )
{
    this->set_token_type( TOKEN_NUM );
    attribute = new string(attr);
}

NumToken::~NumToken()
{
    this->~Token();
}

string * NumToken::get_attribute()
{
    return attribute;
}

void NumToken::set_attribute( const string& attr )
{
    (*attribute) = attr;
}

string * NumToken::to_string() const
{
    string * output;
    //must re-allocate memory for larger string
    output = new string("NUMBER: " + *attribute);
    return output;
}

