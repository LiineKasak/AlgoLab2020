
// NOT WORKING
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <algorithm>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

using namespace std;

struct move {
  long d;
  long t;
  K::FT d_per_t;
};

void solve() {
  int nr_moves, nr_additional;
  long d_sum, t_sum;
  cin >> nr_moves >> nr_additional >> d_sum >> t_sum;
  vector<move> moves(nr_moves);
  for (int i = 0; i < nr_moves; i++) {
    long d, t;
    cin >> d >> t;
    moves[i] = {d, t, (K::FT(d) / K::FT(t))};
  }
  vector<long> additional(nr_additional);
  for (int i = 0; i < nr_additional; i++) cin >> additional[i];

  sort(moves.begin(), moves.end(),
       [](const move &m1, const move &m2) { return m1.d_per_t < m2.d_per_t; });

  long curr_d_sum = 0;
  long curr_t_sum = 0;
  for (move m : moves) {
    curr_d_sum += m.d;
    curr_t_sum += m.t;
    if (curr_d_sum >= d_sum || curr_t_sum >= t_sum) {
      curr_d_sum -= m.d;
      curr_t_sum -= m.t;
    }
  }

  long missing_d = d_sum - curr_d_sum;
  int result = -1;
  for (int si = 0; si < nr_additional; si++) {
    if (additional[si] >= missing_d) result = si + 1;
  }
  if (result == -1 || curr_t_sum >= t_sum)
    cout << "Panoramix captured" << endl;
  else
    cout << result << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}