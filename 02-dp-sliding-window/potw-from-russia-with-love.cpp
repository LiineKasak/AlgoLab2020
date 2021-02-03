#include <iostream>
#include <vector>

using namespace std;

int rec(vector<int> &values, int l, int r, int nr_players, int curr_player,
        int maxi_player, vector<vector<int>> &mini) {
  if (l == r) return maxi_player == curr_player ? values[l] : 0;
  if (l + 1 == r) {
    if (maxi_player == curr_player)
      return max(values[l], values[r]);
    else if (maxi_player == curr_player + 1)
      return min(values[l], values[r]);
    else
      return 0;
  };

  int next_player = curr_player == nr_players ? 0 : curr_player + 1;
  if (curr_player == maxi_player)
    return max(values[l] + rec(values, l + 1, r, nr_players, next_player, maxi_player, mini),
               values[r] + rec(values, l, r - 1, nr_players, next_player, maxi_player, mini));
  if (mini[l][r] == -1)
    mini[l][r] = min(rec(values, l + 1, r, nr_players, next_player, maxi_player, mini),
                    rec(values, l, r - 1, nr_players, next_player, maxi_player, mini));
  return mini[l][r];
}

void solve() {
  int nr_coins, nr_passengers, index_passenger;
  cin >> nr_coins >> nr_passengers >> index_passenger;
  vector<int> coin_values(nr_coins);
  vector<vector<int>> mini(
      vector<vector<int>>(nr_coins, vector<int>(nr_coins, -1)));
  for (int i = 0; i < nr_coins; i++) cin >> coin_values[i];

  cout << rec(coin_values, 0, nr_coins - 1, nr_passengers - 1, 0,
              index_passenger, mini)
       << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}