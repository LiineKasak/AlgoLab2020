/*
Find the largest number of Rebel planets.

The Rebel planets are connected like an MST. The trick is that if our goal is
to connect N Rebel planets, then we compute the EMST on all planets except
the first N planets which will be Empire planets.
This means that at most we can have floor(nr_planets / 2) Rebel planets.

Since we do not have a straightforward way to find the maximal number of
Rebel planets, we run binary search and calculate the number of Rebel planets,
given a goal N. If the max number is at least N, we set the search floor at.
If the max number is less than N, we cap the new search ceiling with N-1.
*/
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <tuple>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

typedef std::pair<K::Point_2, Index> Point;
typedef std::tuple<Index, Index, K::FT> Edge;

using namespace std;

bool max_planets(int nr_empire, const vector<Point>& planets, const K::FT& rr) {
  int nr_planets = planets.size();
  Delaunay t;
  t.insert(begin(planets) + nr_empire, end(planets));

  vector<Edge> edges;
  edges.reserve(3 * (planets.size() - nr_empire));
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second + 1) % 3)->info();
    Index i2 = e->first->vertex((e->second + 2) % 3)->info();
    if (i1 > i2) swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
    return get<2>(e1) < get<2>(e2);
  });

  boost::disjoint_sets_with_storage<> uf(nr_planets);
  vector<int> comp_size(nr_planets, 1);
  int max_comp = 1;

  for (auto e = edges.begin(); e != edges.end(); ++e) {
    // too far for scout vessel
    if (get<2>(*e) > rr) break;

    Index c1 = uf.find_set(get<0>(*e));
    Index c2 = uf.find_set(get<1>(*e));
    if (c1 != c2) {
      uf.link(c1, c2);
      Index new_c = uf.find_set(get<0>(*e));
      int new_size = comp_size[c1] + comp_size[c2];
      comp_size[new_c] = new_size;
      max_comp = max(new_size, max_comp);
      if (max_comp >= nr_empire) break;
    }
  }

  return max_comp >= nr_empire;
}

void solve() {
  Index nr_planets;
  K::FT r;
  cin >> nr_planets >> r;
  K::FT rr = r * r;

  vector<Point> planets(nr_planets);
  for (Index i = 0; i < nr_planets; i++) {
    long x, y;
    cin >> x >> y;
    planets[i] = {K::Point_2(x, y), i};
  }

  int low = 1, high = nr_planets / 2;

  while (low != high) {
    int mid = (low + high + 1) / 2;
    bool success = max_planets(mid, planets, rr);
    if (success) {
      low = mid;
    } else {
      high = mid - 1;
    }
  }

  cout << low << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}