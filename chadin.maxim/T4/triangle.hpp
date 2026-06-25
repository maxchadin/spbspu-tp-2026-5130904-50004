#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"

namespace chadin {
  class Triangle: public Shape {
  public:
    Triangle(point_t a, point_t b, point_t c);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t new_pos) override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;

  private:
    point_t a_;
    point_t b_;
    point_t c_;

    point_t getCenter() const;
  };
}

#endif
