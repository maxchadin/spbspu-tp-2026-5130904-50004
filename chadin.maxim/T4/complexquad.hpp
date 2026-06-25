#ifndef COMPLEXQUAD_HPP
#define COMPLEXQUAD_HPP

#include "shape.hpp"

namespace chadin {
  class Complexquad: public Shape {
  public:
    Complexquad(point_t a, point_t b, point_t c, point_t d);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t new_pos) override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;

  private:
    point_t a_;
    point_t b_;
    point_t c_;
    point_t d_;

    point_t getCenter() const;
  };
}

#endif
