/*
Given a graph G (V, E), and for each edge the weight and capacity of it,
what is the maximum flow within the shortest paths from s to t?

This solution composes of two steps:
1) calculating distances from s (dist_map),
2) calculating max flow on a new graph where we add all edges of the
previous graph where dist_map[u] + edge weight = dist_map[v].
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, long>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

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

using namespace std;

void solve() {
  int n, m, s, f;
  cin >> n >> m >> s >> f;

  weighted_graph WG(n);
  weight_map weights = boost::get(boost::edge_weight, WG);
  edge_desc e;

  map<edge_desc, int> capacities;
  for (int i = 0; i < m; i++) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    e = boost::add_edge(a, b, WG).first;
    weights[e] = d;
    capacities[e] = c;
  }

  std::vector<long> dist_map(n);

  boost::dijkstra_shortest_paths(
      WG, s,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, WG))));

  graph G(n);
  edge_adder adder(G);

  edge_it e_beg, e_end;
  for (boost::tie(e_beg, e_end) = boost::edges(WG); e_beg != e_end; ++e_beg) {
    int from = boost::source(*e_beg, WG), to = boost::target(*e_beg, WG);
    int dist = weights[*e_beg];
    if (dist_map[from] + dist == dist_map[to])
      adder.add_edge(from, to, capacities[*e_beg]);
    if (dist_map[to] + dist == dist_map[from])
      adder.add_edge(to, from, capacities[*e_beg]);
  }
  long flow = boost::push_relabel_max_flow(G, s, f);
  cout << flow << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}