/**
 * nqueens
 * This program finds all possible arrangements of
 * n queens on an n x n chess board, and also reports
 * on how many nodes were visited to find them.
 * @author Jon Beck
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 21 April 2016
 */
typedef unsigned int uint;
#include <cassert>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "matrix.h"
using namespace std;

/**
 * This function checks to see if the current placement of queens on
 * the board is ok.  It is ok if no queen attacks another queen.  A
 * return of true means no conflict.  A return of false means at least
 * two queens conflict.  This function is improved over the previous version,
 * as when a position is found to be without a queen, the next position is
 * checked instead of wastefully checking the entire board.
 * @param board the n x n chessboard
 * @return the absence of a conflict with this board arrangement
 */
bool ok( const Matrix< bool > & board )
{
  uint n = board.numrows();

  // let row and col range over the entire board
  for( uint row = 0; row < n; row++ )
    for( uint col = 0; col < n; col++ )
      // if row and col designate a queen's position, see if it
      // conflicts with any other queen
      
      // first check to see if there is a queen in the current spot
      // if not, we don't even need to check.
      // this helps us avoid checking the entire board for conflicts
      // when there isn't even a queen in the outer loop position.
      if( board.at( row, col ) )
      {
        // there's a queen in the current position, so we 
        // need to check the other spaces for conflicts
        for(uint tryrow = 0; tryrow < n; tryrow++)
          for(uint trycol = 0; trycol < n; trycol++)
          {
            if( board.at(tryrow, trycol) && !(row == tryrow && col == trycol) )
            {
              // check for the same column
              if( col == trycol )
                return false;
                
              // check for the same row
              if( row == tryrow )
                return false;
              
              // check for same 'up' diagonal
              if( n - col + row == n - trycol + tryrow )
                return false;
                
              // check for same 'down' diagonal
              if( row + col + 1 == tryrow + trycol + 1 )
                return false;
            }
          }
      }
             
  // if we made it here, then no two queens conflict
  return true;
}


/**
 * a simple procedure to output an ASCII art horizontal rule with plus
 * signs where columns will intersect
 */
void hr( uint cols )
{
  cout << "    +";
  for( uint col = 0; col < cols; col++ )
    cout << "---+";
  cout << endl;
}

/**
 * this function dumps the board to std output
 * @param board the board whose arrangement to dump
 */
void printBoard( const Matrix< bool> & board )
{
  hr( board.numrows());
  for( uint row = 0; row < board.numrows(); row++)
  {
    cout << ' ' << setw(2) << board.numrows() - row << " |";
    for( uint col = 0; col < board.numrows(); col++ )
    {
      if( board.at(row, col) )
        cout << " X |";
      else
        cout << "   |";
    }
    cout << endl;
    hr( board.numrows());
  }

  cout << "     ";
  for( uint col = 0; col < board.numrows(); col++ )
    cout << ' ' << (char)( 'a' + col ) << "  ";
  cout << endl;
}


/**
 * This function determines if there is a queen in
 * a specified row.
 * @param theBoard the matrix representing a chess board
 * @param n the row to be checked for queens
 * @return the status of a queen in row 'n' - true if
 * there is a queen
 */
bool hasQueenInRow( const Matrix< bool > & theBoard, uint n )
{
  for( uint col = 0; col < theBoard.numcols(); col++ )
  {
    if( theBoard.at( n, col ) == true )
    {
      return true;
    }
  }
  return false;
}

/**
 * This is the recursive backtracking function.  When called, k queens
 * have already been placed on the board in columns 0 .. k-1.  We're
 * trying to place the next queen in column k.  Note that this function
 * continues to run until all possible solutions are found, not only
 * the first one.
 * @param k the column in which to place the current queen
 * @param board the board on which to place the queen
 * @param ansCount stores the total number of nodes visited
 * @param first the status of the first solution being found
 * @param firstAnsCount stores the number of nodes to find first solution
 */
void r_backtrack( uint k, Matrix< bool > & board, uint & totalAnsCount, bool & first,
 uint & firstAnsCount )
{
  
  // are we done?
  if( k == board.numrows() )
  {
    // if so, report and exit
    printBoard( board );
    // if the first solution hasn't been found yet,
    // note that we have now found it, and store
    // how many nodes were visited to get here
    if( first == false )
    {
      first = true;
      firstAnsCount = totalAnsCount;
    }
  }


  // try each row in turn, for this column
  for( uint row = 0; row < board.numrows(); row++ )
  {
    if( hasQueenInRow( board, row ) == false )
    {
      // we've visited a new node.
      totalAnsCount++;

      
      // put a queen here
      board.at(row, k) = true;
      
      //did that cause a conflict?
      if( ok( board ))
      {
        //keep going
        r_backtrack( k + 1, board, totalAnsCount, first, firstAnsCount );
      }
      //if that didn't work, un-try the current attempt
      board.at(row, k) = false;
    
    }
  }
}


/**
 * set up the board and start the backtracking algorithm at the
 * top of the search tree
 */
int main( int argc, char* argv[] )
{
  if( argc != 2 )
  {
    cout << "Usage: " << argv[0] << " n" << endl;
    cout << "       where n is the number of queens to place" << endl;
    cout << "       on an n x n chessboard, with 0 < n < 26" << endl;
    exit( 2 );
  }
  assert (argc == 2);
  stringstream ss( argv[ 1 ]); 
  uint n;
  ss >> n;
  assert (n > 0 && n < 26);
  
  // these variables store information generated during
  // the recursive function run
  uint totalAnswerCount = 0;
  bool hasFirst = false;
  uint firstAnswerCount = 0;

  // initialize a board with no queens
  Matrix< bool > board( n, n );
  for( uint row = 0; row < n; row++)
    for( uint col = 0; col < n; col++ )
      board.at(row, col) = false;

  // start with column 0
  r_backtrack( 0, board, totalAnswerCount, hasFirst, firstAnswerCount );
  
  // if an answer was found, report on the total number of nodes
  // and the number for the first solution
  if( firstAnswerCount != 0 )
  {
    cout << "The first solution was found after visiting "
     << firstAnswerCount << " nodes. " << endl;
    cout <<"The total number of nodes visited was "
     << totalAnswerCount << "." << endl;
  }
  else
  {
    cout << "No solution" << endl;
  }
  
  exit( 1 );
}
