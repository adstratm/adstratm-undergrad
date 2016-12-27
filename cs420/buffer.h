#ifndef BUFFER_H
#define BUFFER_H
/** buffer.h
  * A class to hold characters, strip whitespace and comments, 
  * and unread a character after a read
  * @author Andrew Stratmann
  * @author Dr. Bob Matthews
  * @version 3 October 2016
  */

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <list>

/* We will use the dollar sign to mark the end of the file in the
   buffer. */
#define EOF_MARKER '$'

#define COMMENT_MARKER '#'
#define SPACE ' '
#define TAB '\t'
#define NEW_LINE '\n'

using namespace std;

class Buffer
{
 public:

  /** Create a new buffer
    * @param filename The filepath of the source file
    */
  Buffer(char *filename);
  
  /** Remove a buffer object
    */
  ~Buffer();
  
  /** Get the next character on the buffer
    * @return The next character
    */
  char next_char();
  
  /** Unreads a character from the buffer
    * @param c The character to be placed back on the buffer
    */
  void unread_char (char c);

  
  
 private:

  static const int MAX_BUFFER_SIZE = 1024;

  char storage_buffer[MAX_BUFFER_SIZE];

  int current_position;
  bool is_full;
  bool first_time;
 
  // The stream object for the source file.
  ifstream source_file;
  
  /* If something catastrophic happens in the buffer, print
     an error message and then call this method to exit. */
  void buffer_fatal_error() const;
  
  /** Determines if a character is whitespace
    * @param c The character in question
    * @return The whitespace status of c
    */
  inline bool is_whitespace (const char c)
  {
    return (c == SPACE || c == TAB || c == NEW_LINE);
  }

  /** Determines if a character is alphanumeric
    * @param c The character in question
    * @return The alphanumeric status of c
    */
  inline bool is_alphanum(const char c)
  {
    return ((c >= 'a' && c <= 'z')|(c >= 'A' && c <= 'Z')|(c >= '0' && c <= '9'))?true:false;
  }
  
  /** Add a character to the buffer
    * @param d The character to be added
    */
  void add_next_char(char d);

  /** Compresses whitespace into a single space
    */
  void compress_whitespace();

  /** Fills the buffer until the end of the file or the buffer
    * is completely filled.
    */
  void fill_buffer();

};

#endif
