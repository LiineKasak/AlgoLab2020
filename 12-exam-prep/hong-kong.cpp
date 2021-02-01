/*
Find if balloons can takeoff from a forest.

To contruct this world we construct a triangulation and navigate the faces.
To takeoff, we need a distance between disks of at least r_balloon + r_tree.
So either the circumcenter of the face has a large enough distance
(from the center to a point 4 * (r_balloon + r_tree)**2) or
we can move the disk of a balloon to a face where we can take off.
To move to another face, we need the common edge to have a length of at least
4 * (r_balloon + r_tree)**2.

To avoid multiple calculations, we find the largest takeoff distance for each
face handle and store it in info(). We process every face and see if we can
optimize the takeoff distance of neighboring faces.
To avoid a large number of iterations, we process these faces with a
priority_queue, picking the largest takeoff distances first.

For this problem, we need to use the EPEC kernel to calculate the center of a
face, and the squared distances do not fit into any type.
*/

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <iostream>
#include <limits>
#include <queue>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

using namespace std;

void solve() {
  int nr_trees, nr_balloons;
  K::FT r_tree;
  cin >> nr_trees >> nr_balloons >> r_tree;

  vector<K::Point_2> trees(nr_trees);
  for (int i = 0; i < nr_trees; i++) {
    long x, y;
    cin >> x >> y;
    trees[i] = K::Point_2(x, y);
  }

  Triangulation t;
  t.insert(trees.begin(), trees.end());

  // calculate initial escape distances for faces
  const K::FT MAX = K::FT(numeric_limits<double>::max());
  priority_queue<pair<K::FT, Triangulation::Face_handle>> q;

  for (auto fh : t.all_face_handles()) {
    if (t.is_infinite(fh))
      fh->info() = MAX;
    else
      fh->info() =
          CGAL::squared_radius(fh->vertex(0)->point(), fh->vertex(1)->point(),
                               fh->vertex(2)->point());
    q.push({fh->info(), fh});
  }

  while (!q.empty()) {
    auto pair = q.top();
    auto fh = pair.second;
    q.pop();
    if (pair.first < fh->info()) continue;

    for (int i = 0; i < 3; i++) {
      auto nfh = fh->neighbor(i);
      if (t.is_infinite(nfh)) continue;

      K::Point_2 p1 = fh->vertex(t.cw(i))->point();
      K::Point_2 p2 = fh->vertex(t.ccw(i))->point();
      K::FT edge = CGAL::squared_distance(p1, p2);
      K::FT dist = min(edge, fh->info());

      if (dist > nfh->info()) {
        nfh->info() = dist;
        q.push({dist, nfh});
      }
    }
  }

  while (nr_balloons--) {
    long x, y;
    K::FT r_balloon;
    cin >> x >> y >> r_balloon;
    K::Point_2 balloon = K::Point_2(x, y);

    auto vh = t.nearest_vertex(balloon);
    const K::FT radius = K::FT(r_balloon + r_tree) * K::FT(r_balloon + r_tree);
    const K::FT dist_to_tree = CGAL::squared_distance(balloon, vh->point());
    bool far_enough_from_tree = dist_to_tree >= radius;

    auto fh = t.locate(balloon);
    bool path_to_valid_start = fh->info() >= 4 * radius;

    cout << "ny"[far_enough_from_tree && path_to_valid_start];
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}