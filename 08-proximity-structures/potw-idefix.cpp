#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef std::tuple<Index, Index, K::FT> Edge;

using namespace std;

void solve() {
  long nr_trees, nr_bones, goal_radius, goal_bones;
  cin >> nr_trees >> nr_bones >> goal_radius >> goal_bones;

  vector<pair<K::Point_2, Index>> trees(nr_trees);
  Triangulation t;
  for (int i = 0; i < nr_trees; i++) {
    int x, y;
    cin >> x >> y;
    trees[i] = {K::Point_2(x, y), i};
  }
  t.insert(trees.begin(), trees.end());
  vector<K::Point_2> bones(nr_bones);
  for (int i = 0; i < nr_bones; i++) {
    int x, y;
    cin >> x >> y;
    K::Point_2 point = K::Point_2(x, y);
    bones[i] = point;
  }

  vector<Edge> edges;
  edges.reserve(3 * nr_trees + nr_bones);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second + 1) % 3)->info();
    Index i2 = e->first->vertex((e->second + 2) % 3)->info();
    // segment distance is (2r)**2 = 4r**2 which is same as s value
    long dist = t.segment(e).squared_length();
    edges.emplace_back(i1, i2, dist);
  }

  for (int i = 0; i < nr_bones; i++) {
    auto point = bones[i];
    Index idx = i + nr_trees;
    Triangulation::Vertex_handle v = t.nearest_vertex(point);
    long dist = 4 * CGAL::squared_distance(point, v->point());  // s = 4r**2
    edges.emplace_back(idx, v->info(), dist);
  }

  // sort all edges in ascending distance
  sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
    return std::get<2>(e1) < std::get<2>(e2);
  });

  long R = 0;
  long count = 0;
  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(nr_trees + nr_bones);
  vector<long> bones_in_tree(nr_trees);
  // ... and process edges in order of increasing length
  long prev_bones = 0;
  for (const auto& edge : edges) {
    // determine components of endpoints
    Index comp1 = uf.find_set(get<0>(edge));  // bone or tree
    Index comp2 = uf.find_set(get<1>(edge));  // only tree
    if (comp1 != comp2) {
      // this edge connects two different components => part of the emst
      uf.link(comp1, comp2);  // !!new indice is always c2

      // if c1 is a bone and c2 a tree
      if ((comp1 >= nr_trees) && (comp2 < nr_trees))
        bones_in_tree[comp2]++;
      else {
        long new_bones = bones_in_tree[comp1] + bones_in_tree[comp2];
        bones_in_tree[comp1] = new_bones;
        bones_in_tree[comp2] = new_bones;
      }
      long bones = max(prev_bones, bones_in_tree[comp2]);
      long radius = get<2>(edge);

      if (bones <= goal_bones && prev_bones < bones) R = radius;
      if (radius <= goal_radius) count = bones;
      if (bones >= goal_bones && radius >= goal_radius) break;
      prev_bones = bones;
    }
  }
  cout << count << " " << R << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}