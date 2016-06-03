/**
 * A generic 2-dimensional array class
 * written for C++99, but still works in C++11
 * @author Jon Beck
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
typedef unsigned int uint;

template <class Object>
class Matrix
{
 public:
  Matrix( uint rows, uint cols );
  Object & at( uint row, uint col );
  const Object & at( uint row, uint col ) const;
  ~Matrix();
  Matrix( const Matrix<Object> & m ); // Copy constructor
  Matrix & operator= ( const Matrix<Object> & m ); // Assignment operator
  uint numrows() const;
  uint numcols() const;

 private:
  uint rows;
  uint cols;
  Object* data;
};

template <class Object>
Matrix<Object>::Matrix( uint rows, uint cols )
: rows( rows ), cols( cols )
{
  assert( rows > 0 && cols > 0 );
  data = new Object[ rows * cols ];
}

template <class Object>
Matrix<Object>::~Matrix()
{
  delete[] data;
}

template <class Object>
Object & Matrix<Object>::at( uint row, uint col )
{
  assert( row < rows && col < cols );
  return data[ cols * row + col ];
}

template <class Object>
const Object & Matrix<Object>::at( uint row, uint col ) const
{
  assert( row < rows && col < cols );
  return data[ cols * row + col ];
}

template <class Object>
uint Matrix<Object>::numrows() const
{
  return rows;
}

template <class Object>
uint Matrix<Object>::numcols() const
{
  return cols;
}
#endif
