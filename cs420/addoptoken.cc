/** addoptoken.cc
  * An implementation of addoptoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "addoptoken.h"

AddopToken::AddopToken()
{
    this->set_token_type( TOKEN_ADDOP );
    attribute = ADDOP_NO_ATTR;
}

AddopToken::AddopToken( addop_attr_type attr )
{
    this->set_token_type( TOKEN_ADDOP );
    attribute = attr;
}

AddopToken::~AddopToken()
{
    this->~Token();
}

addop_attr_type AddopToken::get_attribute()
{
    return attribute;
}

void AddopToken::set_attribute( addop_attr_type attr )
{
    attribute = attr;
}

string * AddopToken::to_string() const
{
    string *output;
    if(attribute == ADDOP_ADD)
    {
        output = new string("ADDITIVE OPERATOR ADDITION");
    }
    else if(attribute == ADDOP_SUB)
    {
        output = new string("ADDITIVE OPERATOR SUBTRACTION");
    }
    else if(attribute == ADDOP_OR)
    {
        output = new string("ADDITIVE OPERATOR OR");
    }
    else
    {
        output = new string("ADDITIVE OPERATOR NO ATTRIBUTE");
    }
    return output;
}
