/*
Find the maximum number of photograph that can be collected
given a set of one-way streets, and locations for stations and  photographs.

Simple flow problem. Duplicate vertices of stations and edges,
so that from the stations to the photographs edge capacities are unlimited,
and from the photographs to the stations edge capacity is 1.
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
typedef traits::edge_descriptor edge_desc;

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
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int nr_vertices, nr_edges, nr_stations, nr_photographs;
  std::cin >> nr_vertices >> nr_edges >> nr_stations >> nr_photographs;
  int source = 2 * nr_vertices, sink = 2 * nr_vertices + 1;
  graph G(2 * nr_vertices + 2);
  edge_adder adder(G);

  int police;
  for (int i = 0; i < nr_stations; i++) {
    std::cin >> police;
    adder.add_edge(source, police, 1);
    adder.add_edge(police + nr_vertices, sink, 1);
  }

  int dest;
  for (int i = 0; i < nr_photographs; i++) {
    std::cin >> dest;
    adder.add_edge(dest, dest + nr_vertices, 1);
  }

  int a, b;
  for (int i = 0; i < nr_edges; i++) {
    std::cin >> a >> b;
    // without photograph no limit on capacity
    adder.add_edge(a, b, nr_stations);
    // with photograph only one policeman can use a street
    adder.add_edge(a + nr_vertices, b + nr_vertices, 1);
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