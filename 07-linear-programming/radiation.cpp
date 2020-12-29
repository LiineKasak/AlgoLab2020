/*
Find the minimal degree polynomial to separate a set of points in 3D.

Remarks:
- no measured cell on the boundary p = 0
    -> our cells lie on the negative side of -1 or positive side of 1
- our combination of coefficients might be a big number -> double (long smaller)
*/

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <iostream>
#include <vector>

typedef double IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

struct coord {
  int x;
  int y;
  int z;
};

vector<coord> read_coords(int nr_coords) {
  vector<coord> coords(nr_coords);
  for (int i = 0; i < nr_coords; i++) {
    int x, y, z;
    cin >> x >> y >> z;
    coords[i] = {x, y, z};
  }
  return coords;
}

vector<IT> get_terms(coord p, int degree) {
  // all terms are combinations of x^x_pow * y^y_pow * z^z_pow
  // such that x_pow + y_pow + z_pow <= degree
  // this product might be large so we need to use IT = double
  vector<IT> terms;
  for (int x_pow = 0; x_pow <= degree; x_pow++)
    for (int y_pow = 0; y_pow <= degree - x_pow; y_pow++)
      for (int z_pow = 0; z_pow <= degree - x_pow - y_pow; z_pow++)
        terms.push_back(pow(p.x, x_pow) * pow(p.y, y_pow) * pow(p.z, z_pow));
  return terms;
}

bool try_separation(int degree, vector<coord> &A, vector<coord> &B) {
  // try to separate points with a polynomial of some degree

  const int nr_A = A.size(), nr_B = B.size();
  Program lp(CGAL::SMALLER, false, 0, false, 0);

  // c1 * term1 + c2 * term2 + .. <= -1
  for (int i_eq = 0; i_eq < nr_A; i_eq++) {
    coord point = A[i_eq];
    vector<IT> terms = get_terms(point, degree);
    for (int i_term = 0; i_term < (int)terms.size(); i_term++)
      lp.set_a(i_term, i_eq, terms[i_term]);
    lp.set_b(i_eq, -1);
  }

  // c1 * term1 + c2 * term2 + .. >= 1
  for (int i = 0; i < nr_B; i++) {
    int i_eq = i + nr_A;
    coord point = B[i];
    vector<IT> terms = get_terms(point, degree);
    for (int i_term = 0; i_term < (int)terms.size(); i_term++)
      lp.set_a(i_term, i_eq, -terms[i_term]);
    lp.set_b(i_eq, -1);
  }

  // to avoid cycling.. happened in some test cases
  // NB! is slower, only use if necessary
  CGAL::Quadratic_program_options options;
  options.set_pricing_strategy(CGAL::QP_BLAND);

  Solution s = CGAL::solve_linear_program(lp, ET(), options);
  return !s.is_infeasible();
}

void solve() {
  int nr_healthy, nr_tumor;
  cin >> nr_healthy >> nr_tumor;

  vector<coord> healthy = read_coords(nr_healthy);
  vector<coord> tumors = read_coords(nr_tumor);

  // case when we only have one set of points - they're already separated
  if (nr_healthy == 0 || nr_tumor == 0) {
    cout << 0 << endl;
    return;
  }

  for (int degree = 1; degree <= 30; degree++) {
    if (try_separation(degree, healthy, tumors)) {
      cout << degree << endl;
      return;
    }
  }
  cout << "Impossible!" << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}