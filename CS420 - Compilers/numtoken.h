/** numtoken.h
  * A class for numerical tokens in TruPL
  * @author Dr. Bob Matthews
  * @author Andrew Stratmann
  * @version 16 December 2016
  */
#ifndef NUMTOKEN_H
#define NUMTOKEN_H

using namespace std;

#include "token.h"
#include <string>

class NumToken : public Token
{
 public:
  NumToken();
  NumToken(const string& attr);
  ~NumToken();

  string *get_attribute();
  void set_attribute(const string& attr);

  string *to_string() const;

 private:
  string *attribute;
};

#endif
