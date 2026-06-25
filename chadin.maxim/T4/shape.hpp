#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "base-types.hpp"

namespace chadin {

class Shape {
public:
  virtual ~Shape() = default;
  virtual double getArea() const = 0;
  virtual rectangle_t getFrameRect() const = 0;
  virtual void move(point_t new_pos) = 0;
  virtual void move(double dx, double dy) = 0;
  virtual void scale(double coefficient) = 0;
};

}

#endif
