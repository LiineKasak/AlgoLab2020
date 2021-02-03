/*
Finding the oldest ancestor under a certain age.

This problem is similar to New York.
Since we have many queries, then it's useful to remember the queries
before traversing the tree. While running dfs on a tree, we remember
the current path. For every visited node, we solve all queries,
knowing the path we took to the node.
*/

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

vector<vector<pair<int, int>>> queries;
vector<vector<int>> children;
vector<int> result;
vector<int> ages;

void dfs(int node, vector<int> &path) {
  path.push_back(node);

  int pi = 0;
  for (auto &q : queries[node]) {
    while (ages[path[pi++]] > q.first) {
    }
    result[q.second] = path[--pi];
  }

  for (int child : children[node]) dfs(child, path);

  path.erase(path.end() - 1);
}

void solve() {
  int n, q;
  cin >> n >> q;

  ages = vector<int>(n);
  result = vector<int>(q);
  children = vector<vector<int>>(n);
  queries = vector<vector<pair<int, int>>>(n);

  map<string, int> idx;
  vector<string> names(n);
  
  int start_node = 0, max_age = 0;
  for (int i = 0; i < n; i++) {
    string n;
    int a;
    cin >> n >> a;
    idx[n] = i;
    names[i] = n;
    ages[i] = a;
    if (a > max_age) {
      max_age = a;
      start_node = i;
    }
  }
  string child, parent;
  for (int i = 0; i < n - 1; i++) {
    cin >> child >> parent;
    children[idx[parent]].push_back(idx[child]);
  }

  for (int i = 0; i < q; i++) {
    string n;
    int max_age;
    cin >> n >> max_age;
    queries[idx[n]].push_back({max_age, i});
  }

  for (auto &qv : queries) {
    sort(begin(qv), end(qv),
         [](const auto q1, const auto q2) { return q1.first > q2.first; });
  }

  vector<int> path;
  dfs(start_node, path);

  for (int i : result) cout << names[i] << " ";
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}