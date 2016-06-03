/**
 * calculate the optimum number of coins to make an amount of money
 * @author Jon Beck
 * @author Andrew Stratmann
 * @version 3 April 2016
 */
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <climits>
#include "matrix.h"
using namespace std;
typedef unsigned int uint;

// set infinity to one less than the maximum so we can add one to
// infinity and not overflow around to zero
const uint INF = UINT_MAX - 1;

/**
 * the recursive, memoized optimum function for making an amount of money
 * using coins
 * @param i the index of the denomination vector
 * @param a the amount of money we're trying to make
 * @param denom the vector of coin denominations denom[0] is always 1
 * @param memo the memo table. rows is indexed by i, columns is indexed
 * by amounts of money
 * @return the optimum (minimum) number of coins to make this amount, using
 * coin denominations at or below i
 */
uint opt( uint i, uint a, 
          const vector< uint > & denom, 
          const Matrix< uint > & memo )
{
  // base cases are built into the memo table with entries of INF
  if( memo.at(i, a) != INF )
    return memo.at(i, a);

  if( a < denom.at(i) )
  {
    memo.at(i, a) = opt( i - 1, a, denom, memo );
    return memo.at(i, a);
  }

  memo.at(i, a) = min( opt( i, a - denom.at(i), denom, memo ) + 1,
                       opt( i - 1, a, denom, memo ));

  return memo.at(i, a);
}

/**
 * get the amount of money to be made from the command line
 * set up the denominations and the memo table
 * print the results
 */
int main( int argc, char * argv [] )
{
  if( argc != 2 )
  {
    cerr << "Usage: " << argv[0] << " a where a is the amount of money"
         << endl;
    return 1;
  }

  stringstream ss( argv[ 1 ]); // get the command line parameter
  uint amount = 0;
  ss >> amount;

  vector< uint > denom{1, 6, 10};
  Matrix< uint > memo( denom.size(), amount + 1 );
  for( uint row = 1; row < denom.size(); row++ )
    for( uint col = 1; col <= amount; col++ )
      memo.at( row, col ) = INF;

  for( uint row = 0; row < denom.size(); row++ )
    memo.at( row, 0 ) = 0;

  for( uint col = 0; col <= amount; col++ )
    memo.at( 0, col ) = col;

  uint result = opt( denom.size() - 1, amount, denom, memo );

  cout << "Using denominations";
  for( uint i = 0; i < denom.size(); i++ )
    cout << " " << denom.at(i);

  cout << endl << "The optimal number of coins for " <<
    amount << " is " << result << endl;

  cout << endl << "The completed memo table: " << endl << endl;

  for( unsigned int row = 0; row < memo.numrows(); row++ )
  {
    for( unsigned int col = 0; col < memo.numcols(); col++ )
    {
      if( memo.at( row, col ) == INF )
        cout << "  -";
      else
        cout << setw( 3 ) << memo.at( row, col );
    }
    cout << endl;
  }
  
  // Newly added code begins here
  
  vector< uint > coins_used;
  for( uint i = 0; i < denom.size(); i ++)
  {
    coins_used.push_back( 0 );
  }
  int currentRow = denom.size() - 1;
  int currentCol = amount;
  
  while( currentCol > 0 && currentRow >= 0 )
  {
    if(currentRow > 0 && ( memo.at( currentRow, currentCol ) ==
        memo.at( currentRow - 1, currentCol ) ) )
    {
      currentRow = currentRow - 1;
    }
    else
    {
      coins_used.at( currentRow ) = coins_used.at( currentRow ) + 1;
      currentCol = currentCol - denom.at( currentRow );
    } 
  }

  cout << endl;
  cout << "Coins used: " << endl;
  for( uint j = 0; j < coins_used.size(); j++ )
  {
    cout << denom.at( j ) << ": " << coins_used.at( j ) << endl;
  }
  
  return 0;
}
