/*
Find the cheapest combination of beverages which amount to k liters.
If the cost is equal for multiple combinations, find the one with
the largest number of distinct beverages.
Note: one beverage can be bought more than one time.

Since the input sizes are rather small, we can use DP to precompute the values.
For the i'th beverage and j liters we look at two values:
1) the current beverage + the (j-liters[i])'th entry of the same i'th row,
2) the value for the jth entry of the (i-1)'th beverage.

We prefer the value which is of lower cost or if the costs are the same then the
higher number of distinct beverages.
*/

#include <iostream>
#include <limits>
#include <vector>

using namespace std;

const long inf = numeric_limits<int>::max();

struct state {
  long price;
  long bevs;
  bool use;
};

struct bev {
  int cost;
  int volume;
};

state best(state &s1, state &s2) {
  if (s1.price < s2.price)
    return s1;
  else if (s2.price < s1.price)
    return s2;
  else
    return (s1.bevs > s2.bevs) ? s1 : s2;
}

void solve() {
  int n, k;
  cin >> n >> k;
  vector<bev> bevs(n);
  for (int i = 0; i < n; i++) {
    int c, v;
    cin >> c >> v;
    bevs[i] = {c, v};
  }

  vector<vector<state>> dp(n + 1, vector<state>(k + 1));
  // for every beverage the 0l amount costs 0
  for (int i = 0; i <= n; i++) dp[i][0] = {0, 0, false};
  // since there is no 0th beverage the cost is inf
  for (int i = 0; i <= k; i++) dp[0][i] = {inf, 0, false};

  for (int i_bev = 1; i_bev <= n; i_bev++) {
    bev b = bevs[i_bev - 1];

    for (int i_volume = 1; i_volume <= k; i_volume++) {
      int needed = max(0, i_volume - b.volume);
      state comp = dp[i_bev][needed];
      // only add one to count if the i-th beverage has not been used yet
      int add_count = !comp.use;
      state with_bev = {comp.price + b.cost, comp.bevs + add_count, true};

      state without_bev = dp[i_bev - 1][i_volume];
      without_bev.use = false;
      dp[i_bev][i_volume] = best(with_bev, without_bev);
    }
  }
  state best = dp[n][k];
  cout << best.price << " " << best.bevs << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}