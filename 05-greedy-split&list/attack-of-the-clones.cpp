/*
Scheduling task with a twist:
due to the circular structure, we need to
1. Find a segment with the least conflicts
2. Test scheduling score with choosing each conflict once
3. If there are no conflicts, start with this segment
*/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct jedi {
  int start;
  int end;
  long mod_s;
  long mod_e;
};

struct event {
  int segment;
  int addable;
};

void read_jedi(vector<jedi> &jedis, vector<event> &events) {
  for (int i = 0; i < (int)jedis.size(); i++) {
    int seg_start, seg_end;
    cin >> seg_start >> seg_end;
    jedis[i] = {--seg_start, --seg_end, seg_start, seg_end};
    events[2 * i] = {seg_start, 1};
    events[2 * i + 1] = {seg_end, -1};
  }
}

int find_start(vector<event> &events, int nr_segments) {
  // try to find a spot with least conflicts
  sort(events.begin(), events.end(), [](const event &j1, const event &j2) {
    return j1.segment < j2.segment;
  });

  int start = -2;
  int sum = 0;
  int min = 11;
  for (auto el : events) {
    sum += el.addable;
    if (sum < min) {
      min = sum;
      start = el.segment;
    }
  }
  return start;
}

vector<jedi> modify_jedi(vector<jedi> &jedis, int nr_segments, int start) {
  vector<jedi> new_jedis;
  for (jedi j : jedis) {
    if (j.end < j.start) j.mod_e += nr_segments;
    if (j.mod_s <= start) {
      j.mod_s += nr_segments;
      j.mod_e += nr_segments;
    }
    if (j.mod_e <= start + nr_segments) new_jedis.push_back(j);
  }

  sort(new_jedis.begin(), new_jedis.end(),
       [&](const jedi &j1, const jedi &j2) { return j1.mod_e < j2.mod_e; });
  return new_jedis;
}

int count_jedi_from(const int start, int nr_segments, vector<jedi> &jedis) {
  int end = start;
  int count = 0;
  for (auto j : jedis) {
    if (j.mod_s <= end) continue;
    end = j.mod_e;
    count++;
  }
  return count;
}

void solve() {
  int nr_jedi, nr_segments;
  cin >> nr_jedi >> nr_segments;

  vector<jedi> jedis(nr_jedi);
  vector<event> events(2 * nr_jedi);
  read_jedi(jedis, events);

  int start = find_start(events, nr_segments) + 1;
  int best = -1;

  for (jedi j : jedis) {
    // for all segments our start overlapped with
    // test them for better results
    bool overlaps = j.mod_s <= start && start < j.mod_e;
    if (overlaps) {
      vector<jedi> overlap_jedi = modify_jedi(jedis, nr_segments, j.start - 1);
      int res = count_jedi_from(j.start - 1, nr_segments, overlap_jedi);
      best = max(best, res);
    }
  }
  if (best == -1) {
    // we only need to test with start if nr of overlapping segments is 0
    vector<jedi> new_jedi = modify_jedi(jedis, nr_segments, start);
    best = count_jedi_from(start, nr_segments, new_jedi);
  }

  cout << best << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}