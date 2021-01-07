/*
Finding the cheapest cost subgraph that is not a MST.

Implementation:
1) compute the real MST, and mark edges that are included in it
2) for every edge that is included in the MST, try building the MST
without this edge and save the cost
3) return lowest such cost
*/

#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <vector>

using namespace std;

struct edge {
  int vertex1;
  int vertex2;
  int cost;
  int index;
  bool in_mst;
};

int get_cost(int nr_vertices, int skipped_index, vector<edge> &edges) {
  boost::disjoint_sets_with_storage<> uf(nr_vertices);
  int cost_sum = 0;

  for (auto &e : edges) {
    if (e.index == skipped_index) continue;

    int comp1 = uf.find_set(e.vertex1);
    int comp2 = uf.find_set(e.vertex2);
    if (comp1 != comp2) {
      if (skipped_index == -1) e.in_mst = true;

      uf.link(comp1, comp2);
      cost_sum += e.cost;
      if (--nr_vertices == 1) break;
    }
  }
  return cost_sum;
}

void solve() {
  int nr_vertices, v_source;
  cin >> nr_vertices >> v_source;
  vector<edge> edges;
  for (int v1 = 0, i = 0; v1 < nr_vertices - 1; v1++) {
    for (int v2 = v1 + 1; v2 < nr_vertices; v2++) {
      int c;
      cin >> c;
      edges.push_back({v1, v2, c, i++, false});
    }
  }

  sort(edges.begin(), edges.end(),
       [&](const edge &e1, const edge &e2) { return e1.cost < e2.cost; });

  get_cost(nr_vertices, -1, edges);

  int min_non_mst_cost = 276447232;
  for (auto &e : edges) {
    if (e.in_mst) {
      int cost = get_cost(nr_vertices, e.index, edges);
      min_non_mst_cost = min(min_non_mst_cost, cost);
    }
  }
  cout << min_non_mst_cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}