/*
Just sum of addends.
*/

#include <iostream>

using namespace std;

void solve() {
  int addends_nr, sum = 0, addend;
  cin >> addends_nr;
  for (int i = 0; i < addends_nr; i++) {
    cin >> addend;
    sum += addend;
  }
  cout << sum << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}