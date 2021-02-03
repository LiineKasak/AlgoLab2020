/*
Find max nr of players we can attack, given strength for each player,
attack strength and number of attackers.

1. Sliding window to find all segments we can attack such that
sum of defenses = attack strength.
2. Recursion with memoization or DP on segments to maximize result.

Debug stuff: need to mark empty spot in memo with -2, since our rec can return
-1.
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int rec(vector<int> &segments, int pos, int nr_att, vector<vector<int>> &memo) {
  if (nr_att == 0) return 0;
  if (pos >= -1 + (int)segments.size()) return -1;

  if (memo[pos][nr_att] == -2) {
    int not_chosen = rec(segments, pos + 1, nr_att, memo);
    memo[pos][nr_att] = not_chosen;

    int next_pos = segments[pos];
    if (next_pos != -1) {
      int chosen_rec = rec(segments, next_pos + 1, nr_att - 1, memo);
      int chosen = 1 + next_pos - pos + chosen_rec;
      if (chosen_rec != -1) memo[pos][nr_att] = max(chosen, not_chosen);
    }
  }
  return memo[pos][nr_att];
}

void solve() {
  int nr_players, nr_attackers, attack_strength;
  cin >> nr_players >> nr_attackers >> attack_strength;
  vector<int> players(nr_players);
  for (int i = 0; i < nr_players; i++) cin >> players[i];

  // sliding window to find all segments with defense == attack_strength
  int sum = 0;
  vector<int> segments(nr_players,
                       -1);  // mapping index to right side of segment
  for (int player_right = 0, player_left = 0; player_right < nr_players;
       player_right++) {
    sum += players[player_right];
    while (sum >= attack_strength) {
      if (sum == attack_strength) segments[player_left] = player_right;
      sum -= players[player_left];
      player_left++;
    }
  }

  // recursion with memoization
  vector<vector<int>> memo(nr_players + 1, vector<int>(nr_attackers + 1, -2));
  int val = rec(segments, 0, nr_attackers, memo);
  if (val == -1)
    cout << "fail" << endl;
  else
    cout << val << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}