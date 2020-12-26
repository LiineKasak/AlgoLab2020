/*
Greedy scheduling task.
Twist: the ring which the boat is connected to
can be anywhere within the length of the boat.

Solve as a typical greedy scheduling task,
but add checks to see if next boats give better value.
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct boat {
  int length;
  int pos; // position for the ring
};

bool sort_by_position(const boat &boat1, const boat &boat2) {
  if (boat1.pos == boat2.pos) return boat1.length < boat2.length;
  return boat1.pos < boat2.pos;
}

void solve() {
  int nr_boats;
  cin >> nr_boats;
  vector<boat> boats(nr_boats);

  for (int i = 0; i < nr_boats; i++) {
    int length, position;
    cin >> length >> position;
    boats[i] = {length, position};
  }

  sort(boats.begin(), boats.end(), sort_by_position);

  int nr = 0, end = -1000000;
  for (int i = 0; i < nr_boats; i++) {
    auto boat = boats[i];

    // cannot fit anyways if pos is smaller
    if (boat.pos < end) continue;

    int new_end = max(end + boat.length, boat.pos);  // pos has to be in length
    bool use = true;
    // check for better endings
    for (int j = i + 1; j < nr_boats; j++) {
      // if j-th boat pos is after this boats end,
      // no other boat will be before new end either
      if (boats[j].pos > new_end) break;
      // if the choice of the next boat will give a better ending
      int next_end = max(end + boats[j].length, boats[j].pos);
      if (next_end < new_end) {
        use = false;
        break;
      }
    }
    if (use) {
      nr++;
      end = new_end;
    }
  }

  cout << nr << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}