/*
Find rays which don't intersect with other rays or their tracks, when
rays start at x0 = 0 with different y0, and travel with the same speed.

Solution is a little bit greedy: we order by starting position,
and take all rays whose slope is smaller than the previous.

We have a problem if slopes are negative:
now we have rays whose slopes are smaller by absolute value but have larger slopes,
compared to the previous rays.
These rays can "kill" previous rays as long as they are
1. smaller in absolute value -> shorter segment to intersection
2. larger otherwise -> the intersection happens
*/
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <algorithm>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

using namespace std;

struct biker {
  int i;
  long y0;
  K::FT slope;
};

bool compare_y0(const biker &b1, const biker &b2) { return b1.y0 > b2.y0; }
bool compare_index(const biker &b1, const biker &b2) { return b1.i < b2.i; }

void solve() {
  int n;
  cin >> n;
  vector<biker> bikers(n);
  for (int i = 0; i < n; i++) {
    long y0, x1, y1;
    cin >> y0 >> x1 >> y1;
    bikers[i] = {i, y0, K::FT(y1 - y0) / K::FT(x1)};
  }
  // for greedy selection, sort by starting position
  sort(bikers.begin(), bikers.end(), compare_y0);

  vector<biker> winners;
  winners.push_back(bikers[0]);
  int last = 0;

  auto kills = [](const biker &killer, const biker &ray) {
    return abs(killer.slope) <= abs(ray.slope) && killer.slope > ray.slope;
  };
  for (int i = 1; i < n; i++) {
    biker biker = bikers[i];
    // we have a smaller slope than last
    // so the current bike and above bikes never cross
    if (biker.slope <= winners[last].slope) {
      winners.push_back(biker);
      last++;
    }
    // a lower ray "kills" higher rays
    else if (kills(biker, winners[last])) {
      while (last >= 0 && kills(biker, winners[last])) last--;
      winners.erase(winners.begin() + last + 1, winners.end());

      if (last == -1 || biker.slope <= winners[last].slope) {
        winners.push_back(biker);
        last++;
      }
    }
  }

  sort(winners.begin(), winners.end(), compare_index);

  for (auto winner : winners) cout << winner.i << " ";
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}