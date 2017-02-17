/**
 * A simple framework to implement a first-child, right-sibling
 * tree with unlimited children per node.
 * @author Original Author Jon Beck
 * @author Modified by Andrew Stratmann
 * @version 22 February 2016
 */

#include <iostream>
using namespace std;

/**
 * a simple tree_node class
 */
class tree_node
{
public:
  tree_node( char data, tree_node * first_child, tree_node * right_sibling )
    : data( data ), first_child( first_child ), right_sibling( right_sibling )
  {}

  char get_data() const
  {
    return data;
  }

  tree_node * get_first_child() const
  {
    return first_child;
  }

  tree_node * get_right_sibling() const
  {
    return right_sibling;
  }

private:
  char data;
  tree_node * first_child;
  tree_node * right_sibling;
};

void visit( tree_node * node )
{
  cout << node->get_data() << ' ';
}

void preorder( tree_node * node )
{
  if( node != nullptr )
  {
    visit( node );
    preorder( node->get_first_child() );
    preorder( node->get_right_sibling() );
  }
}



void postorder( tree_node * node )
{
    if( node != nullptr )
    {
        postorder( node->get_first_child() );
        visit(node);
        postorder( node->get_right_sibling() );    
    }
}


int main()
{
  tree_node * q = new tree_node{ 'Q', nullptr, nullptr };
  tree_node * p = new tree_node{ 'P', nullptr, q };
  tree_node * o = new tree_node{ 'O', nullptr, p };
  tree_node * n = new tree_node{ 'N', nullptr, nullptr };
  tree_node * m = new tree_node{ 'M', nullptr, n };
  tree_node * l = new tree_node{ 'L', nullptr, nullptr };
  tree_node * k = new tree_node{ 'K', nullptr, l };
  tree_node * j = new tree_node{ 'J', nullptr, k };
  tree_node * i = new tree_node{ 'I', o, nullptr };
  tree_node * h = new tree_node{ 'H', nullptr, i };
  tree_node * g = new tree_node{ 'G', nullptr, h };
  tree_node * f = new tree_node{ 'F', m, nullptr };
  tree_node * e = new tree_node{ 'E', j, nullptr };
  tree_node * d = new tree_node{ 'D', g, e };
  tree_node * c = new tree_node{ 'C', nullptr, d };
  tree_node * b = new tree_node{ 'B', f, c };
  tree_node * a = new tree_node{ 'A', b, nullptr };
  
  
  /* I created another tree to ensure that my postorder traversal
   * method was indeed valid for trees other than the one
   * given.
   */
  tree_node * newj = new tree_node{ 'J', nullptr, nullptr };
  tree_node * newi = new tree_node{ 'I', nullptr, newj };
  tree_node * newh = new tree_node{ 'H', nullptr, nullptr };
  tree_node * newg = new tree_node{ 'G', nullptr, newh };
  tree_node * newf = new tree_node{ 'F', nullptr, newg };
  tree_node * newe = new tree_node{ 'E', nullptr, nullptr };
  tree_node * newd = new tree_node{ 'D', newi, nullptr };
  tree_node * newc = new tree_node{ 'C', newf, newd };
  tree_node * newb = new tree_node{ 'B', newe, newc };
  tree_node * newa = new tree_node{ 'A', newb, nullptr };

  cout << "Preorder for original tree: ";
  preorder( a );
  cout << endl;
  
  cout << "Postorder for original tree: ";
  postorder( a );
  cout << endl;
  
  cout << "Preorder for new (arbitrarily created) tree: ";
  preorder( newa );
  cout << endl;
  
  cout << "Postorder for new (arbitrarily created) tree: ";
  postorder( newa );
  cout << endl;
  
  //Test to ensure that a null tree is traversed without error.
  postorder( nullptr );
  
  

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;
  delete f;
  delete g;
  delete h;
  delete i;
  delete j;
  delete k;
  delete l;
  delete m;
  delete n;
  delete o;
  delete p;
  delete q;
  
  delete newa;
  delete newb;
  delete newc;
  delete newd;
  delete newe;
  delete newf;
  delete newg;
  delete newh;
  delete newi;
  delete newj;

  return 0;
}
