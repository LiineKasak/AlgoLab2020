/*
Sliding window over words in a text to find the shortest possible
snippet length containing all required words.
*/

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<int> word_occurence(n);
  for (int i = 0; i < n; i++) cin >> word_occurence[i];
  vector<pair<int, int>> positions;
  int min_valid = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < word_occurence[i]; j++) {
      int pos;
      cin >> pos;
      if (j == 0) min_valid = max(min_valid, pos);
      positions.push_back({pos, i});
    }
  }

  sort(positions.begin(), positions.end(),
       [&](const auto p1, const auto p2) { return p1 < p2; });

  vector<int> last_seen(n, -1);
  queue<pair<int, int>> Q;
  int from = positions[0].first, to = 0;
  int shortest_length = 1410065408;
  for (auto p : positions) {
    to = p.first;
    Q.push(p);
    last_seen[p.second] = p.first;
    if (p.first < min_valid) continue;

    while (last_seen[Q.front().second] > Q.front().first) {
      Q.pop();
      from = Q.front().first;
    }
    shortest_length = min(shortest_length, to - from + 1);
  }
  cout << shortest_length << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}