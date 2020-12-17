#include <iostream>
#include <vector>

using namespace std;

void solve() {
  int n; cin >> n; n++;
  vector<vector<int>> m(n, vector<int>(n));
  
  for (int i = 1; i < n; i++)
    for (int j = 1; j < n; j++)
      cin >> m[i][j];
  
  // matrix holding the number of ones in a submatrix
  vector<vector<int>> pre(n, vector<int>(n));
  for (int i = 1; i < n; i++)
    for (int j = 1; j < n; j++)
      pre[i][j] = m[i][j] + pre[i-1][j] + pre[i][j-1] - pre[i-1][j-1];

  
  int even = 0;
  for (int i1 = 1; i1 < n; i1++) {
    for (int i2 = i1; i2 < n; i2++) {
      vector<int> s(n); // even pairs on s
      vector<int> partial(n); // partials sums of s
      for (int j = 1; j <= n; j++) {
        s[j] = pre[i2][j] - pre[i2][j-1] - pre[i1-1][j] + pre[i1-1][j-1];
        partial[j] = partial[j-1] + s[j];
      }
      
      int i_even = 0, i_odd = 0;
      for (int j = 1; j < n; j++) {
        if (partial[j] % 2 == 0) i_even++;
        else i_odd++;
      }
      
      even += i_even*(i_even-1)/2 + i_odd*(i_odd-1)/2 + i_even;
    }
  }
  cout << even << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
  
  return 0;
}