/*
Simple check if max flow is the number of shops needed to visit.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>

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

  void add_edge(int from, int to) {
    const int capacity = 1;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int nr_intersections, nr_streets, nr_shops;
  std::cin >> nr_intersections >> nr_streets >> nr_shops;

  // one extra vertex to connect shops to sink
  graph G(nr_intersections + 1);
  edge_adder adder(G);
  int source = 0, sink = nr_intersections;

  for (int i = 0; i < nr_shops; i++) {
    int shop;
    std::cin >> shop;
    adder.add_edge(shop, sink);
  }

  while (nr_streets--) {
    int int1, int2;
    std::cin >> int1 >> int2;
    // we add two edges because undirectional graph
    adder.add_edge(int1, int2);
    adder.add_edge(int2, int1);
  }

  int flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << (flow == nr_shops ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t--) solve();
  return 0;
}