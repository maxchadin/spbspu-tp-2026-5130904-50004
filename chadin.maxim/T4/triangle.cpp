#include "triangle.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace chadin {
  Triangle::Triangle(point_t a, point_t b, point_t c):
    a_(a),
    b_(b),
    c_(c)
  {
    if (getArea() <= 0.0) {
      throw std::invalid_argument("Invalid triangle vertices");
    }
  }

  double Triangle::getArea() const
  {
    return 0.5 * std::abs(a_.x * (b_.y - c_.y) + b_.x * (c_.y - a_.y) + c_.x * (a_.y - b_.y));
  }

  rectangle_t Triangle::getFrameRect() const
  {
    const double min_x = std::min({a_.x, b_.x, c_.x});
    const double max_x = std::max({a_.x, b_.x, c_.x});
    const double min_y = std::min({a_.y, b_.y, c_.y});
    const double max_y = std::max({a_.y, b_.y, c_.y});
    return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
  }

  point_t Triangle::getCenter() const
  {
    return {(a_.x + b_.x + c_.x) / 3.0, (a_.y + b_.y + c_.y) / 3.0};
  }

  void Triangle::move(point_t new_pos)
  {
    const point_t center = getCenter();
    move(new_pos.x - center.x, new_pos.y - center.y);
  }

  void Triangle::move(double dx, double dy)
  {
    a_.x += dx;
    a_.y += dy;
    b_.x += dx;
    b_.y += dy;
    c_.x += dx;
    c_.y += dy;
  }

  void Triangle::scale(double coefficient)
  {
    if (coefficient <= 0.0) {
      throw std::invalid_argument("Invalid scale coefficient");
    }
    const point_t center = getCenter();
    a_.x = center.x + (a_.x - center.x) * coefficient;
    a_.y = center.y + (a_.y - center.y) * coefficient;
    b_.x = center.x + (b_.x - center.x) * coefficient;
    b_.y = center.y + (b_.y - center.y) * coefficient;
    c_.x = center.x + (c_.x - center.x) * coefficient;
    c_.y = center.y + (c_.y - center.y) * coefficient;
  }
}
