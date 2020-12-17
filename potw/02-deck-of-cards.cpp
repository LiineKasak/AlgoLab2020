#include <iostream>
#include <vector>

using namespace std;

void solve() {
  int nr_cards, target;
  cin >> nr_cards >> target;
  vector<int> cards(nr_cards);
  for (int i = 0; i < nr_cards; i++) cin >> cards[i];

  int best_left = 0, left = 0, best_right = 0, right = -1, sum = 0,
      best_diff = target, diff;  // best left right sum diff

  while (right < nr_cards) {
    // curr diff
    diff = target - sum;
    if (abs(diff) < best_diff) {
      // new best diff
      best_left = left;
      best_right = right;
      best_diff = abs(diff);
      if (best_diff == 0) {
        break;
      }
    }
    // move left index to right
    // since there can't be a better result with higher sum
    if (diff < 0) {
      sum -= cards[left];
      left++;
      continue;
    }
    right++;
    sum += cards[right];
  }
  cout << best_left << " " << best_right << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}