/*
Given a set of tents and minimum number of tents given to family, find
1) maximum distance between different family tents, given number of families
2) maximum number of families, given the required distance between tents

Implementation:
- Number of tents is high, thus we only look at the shortest edges - given by
a triangulation of the tent locations.
- We only need the MST edges, so we sort the triangulation edges in ascending
order to iterate over the edges and make clusters of tents.
- For a minimum number of tents larger than 1 it gets complicated. A family can
have tents in "different clusters". We have to save the number of tent clusters
for sizes 1, 2, 3 and 4+. This way we can combine for example sizes 1 and 2 to
get a tent cluster for families with required tents of 3.
*/

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

using namespace std;

const int MAX_TENTS = 4;

int max_families(vector<int>& counts, const int tents_per_family) {
  if (tents_per_family == 1)
    return accumulate(counts.begin(), counts.end(), 0);
  else if (tents_per_family == 2)
    return counts[1] / 2 + counts[2] + counts[3] + counts[4];
  else if (tents_per_family == 3) {
    if (counts[1] > counts[2])  // abundant 1s after 1+2 pairs
      return (counts[1] - counts[2]) / 3 + counts[2] + counts[3] + counts[4];
    else  // abundant 2s after 1+2 pairs
      return counts[1] + (counts[2] - counts[1]) / 2 + counts[3] + counts[4];
  } else {
    int count = counts[4];
    count += min(counts[1], counts[3]);
    int ones_left = max(0, counts[1] - counts[3]);
    int threes_left = max(0, counts[3] - counts[1]);
    int twos = counts[2] + threes_left;
    count += twos / 2;
    count += ones_left / 4;
    if (twos % 2 == 1 && ones_left % 4 >= 2) count++;
    return count;
  }
}

void solve() {
  // f0 given families 1st q
  // s0 given min sqaured dist 2nd q
  int nr_tents, tents_per_family, f0;
  K::FT s0;
  cin >> nr_tents >> tents_per_family >> f0 >> s0;

  vector<pair<K::Point_2, Index>> tents(nr_tents);
  for (int i = 0; i < nr_tents; i++) {
    int x, y;
    cin >> x >> y;
    tents[i] = {K::Point_2(x, y), i};
  }
  Triangulation t(tents.begin(), tents.end());

  EdgeV edges;
  edges.reserve(3 * nr_tents);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second + 1) % 3)->info();
    Index i2 = e->first->vertex((e->second + 2) % 3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
            [](const Edge& e1, const Edge& e2) -> bool {
              return std::get<2>(e1) < std::get<2>(e2);
            });
  vector<int> comp_size(nr_tents, 1);
  vector<int> counts(5, 0);
  counts[1] = nr_tents;
  long max_s_for_f0 = 0;
  long max_f_for_s0 = 1;

  boost::disjoint_sets_with_storage<> uf(nr_tents);
  Index n_components = nr_tents;
  int nr_families = (tents_per_family == 1) ? nr_tents : 0;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      K::FT edge_length = get<2>(*e);
      if (nr_families >= f0) max_s_for_f0 = edge_length;
      if (edge_length >= s0 && nr_families > max_f_for_s0) max_f_for_s0 = nr_families;
      if (nr_families < f0 && edge_length > s0) break;

      uf.link(c1, c2);
      if (--n_components == 1) break;

      Index new_comp = uf.find_set(std::get<0>(*e));
      int new_size = min(comp_size[c2] + comp_size[c1], MAX_TENTS);
      counts[comp_size[c1]]--;
      counts[comp_size[c2]]--;
      counts[new_size]++;
      comp_size[new_comp] = new_size;
      nr_families = max_families(counts, tents_per_family);
    }
  }

  cout << max_s_for_f0 << " " << max_f_for_s0 << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}