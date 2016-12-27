/** keywordtoken.cc
  * An implementation of keywordtoken.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "keywordtoken.h"

KeywordToken::KeywordToken()
{
    this->set_token_type( TOKEN_KEYWORD );
    attribute = KW_NO_ATTR;
}

KeywordToken::~KeywordToken()
{
    this->~Token();
}

KeywordToken::KeywordToken( keyword_attr_type attr )
{
    this->set_token_type( TOKEN_KEYWORD );
    attribute = attr;
}

keyword_attr_type KeywordToken::get_attribute()
{
    return attribute;
}

void KeywordToken::set_attribute( keyword_attr_type attr )
{
    attribute = attr;
}

string * KeywordToken::to_string() const
{
    string *output;
    if(attribute == KW_PROGRAM)
    {
        output = new string("KEYWORD PROGRAM");
    }
    else if(attribute == KW_PROCEDURE)
    {
        output = new string("KEYWORD PROCEDURE");
    }
    else if(attribute == KW_INT)
    {
        output = new string("KEYWORD INT");
    }
    else if(attribute == KW_BOOL)
    {
        output = new string("KEYWORD BOOL");
    }
    else if(attribute == KW_BEGIN)
    {
        output = new string("KEYWORD BEGIN");
    }
    else if(attribute == KW_END)
    {
        output = new string("KEYWORD END");
    }
    else if(attribute == KW_IF)
    {
        output = new string("KEYWORD IF");
    }
    else if(attribute == KW_THEN)
    {
        output = new string("KEYWORD THEN");
    }
    else if(attribute == KW_ELSE)
    {
        output = new string("KEYWORD ELSE");
    }
    else if(attribute == KW_WHILE)
    {
        output = new string("KEYWORD_WHILE");
    }
    else if(attribute == KW_LOOP)
    {
        output = new string("KEYWORD LOOP");
    }
    else if(attribute == KW_PRINT)
    {
        output = new string("KEYWORD PRINT");
    }
    else if(attribute == KW_NOT)
    {
        output = new string("KEYWORD NOT");
    }
    else
    {
        output = new string("KEYWORD NO ATTRIBUTE");
    }
    return output;
}
