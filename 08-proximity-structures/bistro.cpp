/*
Use triangulation to easily find the closest point to a random new point.
*/

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>  // to calculate euclidean distance

#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

using namespace std;

void solve(int nr_restaurants) {
  vector<K::Point_2> locations(nr_restaurants);  // current locations of restaurants
  for (int i = 0; i < nr_restaurants; i++) {
    int x, y;
    cin >> x >> y;
    locations[i] = K::Point_2(x, y);
  }

  Triangulation t;
  t.insert(locations.begin(), locations.end());

  int m;
  cin >> m;  // nr of possible locations
  for (int i = 0; i < m; i++) {
    int x, y;
    cin >> x >> y;
    K::Point_2 point = K::Point_2(x, y);

    // calculate distance to nearest vertex (current location)
    Triangulation::Vertex_handle v = t.nearest_vertex(K::Point_2(x, y));
    long dist = CGAL::squared_distance(point, v->point());
    cout << dist << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n;
    cin >> n;
    if (n == 0) break;
    solve(n);
  }
  return 0;
}