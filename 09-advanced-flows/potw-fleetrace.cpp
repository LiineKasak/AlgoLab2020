/*
Finding the maximum sum of spactacle coefficients on a pair selection, given
coefficents for each pair. The tricky part is that we do not necessarily have
to have maximum matching, just maximize the coefficents.

Implementation: max flow min cost.

Initial solution was with binary search on the number on pairs (max flow size),
trying to find the maximal sum of coefficents (min cost).

However, we can assure that we get the best cost by adding an extra edge from
all boats to the sink with 0 coefficent (max cost 50). This way we always have
the same max flow and find the min cost with just one flow graph.
*/

#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

using namespace std;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

void solve() {
  int nr_boats, nr_sailors, nr_pairs;
  cin >> nr_boats >> nr_sailors >> nr_pairs;

  // vertex indices
  int source = 0, sink = nr_boats + nr_sailors + 1;
  auto boat_v = [](int i) { return i + 1; };
  auto sailor_v = [&](int i) { return nr_boats + i + 1; };

  graph G(nr_boats + nr_sailors + 2);
  edge_adder adder(G);

  for (int i = 0; i < nr_boats; i++) {
    adder.add_edge(source, boat_v(i), 1, 0);
    // NB! extra edge from every boat to sink
    // to avoid binary search on the flow size
    // give the highest cost so will be only used if necessary
    adder.add_edge(boat_v(i), sink, 1, 50);
  }
  for (int i = 0; i < nr_sailors; i++) adder.add_edge(sailor_v(i), sink, 1, 0);

  for (int i = 0; i < nr_pairs; i++) {
    int boat, sailor, coefficient;
    cin >> boat >> sailor >> coefficient;
    // max coefficient is 50 and goal is to max, but use min cost
    int cost = 50 - coefficient;
    adder.add_edge(boat_v(boat), sailor_v(sailor), 1, cost);
  }

  // calculate cost
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);

  // calculate flow
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  int flow = 0;
  out_edge_it e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G);
       e != eend; ++e)
    flow += c_map[*e] - rc_map[*e];

  int real_cost = flow * 50 - cost;
  cout << real_cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}