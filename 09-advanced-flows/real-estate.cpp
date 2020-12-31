/*
Typical max flow max cost problem.
Since we know what the maximal cost can be, for each cost,
we do new_cost = max_cost - cost.
Then we can use the max flow min cost algorithm.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <iostream>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<
            boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                            boost::property<boost::edge_weight_t, long> > > > >
    graph;  // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;  // Iterator

using namespace std;

// Custom edge adder class
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
    w_map[e] = cost;       // new assign cost
    w_map[rev_e] = -cost;  // new negative cost
  }
};

void solve() {
  int nr_buyers, nr_sites, nr_states;
  cin >> nr_buyers >> nr_sites >> nr_states;

  // vertex indices
  int source = 0, sink = nr_buyers + nr_sites + 1;
  auto get_state_idx = [](int i) { return i + 1; };
  auto get_site_idx = [&](int i) { return nr_states + i + 1; };
  auto get_buyer_idx = [&](int i) { return nr_states + nr_sites + i + 1; };

  graph G(s + n + m + 2);
  edge_adder adder(G);

  for (int i = 0; i < nr_states; i++) {
    int limit;
    cin >> limit;
    adder.add_edge(source, get_state_idx(i), limit, 0);
  }

  for (int i = 0; i < nr_sites; i++) {
    int site_state;
    cin >> site_state;  // problem starts counting from 1...
    adder.add_edge(get_state_idx(site_state - 1), get_site_idx(i), 1, 0);
  }

  for (int b = 0; b < nr_buyers; b++) {
    int buyer = get_buyer_idx(b);
    for (int i = 0; i < nr_sites; i++) {
      double offer;
      cin >> offer;
      int site = get_site_idx(i);

      // since max offer is 100, this will be positive
      double cost = 100 - offer;
      adder.add_edge(site, buyer, 1, cost);
    }

    // every buyer purchases at most one land!!!!
    adder.add_edge(buyer, sink, 1, 0);
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

  // since for cost c we did 100 - c
  // now for real c_sum we have to do flow*100 - cost
  int real_cost = flow * 100 - cost;
  cout << flow << " " << real_cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}