#include "graph_matrix.h"
#include "graph_list.h"

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
}