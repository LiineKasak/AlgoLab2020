/*
Find the minimum cost to repair cities for a racing event. Each city must either
be repaired or be connected to a city which has been repaired.

The problem can be modelled as running DFS on a tree, and recursively trying to
find the minimum cost if its parent was repaired and if it wasn't.

We observe that if a nodes parent was not repaired, we need at least one child
of the node to be repaired. We calculate the cost for each child of the node if
it was repaired and if not and for the repaired child we choose the child with
the minimum repaired_cost - not_repaired_cost. For the other childs, we simply
choose either which is lesser.
*/

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

vector<vector<int>> children;
vector<int> parent;
vector<int> cost;
vector<pair<int, int>> memo_parent;
vector<pair<int, int>> memo_noparent;

void rec(int node) {
  int included = cost[node];
  int not_included = 0;

  if (children[node].empty()) {
    memo_parent[node] = {not_included, not_included};
    memo_noparent[node] = {included, included};
  }

  int best_diff = numeric_limits<int>::max();

  for (auto child : children[node]) {
    rec(child);
    const auto& inc = memo_parent[child];
    const auto& not_inc = memo_noparent[child];

    included += min(inc.first, inc.second);
    not_included += min(not_inc.first, not_inc.second);

    best_diff = min(best_diff, not_inc.first - not_inc.second);
  }

  best_diff = max(0, best_diff);  // we do not want to subtract from the cost..
  memo_parent[node] = {included, not_included};
  memo_noparent[node] = {included, not_included + best_diff};
}

void solve() {
  int nr_cities;
  cin >> nr_cities;

  children.resize(nr_cities);
  parent.resize(nr_cities);
  cost.resize(nr_cities);
  memo_parent.resize(nr_cities);
  memo_noparent.resize(nr_cities);

  for (int i = 0; i < nr_cities - 1; i++) {
    int from, to;
    cin >> from >> to;
    children[from].push_back(to);
    parent[to] = from;
  }
  for (int i = 0; i < nr_cities; i++) cin >> cost[i];

  rec(0);
  pair<int, int> result = memo_noparent[0];
  cout << min(result.first, result.second) << endl;

  children.clear();
  parent.clear();
  cost.clear();
  memo_parent.clear();
  memo_noparent.clear();
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}