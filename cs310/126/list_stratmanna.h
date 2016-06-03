#ifndef LIST_STRATMANNA
#define LIST_STRATMANNA
#include <cassert>
#include <iostream>

using namespace std;
typedef unsigned int uint;


/**
 * a simplified generic singly linked list class to illustrate C++ concepts
 * @author Jon Beck
 * modified by Andrew Stratmann
 * @version 24 January 2016
 */
template< typename Object >
class List
{
 private:
  /**
   * A class to store data and provide a link to the next node in the list
   */
  class Node
  {
  public:
    /**
     * The constructor
     * @param data the data to be stored in this node
     */
    explicit Node( const Object& data )
      : data( data ), next( NULL ) {}

    Object data;
    Node * next;
  };

 public:
  /**
   * The constructor for an empty list
   */
  List()
    : size( 0 ), first( NULL ), last( NULL ) {}

  /**
   * disallow the copy constructor
   * @param rhs the list to be copied in
   */
  List( const List & rhs ) = delete;

  /**
   * The destructor that gets rid of everything that's in the list and
   * resets it to empty. If the list is already empty, do nothing.
   */
  ~List()
  {
    if( size != 0 )
    {
      Node * current = first;
      Node * temp;

      while( current != NULL )
      {
        temp = current;
        current = current->next;
        delete temp;
      }
    }
  }
  
  /**
   * disallow the move constructor
   * @param rhs the list to be moved
   */
  List( List && rhs) = delete;
  
  /**
   * The copy assignment operator, set to default.
   * Allows the data behind one object to be copied
   * to another object.
   * @param rhs the list whose data will comprise the new list
   */
  List & operator= ( const List & rhs ) = default;
  
  /**
   * The move assignment operator (disallowed)
   * @param rhs the list to be moved
   *
   */
  List & operator= ( List && rhs ) = delete;
  
  

  /**
   * Put a new element onto the beginning of the list
   * @param item the data the new element will contain
   */
  void push_front( const Object& item )
  {            
      Node * new_node = new Node( item );
      if( size == 0 )
      {
          first = new_node;
          last = new_node;
      }
      else
      {
          (*new_node).next = first;
          first = new_node;
      }
      size++;
    
  }
    
  /**
   * Remove the element that's at the front of the list. Causes an
   * assertion error if the list is empty.
   */
  void pop_front()
  {
    assert( !is_empty() );
    Node * temp = first;

    if( first == last )
    {
      first = last = NULL;
    }
    else
    {
      first = first->next;
    }
    delete temp;
    size--;
  }

  /**
   * Accessor to return the data of the element at the front of the list.
   * Causes an assertion error if the list is empty.
   * @return the data in the front element
   */
  const Object& front()
  {
    assert( !is_empty() );
    return first->data;
  }

  /**
   * Accessor to determine whether the list is empty
   * @return a boolean corresponding to the emptiness of the list
   */
  bool is_empty() const
  {
      if(size == 0)
      {
          return true;
      }
      else
      {
          return false;
      }
  }

  /**
   * Just for debugging, a helper accessor to dump the contents of the list
   * all on one line. Not for production use.
   */
  void dump() const
  {
    Node * current = first;
    for( uint i = 0; i < size; i++ )
    {
      cout << current->data << ' ';
      current = current->next;
    }
    cout << endl;
  }

 private:
  uint size;
  Node * first;
  Node * last;
};


/**
 * Execution Times:
 *      n        list       vector
 *      10000    0:00.00    0:00.00
 *     100000    0:00.01    0:00.00
 *    1000000    0:00.13    0:00.03
 *   10000000    0:01.25    0:00.38
 *  100000000    0:12.43    0:03.54
 * 1000000000    2:36.80    0:33.62
 * uname -a: Darwin andrews-mbp.truman.edu 15.2.0 Darwin Kernel Version 15.2.0:
 * Fri Nov 13 19:56:56 PST 2015; root:xnu-3248.20.55~2/RELEASE_X86_64 x86_64
 */

#endif
