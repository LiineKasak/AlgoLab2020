/*
Finding the shortest path using a minimum required number (k) of
"special edges".

Trick: Duplicate the graph on k+1 levels and the only way
to get to the next level is using a "special edge".
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

using namespace std;

void solve() {
  int nr_cities, nr_roads, min_rivers_used, start, end;
  cin >> nr_cities >> nr_roads >> min_rivers_used >> start >> end;

  int nodes = nr_cities * (min_rivers_used + 1);
  weighted_graph G(nodes);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;

  for (int i = 0; i < nr_roads; i++) {
    int city1, city2, cost, is_river;
    cin >> city1 >> city2 >> cost >> is_river;

    for (int i_level = 0; i_level <= min_rivers_used; i_level++) {
      // use min_rivers_used + 1 levels,
      // to ensure using at least the number of necessary rivers
      int level = nr_cities * i_level;

      // if is a normal road, connect the two cities on all levels
      if (!is_river || i_level == min_rivers_used) {
        e = boost::add_edge(city1 + level, city2 + level, G).first;
        weights[e] = cost;
      }
      // if we have a river road and we're not on the last level
      // add an edge from both cities to the other city on the upper level
      if (is_river && i_level != min_rivers_used) {
        e = boost::add_edge(city1 + level, city2 + level + nr_cities, G).first;
        weights[e] = cost;
        e = boost::add_edge(city1 + level + nr_cities, city2 + level, G).first;
        weights[e] = cost;
      }
    }
  }

  // run dijkstra to find shortest path
  vector<int> dist_map(nodes);
  boost::dijkstra_shortest_paths(
      G, start,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));
  int top_level_end = end + nr_cities * min_rivers_used;
  cout << dist_map[top_level_end] << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}