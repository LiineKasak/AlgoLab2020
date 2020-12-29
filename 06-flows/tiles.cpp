/*
Find if it's possible to cover a floor with 2:1 tiles.

General maximum matching problem - can use Edmonds Maximum Cardinality Matching.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <iostream>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

using namespace std;

void solve() {
  int width, height;
  cin >> width >> height;

  int nr_vertices = width * height;
  vector<bool> valid(nr_vertices, true);
  graph G(nr_vertices);

  int valid_count = nr_vertices;
  char val;
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      cin >> val;
      int index = row * width + col;
      if (val == 'x') {
        valid[index] = 0;
        valid_count--;
      } else {
        int index = row * width + col;
        int left_index = index - 1;
        int up_index = index - width;
        if (col > 0 && valid[left_index]) boost::add_edge(index, left_index, G);

        if (row > 0 && valid[up_index]) boost::add_edge(index, up_index, G);
      }
    }
  }
  if (valid_count % 2 != 0) {
    cout << "no" << endl;
    return;
  }

  std::vector<vertex_desc> mate_map(nr_vertices);

  boost::edmonds_maximum_cardinality_matching(
      G, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(
      G, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, G)));

  cout << (matching_size == valid_count / 2 ? "yes" : "no") << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}