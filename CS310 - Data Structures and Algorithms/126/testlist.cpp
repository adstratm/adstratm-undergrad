#include <iostream>
#include <vector>
#include "list_stratmanna.h"

using namespace std;
typedef unsigned int uint;

int main()
{

  List< uint > integer_list;

  integer_list.push_front( 1 );
  integer_list.push_front( 3 );
  integer_list.push_front( 5 );
  integer_list.push_front( 4 );
  integer_list.push_front( 2 );

  integer_list.dump();

  List< string > string_list;

  string_list.push_front( "Ali" );
  string_list.push_front( "Bec" );
  string_list.push_front( "Cai" );
  string_list.push_front( "Dov" );
  cout << "The front element is: " << string_list.front() << endl;
  cout << "The entire list is" << endl;
  string_list.dump();

  string_list.pop_front();
  cout << "After removing the first element" << endl;
  string_list.dump();

  return 0;
}
