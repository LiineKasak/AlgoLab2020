/*
Calculate number of even and odd sequences from 0...n,
then calculate number of even combinations for all pairs of i...j.
*/

#include <iostream>

using namespace std;

void solve() {
  int n, t;
  cin >> n;
  bool is_even = true;
  int even = 0, odd = 0;  // number of even and odd S_i

  for (int i = 0; i < n; i++) {
    cin >> t;
    if (t == 1) is_even = !is_even;

    if (is_even)
      even++;
    else
      odd++;
  }

  cout << even * (even - 1) / 2  // nr of pairs of S_(i-1) and S_j when both even
    + odd * (odd - 1) / 2  // nr of pairs of S_(i-1) and S_j when both odd
    + even   // nr of x_0...x_i even sums
    << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}