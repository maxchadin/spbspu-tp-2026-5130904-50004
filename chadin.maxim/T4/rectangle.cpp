#include "rectangle.hpp"
#include <stdexcept>

namespace chadin {
  Rectangle::Rectangle(double width, double height, point_t pos):
    width_(width),
    height_(height),
    pos_(pos)
  {
    if (width <= 0.0 || height <= 0.0) {
      throw std::invalid_argument("Invalid rectangle dimensions");
    }
  }

  double Rectangle::getArea() const
  {
    return width_ * height_;
  }

  rectangle_t Rectangle::getFrameRect() const
  {
    return {width_, height_, pos_};
  }

  void Rectangle::move(point_t new_pos)
  {
    pos_ = new_pos;
  }

  void Rectangle::move(double dx, double dy)
  {
    pos_.x += dx;
    pos_.y += dy;
  }

  void Rectangle::scale(double coefficient)
  {
    if (coefficient <= 0.0) {
      throw std::invalid_argument("Invalid scale coefficient");
    }
    width_ *= coefficient;
    height_ *= coefficient;
  }
}
