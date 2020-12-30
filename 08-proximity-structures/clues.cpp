/*
Find if it's possible to talk with two radios in a network with 2 frequencies.

1. Check for interference - if it's possible to two-color a network
2. While doing that, enumerate which component a station belongs to
3. Success if radios are close enough to a station and no interferences
*/

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/squared_distance_2.h>

#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <queue>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

using namespace std;

bool can_2_color(Triangulation &t, int stations_nr, long radius_sq,
                 vector<int> &components) {
  enum color { none, white, black };
  vector<color> colors(stations_nr, none);
  auto opposite = [](color c) { return c == black ? white : black; };
  vector<bool> visited(stations_nr, false);

  vector<K::Point_2> set1, set2;
  set1.reserve(stations_nr);
  set2.reserve(stations_nr);
  int component_nr = 1;

  for (auto vh : t.finite_vertex_handles()) {
    if (visited[vh->info()]) continue;

    colors[vh->info()] = white;

    queue<Triangulation::Vertex_handle> Q;
    Q.push(vh);

    while (!Q.empty()) {
      auto curr = Q.front();
      Q.pop();

      int v1 = curr->info();
      color color1 = colors[v1];
      components[v1] = component_nr;

      auto vc = t.incident_vertices(curr);
      do {
        if (t.is_infinite(vc)) continue;

        long dist = CGAL::squared_distance(curr->point(), vc->point());
        if (dist <= radius_sq) {
          int v2 = vc->info();
          color color2 = colors[v2];
          if (color2 == none) Q.push(vc);

          if (color1 != none) {
            if (color1 == color2)
              return false;
            else if (color2 == none)
              colors[v2] = opposite(color1);
          } else if (color2 != none)
            color1 = opposite(color2);
        }
      } while (++vc != t.incident_vertices(curr));
      colors[v1] = color1;
      if (!visited[v1]) {
        if (color1 == white)
          set1.emplace_back(curr->point());
        else
          set2.emplace_back(curr->point());
      }
      visited[v1] = true;
    }
    component_nr++;
  }

  // make two triangulations to check that
  // there are no overlaps in two sets of same frequence
  Triangulation t1, t2;
  t1.insert(set1.begin(), set1.end());
  t2.insert(set2.begin(), set2.end());

  for (auto e : t1.finite_edges())
    if (t1.segment(e).squared_length() <= radius_sq) return false;

  for (auto e : t2.finite_edges())
    if (t2.segment(e).squared_length() <= radius_sq) return false;
  return true;
}

void solve(int T) {
  int stations_nr, clues_nr;
  long radius;
  cin >> stations_nr >> clues_nr >> radius;
  const long radius_sq = radius * radius;

  typedef pair<K::Point_2, Index> IPoint;
  vector<IPoint> stations(stations_nr);
  for (int i = 0; i < stations_nr; i++) {
    K::Point_2 p;
    cin >> p;
    stations[i] = {p, i};
  }

  // construct triangulation
  Triangulation t;
  t.insert(stations.begin(), stations.end());

  vector<int> components(stations_nr);
  bool no_interference = can_2_color(t, stations_nr, radius_sq, components);

  // for each set of locations, check if communication is routable
  while (clues_nr--) {
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;

    if (!no_interference) {
      cout << "n";
      continue;
    }

    K::Point_2 point1 = K::Point_2(x1, y1);
    K::Point_2 point2 = K::Point_2(x2, y2);

    long dist = CGAL::squared_distance(point1, point2);
    if (dist <= radius_sq) {
      cout << "y";
      continue;
    }

    // calculate distance to nearest vertex for both points
    Triangulation::Vertex_handle v1 = t.nearest_vertex(point1);
    Triangulation::Vertex_handle v2 = t.nearest_vertex(point2);

    long dist1 = CGAL::squared_distance(point1, v1->point());
    long dist2 = CGAL::squared_distance(point2, v2->point());

    bool reachable = dist1 <= radius_sq && dist2 <= radius_sq;
    bool same_component = components[v1->info()] == components[v2->info()];

    cout << "ny"[reachable && same_component];
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve(t);
  return 0;
}