#include "complexquad.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

static bool getIntersection(const chadin::point_t& p1, const chadin::point_t& p2,
    const chadin::point_t& p3, const chadin::point_t& p4, chadin::point_t& out)
{
  const double den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
  if (std::abs(den) < 1e-9) {
    return false;
  }
  const double t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / den;
  const double u = ((p1.x - p3.x) * (p1.y - p2.y) - (p1.y - p3.y) * (p1.x - p2.x)) / den;
  if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
    out.x = p1.x + t * (p2.x - p1.x);
    out.y = p1.y + t * (p2.y - p1.y);
    return true;
  }
  return false;
}

namespace chadin {
  Complexquad::Complexquad(point_t a, point_t b, point_t c, point_t d):
    a_(a),
    b_(b),
    c_(c),
    d_(d)
  {
    point_t center;
    if (!getIntersection(a_, b_, c_, d_, center) && !getIntersection(b_, c_, d_, a_, center)) {
      throw std::invalid_argument("Vertices do not form a self-intersecting quadrilateral");
    }
  }

  point_t Complexquad::getCenter() const
  {
    point_t center;
    if (getIntersection(a_, b_, c_, d_, center)) {
      return center;
    }
    if (getIntersection(b_, c_, d_, a_, center)) {
      return center;
    }
    return {0.0, 0.0};
  }

  double Complexquad::getArea() const
  {
    const point_t p = getCenter();
    double area1 = 0.0;
    double area2 = 0.0;
    point_t temp;
    if (getIntersection(a_, b_, c_, d_, temp)) {
      area1 = 0.5 * std::abs(a_.x * (p.y - d_.y) + p.x * (d_.y - a_.y) + d_.x * (a_.y - p.y));
      area2 = 0.5 * std::abs(b_.x * (c_.y - p.y) + c_.x * (p.y - b_.y) + p.x * (b_.y - c_.y));
    } else {
      area1 = 0.5 * std::abs(p.x * (c_.y - d_.y) + c_.x * (d_.y - p.y) + d_.x * (p.y - c_.y));
      area2 = 0.5 * std::abs(p.x * (a_.y - b_.y) + a_.x * (b_.y - p.y) + b_.x * (p.y - a_.y));
    }
    return area1 + area2;
  }

  rectangle_t Complexquad::getFrameRect() const
  {
    const double min_x = std::min({a_.x, b_.x, c_.x, d_.x});
    const double max_x = std::max({a_.x, b_.x, c_.x, d_.x});
    const double min_y = std::min({a_.y, b_.y, c_.y, d_.y});
    const double max_y = std::max({a_.y, b_.y, c_.y, d_.y});
    return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
  }

  void Complexquad::move(point_t new_pos)
  {
    const point_t center = getCenter();
    move(new_pos.x - center.x, new_pos.y - center.y);
  }

  void Complexquad::move(double dx, double dy)
  {
    a_.x += dx;
    a_.y += dy;
    b_.x += dx;
    b_.y += dy;
    c_.x += dx;
    c_.y += dy;
    d_.x += dx;
    d_.y += dy;
  }

  void Complexquad::scale(double coefficient)
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
