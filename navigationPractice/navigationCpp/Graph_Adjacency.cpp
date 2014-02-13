#include <stdint.h>
#include <iostream>

#include "Graph_Adjacency.h"
#include "Graph.h"

#define NO_CONNECTION -1

Graph_Adjacency::Graph_Adjacency( uint16_t numberOfNodes ): Graph( numberOfNodes )
{
   // implementing a half diagonal matrix to save space and eliminate reduncdancy
   //   0 1 2 3
   // 3 x x x
   // 2 x x
   // 1 x
   // 0
   // too complicated for right now
   
   m_AdjacencyMatrix = new matrixValue_t*[ m_NumberOfNodes ];
   for ( graphSize_t y = 0; y < m_NumberOfNodes; y++ )
      m_AdjacencyMatrix[ y ] = new matrixValue_t[ m_NumberOfNodes ];
  
   // set each node to default of no connections
   for ( graphSize_t x = 0; x < m_NumberOfNodes; x++ )
      for ( graphSize_t y = 0; y < m_NumberOfNodes; y++ )
         m_AdjacencyMatrix[ x ][ y ] = NO_CONNECTION;
}

Graph_Adjacency::~Graph_Adjacency() 
{
   for ( graphSize_t i = 0; i < m_NumberOfNodes; i++ )
      delete m_AdjacencyMatrix[ i ];
   delete m_AdjacencyMatrix;
}

void Graph_Adjacency::AddEdge( graphSize_t firstNodeId, graphSize_t secondNodeId, inches_t distance )
{
   // done twice to account for matrix reflection across diagonal
   m_AdjacencyMatrix[ firstNodeId ][ secondNodeId ] = distance;
   m_AdjacencyMatrix[ secondNodeId ][ firstNodeId ] = distance;
}

void Graph_Adjacency::PrintGraph()
{
   std::cout << std::endl;
   std::cout << "    ";
   for ( int x = 0; x < m_NumberOfNodes; x++ )
   {
      std::cout.width( 4 );
      std::cout << x;
   }
   std::cout << std::endl;
   std::cout << std::endl;
   for ( int y = 0; y < m_NumberOfNodes; y++ )
   {
      std::cout.width( 4 );
      std::cout << y;
      for ( int x = 0; x < m_NumberOfNodes; x++ )
      {
         if ( m_AdjacencyMatrix[ x ][ y ] == -1 )
         {
            std::cout.width( 4 );
            std::cout << ".";
         }
         else
         {
            std::cout.width( 4 );
            std::cout << m_AdjacencyMatrix[ x ][ y ];
         }
      }
      std::cout << std::endl;
      std::cout << std::endl;
   }
}

void Graph_Adjacency::Dijkstra( graphSize_t sourceNodeId, graphSize_t targetNodeId )
{
   bool beenVisited[ m_NumberOfNodes ];
   graphSize_t previousNode[ m_NumberOfNodes ];
   inches_t distFromA[ m_NumberOfNodes ];

   for ( graphSize_t i = 0; i < m_NumberOfNodes; i++ )
   {
      beenVisited[ i ] = false;
      distFromA[ i ] = 0x7FFF;
   }

   distFromA[ sourceNodeId ] = 0;
   graphSize_t currentNode = sourceNodeId;
   inches_t closestDistToA;

   for ( graphSize_t i = 0; i < m_NumberOfNodes; i++ )
   {
      closestDistToA = 0x7FFF;
      for ( graphSize_t neighbor = 0; neighbor < m_NumberOfNodes; neighbor++ )
      {
         if ( ( m_AdjacencyMatrix[ currentNode ][ neighbor ] != NO_CONNECTION ) &&
            ( !beenVisited[ neighbor ] ) &&
            ( distFromA[ neighbor ] > ( distFromA[ currentNode ] + m_AdjacencyMatrix[ currentNode ][ neighbor ] ) ) )
         {
            previousNode[ neighbor ] = currentNode;
            distFromA[ neighbor ] = distFromA[ currentNode ] + m_AdjacencyMatrix[ currentNode ][ neighbor ];
         }
      }
      if ( currentNode == targetNodeId ) break;
      beenVisited[ currentNode ] = true;

      // determine closest unvisited node
      for ( graphSize_t j = 0; j < m_NumberOfNodes; j++ )
      {
         if ( !beenVisited[ j ] &&
            distFromA[ j ] < closestDistToA )
         {
            currentNode = j;
            closestDistToA = distFromA[ j ];
         }
      }
   }
   // trace back from currentNode
   while ( currentNode != sourceNodeId )
   {
      std::cout << previousNode[ currentNode ] << std::endl;
      currentNode = previousNode[ currentNode ];
   }
}
