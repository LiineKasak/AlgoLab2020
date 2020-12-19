/*
Finding if a ray intersects with any segments.
Epic is okay since we don't need to calculate the value of the intersection.
*/

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int main() {
  ios_base::sync_with_stdio(false);

  while (true) {
    int n;
    cin >> n;
    if (n == 0) break;

    long x, y, a, b;
    cin >> x >> y >> a >> b;
    K::Ray_2 path = K::Ray_2(K::Point_2(x, y), K::Point_2(a, b));

    K::Point_2 p1, p2;
    bool intersect = false;
    while (n--) {
      long r, s, t, u;
      cin >> r >> s >> t >> u;
      if (intersect) continue;
      K::Segment_2 seg = K::Segment_2(K::Point_2(r, s), K::Point_2(t, u));
      intersect = CGAL::do_intersect(seg, path);
    }

    K::Ray_2 g = K::Ray_2(K::Point_2(x, y), K::Point_2(a, b));

    cout << (intersect ? "yes" : "no") << endl;
  }

  return 0;
}
