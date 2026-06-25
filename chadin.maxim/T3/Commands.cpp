#include "Commands.hpp"
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>

namespace chadin {
  namespace detail {

    struct multiply_cross
    {
      double operator()(const Point& a, const Point& b) const
      {
        return static_cast<double>(a.x) * b.y - static_cast<double>(a.y) * b.x;
      }
    };

    struct compare_x
    {
      bool operator()(const Point& a, const Point& b) const { return a.x < b.x; }
    };

    struct compare_y
    {
      bool operator()(const Point& a, const Point& b) const { return a.y < b.y; }
    };

    struct compare_points
    {
      bool operator()(const Point& a, const Point& b) const
      {
        if (a.x != b.x) {
          return a.x < b.x;
        }
        return a.y < b.y;
      }
    };

    double getArea(const Polygon& p)
    {
      if (p.points.size() < 3) {
        return 0.0;
      }
      std::vector<double> products(p.points.size());
      std::vector<Point> shifted(p.points.size());
      std::copy(p.points.begin() + 1, p.points.end(), shifted.begin());
      shifted.back() = p.points.front();
      std::transform(p.points.begin(), p.points.end(), shifted.begin(), products.begin(), multiply_cross());
      double sum = std::accumulate(products.begin(), products.end(), 0.0);
      return std::abs(sum) / 2.0;
    }

    size_t getVertexes(const Polygon& p)
    {
      return p.points.size();
    }

    int getMinX(const Polygon& p)
    {
      return std::min_element(p.points.begin(), p.points.end(), compare_x())->x;
    }

    int getMaxX(const Polygon& p)
    {
      return std::max_element(p.points.begin(), p.points.end(), compare_x())->x;
    }

    int getMinY(const Polygon& p)
    {
      return std::min_element(p.points.begin(), p.points.end(), compare_y())->y;
    }

    int getMaxY(const Polygon& p)
    {
      return std::max_element(p.points.begin(), p.points.end(), compare_y())->y;
    }

    bool has_extra_garbage(std::istringstream& iss)
    {
      std::string extra;
      return (iss >> extra) ? true : false;
    }

    struct check_intersect
    {
      int minX, maxX, minY, maxY;
      bool operator()(const Polygon& p) const
      {
        int pMinX = getMinX(p);
        int pMaxX = getMaxX(p);
        int pMinY = getMinY(p);
        int pMaxY = getMaxY(p);
        return (pMaxX >= minX && pMinX <= maxX && pMaxY >= minY && pMinY <= maxY);
      }
    };

    struct check_same
    {
      const Polygon& target;
      check_same(const Polygon& t) : target(t) {}
      bool operator()(const Polygon& p) const
      {
        if (p.points.size() != target.points.size()) {
          return false;
        }
        if (p.points.empty()) {
          return true;
        }
        int cur_dx = p.points[0].x - target.points[0].x;
        int cur_dy = p.points[0].y - target.points[0].y;
        struct point_cmp
        {
          int dx, dy;
          bool operator()(const Point& a, const Point& b) const
          {
            return a.x - b.x == dx && a.y - b.y == dy;
          }
        } cmp{cur_dx, cur_dy};
        return std::equal(p.points.begin(), p.points.end(), target.points.begin(), cmp);
      }
    };

    struct maxseq_counter
    {
      const Polygon& target;
      mutable int current_max;
      mutable int current_len;
      bool operator()(const Polygon& p) const
      {
        if (p == target) {
          current_len++;
          if (current_len > current_max) {
            current_max = current_len;
          }
        } else {
          current_len = 0;
        }
        return false;
      }
    };

    struct check_perms
    {
      Polygon target_sorted;
      check_perms(Polygon t) : target_sorted(std::move(t))
      {
        std::sort(target_sorted.points.begin(), target_sorted.points.end(), compare_points());
      }
      bool operator()(Polygon p) const
      {
        if (p.points.size() != target_sorted.points.size()) {
          return false;
        }
        std::sort(p.points.begin(), p.points.end(), compare_points());
        return p == target_sorted;
      }
    };

    struct echo_inserter
    {
      std::vector<Polygon>& dest;
      const Polygon& target;
      mutable int added;
      bool operator()(const Polygon& p) const
      {
        dest.push_back(p);
        if (p == target) {
          dest.push_back(p);
          added++;
        }
        return false;
      }
    };

  }

  void processCommands(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
  {
    using namespace std::placeholders;
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) {
        continue;
      }
      std::istringstream iss(line);
      std::string cmd;
      if (!(iss >> cmd)) {
        continue;
      }

      if (cmd == "AREA") {
        std::string sub;
        if (!(iss >> sub) || detail::has_extra_garbage(iss)) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        if (sub == "EVEN") {
          std::vector<Polygon> filtered;
          auto is_even = std::bind(std::equal_to<size_t>(),
                                   std::bind(std::modulus<size_t>(), std::bind(detail::getVertexes, _1), 2), 0);
          std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), is_even);
          std::vector<double> areas(filtered.size());
          std::transform(filtered.begin(), filtered.end(), areas.begin(), detail::getArea);
          double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
          out << std::fixed << std::setprecision(1) << sum << '\n';
        } else if (sub == "ODD") {
          std::vector<Polygon> filtered;
          auto is_odd = std::bind(std::not_equal_to<size_t>(),
                                  std::bind(std::modulus<size_t>(), std::bind(detail::getVertexes, _1), 2), 0);
          std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), is_odd);
          std::vector<double> areas(filtered.size());
          std::transform(filtered.begin(), filtered.end(), areas.begin(), detail::getArea);
          double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
          out << std::fixed << std::setprecision(1) << sum << '\n';
        } else if (sub == "MEAN") {
          if (polygons.empty()) {
            out << "<INVALID COMMAND>\n";
          } else {
            std::vector<double> areas(polygons.size());
            std::transform(polygons.begin(), polygons.end(), areas.begin(), detail::getArea);
            double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
            out << std::fixed << std::setprecision(1) << sum / polygons.size() << '\n';
          }
        } else {
          try {
            const int n = std::stoi(sub);
            if (n < 3) {
              throw std::invalid_argument("n < 3");
            }
            std::vector<Polygon> filtered;
            auto has_n = std::bind(std::equal_to<size_t>(), std::bind(detail::getVertexes, _1), static_cast<size_t>(n));
            std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), has_n);
            std::vector<double> areas(filtered.size());
            std::transform(filtered.begin(), filtered.end(), areas.begin(), detail::getArea);
            double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
            out << std::fixed << std::setprecision(1) << sum << '\n';
          } catch (...) {
            out << "<INVALID COMMAND>\n";
          }
        }
      } else if (cmd == "MAX") {
        std::string sub;
        if (!(iss >> sub) || detail::has_extra_garbage(iss)) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        if (polygons.empty()) {
          out << "<INVALID COMMAND>\n";
        } else if (sub == "AREA") {
          auto cmp = std::bind(std::less<double>(), std::bind(detail::getArea, _1), std::bind(detail::getArea, _2));
          auto it = std::max_element(polygons.begin(), polygons.end(), cmp);
          out << std::fixed << std::setprecision(1) << detail::getArea(*it) << '\n';
        } else if (sub == "VERTEXES") {
          auto cmp = std::bind(std::less<size_t>(), std::bind(detail::getVertexes, _1), std::bind(detail::getVertexes, _2));
          auto it = std::max_element(polygons.begin(), polygons.end(), cmp);
          out << detail::getVertexes(*it) << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "MIN") {
        std::string sub;
        if (!(iss >> sub) || detail::has_extra_garbage(iss)) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        if (polygons.empty()) {
          out << "<INVALID COMMAND>\n";
        } else if (sub == "AREA") {
          auto cmp = std::bind(std::less<double>(), std::bind(detail::getArea, _1), std::bind(detail::getArea, _2));
          auto it = std::min_element(polygons.begin(), polygons.end(), cmp);
          out << std::fixed << std::setprecision(1) << detail::getArea(*it) << '\n';
        } else if (sub == "VERTEXES") {
          auto cmp = std::bind(std::less<size_t>(), std::bind(detail::getVertexes, _1), std::bind(detail::getVertexes, _2));
          auto it = std::min_element(polygons.begin(), polygons.end(), cmp);
          out << detail::getVertexes(*it) << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "COUNT") {
        std::string sub;
        if (!(iss >> sub) || detail::has_extra_garbage(iss)) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        if (sub == "EVEN") {
          auto is_even = std::bind(std::equal_to<size_t>(),
                                   std::bind(std::modulus<size_t>(), std::bind(detail::getVertexes, _1), 2), 0);
          out << std::count_if(polygons.begin(), polygons.end(), is_even) << '\n';
        } else if (sub == "ODD") {
          auto is_odd = std::bind(std::not_equal_to<size_t>(),
                                  std::bind(std::modulus<size_t>(), std::bind(detail::getVertexes, _1), 2), 0);
          out << std::count_if(polygons.begin(), polygons.end(), is_odd) << '\n';
        } else {
          try {
            const int n = std::stoi(sub);
            if (n < 3) {
              throw std::invalid_argument("n < 3");
            }
            auto has_n = std::bind(std::equal_to<size_t>(), std::bind(detail::getVertexes, _1), static_cast<size_t>(n));
            out << std::count_if(polygons.begin(), polygons.end(), has_n) << '\n';
          } catch (...) {
            out << "<INVALID COMMAND>\n";
          }
        }
      } else if (cmd == "RMECHO") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          auto is_echo = std::bind(std::logical_and<bool>(),
                                   std::bind(std::equal_to<Polygon>(), _1, _2),
                                   std::bind(std::equal_to<Polygon>(), _1, target));
          const size_t initial_size = polygons.size();
          auto it = std::unique(polygons.begin(), polygons.end(), is_echo);
          polygons.erase(it, polygons.end());
          out << initial_size - polygons.size() << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "ECHO") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          std::vector<Polygon> new_poly;
          int count = std::count(polygons.begin(), polygons.end(), target);
          new_poly.reserve(polygons.size() + count);
          detail::echo_inserter inserter{new_poly, target, 0};
          static_cast<void>(std::any_of(polygons.begin(), polygons.end(), std::ref(inserter)));
          polygons = std::move(new_poly);
          out << inserter.added << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "LESSAREA") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          double targetArea = detail::getArea(target);
          auto cmp = std::bind(std::less<double>(), std::bind(detail::getArea, _1), targetArea);
          out << std::count_if(polygons.begin(), polygons.end(), cmp) << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "SAME") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          out << std::count_if(polygons.begin(), polygons.end(), detail::check_same(target)) << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "INTERSECTIONS") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          detail::check_intersect cmp{detail::getMinX(target), detail::getMaxX(target),
                                      detail::getMinY(target), detail::getMaxY(target)};
          out << std::count_if(polygons.begin(), polygons.end(), cmp) << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "MAXSEQ") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          detail::maxseq_counter cnt{target, 0, 0};
          static_cast<void>(std::any_of(polygons.begin(), polygons.end(), std::ref(cnt)));
          out << cnt.current_max << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "PERMS") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          out << std::count_if(polygons.begin(), polygons.end(), detail::check_perms(target)) << '\n';
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else if (cmd == "INFRAME") {
        Polygon target;
        if (iss >> target && !detail::has_extra_garbage(iss)) {
          if (polygons.empty()) {
            out << "<FALSE>\n";
          } else {
            std::vector<int> minXs(polygons.size());
            std::vector<int> maxXs(polygons.size());
            std::vector<int> minYs(polygons.size());
            std::vector<int> maxYs(polygons.size());

            std::transform(polygons.begin(), polygons.end(), minXs.begin(), detail::getMinX);
            std::transform(polygons.begin(), polygons.end(), maxXs.begin(), detail::getMaxX);
            std::transform(polygons.begin(), polygons.end(), minYs.begin(), detail::getMinY);
            std::transform(polygons.begin(), polygons.end(), maxYs.begin(), detail::getMaxY);

            const int globalMinX = *std::min_element(minXs.begin(), minXs.end());
            const int globalMaxX = *std::max_element(maxXs.begin(), maxXs.end());
            const int globalMinY = *std::min_element(minYs.begin(), minYs.end());
            const int globalMaxY = *std::max_element(maxYs.begin(), maxYs.end());

            const int targetMinX = detail::getMinX(target);
            const int targetMaxX = detail::getMaxX(target);
            const int targetMinY = detail::getMinY(target);
            const int targetMaxY = detail::getMaxY(target);

            if (targetMinX >= globalMinX && targetMaxX <= globalMaxX &&
                targetMinY >= globalMinY && targetMaxY <= globalMaxY) {
              out << "<TRUE>\n";
            } else {
              out << "<FALSE>\n";
            }
          }
        } else {
          out << "<INVALID COMMAND>\n";
        }
      } else {
        out << "<INVALID COMMAND>\n";
      }
    }
  }

}
