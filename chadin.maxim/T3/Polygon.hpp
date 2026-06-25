#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <istream>

namespace chadin {

  struct Point
  {
    int x, y;
  };

  bool operator==(const Point& lhs, const Point& rhs);
  std::istream& operator>>(std::istream& in, Point& point);

  struct Polygon
  {
    std::vector<Point> points;
  };

  bool operator==(const Polygon& lhs, const Polygon& rhs);
  std::istream& operator>>(std::istream& in, Polygon& polygon);

}

#endif
