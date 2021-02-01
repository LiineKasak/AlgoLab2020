#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

using namespace std;

// Custom edge adder class
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    // cout << "edge " << from << " " << to << "  " << capacity << "  "     <<
    // cost << endl;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);  // new!
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
  int nr_bookings, nr_stations;
  cin >> nr_bookings >> nr_stations;
  // cout << nr_bookings << " " << nr_stations << endl;
  const int MAX_TIME = 100'000;

  graph G(0);
  edge_adder adder(G);
  long total_cars = 0;
  vector<int> station_cars(nr_stations);
  for (int i = 0; i < nr_stations; i++) {
    cin >> station_cars[i];
    total_cars += station_cars[i];
  }

  vector<map<long, long>> time_index(nr_stations);
  while (nr_bookings--) {
    int from, to, profit;
    long t1, t2;
    cin >> from >> to >> t1 >> t2 >> profit;
    // cout << from << " " << to << " " << t1 << " " << t2 << endl;
    from--;
    to--;
    if (time_index[from].find(t1) == time_index[from].end())
      time_index[from].emplace(t1, boost::add_vertex(G));
    if (time_index[to].find(t2) == time_index[to].end())
      time_index[to].emplace(t2, boost::add_vertex(G));
    long i_from = time_index[from].find(t1)->second;
    long i_to = time_index[to].find(t2)->second;
    // cout << (t2-t1) << " " << profit<<endl;
    long cost = (t2 - t1) * 100 - profit;
    adder.add_edge(i_from, i_to, 1, cost);
  }

  int v_source = boost::add_vertex(G), v_sink = boost::add_vertex(G);

  for (int station = 0; station < nr_stations; station++) {
    auto first = time_index[station].begin();
    if (first == time_index[station].end()) {
      adder.add_edge(v_source, v_sink, station_cars[station], MAX_TIME * 100);
      continue;
    }
    auto last = time_index[station].rbegin();
    adder.add_edge(v_source, first->second, station_cars[station],
                   first->first * 100);
    adder.add_edge(last->second, v_sink, total_cars,
                   (MAX_TIME - last->first) * 100);
    auto previous = time_index[station].begin();
    for (auto t = ++time_index[station].begin(); t != time_index[station].end();
         ++t) {
      adder.add_edge(previous->second, t->second, total_cars,
                     (t->first - previous->first) * 100);
      previous = t;
    }
  }

  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  long cost = boost::find_flow_cost(G);

  long total_extra_cost = total_cars * MAX_TIME * 100;
  long profit = total_extra_cost - cost;
  cout << profit << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
}