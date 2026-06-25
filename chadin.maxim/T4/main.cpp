#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "rectangle.hpp"
#include "triangle.hpp"
#include "complexquad.hpp"

namespace {
  void printInfo(const std::vector<std::shared_ptr<chadin::Shape>>& shapes)
  {
    for (const auto& shape: shapes) {
      const auto rect = shape->getFrameRect();
      std::cout << shape->getArea() << " " << rect.width << " " << rect.height << " ";
      std::cout << rect.pos.x << " " << rect.pos.y << "\n";
    }
  }
}

int main()
{
  std::vector<std::shared_ptr<chadin::Shape>> shapes;

  try {
    shapes.push_back(std::make_shared<chadin::Rectangle>(
      10.0, 20.0, chadin::point_t{0.0, 0.0}
    ));
    shapes.push_back(std::make_shared<chadin::Triangle>(
      chadin::point_t{0, 0}, chadin::point_t{3, 0}, chadin::point_t{0, 4}
    ));
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  printInfo(shapes);

  chadin::point_t scale_center;
  double factor = 0.0;

  if (!(std::cin >> scale_center.x >> scale_center.y >> factor)) {
    if (std::cin.eof()) {
      return 0;
    }
    std::cerr << "Invalid input\n";
    return 1;
  }

  if (factor <= 0.0) {
    std::cerr << "Invalid scaling factor\n";
    return 1;
  }

  for (auto& shape: shapes) {
    const auto pos_before = shape->getFrameRect().pos;
    shape->scale(factor);
    const auto pos_after = shape->getFrameRect().pos;

    const double target_x = (pos_before.x - scale_center.x) * factor + scale_center.x;
    const double target_y = (pos_before.y - scale_center.y) * factor + scale_center.y;

    const double dx = target_x - pos_after.x;
    const double dy = target_y - pos_after.y;

    shape->move(dx, dy);
  }

  printInfo(shapes);
  return 0;
}
