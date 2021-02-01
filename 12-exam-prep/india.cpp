#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <iostream>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

using namespace std;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
    return e;
  }
};

using namespace std;

int get_cost(graph &G, edge_desc &e, int flow, int source, int sink) {
  auto c_map = boost::get(boost::edge_capacity, G);
  c_map[e] = flow;
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  return boost::find_flow_cost(G);
}

void solve() {
  int nr_cities, nr_guides, budget, v_start, v_end;
  cin >> nr_cities >> nr_guides >> budget >> v_start >> v_end;

  graph G(nr_cities + 1);
  edge_adder adder(G);
  int start_flow = 0, end_flow = 0;

  while (nr_guides--) {
    int from, to, cost, capacity;
    cin >> from >> to >> cost >> capacity;
    adder.add_edge(from, to, capacity, cost);
    if (from == v_start) start_flow += capacity;
    if (to == v_end) end_flow += capacity;
  }

  int low = 0, high = min(start_flow, end_flow);

  // add control edge and check if high is more than actual flow
  int v_source = nr_cities;
  edge_desc control = adder.add_edge(v_source, v_start, high, 0);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_end);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const int f = c_map[control] - rc_map[control];
  high = min(high, f);

  // binary search to find max flow which is under budget
  while (low != high) {
    int mid = (high + low + 1) / 2;
    int mid_cost = get_cost(G, control, mid, v_source, v_end);

    if (mid_cost > budget)
      high = mid - 1;
    else
      low = mid;
  }
  cout << low << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}