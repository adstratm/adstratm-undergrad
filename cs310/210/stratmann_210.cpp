#include <iostream>
#include <sstream>

using namespace std;
typedef unsigned int uint;

int greatestPower( uint n, uint & counter );

/** A program to find the largest power of 2
  * less than a given number.
  * @Author Andrew Stratmann
  * Adapted from an algorithm provided by
  * Dr. Jon Beck
  * @version 9 February 2016
  */
int main( int argc, char * argv [] )
{   
    if( argc != 2 )
    {
        cerr << "Usage: " << argv[0] << 
            " n where n is the number of elements" << endl;
        return 1;
    }
    
    stringstream ss( argv[ 1 ]); // get the command line parameter
    uint input = 0;
    ss >> input;

    uint operationCounter = 0;
    uint & opsNo = operationCounter;
    cout << "The highest power of 2 in " << input << 
        " is " << greatestPower( input, opsNo ) << "." << endl;
        
    cerr << input << " " << opsNo << endl;
    
    return 0;
}

/** A function to return the greatest power of 
  * 2 less than a given input, n.
  * @param n The number in question
  * @param counter A pointer to a uint to keep of how many basic ops occur
  */
int greatestPower( uint n, uint & counter )
{
    
    uint i = n;
    uint j = i & (i - 1);
    counter++;
    while( j != 0 ) 
    {
        i = j;
        j = i & (i - 1);
        counter++;
    }
    
    return i;
}
