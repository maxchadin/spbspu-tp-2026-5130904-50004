#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
#include "Polygon.hpp"
#include "Commands.hpp"

namespace chadin {
  namespace detail {

    struct LineReader
    {
      std::string line;
      operator std::string() const { return line; }
      friend std::istream& operator>>(std::istream& in, LineReader& lr)
      {
        return std::getline(in, lr.line);
      }
    };

    struct parse_polygon
    {
      Polygon operator()(const std::string& str) const
      {
        std::istringstream iss(str);
        Polygon p;
        if (iss >> p) {
          std::string extra;
          if (iss >> extra) {
            return {};
          }
          return p;
        }
        return {};
      }
    };

    bool isEmptyPolygon(const Polygon& p)
    {
      return p.points.empty();
    }

  }
}

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Error: filename required\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: cannot open file\n";
    return 1;
  }

  std::vector<std::string> lines;
  std::copy(std::istream_iterator<chadin::detail::LineReader>(file),
            std::istream_iterator<chadin::detail::LineReader>(),
            std::back_inserter(lines));

  std::vector<chadin::Polygon> polygons(lines.size());
  std::transform(lines.begin(), lines.end(), polygons.begin(), chadin::detail::parse_polygon());

  auto it = std::remove_if(polygons.begin(), polygons.end(), chadin::detail::isEmptyPolygon);
  polygons.erase(it, polygons.end());

  chadin::processCommands(polygons, std::cin, std::cout);

  return 0;
}
