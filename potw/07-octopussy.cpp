/*
Find if Bond can deactivate the bombs in time,
taking into account the pyramid structure of the bombs.
Bottom bombs must be deactivated first.

Simple greedy task. The real time of explosion
for a bomb is min(upper_bomb + 1, current_bomb).
After adjusting times, we sort the bombs.
In the sorted array, a bomb at index i has to be
deactivated before time = i + 1.
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<int> explosion(n);
  for (int i = 0; i < n; i++) {
    int e;
    cin >> e;
    explosion[i] = e;
  }

  for (int t = 0; t < (n - 1) / 2; t++) {
    int left_index = 2 * t + 1, right_index = 2 * t + 2;

    explosion[left_index] = min(explosion[t] - 1, explosion[left_index]);
    explosion[right_index] = min(explosion[t] - 1, explosion[right_index]);
    if (explosion[left_index] <= 0 || explosion[right_index] <= 0) {
      cout << "no" << endl;
      return;
    }
  }
  sort(explosion.begin(), explosion.end());

  for (int t = 1; t < n; t++)
    if (explosion[t - 1] < t) {
      cout << "no" << endl;
      return;
    }

  cout << "yes" << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}