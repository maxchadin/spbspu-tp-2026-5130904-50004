#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iostream>

namespace chadin {
  struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
    bool isValid;

    DataStruct();
  };

  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
