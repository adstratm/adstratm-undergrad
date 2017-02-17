#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
typedef unsigned int uint;


int main( int argc, char * argv [] )
{
  if( argc != 2 )
  {
      cerr << "Usage: " << argv[0] << " n where n is the number of elements" << endl;
      return 1;
  }

  stringstream ss( argv[ 1 ]); // get the command line parameter
  uint n = 0;
  ss >> n;

  //cout << "you entered " << n << " on the command line" << endl;
  
  
  //create a list of integers
  vector< uint > integer_vector;
  
  //add an integer to the list, until 'n' have been added
  for(int i = 0; i < n; i++)
  {
      integer_vector.push_back( 0 );
  }
  
  
  return 0;
}
