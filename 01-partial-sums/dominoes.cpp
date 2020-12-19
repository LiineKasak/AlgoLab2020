/*
Finding the sum of dominoes until a dominoe's height
does not reach the next dominoe.
*/

#include <iostream>
#include <vector>

using namespace std;

void solve() {
  int dominoes_nr;
  cin >> dominoes_nr;

  vector<int> domino_heights(dominoes_nr);
  for (int i = 0; i < dominoes_nr; i++) cin >> domino_heights[i];

  int falling_dominoes = 1;
  int curr, prev = domino_heights[0];

  for (int i = 1; i < dominoes_nr; i++) {
    if (prev <= 1) break;
    falling_dominoes++;
    curr = domino_heights[i];
    prev--;
    if (curr > prev) prev = curr;
  }
  cout << falling_dominoes << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}