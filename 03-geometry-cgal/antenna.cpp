#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

long double_to_sqrt_ceil(const K::FT &val) {
  long a = std::ceil(sqrt(CGAL::to_double(val)));
   while (K::FT(a)*K::FT(a) < val) a += 1;
   while (K::FT(a-1)*K::FT(a-1) >= val) a -= 1;
   return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int nr_points; std::cin >> nr_points;
  
  while (nr_points) {
    std::vector<K::Point_2> points(nr_points);
    for (int i = 0; i < nr_points; i++) {
      long x, y; std::cin >> x >> y;
      points[i] = K::Point_2(x, y);
    }
    
    Min_circle min_circle(points.begin(), points.end(), true);
    Traits::Circle circle = min_circle.circle();
    std::cout << double_to_sqrt_ceil(circle.squared_radius()) << std::endl;
 
    std::cin >> nr_points;
  }
}