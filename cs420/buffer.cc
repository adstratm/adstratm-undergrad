/** buffer.cc
  * An implementation of buffer.h
  * @author Andrew Stratmann
  * @version 3 October 2016
  */
#include "buffer.h"

Buffer::Buffer (char *filename)
{
  // Open the file and fill the buffer.
  source_file.open (filename);
  is_full = false;
  first_time = true;
  if (source_file.fail()) {
    // Failed to open source file.
    cerr << "Can't open source file " << *filename << endl;
    buffer_fatal_error();
  }

  //fills the buffer the first time
  this->fill_buffer();
}

Buffer::~Buffer()
{
	source_file.close();
}


char Buffer::next_char()
{
  if(current_position < (MAX_BUFFER_SIZE))
  {
    // we aren't at the end of the buffer, get a char
    char current = storage_buffer[current_position];
    current_position++;
    if(current > 0)
    {
        return current;
    }
    else
    {
        return this->next_char();
    }

  }
  else
  {
    // we need to refill
    is_full = false;
    this->fill_buffer();
    char current = storage_buffer[current_position];
    current_position++;
    if(current > 0)
    {
        return current;
    }
    else
    {
        return this->next_char();
    }
  }
}



void Buffer::unread_char( char c )
{
  if(current_position > 0)
  {
    current_position -= 1;
    storage_buffer[current_position] = c;
  }
  else
  {
    cout << "Unread Error!" << endl;
  }
}

//assumes there's room for one more
void Buffer::add_next_char(char d)
{
  current_position++;
  storage_buffer[current_position] = d;
}

//filling algo, adds two chars
void Buffer::compress_whitespace()
{
  //first add one space to the buffer if not already there
  if(storage_buffer[current_position] != ' ')
  {  
    this->add_next_char(SPACE);
  }
  
  //next throw away all whitespace characters,
  // and when a nonwhitespace is encountered exit the function
  char c = source_file.get();
  while(is_whitespace(c) && (c != EOF))
  {
    //do nothing but get the next char in c
    c = source_file.get();
  }
  //at this point c is not whitespace, put it back on source
  source_file.putback(c);
}

//filling algorithm, make sure to not overfill
void Buffer::fill_buffer()
{
  if(is_full)
  {
    cout << "ERROR, buffer already full" << endl;
  }
  else
  {
    //first, take the current character and put it in pos 0
    if(!first_time)
    {
      //after the first filling we need to get the last
      //char in case the user wants to unread
      storage_buffer[0] = storage_buffer[current_position];
    }
    current_position = 1;
    char t;
    int current_state = 0;

    while(((t = source_file.get())!= EOF) &&
         (current_position < MAX_BUFFER_SIZE - 1))
    {
      //as long as there are characters in the source, and room, get
      //them and put in the buffer, starting with position 1
      //this is the place to deal with comments
      switch(current_state)
      {
        case 0:
        {
          if(is_alphanum(t)|(t == '+')|(t == '-')|(t == '*')|(t == ';')
             |(t == ':')|(t == '(')|(t == ')')|(t == ',')|(t == '=')
             |(t == '<')|(t == '>')|(t == '/'))
          {
            //we're done, add the char to buffer and break
            add_next_char(t);
            current_state = 0;
            break;
          }
          else if(is_whitespace(t)|(t == 0))
          {
            //we have a whitespace, compress and move on
            compress_whitespace();
            current_state = 0;
            break;
          }
          else if(t == '#')
          {
            //move to state ready for comments
            current_state = 1;
            compress_whitespace();
            break;
          }
          else
          {
            //illegal character, error message and break
            buffer_fatal_error();
            break;
          }
        }
        case 1:
        {
          // The state for a comment
          if(t != NEW_LINE)
          {
            current_state = 1;
          }
          else
          {
            current_state = 0;
          }
          break;
        }  
      }
    }
    source_file.putback(t);
    //we can't fill any more, set position to 1 and
    //check for end of file
    if((t = source_file.get())== EOF)
    {
      add_next_char( EOF_MARKER );
    }
    source_file.putback(t);
    current_position = 1;
    first_time = false;
  }
}


void Buffer::buffer_fatal_error() const
{
  cerr << "Exiting on BUFFER FATAL ERROR" << endl;
  exit (-1);
}
