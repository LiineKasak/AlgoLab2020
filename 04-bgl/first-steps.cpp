/*
Find total MST weight and furtest node from 0 vertex.
1. BGL Kruskal MST algorithm
2. BGL Dijkstra algorithm
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
#include <vector>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;

void solve() {
  int nr_vertices, nr_edges;
  cin >> nr_vertices >> nr_edges;

  weighted_graph G(nr_vertices);
  weight_map weights = get(boost::edge_weight, G);

  while (nr_edges--) {
    int s, t, w;
    cin >> s >> t >> w;
    edge_desc e = boost::add_edge(s, t, G).first;
    weights[e] = w;
  }

  // MST weight
  vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, back_inserter(mst));

  int weight_sum = 0;
  for (auto edge : mst) weight_sum += weights[edge];
  cout << weight_sum << " ";

  // Longest path
  std::vector<int> dist_map(nr_vertices);
  boost::dijkstra_shortest_paths(
      G, 0,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));

  cout << *max_element(dist_map.begin(), dist_map.end()) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}