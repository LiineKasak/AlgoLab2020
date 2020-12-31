/*
Find the maximum number of knights we can place on a chessboard
with holes (no knight allowed) without threatening each other.

After modelling a few examples, it is easy to see that
this is a Maximum Independent Set problem.

We run the Push Relabel Max Flow to compute the residual graph.
This constructs a maximum matching from one set to the other.
Using the residual capacity graph, we run BFS from the source vertex
and we follow an edge if the residual capacity is 0
(this edge is used in the matching).
Now the MaxIS is all visited in one set and all unvisited in the other set.
*/

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>

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

using namespace std;

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

vector<int> threatens(int i, int n) {
  int x = i % n, y = floor(i / n);
  vector<int> ts;
  ts.reserve(8);
  if (x > 0 && y > 1) ts.emplace_back(i - 1 - 2 * n);
  if (x > 0 && y + 2 < n) ts.emplace_back(i - 1 + 2 * n);
  if (x + 1 < n && y > 1) ts.emplace_back(i + 1 - 2 * n);
  if (x + 1 < n && y + 2 < n) ts.emplace_back(i + 1 + 2 * n);
  if (x > 1 && y > 0) ts.emplace_back(i - 2 - n);
  if (x > 1 && y + 1 < n) ts.emplace_back(i - 2 + n);
  if (x + 2 < n && y > 0) ts.emplace_back(i + 2 - n);
  if (x + 2 < n && y + 1 < n) ts.emplace_back(i + 2 + n);
  return ts;
}

bool is_white(int i, int n) {
  int y = floor(i / n);
  int x = i % n;

  if (y % 2 == 0)
    return x % 2 == 0;
  else
    return x % 2 == 1;
}

void solve() {
  // placing knights should be a Maximum Inpedentent Set problem

  int length;
  cin >> length;
  int nr_fields = length * length;
  vector<short> valid(nr_fields);
  vector<int> set(nr_fields);
  set[0] = 1;  // sets are 1 and -1, 0 is unassigned

  for (int i = 0; i < nr_fields; i++) cin >> valid[i];

  int source = nr_fields, sink = nr_fields + 1;
  graph G(nr_fields + 2);
  edge_adder adder(G);
  auto residual_capacity = boost::get(boost::edge_residual_capacity, G);

  for (int i = 0; i < nr_fields; i++) {
    if (!valid[i]) continue;

    // add edges from source or to sink if is a valid position
    if (valid[i]) {
      if (is_white(i, length))
        adder.add_edge(source, i, 1);
      else
        adder.add_edge(i, sink, 1);
    }

    // add edges to threatened cells
    vector<int> threats = threatens(i, length);
    for (int t : threats) {
      if (!valid[t] || t < i) continue;

      if (is_white(i, length))
        adder.add_edge(i, t, 1);
      else
        adder.add_edge(t, i, 1);
    }
  }

  // calculate max flow to get residual capacity map
  boost::push_relabel_max_flow(G, source, sink);

  // BFS to find vertex set S
  vector<int> visited(nr_fields, false);
  queue<int> Q;
  visited[source] = true;
  Q.push(source);
  while (!Q.empty()) {
    const int curr = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(curr, G); ebeg != eend;
         ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (residual_capacity[*ebeg] == 0 || visited[v]) continue;
      visited[v] = true;
      Q.push(v);
    }
  }

  // Maximum Independent Set calculation
  int maxIS = 0;
  for (int i = 0; i < nr_fields; ++i) {
    bool is_independent = (is_white(i, length) && visited[i]) ||
                          (!is_white(i, length) && !visited[i]);
    if (is_independent && valid[i]) maxIS++;
  }
  cout << maxIS << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}