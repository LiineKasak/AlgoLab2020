/*
Calculate the maximum preparation time when surrounded by
lines of legions closing in with the same velocity.

For each legion add a row to the LP.
Find out on which side of a line we are and change the sign accordingly.
*/

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <cmath>
#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int double_to_floor(CGAL::Quotient<ET> x) { return floor(to_double(x)); }

void solve() {
  long x, y, nr_legions;
  cin >> x >> y >> nr_legions;

  Program lp(CGAL::SMALLER, false, 0, false, 0);
  const int X = 0, Y = 1, R = 2;

  for (int i = 0; i < nr_legions; i++) {
    long a, b, c, v;
    cin >> a >> b >> c >> v;

    long norm = sqrt(a * a + b * b);

    // check if should be >= instead
    int sign = (a * x + b * y + c) < 0 ? 1 : -1;
    lp.set_a(R, i, norm * v);  // ||legion|| * v * r
    lp.set_a(X, i, sign * a);  // + ax
    lp.set_a(Y, i, sign * b);  // + by
    lp.set_b(i, -sign * c);    // <= -c
  }

  lp.set_l(R, true, 0);  // radius can only be positive
  lp.set_c(R, -1);       // get maximum radius from lines

  Solution s = CGAL::solve_linear_program(lp, ET());

  // solution should always be feasible as asterix and panoramix are completely
  // surrounded
  cout << double_to_floor(-s.objective_value()) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}