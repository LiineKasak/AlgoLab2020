/*
Maximizing the radius of a ball inside a polyhedron.

For each constraint a_i^T*x <= b_i,
the point z_i where the ball touches the constraint is
z_i = y + r * (a_i / ||a_i||_2),
where y is the center of the ball.

So for a set of constraints we can generalize
a_i^T * z_i <= b_i  ->
a_i^T * (y + r * a_i/||a_i||_2) <= b_i ->
a_i^T * y + ||a_i||r <= b_i
*/

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <cmath>
#include <iostream>

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void solve(int n) {
  int dim;
  cin >> dim;

  // create an LP with Ax <= b, no lower bound and no upper bound
  Program lp(CGAL::SMALLER, false, 0, false, 0);
  const int R = dim; // we have dimension many variables

  for (int i = 0; i < n; i++) {
    int square_sum = 0;
    for (int d = 0; d < dim; d++) {
      int c;
      cin >> c;
      square_sum += c * c;
      lp.set_a(d, i, c);  // .. + c * a_id + ..
    }
    int norm = sqrt(square_sum);
    lp.set_a(R, i, norm);  // + ||a_i|| * r
    int b;
    cin >> b;
    lp.set_b(i, b);  // <= b
  }
  lp.set_l(R, true, 0);  // radius can only be positive

  lp.set_c(R, -1);  // we minimize the max radius - change sign

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible())
    cout << "none" << endl;
  else if (s.is_unbounded())
    cout << "inf" << endl;
  // since we changed the sign of objective function
  else
    cout << floor(to_double((-s.objective_value()))) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n;
    cin >> n;
    if (n != 0)
      solve(n);
    else
      break;
  }
  return 0;
}