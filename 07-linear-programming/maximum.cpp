/*
Simply utilizing the CGAL QP library.
*/

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <iostream>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolutionValue;
typedef long long ll;

ll ceil(const SolutionValue& x) {
  double a = round(CGAL::to_double(x));
  while (a - 1 >= x) a -= 1;
  while (a < x) a += 1;
  return llround(a);
}

using namespace std;

void maximize() {
  int a, b;
  cin >> a >> b;

  // create an LP with Ax <= b, lower bound 0 and no upper bound
  Program lp(CGAL::SMALLER, true, 0, false, 0);

  // set the coefficients of A and b
  const int X = 0;
  const int Y = 1;

  // x + y <= -4
  lp.set_a(X, 0, 1);
  lp.set_a(Y, 0, 1);
  lp.set_b(0, 4);
  
  // 4x + 2y <= ab
  lp.set_a(X, 1, 4);
  lp.set_a(Y, 1, 2);
  lp.set_b(1, a * b);

  // -x + y <= 1
  lp.set_a(X, 2, -1);
  lp.set_a(Y, 2, 1);
  lp.set_b(2, 1);

  // objective function, max by - ax
  lp.set_c(X, a);
  lp.set_c(Y, -b);

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_optimal()) {
    cout << -ceil(s.objective_value()) << endl;
  } else if (s.is_infeasible()) {
    cout << "no" << endl;
  } else if (s.is_unbounded()) {
    cout << "unbounded" << endl;
  }
}

void minimize() {
  int a, b;
  cin >> a >> b;

  // create an LP with Ax >= b, no lower bound and upper bound 0
  Program lp(CGAL::LARGER, false, 0, true, 0);

  // set the coefficients of A and b
  const int X = 0;
  const int Y = 1;
  const int Z = 2;

  // x + y >= -4
  lp.set_a(X, 0, 1);
  lp.set_a(Y, 0, 1);
  lp.set_a(Z, 0, 0);
  lp.set_b(0, -4);

  // 4x + 2y + z >= -ab
  lp.set_a(X, 1, 4);
  lp.set_a(Y, 1, 2);
  lp.set_a(Z, 1, 1);
  lp.set_b(1, -a * b);

  // -x + y >= -1
  lp.set_a(X, 2, -1);
  lp.set_a(Y, 2, 1);
  lp.set_a(Z, 2, 0);
  lp.set_b(2, -1);

  // objective function: ax + by + z
  lp.set_c(X, a);
  lp.set_c(Y, b);
  lp.set_c(Z, 1);

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_optimal()) {
    cout << ceil(s.objective_value()) << endl;
  } else if (s.is_infeasible()) {
    cout << "no" << endl;
  } else if (s.is_unbounded()) {
    cout << "unbounded" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);

  while (true) {
    int p;
    cin >> p;
    if (p == 0) break;
    else if (p == 1) maximize();
    else minimize();
  }

  return 0;
}