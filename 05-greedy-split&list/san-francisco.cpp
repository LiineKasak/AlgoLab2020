/*
Find the minimum number of moves to achieve target score.

We can do this by using DP.
For the i-th move we look at all the s-t edges
for all s vertices which have a score in the i-1-th move.

score[i][t] = max(score[i][t], score[i-1][s] + st.score);
*/

#include <iostream>
#include <vector>

using namespace std;

struct canal {
  int hole;
  int points;
};

int precompute_scores(long target_score, int max_moves,
                      vector<vector<canal>> &canals) {
  int nr_holes = canals.size();
  vector<vector<long>> scores(max_moves, vector<long>(nr_holes, -1));

  // 0th row is move 1
  for (canal to : canals[0]) scores[0][to.hole] = to.points;

  for (int move = 1; move < max_moves; move++) {
    for (int from = 0; from < nr_holes; from++) {
      if (scores[move - 1][from] != -1) {
        for (canal to : canals[from]) {
          scores[move][to.hole] =
              max(scores[move][to.hole], scores[move - 1][from] + to.points);
          if (scores[move][to.hole] >= target_score) return move + 1;
        }
      }
    }
  }
  return -1;
}

void solve() {
  int nr_holes, nr_canals, max_moves;
  long target_score;
  cin >> nr_holes >> nr_canals >> target_score >> max_moves;

  vector<vector<canal>> canals(nr_holes);
  vector<bool> is_end(nr_holes, true);
  for (int i = 0; i < nr_canals; i++) {
    int from, to, points;
    cin >> from >> to >> points;
    canals[from].push_back({to, points});
    is_end[from] = false;
  }
  for (int hole = 1; hole < nr_holes; hole++) {
    if (is_end[hole]) {
      // end holes can get anywhere start can get
      for (canal to : canals[0]) canals[hole].push_back(to);
    }
  }

  int moves = precompute_scores(target_score, max_moves, canals);
  if (moves == -1)
    cout << "Impossible" << endl;
  else
    cout << moves << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}