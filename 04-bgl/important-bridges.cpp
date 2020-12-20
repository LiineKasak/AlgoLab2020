/*
Find the number of edges after which removal the graph is unconnected.
These edges are in a BICONNECTED component with one edge.
A vertex which belongs to MORE THAN 1 biconnected component is an
ARTICULATION POINT (if removed from graph, the graph becomes unconnected).
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <iostream>
#include <vector>

namespace boost {
struct edge_component_t {
  enum { num = 555 };
  typedef edge_property_tag kind;
} edge_component;
}  // namespace boost

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_component_t, size_t>>
    graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

using namespace std;

void solve() {
  int n, m;
  cin >> n >> m;

  graph G(n);
  for (int i = 0; i < m; i++) {
    int i1, i2;
    cin >> i1 >> i2;
    boost::add_edge(i1, i2, G);
  }

  // get biconnected components
  auto component = boost::get(boost::edge_component, G);
  int component_nr = biconnected_components(G, component);

  // count number of edges in each component
  edge_it ei, ei_end;
  vector<int> edges_nr(component_nr);
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei)
    edges_nr[component[*ei]]++;

  // iterate over edges, if edge is in a component with edge count one,
  // it is an important bridge
  int count = 0;
  vector<pair<int, int>> edges;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
    if (edges_nr[component[*ei]] == 1) {
      count++;
      int s = boost::source(*ei, G), t = boost::target(*ei, G);

      if (s < t)
        edges.push_back({s, t});
      else
        edges.push_back({t, s});
    }
  }

  sort(edges.begin(), edges.end());

  cout << count << endl;
  for (auto e : edges) cout << e.first << " " << e.second << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}