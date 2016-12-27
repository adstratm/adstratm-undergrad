/** muloptoken.cc
  * An implementation of muloptoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "muloptoken.h"

MulopToken::MulopToken()
{
    this->set_token_type( TOKEN_MULOP );
    attribute = MULOP_NO_ATTR;
}

MulopToken::MulopToken( mulop_attr_type attr )
{
    this->set_token_type( TOKEN_MULOP );
    attribute = attr;
}

MulopToken::~MulopToken()
{
    this->~Token();
}

mulop_attr_type MulopToken::get_attribute()
{
    return attribute;
}

void MulopToken::set_attribute( mulop_attr_type attr )
{
    attribute = attr;
}

string * MulopToken::to_string() const
{
    string *output;
    if(attribute == MULOP_MUL)
    {
        output = new string("MULTIPLICATIVE OPERATOR MULTIPLICATION");
    }
    else if(attribute == MULOP_DIV)
    {
        output = new string("MULTIPLICATIVE OPERATOR DIVISION");
    }
    else if(attribute == MULOP_AND)
    {
        output = new string("MULTIPLICATIVE OPERATOR AND");
    }
    else
    {
        output = new string("MULTIPLICATIVE OPERATOR NO ATTRIBUTE");
    }
    return output;
}

