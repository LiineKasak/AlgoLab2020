/*
Finidng the first intersection on a Ray.
Epec kernel because we need to calculate exact values of intersections.

Tip: When one intersection found, use a new Segment(ray_start, intersection)
to find new intersections.
*/

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

int main() {
  ios_base::sync_with_stdio(false);

  while (true) {
    int n;
    cin >> n;
    if (n == 0) break;
    long x, y, a, b;
    cin >> x >> y >> a >> b;

    K::Point_2 p1, p2;
    vector<vector<long>> segments(n);

    for (int i = 0; i < n; i++) {
      long r, s, t, u;
      cin >> r >> s >> t >> u;
      segments[n] = {r, s, t, u};
    }

    K::Point_2 start = K::Point_2(x, y);
    auto ray = K::Ray_2(start, K::Point_2(a, b));
    K::Segment_2 ray_seg;
    bool is_intersect = false;
    K::Point_2 min_point;

    for (vector<long> seg_points : segments) {
      K::Segment_2 seg = K::Segment_2(K::Point_2(seg_points[0], seg_points[1]),
                                      K::Point_2(seg_points[2], seg_points[3]));
      bool intersect = is_intersect ? CGAL::do_intersect(seg, ray_seg)
                                    : CGAL::do_intersect(seg, ray);
      if (intersect) {
        is_intersect = true;
        auto o = CGAL::intersection(seg, ray);
        if (const K::Point_2* op = boost::get<K::Point_2>(&*o))
          min_point = *op;
        else if (const K::Segment_2* os = boost::get<K::Segment_2>(&*o)) {
          if (K::Segment_2(start, os->source()).squared_length() <
              K::Segment_2(start, os->target()).squared_length())
            min_point = os->source();
          else
            min_point = os->target();
        }
        ray_seg = K::Segment_2(start, min_point);
      }
    }

    if (is_intersect)
      cout << (long)floor_to_double(min_point.x()) << " "
           << (long)floor_to_double(min_point.y()) << endl;
    else
      cout << "no" << endl;
  }

  return 0;
}
