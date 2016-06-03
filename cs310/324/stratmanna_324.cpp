/**
 * Adapted from Weiss 4th page 399
 * dijkstra's algorithm
 * @author Jon Beck
 * @author Andrew Stratmann
 * @version 23 March 2016
 */

#include <iostream>
#include <vector>
#include <climits>
#include <list>
#include <queue>

typedef unsigned int uint;

using namespace std;

/**
 * create a class that can compare two pairs by their second
 * element. boolean operator is reversed because priority_queue
 * creates a max-heap by default
 */
class ComparePair
{
public:
  bool operator() (const pair<uint, uint> & a, const pair<uint, uint> & b )
  {
    return a.second > b.second;
  }
};


/**
 * A recursive method to find the reverse path
 * from a start node to any node in the graph.
 * @param n The end node
 * @param thePath The vector containing path information
 */
void displayPath( uint n, uint thePath[] )
{
  if( thePath[n] == 0 )
  {
    cout << " ";
  }
  else
  {
    cout << thePath[n] << " ";
    displayPath( thePath[n], thePath );
  }
}


/**
 * A function to find the shortest distance and the corresponding
 * paths from a start node.  This function also displays the 
 * correct distances and paths.
 * @param s The starting node
 * @param graph A adjacency lists representation of a connected
 * graph.
 */
void dijkstra( uint s, const vector<list<pair<uint, uint> > > & graph )
{

  uint dist [graph.size()];
  bool known [graph.size()];
  uint path [graph.size()];
  
  priority_queue< pair<uint, uint>, vector<pair<uint, uint> >, ComparePair > pq;
  
  for( uint i = 0; i < graph.size(); i++)
  {
    dist[i] = UINT_MAX;
    known[i] = false;
  }
  
  dist[s] = 0;
  pair<uint, uint> startPair = make_pair( s, dist[s] );
  pq.push( startPair );
  
  for( uint j = 0; j < graph.size(); j++ )
  {
    uint currentVertex = pq.top().first;
    uint currentDistance = pq.top().second;
    pq.pop();
    
    known[currentVertex] = true;
    
    for( auto iter = graph.at(currentVertex).begin();
        iter != graph.at(currentVertex).end(); ++iter )
    {
      if( known[iter->first] == false )
      {
        uint distCVW = iter->second;
        
        if( dist[currentVertex] + distCVW < dist[iter->first] )
        {
          dist[iter->first] = currentDistance + distCVW;
          path[iter->first] = currentVertex;
          pair<uint, uint> nextPair = make_pair( iter->first, dist[iter->first] );
          pq.push( nextPair );
        }
      }
    }
    
  }


  cout << "The distance from " << s << " to:" << endl;
  for( uint i = 1; i < graph.size(); i++ )
  {
    cout << "  " << i << " is " << dist[i] << endl;
  }
  
  cout << "The paths (backwards) are: " << endl;
  for( uint k = 1; k < graph.size(); k++ )
  {
    cout << "v" << k << ": ";
    displayPath( k, path );
    cout << " " << endl;
  }
  
}



int main()
{
  vector<list<pair<uint, uint> > > graph;

  // the 0 position is an unused dummy, Weiss starts nodes at 1
  // in fact, this is just like an unreachable node
  list<pair<uint, uint> > v0list;
  graph.push_back( v0list );

  list<pair<uint, uint> > v1list;
  v1list.push_back( make_pair( 2, 2));
  v1list.push_back( make_pair( 4, 1));
  graph.push_back( v1list );

  list<pair<uint, uint> > v2list;
  v2list.push_back( make_pair( 4, 3 ));
  v2list.push_back( make_pair( 5, 10 ));
  graph.push_back( v2list );

  list<pair<uint, uint> > v3list;
  v3list.push_back( make_pair( 1, 4 ));
  v3list.push_back( make_pair( 6, 5 ));
  graph.push_back( v3list );

  list<pair<uint, uint> > v4list;
  v4list.push_back( make_pair( 3, 2 ));
  v4list.push_back( make_pair( 5, 2 ));
  v4list.push_back( make_pair( 6, 8 ));
  v4list.push_back( make_pair( 7, 4 ));
  graph.push_back( v4list );

  list<pair<uint, uint> > v5list;
  v5list.push_back( make_pair( 7, 6 ));
  graph.push_back( v5list );

  list<pair<uint, uint> > v6list;
  graph.push_back( v6list );

  list<pair<uint, uint> > v7list;
  v7list.push_back( make_pair( 6, 1 ));
  graph.push_back( v7list );

  dijkstra( 1, graph );

  return 0;
}

