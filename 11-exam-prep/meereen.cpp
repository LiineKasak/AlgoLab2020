#include <iostream>
#include <vector>

using namespace std;

struct state {
  int type;
  int size_diff;
  int total_score;
};

long score(int last, int this, int size_diff) {
    return (1 + last != this) * 1000 - pow(2, abs(diff));
}

void solve() {
  int n, k, m;
  cin >> n >> k >> m;

  int first_type;
  cin >> first_type;
  n--;

  // nnn, snn, nsn, ssn
  // sss, nss, sns, nns

  // nn, sn, ss, ns
  vector<vector<state>> states(4);
  for (int i = 0; i < 2; i++)
    states[i].push_back({first_type, 1, 998});  // north last
  for (int i = 2; i < 4; i++)
    states[i].push_back({first_type, -1, 998});  // south last
  long total_score = 998;

  int i_state = 0;
  while (n-- && ++i_state) {
    int type;
    cin >> type;

    state last_n, last_s;
    if (states[0][i_state-1].total_score > states[1][i_state-1].total_score)
        last_n = states[0][i_state-1];
    else last_n = states[1][i_state-1];
    if (states[2][i_state-1].total_score > states[3][i_state-1].total_score)
        last_s = states[2][i_state-1];
    else last_s = states[3][i_state-1];
    
    states[0].push_back({type, last_n.size_diff+1, last_n.total_score + score(last_n.type, type, last_n.size_diff+1)});
    states[1].push_back({type, last_s.size_diff+1, last_s.total_score + score(last_s.type, type, last_s.size_diff+1)});
    states[2].push_back({type, last_s.size_diff-1, last_s.total_score + score(last_s.type, type, last_s.size_diff-1)});
    states[3].push_back({type, last_n.size_diff-1, last_n.total_score + score(last_n.type, type, last_n.size_diff-1)});
  }

  for (int i = 0; i < 4; i++) {
      state s = states[i][i_state];
      cout << s.type << " " << s.size_diff << " " << s.total_score;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}