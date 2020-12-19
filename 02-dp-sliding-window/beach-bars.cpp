/*
Sliding window over possible coordinates
to find the most parasols within 100m range.
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void solve() {
  int nr_parasol;
  cin >> nr_parasol;
  vector<int> coordinates(nr_parasol);
  for (int i = 0; i < nr_parasol; i++) cin >> coordinates[i];

  sort(coordinates.begin(), coordinates.end());

  int max_nr = 0, best_dist, curr_dist;
  int index_left = 0, index_curr = 0,
      index_right = -1;  // index for new el in p
  int coord = coordinates[0] - 100, ld = 0, rd = 0, curr_nr = 0;
  vector<int> optimal_positions;

  while (coordinates[nr_parasol - 1] >= coord) {
    // last left out of bounds
    if (coord - coordinates[index_left] > 100) {
      curr_nr--;
      index_left++;
    }
    // new right in bounds
    if (index_right != nr_parasol - 1 &&
        coordinates[index_right + 1] - coord == 100) {
      curr_nr++;
      index_right++;
    }

    // handle p in curr midpoint
    if (coord == coordinates[index_curr]) {
      index_curr++;
    }

    // check if new best
    curr_dist =
        max(coord - coordinates[index_left], coordinates[index_right] - coord);
    // cout << "calc max d: curr " << x << " l, r " << p[li] << " " << p[ri]
    // << endl;
    if (max_nr < curr_nr) {
      max_nr = curr_nr;
      best_dist = curr_dist;
      optimal_positions.clear();
      optimal_positions.push_back(coord);
    } else if (max_nr == curr_nr) {
      if (curr_dist < best_dist) {
        best_dist = curr_dist;
        optimal_positions.clear();
      }
      if (best_dist == curr_dist) {
        optimal_positions.push_back(coord);
      }
    }

    coord += 1;
  }

  cout << max_nr << " " << best_dist << endl;
  for (int opt : optimal_positions) {
    cout << opt << " ";
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}