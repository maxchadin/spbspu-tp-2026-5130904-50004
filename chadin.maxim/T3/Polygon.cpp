#include "Polygon.hpp"
#include <algorithm>
#include <iterator>

namespace chadin {
  namespace detail {

    struct read_point
    {
      std::istream& in;
      Point operator()() const
      {
        Point p;
        in >> p;
        return p;
      }
    };

  }

  bool operator==(const Point& lhs, const Point& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c1 = '\0';
    char c2 = '\0';
    char c3 = '\0';
    int x = 0;
    int y = 0;
    if (in >> c1 >> x >> c2 >> y >> c3) {
      if (c1 == '(' && c2 == ';' && c3 == ')') {
        point.x = x;
        point.y = y;
      } else {
        in.setstate(std::ios::failbit);
      }
    }
    return in;
  }

  bool operator==(const Polygon& lhs, const Polygon& rhs)
  {
    if (lhs.points.size() != rhs.points.size()) {
      return false;
    }
    return std::equal(lhs.points.begin(), lhs.points.end(), rhs.points.begin());
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    size_t count = 0;
    if (!(in >> count)) {
      return in;
    }
    if (count < 3) {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector<Point> temp;
    std::generate_n(std::back_inserter(temp), count, detail::read_point{in});
    if (in && temp.size() == count) {
      polygon.points = std::move(temp);
    } else {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

}
