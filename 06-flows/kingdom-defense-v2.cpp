/*
Determine if it is possible to move soldiers to strategic locations,
given BOTH min and max capacities for paths.

Trick: play with demand and supply to construct a graph with just max capacity.
This is faster than v1 where we just add extra edges always,
instead of calculating this demand or supply for every vertex
and then cancelling them out.
*/

#include <algorithm>
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

typedef traits::vertex_descriptor vertex_desc;
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
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int nr_locations, nr_paths;
  std::cin >> nr_locations >> nr_paths;

  graph G(nr_locations);
  edge_adder adder(G);

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  std::vector<long> demand(nr_locations);
  std::vector<long> supply(nr_locations);

  for (int i = 0; i < nr_locations; i++) {
    int nr_stationed, nr_needed;
    std::cin >> nr_stationed >> nr_needed;
    supply[i] = nr_stationed;
    demand[i] = nr_needed;
  }

  const auto c_map = boost::get(boost::edge_capacity, G);
  while (nr_paths--) {
    int from, to, min_traversed, max_capacity;
    std::cin >> from >> to >> min_traversed >> max_capacity;
    adder.add_edge(from, to, max_capacity - min_traversed);

    // to fulfill min_traversed
    // we subtract min traversed from the "from" vertex
    // and add it to the "to" vertex
    demand[from] += min_traversed;
    demand[to] -= min_traversed;
  }

  long demanded_sum = 0;
  for (int v_location = 0; v_location < nr_locations; v_location++) {
    if (demand[v_location] > 0) {
      // demand
      adder.add_edge(v_location, v_sink, demand[v_location]);
      adder.add_edge(v_source, v_location, supply[v_location]);
      demanded_sum += demand[v_location];
    } else {
      // supply
      adder.add_edge(v_source, v_location,
                     supply[v_location] - demand[v_location]);
    }
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << (demanded_sum <= flow ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t--) testcase();
  return 0;
}