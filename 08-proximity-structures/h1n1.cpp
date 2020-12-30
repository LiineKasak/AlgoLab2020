/*
Find if it's possible to escpape infected people with a radius,
i.e. if we can move a disk through points to escape.

The dual of Delaunay triangulation - Voronoi diagram - helps here.
We don't construct it, but rather move along Voronoi diagram by idea.
In implementation, to avoid computing the same thing again,
we precompute for each face what is the radius of the biggest disk that can
escape.

We start by finding the infinite faces, and saving the maximum radius
allowed to escape it's finite neighbor face. These faces we add to our queue.

Now we go over the queued faces, and see if we can find a better escape value
for each neighboring faces (or if they don't have one yet).
If we update a neighbors escape value, we add it to the queue.

So when finally querying for a user if it's possible to escape given
a radius and location, we just locate its face and
compare the given radius to the maximum allowed disk radius.
*/

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

typedef int Index;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> T;

using namespace std;

void solve(int n) {
  vector<K::Point_2> points(n);
  for (Index i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    points[i] = K::Point_2(x, y);
  }

  T t;
  t.insert(points.begin(), points.end());

  Index i = 0;
  // initialize face indices. we don't need indices for infinite faces
  for (auto f : t.finite_face_handles()) f->info() = i++;

  // find the outmost faces via infinite faces
  // initialize their escape distance and add to queue
  queue<T::Face_handle> q;
  vector<K::FT> escape_dist(t.number_of_faces(), -1);

  for (T::Face_handle f : t.all_face_handles()) {
    if (t.is_infinite(f)) {
      // find infinite vertex
      int i_vertex;
      for (int i = 0; i < 3; i++)
        if (t.is_infinite(f->vertex(i))) i_vertex = i;

      // the escape distance is the edge between finite vertices
      K::FT d = CGAL::squared_distance(f->vertex(T::cw(i_vertex))->point(),
                                       f->vertex(T::ccw(i_vertex))->point());
      int ni = f->neighbor(i_vertex)->info();
      if (escape_dist[ni] == -1)
        escape_dist[ni] = d;
      else
        escape_dist[ni] = min(escape_dist[ni], d);
      q.push(f->neighbor(i_vertex));
    }
  }

  // update values of neighbor faces and if updated, then add to queue
  while (!q.empty()) {
    T::Face_handle fh = q.front();
    q.pop();
    Index ci = fh->info();

    for (int i = 0; i < 3; i++) {
      T::Face_handle nfh = fh->neighbor(i);
      Index ni = nfh->info();
      if (t.is_infinite(nfh)) continue;

      K::FT edge_length = CGAL::squared_distance(
          fh->vertex(T::cw(i))->point(), fh->vertex(T::ccw(i))->point());
      K::FT this_escape = min(escape_dist[ci], edge_length);

      // only add to queue if we updated the value
      if (escape_dist[ni] == -1 || this_escape > escape_dist[ni]) {
        escape_dist[ni] = this_escape;
        q.push(nfh);
      }
    }
  }

  int nr_users;
  cin >> nr_users;
  while (nr_users--) {
    int x, y;
    K::FT d;
    cin >> x >> y >> d;
    K::Point_2 p = K::Point_2(x, y);
    T::Face_handle fh = t.locate(p);

    // has to be far enough from closest vertex to have a valid start
    K::FT nearest = CGAL::squared_distance(p, t.nearest_vertex(p)->point());
    if (nearest < d)
      cout << 'n';
    else if (t.is_infinite(fh))
      cout << 'y';
    else {
      K::FT radius = escape_dist[fh->info()] / 4;
      cout << "ny"[radius >= d];
    }
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int n;
  while (cin >> n && n != 0) solve(n);
  return 0;
}