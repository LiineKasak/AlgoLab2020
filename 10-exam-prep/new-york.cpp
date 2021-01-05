/*
Finding the start of feasible paths given the desired path length and maximal
allowed temperature differences along the stops. Since for every point there
exists exactly only one path from New York (point 0), the graph can be
represented as a tree.

We run DFS on the tree, remembering the full path from the root to the current
node. We also keep track of the temperatures for a stack of nodes with the
desired path length. For each new node, we check if the path is feasible
(correct length and temperature difference is less or equal to max risk).

Implementation details:
* Passing newly constructed data structures while recursing is slow and requires
too much memory, thus we have to pass on the reference while recursing or hold
the data structures globally, and restore the data structures after processing a
subtree.
* We hold the temperatures of the current path in a multiset, to quickly
access min and max values. This is also faster than using a map (2.2s v 2.47s).
*/

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

int max_risk;
int goal_length;
vector<int> temps;
vector<vector<int>> routes;
set<int> result;

int temp_diff(multiset<int> &temps) {
  const int &curr_min = *temps.begin();
  const int &curr_max = *(--temps.end());
  return curr_max - curr_min;
}

void calc_feasible(int point, multiset<int> &curr_temps, vector<int> &path) {
  // update path and temperatures
  const int &temp = temps[point];
  curr_temps.insert(temp);
  int length = curr_temps.size();
  path.push_back(point);

  // resize to always keep goal length in our current temps
  if (length > goal_length) {
    int start = path[path.size() - length];
    curr_temps.erase(curr_temps.find(temps[start]));
    length--;
  }

  // check if feasible path
  int start = path[path.size() - length];
  if (length == goal_length && temp_diff(curr_temps) <= max_risk)
    result.insert(start);

  // recurse for subtrees
  for (int next : routes[point]) calc_feasible(next, curr_temps, path);

  // delete current node from the end of path and temps
  curr_temps.erase(curr_temps.find(temp));
  path.erase(--path.end());

  // restore deleted path parent
  if ((int)path.size() >= goal_length) {
    int before_start = path[path.size() - goal_length];
    curr_temps.insert(temps[before_start]);
  }
}

void solve() {
  int nr_points;
  cin >> nr_points >> goal_length >> max_risk;

  temps.resize(nr_points);
  for (int i = 0; i < nr_points; i++) cin >> temps[i];
  routes.resize(nr_points);
  for (int i = 0; i < nr_points - 1; i++) {
    int from, to;
    cin >> from >> to;
    routes[from].push_back(to);
  }

  multiset<int> curr_temps;
  vector<int> path;

  calc_feasible(0, curr_temps, path);

  if (result.size() == 0)
    cout << "Abort mission";
  else
    for (int point : result) cout << point << " ";
  cout << endl;

  result.clear();
  temps.clear();
  routes.clear();
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}