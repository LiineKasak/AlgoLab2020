/*
Find the maximum number of knights who can escape from the tunnel grid.
They can escape if they reach a vertex which is connected to the outside.
Tip: two knights can escape from a vertex at the corner of the grid.

Find a max flow where we have a capacity on
how many times we can use 1) an edge, 2) a vertex.
Vertex capacity: we use two vertices to represent one vertex.
We connect the two vertices with an edge with the capacity
equal to the capacity of the vertex.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>
#include <vector>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int m, n, k, c;
  std::cin >> m >> n >> k >> c;

  std::vector<std::pair<int, int>> knights(k, {0, 0});
  for (int i = 0; i < 2 * k; i++)
    std::cin >> (i % 2 == 0 ? knights[i / 2].first : knights[i / 2].second);

  // construct a graph
  // every intersection has "from" and "to" vertex,
  // where edge from "to" to "from" is c capacity
  int vert = n * m * 2;
  graph G(vert + 2);  // source and sink included
  edge_adder adder(G);
  int source = vert, sink = vert + 1;

  for (int row = 0; row < n; row++) {
    for (int col = 0; col < m; col++) {
      int vertex = row * m + col;
      int vertex_from = vertex + n * m;

      // edge from our intersections "to" to "from"
      adder.add_edge(vertex, vertex_from, c);

      // edge to upper intersection
      if (col > 0) {
        adder.add_edge(vertex_from, vertex - 1, 1);
        adder.add_edge(vertex_from - 1, vertex, 1);
      }
      // edge to left intersection
      if (row > 0) {
        adder.add_edge(vertex_from, vertex - m, 1);
        adder.add_edge(vertex_from - m, vertex, 1);
      }
      // if can exit then connect to sink
      // corner capacity is 2!
      if (col == 0 || col == m - 1) adder.add_edge(vertex_from, sink, 1);
      if (row == 0 || row == n - 1) adder.add_edge(vertex_from, sink, 1);
    }
  }

  // connect all knights to the source
  for (auto knight : knights) {
    int col = knight.first, row = knight.second;
    int vertex = row * m + col;
    adder.add_edge(source, vertex, 1);
  }

  int flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  while (t--) solve();

  return 0;
}