/*
Determine if it is possible to move soldiers to strategic locations,
given BOTH min and max capacities for paths.

Trick: use an extra vertex to control the min required flow through an edge.
*/

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;
typedef traits::edge_descriptor edge_desc;

using namespace std;

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
  int nr_locations, nr_paths;
  cin >> nr_locations >> nr_paths;

  int target_flow = 0;
  int n = nr_locations + nr_paths + 2;
  int source = n - 2, sink = n - 1;
  graph G(n);
  edge_adder adder(G);

  for (int i = 0; i < nr_locations; i++) {
    int in, out;
    cin >> in >> out;
    adder.add_edge(source, i, in);
    adder.add_edge(i, sink, out);
    target_flow += out;
  }

  // construct all edges with new "middle vertex" for every edge
  for (int i = 0; i < nr_paths; i++) {
    int from, to, min_capacity, max_capacity;
    cin >> from >> to >> min_capacity >> max_capacity;
    int v = nr_locations + i;
    adder.add_edge(from, v, max_capacity);  // into vertex of the edge
    adder.add_edge(v, sink, min_capacity);  // to the sink - necessary min capacity
    adder.add_edge(v, to, max_capacity - min_capacity);  // remaining flow into target vertex
    adder.add_edge(source, to, min_capacity);  // give deducted flow back to target vertex
    target_flow += min_capacity;
  }

  int flow = boost::push_relabel_max_flow(G, source, sink);
  cout << (target_flow <= flow ? "yes" : "no") << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t--) solve();
  return 0;
}
