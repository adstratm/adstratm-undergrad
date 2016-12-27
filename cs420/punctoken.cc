/** punctoken.cc
  * An implementation of punctoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "punctoken.h"

PuncToken::PuncToken()
{
    this->set_token_type( TOKEN_PUNC );
    attribute = PUNC_NO_ATTR;
}

PuncToken::~PuncToken()
{
    this->~Token();
}

PuncToken::PuncToken( punc_attr_type attr )
{
    this->set_token_type( TOKEN_PUNC );
    attribute = attr;
}

punc_attr_type PuncToken::get_attribute()
{
    return attribute;
}

void PuncToken::set_attribute( punc_attr_type attr )
{
    attribute = attr;
}

string * PuncToken::to_string() const
{
    string *output;
    if(attribute == PUNC_SEMI)
    {
        output = new string("PUNCTUATION SEMICOLON");
    }
    else if(attribute == PUNC_COLON)
    {
        output = new string("PUNCTUATION COLON");
    }
    else if(attribute == PUNC_COMMA)
    {
        output = new string("PUNCTUATION COMMA");
    }
    else if(attribute == PUNC_ASSIGN)
    {
        output = new string("PUNCTUATION ASSIGNMENT");
    }
    else if(attribute == PUNC_OPEN)
    {
        output = new string("PUNCTUATION OPEN");
    }
    else if(attribute == PUNC_CLOSE)
    {
        output = new string("PUNCTUATION CLOSE");
    }
    else
    {
        output = new string("PUNCTUATION NO ATTRIBUTE");
    }
    return output;
}
