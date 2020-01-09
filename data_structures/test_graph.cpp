#include "graph_matrix.h"
#include "graph_adjacency_list.h"
#include "graph_edge_list.h"

void test_graph_impl() {
  int size = 5;
  GraphL g(size);

  g.addEdge(0, 1);
  g.addEdge(0, 4);
  g.addEdge(1, 2);
  g.addEdge(1, 3);
  g.addEdge(1, 4);
  g.addEdge(2, 3);
  g.addEdge(3, 4);
  g.print();

  g.dfs();
  g.bfs();

  GraphM gM(size);
  gM.addEdge(0, 1);
  gM.addEdge(0, 4);
  gM.addEdge(1, 2);
  gM.addEdge(1, 3);
  gM.addEdge(1, 4);
  gM.addEdge(2, 3);
  gM.addEdge(3, 4);

  gM.print();
  gM.dfs();
  gM.bfs();


  // Edges of minimal spanning tree are:
  // 6 -> 7, w = 1
  // 2 -> 8, w = 2
  // 5 -> 6, w = 2
  // 0 -> 1, w = 4
  // 2 -> 5, w = 4
  // 2 -> 3, w = 7
  // 0 -> 7, w = 8
  // 3 -> 4, w = 9
  // Weight of MST is: 37
  GraphE e;
  e.addEdge(0, 1, 4);
  e.addEdge(0, 7, 8);
  e.addEdge(1, 2, 8);
  e.addEdge(1, 7, 11);
  e.addEdge(2, 3, 7);
  e.addEdge(2, 8, 2);
  e.addEdge(2, 5, 4);
  e.addEdge(3, 4, 9);
  e.addEdge(3, 5, 14);
  e.addEdge(4, 5, 10);
  e.addEdge(5, 6, 2);
  e.addEdge(6, 7, 1);
  e.addEdge(6, 8, 6);
  e.addEdge(7, 8, 7);

  e.kruskalMST();

}