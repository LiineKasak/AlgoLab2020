#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <cassert>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

using namespace std;

vector<K::Point_2> read_points(int points_nr) {
  vector<K::Point_2> points(points_nr);
  for (int i = 0; i < points_nr; i++) {
    int x, y;
    cin >> x >> y;
    points[i] = K::Point_2(x, y);
  }
  return points;
}

struct Triangle {
  vector<K::Point_2> points;  // size 6
  vector<bool> is_rights;     // size 3
};

vector<Triangle> read_triangles(int triangles_nr) {
  // for each two points of a line save if point should be on the right side to
  // be in the triangle
  vector<Triangle> triangles(triangles_nr);
  for (int i_tri = 0; i_tri < triangles_nr; i_tri++) {
    vector<K::Point_2> points = read_points(6);
    vector<bool> is_rights(3);
    for (int i_edge = 0; i_edge < 3; i_edge++) {
      K::Point_2 p = points[i_edge * 2], q = points[i_edge * 2 + 1],
                 r = points[(i_edge * 2 + 2) % 6];
      is_rights[i_edge] = CGAL::right_turn(p, q, r);
    }
    triangles[i_tri] = {points, is_rights};
  }
  return triangles;
}

bool in_triangle(Triangle &triangle, K::Point_2 &point) {
  for (int i_edge = 0; i_edge < 3; i_edge++) {
    K::Point_2 p1 = triangle.points[i_edge * 2], p2 = triangle.points[i_edge * 2 + 1];
    bool in = CGAL::right_turn(p1, p2, point) == triangle.is_rights[i_edge] ||
              CGAL::collinear(p1, p2, point);
    if (!in) return false;  // has to be one of them
  }
  return true;
}

bool all_positive(vector<int> counts) {
  for (auto count : counts)
    if (count == 0) return false;
  return true;
}

void solve() {
  int nr_points, nr_tri;
  cin >> nr_points >> nr_tri;
  vector<K::Point_2> points = read_points(nr_points);
  vector<Triangle> triangles = read_triangles(nr_tri);

  vector<vector<bool>> contains_point(nr_tri, vector<bool>(nr_points));
  for (int i_tri = 0; i_tri < nr_tri; i_tri++) {
    Triangle t = triangles[i_tri];
    for (int i_point = 0; i_point < nr_points; i_point++)
      contains_point[i_tri][i_point] = in_triangle(t, points[i_point]);
  }

  vector<int> segment_count(nr_points - 1);
  int triangle_count = 0, best_count = nr_tri, i_left_tri = 0;
  for (int i_right_tri = 0; i_right_tri < nr_tri; i_right_tri++) {
    for (int i_point = 0; i_point < nr_points - 1; i_point++)
      segment_count[i_point] += contains_point[i_right_tri][i_point] && contains_point[i_right_tri][i_point + 1];
    triangle_count++;
    while (all_positive(segment_count)) {
      best_count = min(triangle_count, best_count);
      for (int i_point = 0; i_point < nr_points - 1; i_point++)
        segment_count[i_point] -= contains_point[i_left_tri][i_point] && contains_point[i_left_tri][i_point + 1];
      i_left_tri++;
      triangle_count--;
    }
  }
  cout << best_count << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
}