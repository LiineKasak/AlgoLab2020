#include <iostream>

using namespace std;

void solve() {
  int n, s = 0, t;
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> t;
    s += t;
  }
  cout << s << endl;
}

int main() {
  ios_base::sync_with_stdio(false);

  int t;
  cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}