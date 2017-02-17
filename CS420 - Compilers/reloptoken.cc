/** reloptoken.cc
  * An implementation of reloptoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "reloptoken.h"

RelopToken::RelopToken()
{
    this->set_token_type( TOKEN_RELOP );
    attribute = RELOP_NO_ATTR;
}

RelopToken::~RelopToken()
{
    this->~Token();
}

RelopToken::RelopToken( relop_attr_type attr )
{
    this->set_token_type( TOKEN_RELOP );
    attribute = attr;
}

relop_attr_type RelopToken::get_attribute()
{
    return attribute;
}

void RelopToken::set_attribute( relop_attr_type attr )
{
    attribute = attr;
}

string * RelopToken::to_string() const
{
    string *output;
    if(attribute == RELOP_EQ)
    {
        output = new string("RELATIONAL OPERATOR EQUAL");
    }
    else if(attribute == RELOP_NE)
    {
        output = new string("RELATIONAL OPERATOR NOT EQUAL");
    }
    else if(attribute == RELOP_GT)
    {
        output = new string("RELATIONAL OPERATOR GREATER THAN");
    }
    else if(attribute == RELOP_GE)
    {
        output = new string("RELATIONAL OPERATOR GREATER OR EQUAL");
    }
    else if(attribute == RELOP_LT)
    {
        output = new string("RELATIONAL OPERATOR LESS THAN");
    }
    else if(attribute == RELOP_LE)
    {
        output = new string("RELATIONAL OPERATOR LESS OR EQUAL");
    }
    else
    {
        output =  new string("RELATIONAL OPERATOR NO ATTRIBUTE");
    }
    return output;
}

