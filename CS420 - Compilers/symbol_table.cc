#include "symbol_table.h"

Symbol_Table::Symbol_Table()
{
  stab_size = 0;
}


Symbol_Table::~Symbol_Table()
{
}


void Symbol_Table::install (const string *id, const string *env, 
			    const expr_type t)
{
  /* Install an identifier from environment env with type t into
     symbol table.  Does not check for duplicates. */

  STAB_ENTRY *new_entry = new STAB_ENTRY;
  new_entry->id = *id;
  new_entry->env = *env;
  new_entry->position = -1;
  new_entry->type = t;
  stab.push_back (*new_entry);
  stab_size++;
  //cout << "Entry Name: " << new_entry->id << endl;
  //cout << "Entry Environment: " << new_entry->env << endl;
  //cout << "Entry Position: " << new_entry->position << endl;
  //cout << "Entry Type: " << new_entry->type << endl << endl;
}


void Symbol_Table::install (const string *id, const string *env, 
				 const expr_type t, const int pos)
{
  /* Install an identifier from environment env with type t into
     symbol table.  Does not check for duplicates. */

  STAB_ENTRY *new_entry = new STAB_ENTRY;
  new_entry->id = *id;
  new_entry->env = *env;
  new_entry->position = pos;
  new_entry->type = t;
  stab_size++;
  //cout << "Entry Name: " << new_entry->id << endl;
  //cout << "Entry Environment: " << new_entry->env << endl;
  //cout << "Entry Position: " << new_entry->position << endl;
  //cout << "Entry Type: " << new_entry->type << endl << endl;
  stab.push_back (*new_entry);

  
}

bool Symbol_Table::is_decl (const string *id, const string *env)
{
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->id.compare(*id) == 0 && it->env.compare(*env) == 0) {
      return true;
    }
  }
  return false;
}


expr_type Symbol_Table::get_type (string *id, string *env)
{
  // Return the type of identifier id of environment env.  Results in
  // garbage garbage type if (*id, *env) are not in the table.
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->id.compare(*id) == 0 && it->env.compare(*env) == 0) {
      return it->type;
    }
  }
  
  return GARBAGE_T;
}


expr_type Symbol_Table::get_type (string *proc_id, const int pos)
{
  /* Get the type of the formal parameter in the indicated position of 
     the procedure with name proc_id.  The name of the procedure will
     be the identifier's environment in the symbol table. */
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->env.compare(*proc_id) == 0 
	&& it->position == pos) {
      return it->type;
    }
  }

  return GARBAGE_T;
}

void Symbol_Table::update_type (expr_type standard_type_type)
{
  /* Change the type of all symbol table variables with type UNKNOWN_T
     to standard_type_type. */
  vector<STAB_ENTRY>::iterator it;
  for (it = stab.begin(); it != stab.end(); ++it) {
    if (it->type == UNKNOWN_T) {
      it->type = standard_type_type;
      //cout << "Entry Name: " << it->id << endl;
      //cout << "Entry Environment: " << it->env << endl;
      //cout << "Entry Position: " << it->position << endl;
      //cout << "Entry Type: " << it->type << endl << endl;
    }
  }
}


string *Symbol_Table::type_to_string (const expr_type t) const
{
  string *result;

  switch (t) {
  case INT_T:
    result = new string ("INT_T");
    break;
  case BOOL_T:
    result = new string ("BOOL_T");
    break;
  case PROCEDURE_T:
    result = new string ("PROCEDURE_T");
    break;
  case PROGRAM_T:
    result = new string ("PROGRAM_T");
    break;
  case NO_T:
    result = new string ("NO_T");
    break;
  case UNKNOWN_T:
    result = new string ("UNKNOWN_T");
    break;
  default:
    result = new string ("GARBAGE_T");
    break;
  }

  return result;
}

int Symbol_Table::get_size()
{
    return stab_size;
}

string *Symbol_Table::get_id_at_position(int pos)
{
    return &(stab.at(pos).id);
}

bool Symbol_Table::is_intbool_at_position(int pos)
{
    if(stab.at(pos).type == INT_T || stab.at(pos).type == BOOL_T)
    {
        return true;
    }
    else
    {
        return false;
    }
}
