/*
Find the minimum weight path from start to end, using multiple subgraphs.
1. Construct MST for each subgraph (due to the problem - how forest is explored).
2. Hold a main graph, where we either add edge from subgraph MST or change
weight to minimum of existing and new weight.
3. Run Dijkstra to find shortest start-end path.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;

using namespace std;

void solve() {
  int nr_trees, nr_edges, nr_species, start, end;
  cin >> nr_trees >> nr_edges >> nr_species >> start >> end;

  weighted_graph G(nr_trees);
  vector<edge_desc> edges(nr_edges);
  vector<vector<int>> all_weights(nr_species, vector<int>(nr_edges));

  for (int i_edge = 0; i_edge < nr_edges; i_edge++) {
    int t1, t2;
    cin >> t1 >> t2;
    edge_desc edge = boost::add_edge(t1, t2, G).first;
    edges[i_edge] = edge;
    for (int i_species = 0; i_species < nr_species; i_species++) {
      int w;
      cin >> w;
      all_weights[i_species][i_edge] = w;
    }
  }
  vector<int> hives(nr_species);
  for (int i_species = 0; i_species < nr_species; i_species++) {
    int h;
    cin >> h;
    hives[i_species] = h;
  }

  // new main graph
  weighted_graph G2(nr_trees);
  weight_map weights2 = boost::get(boost::edge_weight, G2);

  // calculate subgraph for all species and add to main graph
  weight_map weights = boost::get(boost::edge_weight, G);
  for (int spe = 0; spe < nr_species; spe++) {
    // adjust weights
    for (int i = 0; i < nr_edges; i++) {
      weights[edges[i]] = all_weights[spe][i];
    }

    // find all found edges
    vector<edge_desc> spanning_edges;
    boost::kruskal_minimum_spanning_tree(G, back_inserter(spanning_edges));

    // add all edges to new graph
    for (edge_desc ed : spanning_edges) {
      edge_desc en =
          boost::add_edge(boost::source(ed, G), boost::target(ed, G), G2).first;
      weights2[en] = weights[ed];
    }
  }

  // dijkstra
  vector<int> dist_map(nr_trees);

  boost::dijkstra_shortest_paths(
      G2, start,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G2))));
  cout << dist_map[end] << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}
