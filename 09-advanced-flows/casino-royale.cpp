#include <iostream>
#include <cmath>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

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
  int nr_train_stops, nr_missions, nr_max_allowed;
  cin >> nr_train_stops >> nr_missions >> nr_max_allowed;
  const int max_cost = 150;  // max cost is actually 2**7 = 128

  // vertex indices
  int source = 0, sink = nr_train_stops;

  graph G(nr_train_stops + 1);
  edge_adder adder(G);

  // add control edge to limit agent number
  adder.add_edge(nr_train_stops - 1, sink, nr_max_allowed, max_cost);

  // edges between stops (so if no agents on train)
  for (int i = 0; i < nr_train_stops - 1; i++)
    adder.add_edge(i, i + 1, nr_max_allowed, max_cost);

  // add mission edges with cost = passed_stops * max_cost - priority
  while (nr_missions--) {
    int i_stop1, i_stop2, priority;
    cin >> i_stop1 >> i_stop2 >> priority;
    adder.add_edge(i_stop1, i_stop2, 1, (i_stop2 - i_stop1) * max_cost - priority);
  }

  // calculate cost
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int min_cost = boost::find_flow_cost(G);

  // calculate number of times we added max_cost in flow
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  int added = 0;
  edge_it e, eend;
  for (boost::tie(e, eend) = boost::edges(G); e != eend; ++e) {
    // for each used positive edge we add max_cost * passed_stops
    int from = boost::source(*e, G), to = boost::target(*e, G);
    int used = c_map[*e] - rc_map[*e];

    int passed_stops = to - from;
    if (passed_stops > 0) added += used * passed_stops;
  }

  // calculate correct prio sum
  int priority_sum = added * max_cost - min_cost;
  cout << priority_sum << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}