/*
DP to find the maximum possible sum of chosen coins in a 2 player game.
*/

#include <iostream>
#include <vector>

using namespace std;

int rec(vector<int> &coins, int left, int right, vector<vector<int>> &memo) {
  /*
  F(i, j) represents the maximum value the user
  can collect from i'th coin to j'th coin.

  F(i, j) = Max(Vi + min(F(i+2, j), F(i+1, j-1) ),
                Vj + min(F(i+1, j-1), F(i, j-2) ))
  As user wants to maximise the number of coins.

  Base Cases
      F(i, j) = Vi           If j == i
      F(i, j) = max(Vi, Vj)  If j == i + 1
  */
  if (left == right) return coins[left];
  if (left + 1 == right) return max(coins[left], coins[right]);

  if (memo[left][right] == -1)
    memo[left][right] =
        max(coins[left] + min(rec(coins, left + 2, right, memo), rec(coins, left + 1, right - 1, memo)),
            coins[right] + min(rec(coins, left + 1, right - 1, memo), rec(coins, left, right - 2, memo)));
  return memo[left][right];
}

void solve() {
  int nr_coins;
  cin >> nr_coins;
  vector<int> coins(nr_coins);
  for (int i = 0; i < nr_coins; i++) cin >> coins[i];

  vector<vector<int>> memo(nr_coins, vector<int>(nr_coins, -1));
  cout << rec(coins, 0, nr_coins - 1, memo) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}