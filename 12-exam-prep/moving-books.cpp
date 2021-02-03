/*
Finding the minimum amount of time needed to carry boxes, given the max weight
each friend can carry and the weights of the boxes.
This is equivalent to finding the minimum number of boxes each person carries
at most and using this number to calculate the time.
Easy implementation of using greedy and binary search.
*/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

bool try_max_boxes(int per_person, vector<int> &max_weight,
                   vector<int> &box_weight) {
  for (int i_person = 0, i_box = 0; i_person < (int)max_weight.size();
       i_person++, i_box += per_person) {
    i_box = min(i_box, (int)box_weight.size() - 1);
    if (box_weight[i_box] > max_weight[i_person]) return false;
    if (i_box == (int)box_weight.size() - 1) return true;
  }
  return true;
}

void solve() {
  int n, m;
  cin >> n >> m;

  vector<int> max_weight(n);
  vector<int> box_weight(m);
  for (int i = 0; i < n; i++) cin >> max_weight[i];
  for (int i = 0; i < m; i++) cin >> box_weight[i];
  sort(max_weight.begin(), max_weight.end(), greater<int>());
  sort(box_weight.begin(), box_weight.end(), greater<int>());
  if (box_weight[0] > max_weight[0]) {
    cout << "impossible" << endl;
    return;
  }

  int low = max((int)ceil(m / n), 1), high = m, mid;
  while (low != high) {
    mid = (high + low) / 2;
    bool success = try_max_boxes(mid, max_weight, box_weight);
    if (success)
      high = mid;
    else
      low = mid + 1;
  }

  long time_taken = low * 3 - 1;
  cout << time_taken << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}