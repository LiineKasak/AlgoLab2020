/*
Maximize revenue for selling beer.
Extra twist of proximity with contour lines.

- no need to use K::Circle_2, we just compare squared distances
- some contour lines do not even contain poins, check for proximity
*/

#include <cmath>
#include <iostream>
#include <vector>

// LP includes and typedefs
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// CGAL includes and typedefs
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

using namespace std;

struct warehouse {
  K::Point_2 point;
  int supply;
  int alc_percentage;
};
struct stadium {
  K::Point_2 point;
  int demand;
  int alc_limit;
};
struct contour_line {
  K::Point_2 center;
  K::FT squared_radius;
};

istream& operator>>(istream& is, warehouse& wh) {
  is >> wh.point >> wh.supply >> wh.alc_percentage;
  return is;
}
istream& operator>>(istream& is, stadium& s) {
  is >> s.point >> s.demand >> s.alc_limit;
  return is;
}
istream& operator>>(istream& is, contour_line& cl) {
  is >> cl.center >> cl.squared_radius;
  cl.squared_radius *= cl.squared_radius;
  return is;
}

int get_idx(int wh_i, int s_i, int s_n) {
  // parameter index for warehouse and stadium
  return wh_i * s_n + s_i;
}

void adjust_revenues(vector<vector<int>>& revenues,
                     vector<warehouse>& warehouses, vector<stadium>& stadiums,
                     vector<contour_line>& contours) {
  int wh_n = warehouses.size(), s_n = stadiums.size(), cl_n = contours.size();
  vector<vector<bool>> wh_in_circ(wh_n, vector<bool>(cl_n));
  vector<vector<bool>> s_in_circ(s_n, vector<bool>(cl_n));
  for (int i = 0; i < wh_n; i++)
    for (int ci = 0; ci < cl_n; ci++)
      wh_in_circ[i][ci] = CGAL::squared_distance(warehouses[i].point, contours[ci].center) < contours[ci].squared_radius;
  for (int i = 0; i < s_n; i++)
    for (int ci = 0; ci < cl_n; ci++)
      s_in_circ[i][ci] = CGAL::squared_distance(stadiums[i].point, contours[ci].center) < contours[ci].squared_radius;
  
  for (int wh_i = 0; wh_i < wh_n; wh_i++)
    for (int s_i = 0; s_i < s_n; s_i++)
      for (int ci = 0; ci < cl_n; ci++)
        revenues[wh_i][s_i] -= s_in_circ[s_i][ci] != wh_in_circ[wh_i][ci];
}

void solve() {
  int wh_n, s_n, cl_n;
  cin >> wh_n >> s_n >> cl_n;

  vector<warehouse> warehouses(wh_n);
  for (int i = 0; i < wh_n; i++) cin >> warehouses[i];
  vector<stadium> stadiums(s_n);
  for (int i = 0; i < s_n; i++) cin >> stadiums[i];
  
  // STEP 1: make triangulation to check proximity of contours
  vector<K::Point_2> points(wh_n + s_n);
  int i_point = 0;
  for (auto wh : warehouses) points[i_point++] = wh.point;
  for (auto s : stadiums) points[i_point++] = s.point;
  Triangulation t;
  t.insert(points.begin(), points.end());

  // i-th row is revenue from i-th wh to every s
  vector<vector<int>> revenues(wh_n, vector<int>(s_n));
  for (int wh_i = 0; wh_i < wh_n; wh_i++) {
    for (int s_i = 0; s_i < s_n; s_i++) {
      int revenue;
      cin >> revenue;
      // since we start to take off contour lines which is 0.01 -> 1
      revenues[wh_i][s_i] = revenue * 100;
    }
  }

  vector<contour_line> contours;
  contours.reserve(cl_n);
  for (int i = 0; i < cl_n; i++) {
    contour_line cl;
    cin >> cl;
    // only add to vector if has a point in its radius
    K::Point_2 nearest = t.nearest_vertex(cl.center)->point();
    if (CGAL::squared_distance(nearest, cl.center) < cl.squared_radius)
      contours.emplace_back(cl);
  }

  // STEP 2: subtract 1 revenue per crossed contour line
  adjust_revenues(revenues, warehouses, stadiums, contours);

  // STEP 3: construct LP
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  // (alcohol amounts are only positive)
  Program lp(CGAL::SMALLER, true, 0, false, 0);

  // sum of warehouse outgoing edges less than supply
  int eq = 0;
  for (int wh_i = 0; wh_i < wh_n; wh_i++) {
    for (int s_i = 0; s_i < s_n; s_i++) {
      int i = get_idx(wh_i, s_i, s_n);
      lp.set_a(i, eq, 1);
    }
    lp.set_b(eq, warehouses[wh_i].supply);

    eq++;
  }

  // sum of stadium incoming edges is EXACTLY demand
  for (int s_i = 0; s_i < s_n; s_i++) {
    for (int wh_i = 0; wh_i < wh_n; wh_i++) {
      int i = get_idx(wh_i, s_i, s_n);
      lp.set_a(i, eq, 1);
    }
    lp.set_r(eq, CGAL::EQUAL);  // NB!! REMEMBER THIS
    lp.set_b(eq, stadiums[s_i].demand);

    eq++;
  }

  // all stadium incoming alcohol less than limit
  for (int s_i = 0; s_i < s_n; s_i++) {
    for (int wh_i = 0; wh_i < wh_n; wh_i++) {
      int i = get_idx(wh_i, s_i, s_n);
      int alc = warehouses[wh_i].alc_percentage;
      lp.set_a(i, eq, alc);
    }
    lp.set_b(eq, stadiums[s_i].alc_limit * 100);

    eq++;
  }

  // set objective function
  // we are minimizing but purpose is to max so reverse sign
  for (int wh_i = 0; wh_i < wh_n; wh_i++) {
    for (int s_i = 0; s_i < s_n; s_i++) {
      int i = get_idx(wh_i, s_i, s_n);
      int revenue = revenues[wh_i][s_i];
      if (revenue != 0) {
        lp.set_c(i, -revenue);
      }
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal())
    cout << int(floor(-CGAL::to_double(s.objective_value()) / 100)) << endl;
  else
    cout << "RIOT!" << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}