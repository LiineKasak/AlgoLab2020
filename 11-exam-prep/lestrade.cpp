/*
Find if the cost for solving questions with agents and gang members,
given amount of information leaked per gang member, the sum information needed,
and positions of agents and gang members, is smaller than Sherlocks fee.
An agent may only question the closest gang member.

Implementation:
Step 1 is using proximity structures to find the closest gang members for
each agent.
Step 2 is LP where the variables are how long a gang member was interrogated.

Tips:
- it's faster to add an extra constraint for sherlocks fee, rather than
comparing if the optimal value is equal or less than the fee.
- only increment equation index if an agent was assigned for the gang member.
*/

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <iostream>
#include <vector>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> T;

using namespace std;

// where, who, how leaked in 1 hr
struct gang_member {
  int u;
  int v;
  int w;
};
istream& operator>>(istream& is, gang_member& gm) {
  is >> gm.u >> gm.v >> gm.w;
  return is;
}

void solve() {
  // z - sherlocks fee
  // (u, v, w) - how much info on where, who, how must be gathered
  int fee, u, v, w;
  cin >> fee >> u >> v >> w;
  // number of agents and gang members
  int nr_agents, nr_gang_members;
  cin >> nr_agents >> nr_gang_members;

  // STEP 1: calculate enarest gang members for each agent
  vector<gang_member> gms(nr_gang_members);
  vector<pair<K::Point_2, Index>> pts(nr_gang_members);
  for (int i = 0; i < nr_gang_members; i++) {
    int x, y;
    cin >> x >> y >> gms[i];
    pts[i] = {K::Point_2(x, y), i};
  }

  T t;
  t.insert(pts.begin(), pts.end());

  // cost to interrogate i-th gang member for 1 hr
  vector<int> cost(nr_gang_members, 0);
  for (int i = 0; i < nr_agents; i++) {
    int x, y, z;
    cin >> x >> y >> z;  // agent position
    T::Vertex_handle vh = t.nearest_vertex(K::Point_2(x, y));
    int nearest_gi = vh->info();
    cost[nearest_gi] = cost[nearest_gi] == 0 ? z : min(z, cost[nearest_gi]);
  }

  // STEP 2: linear program to determine lowest possible cost to get information
  Program lp(CGAL::LARGER, true, 0, true, 24);
  const int U = 0;  // sum of u_i >= u
  const int V = 1;  // sum of v_i >= v
  const int W = 2;  // sum of w_i >= w
  const int Z = 3;  // sum of cost cost <= z -> -sum >= -z
  int eq = 0;
  for (int i = 0; i < nr_gang_members; i++) {
    bool agent_assigned = cost[i] != 0;
    if (agent_assigned) {
      lp.set_a(eq, U, gms[i].u);
      lp.set_a(eq, V, gms[i].v);
      lp.set_a(eq, W, gms[i].w);

      lp.set_c(eq, cost[i]);
      lp.set_a(eq++, Z, -cost[i]);
      // only update equation index if agent was assigned
    }
  }
  lp.set_b(U, u);
  lp.set_b(V, v);
  lp.set_b(W, w);
  lp.set_b(Z, -fee);

  Solution s = CGAL::solve_linear_program(lp, ET());
  // cout << "HL"[s.is_optimal() && s.objective_value() <= z] << endl; 2.648s
  cout << "HL"[s.is_optimal()]
       << endl;  // with extra constraint for fee: 1.745s
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}