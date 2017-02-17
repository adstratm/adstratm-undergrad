/** register.cc
  * An implementation of register.h
  * @author Dr. Bob Matthews
  * @author Andrew Stratmann
  * @version 16 December 2016
  */
#include "register.h"

Register::Register(int num)
{
	this->num = num;
	inuse = false;
}

Register::~Register()
{
}

int Register::get_num()
{
	return num;
}

void Register::set_inuse()
{
	inuse = true;
}

void Register::clear_inuse()
{
	inuse = false;
}

bool Register::is_inuse()
{
	return inuse;
}
