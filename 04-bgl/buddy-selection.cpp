/*
Given a set of characteristics for each student,
construct a maximum matching with at least nr_min_common
common characteristics.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <iostream>
#include <map>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

using namespace std;

void solve() {
  int nr_students, nr_char, nr_min_common;
  cin >> nr_students >> nr_char >> nr_min_common;
  map<string, vector<int>> chars;
  string temp_char;

  for (int i_student = 0; i_student < nr_students; i_student++) {
    for (int i_char = 0; i_char < nr_char; i_char++) {
      cin >> temp_char;
      auto it = chars.find(temp_char);
      if (it == chars.end()) chars.insert(make_pair(temp_char, vector<int>()));
      chars[temp_char].push_back(i_student);
    }
  }

  // calculate nr of common characteristics between all students
  vector<vector<int>> common(nr_students, vector<int>(nr_students));
  for (auto const& [charac, v_students] : chars) {
    for (int i_s1 = 0; i_s1 < (int)v_students.size(); i_s1++) {
      for (int i_s2 = i_s1 + 1; i_s2 < (int)v_students.size(); i_s2++) {
        common[v_students[i_s1]][v_students[i_s2]]++;
      }
    }
  }

  // construct a graph
  graph G(nr_students);
  for (int i_s1 = 0; i_s1 < nr_students; i_s1++) {
    for (int i_s2 = 0; i_s2 < nr_students; i_s2++) {
      if (common[i_s1][i_s2] > nr_min_common) boost::add_edge(i_s1, i_s2, G);
    }
  }

  std::vector<vertex_desc> mate_map(nr_students);  // exterior property map

  // maximum matching size has to be half of students number
  boost::edmonds_maximum_cardinality_matching(
      G, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(
      G, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, G)));

  if (matching_size == nr_students / 2)
    cout << "not optimal" << endl;
  else
    cout << "optimal" << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) solve();
  return 0;
}