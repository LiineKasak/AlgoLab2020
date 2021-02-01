/*
This problem has 3 subproblems:

1) is linear separation feasible

2) is the sum of distances from point to line under a constant
The ditance to the line ax + by + c = 0 given a point (x0, y0) is
|ax0 + by0 + c|
We also know which points are under and over the line.
Points over the line will have a negative value.
sum(axi + byi + c) forall i over + sum(-axi - byi - c) forall i under =>
a*sum(s*xi) + b*sum(s*yi) + s*c forall i, where s = -1 if i over line else 1 

3) Minimizing the maximal distance to the perpendicular line.
The perpendicular line is:
-bx + ay + c2 = 0
Since we do not know which side of the perpendicular line the point is
we have two equations for point (x0, y0):
(-bx0 + ay0 + c2) - D <= 0
(-bx0 + ay0 + c2) + D >= 0
We minimize the value of D.
*/

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <iostream>
#include <vector>

typedef long IT; // input type must fit! int -> long
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

struct point {
  int x;
  int y;
};

vector<point> read_points(int size) {
  vector<point> points(size);
  for (int i = 0; i < size; i++) {
    int x, y;
    cin >> x >> y;
    points[i] = {x, y};
  }
  return points;
}

string solve() {
  int n, m;
  long s;
  cin >> n >> m >> s;

  vector<point> noble = read_points(n);
  vector<point> common = read_points(m);


  // STEP 1: check if can do linear separation

  Program lp(CGAL::SMALLER, false, 0, false, 0);
  const int a = 0, b = 1, c = 2;
  // SEWER LINE ax + by + c = 0
  // noble houses on left side of line
  int i = 0;
  for (auto &p : noble) {
    lp.set_a(a, i, p.x);
    lp.set_a(b, i, p.y);
    lp.set_a(c, i++, 1);
  }
  // common houses on right side of line
  for (auto &p : common) {
    lp.set_a(a, i, -p.x);
    lp.set_a(b, i, -p.y);
    lp.set_a(c, i++, -1);
  }
  lp.set_l(a, true, 1);  // sewer line is non-horizontal

  Solution sol = CGAL::solve_linear_program(lp, ET());
  if (sol.is_infeasible()) return "Yuck!";


  // STEP 2: is sum of pipe lengths under required constraint
  
  if (s != -1) {
    long a_sum = 0, b_sum = 0;
    for (point &p : noble) {
      // left side so abs value is negative - subtract
      a_sum -= p.x;
      b_sum -= p.y;
    }
    for (point &p : common) {
      // right side so abs value is positive
      a_sum += p.x;
      b_sum += p.y;
    }
    int c_sum = m - n;
    lp.set_a(a, i, a_sum);
    lp.set_a(b, i, b_sum);
    lp.set_a(c, i, c_sum);
    lp.set_b(i++, s);

    sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible()) return "Bankrupt!";
  }


  // STEP 3: minimize max length to canal
  
  const int c2 = 3;  // perpendicular line free variable
  const int D = 4;   // max dist

  // CANAL LINE perpendicular -bx + ay + c2 = 0
  for (int p_i = 0; p_i < n + m; p_i++) {
    // we want |p_i dist| - max_dist <= 0
    point &p = p_i < n ? noble[p_i] : common[p_i - n];

    // absolute value positive: dist - maxdist <= 0
    lp.set_a(a, i, p.y);
    lp.set_a(b, i, -p.x);
    lp.set_a(c2, i, 1);
    lp.set_a(D, i++, -1);

    // absolute value negative: dist + maxdist >= 0
    lp.set_a(a, i, p.y);
    lp.set_a(b, i, -p.x);
    lp.set_a(c2, i, 1);
    lp.set_a(D, i, 1);
    lp.set_r(i++, CGAL::LARGER);
  }
  lp.set_l(D, true, 0);
  lp.set_c(D, 1);
  sol = CGAL::solve_linear_program(lp, ET());
  return to_string((long)ceil(CGAL::to_double(sol.objective_value())));
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) cout << solve() << endl;
  return 0;
}