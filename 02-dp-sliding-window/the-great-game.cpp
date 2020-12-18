#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void solve() {
  int vertices, edges;
  cin >> vertices >> edges;
  int meeple_sherlock, meeple_moriarty;
  cin >> meeple_sherlock >> meeple_moriarty;
  // so the last winning position = 0
  auto reverse_pos = [&](int pos) { return vertices - pos; };

  vector<vector<int>> incoming_edges(vertices);
  for (int i = 0; i < edges; i++) {
    int s, t;
    cin >> s >> t;
    incoming_edges[reverse_pos(t)].push_back(reverse_pos(s));
  }

  // min and max values for each position
  vector<int> mini(vertices, vertices), maxi(vertices, -1);
  mini[0] = 0;
  maxi[0] = 0;
  for (int pos = 0; pos < vertices; pos++) {
    for (int option : incoming_edges[pos]) {
      mini[option] = min(mini[option], maxi[pos] + 1);
      maxi[option] = max(maxi[option], mini[pos] + 1);
    }
  }

  int moves_sherlock = mini[reverse_pos(meeple_sherlock)];
  int moves_moriarty = mini[reverse_pos(meeple_moriarty)];
  if (moves_sherlock < moves_moriarty ||
      (moves_sherlock == moves_moriarty && moves_sherlock % 2 == 1)) {
    cout << 0 << endl;  // sherlock win
  } else
    cout << 1 << endl;  // moriarty win
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}