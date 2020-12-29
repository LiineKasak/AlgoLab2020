/*
Find the cheapest cost of a diet or if it is even possible,
given a set of foods with certain nutriets, and
the min and max daily amount of a nutrient.

Apply LP to the constraints and maximize the negative sum of cost.
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

void solve(int nutrients_nr, int foods_nr) {
  // row 0 consists of the amount of nutrient 0 in foods 0..m-1;
  vector<int> prices(foods_nr);
  vector<vector<int>> nutrients(nutrients_nr, vector<int>(foods_nr));
  vector<pair<int, int>> nutrient_bounds(nutrients_nr);
  for (int i = 0; i < nutrients_nr; i++) {
    int lower, upper;
    cin >> lower >> upper;
    nutrient_bounds[i] = {lower, upper};
  }

  for (int i = 0; i < foods_nr; i++) {
    cin >> prices[i];
    for (int j = 0; j < nutrients_nr; j++) cin >> nutrients[j][i];
  }

  // create an LP with Ax <= b, lower bound and no upper bound
  Program lp(CGAL::SMALLER, true, 0, false, 0);

  // constraints of nurtient amounts
  for (int nut = 0; nut < nutrients_nr; nut++) {
    // lower bound
    int eq = 2 * nut;
    lp.set_b(eq, -nutrient_bounds[nut].first);
    for (int food = 0; food < foods_nr; food++) {
      lp.set_a(food, eq, -nutrients[nut][food]);
    }

    // upper bound
    eq++;
    lp.set_b(eq, nutrient_bounds[nut].second);
    for (int food = 0; food < foods_nr; food++) {
      lp.set_a(food, eq, nutrients[nut][food]);
    }
  }

  // objective function - sum of prices * food_amount
  for (int food = 0; food < foods_nr; food++) {
    lp.set_c(food, prices[food]);
  }

  // solve the program, using ET as the exact type
  // NB! we can use nonnegative! it's a bit faster
  Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());

  if (s.is_optimal())
    cout << double_to_floor(s.objective_value()) << endl;
  else if (s.is_infeasible())
    cout << "No such diet." << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n, m;
    cin >> n >> m;
    if (n == 0 && m == 0) break;
    else solve(n, m);
  }
  return 0;
}