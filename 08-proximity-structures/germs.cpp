/*
Find the time of 1) first death, 2) bacteria number under 50%,
3) time of death for the last bacteria.

Apply triangluation and calculate time of death for all bacteria.
*/

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <cmath>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef int Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

struct Edge {
  Index v1;
  Index v2;
  K::FT squared_dist;
};
typedef std::vector<Edge> EdgeV;

using namespace std;

int square_dist_to_time(K::FT x) {
  // since radius = time**2 + 1/2 -> time = sqrt(radius - 1/2)
  double radius = sqrt(x);
  if (radius - 0.5 < 0) return 0;
  return ceil(sqrt(radius - 0.5));
}

void solve(int nr_bacteria) {
  int x_min, y_min, x_max, y_max;
  cin >> x_min >> y_min >> x_max >> y_max;

  typedef std::pair<K::Point_2, Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(nr_bacteria);

  vector<K::FT> dist_to_edge(nr_bacteria);

  for (Index i = 0; i < nr_bacteria; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);

    // record distance to dish boundary
    int d1 = y - y_min;
    int d2 = y_max - y;
    int d3 = x - x_min;
    int d4 = x_max - x;
    int d = min(min(d1, d2), min(d3, d4));
    dist_to_edge[i] = K::FT(d)*K::FT(d);
  }

  Triangulation t;
  t.insert(points.begin(), points.end());

  // STEP 1: get all edges, record their length and sort by it
  EdgeV edges;
  for (const auto& e : t.finite_edges()) {
    Index i1 = e.first->vertex((e.second + 1) % 3)->info();
    Index i2 = e.first->vertex((e.second + 2) % 3)->info();
    edges.push_back({i1, i2, t.segment(e).squared_length()});
  }

  sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
    return e1.squared_dist < e2.squared_dist;
  });

  // STEP 2: while there are vertices left, iterate over sorted edges.
  // save the radius when they die
  vector<K::FT> death_time(n, -1);
  int count = nr_bacteria;
  for (Edge e : edges) {
    if (count == 0) break;

    Index idx = e.v1;
    if (death_time[idx] == -1) {
      death_time[idx] = min(dist_to_edge[idx], e.squared_dist / 4);
      count--;
    }
    idx = e.v2;
    if (death_time[idx] == -1) {
      death_time[idx] = min(dist_to_edge[idx], e.squared_dist / 4);
      count--;
    }
  }

  sort(death_time.begin(), death_time.end());

  // moment when first bacterium dies
  int t1 = square_dist_to_time(death_time[0]);

  // moment when amount of bacteria alive goes below 50% (less than n/2 alive)
  int t2 = square_dist_to_time(death_time[n / 2]);

  // moment in which the last bacterium dies
  int t3 = square_dist_to_time(death_time[n - 1]);

  cout << t1 << " " << t2 << " " << t3 << endl;
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