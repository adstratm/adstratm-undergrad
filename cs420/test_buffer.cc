#include <iostream>
#include <stdlib.h>

#include "buffer.h"

/* A program to print all the TruPL tokens and their attributes in 
   a file.
*/
int main (int argc, char **argv)
{

  char *filename;

  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input file name>" << endl;
    exit (-1);
  }

  filename = argv[1];

  // Declare a Buffer object
  Buffer *b;
  b = new Buffer(filename);

  char c;
  //get each character from the buffer
  do
  {
    c = b->next_char();
    cout << c;
  }while(c != '$');
}
