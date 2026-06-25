#ifndef COMMANDS_H
#define COMMANDS_H

#include "Polygon.hpp"
#include <vector>
#include <iosfwd>

namespace chadin {

  void processCommands(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out);

}

#endif
