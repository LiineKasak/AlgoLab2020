/*
Find a partition of vertices which minimizes the
sum of weights of edges goind from one set to the other.
Min cut = max flow.

- edges are directed since one limb reaches from one figure to the other
- it takes too much time to test every combination of s and t (O(n2)),
and it is enough to check all combinations where vertex 0 is included
and where vertex 0 is not included.
*/

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t,
                                        traits::edge_descriptor> > > >
    graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

using namespace std;

void solve() {
  int nr_sculptures, nr_limbs;
  cin >> nr_sculptures >> nr_limbs;

  graph G(nr_sculptures);
  edge_adder adder(G);

  for (int i = 0; i < nr_limbs; i++) {
    int figure1, figure2, cost;
    cin >> figure1 >> figure2 >> cost;
    adder.add_edge(figure1, figure2, cost);
  }

  // test all vertices as the source and sink
  int min_cut = nr_limbs * 1000;        // c max is 1000
  for (int i = 0; i < nr_sculptures; i++) {  // n is max 200
    // any other vertex but 0 would do as well
    int f1 = boost::push_relabel_max_flow(G, 0, i);
    int f2 = boost::push_relabel_max_flow(G, i, 0);

    min_cut = min(min_cut, min(f1, f2));
  }
  cout << min_cut << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}