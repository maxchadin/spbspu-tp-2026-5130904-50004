#include "data_struct.hpp"
#include "io_formats.hpp"
#include <iomanip>

namespace chadin {
  DataStruct::DataStruct():
    key1(0.0),
    key2(0),
    key3(""),
    isValid(false)
  {
  }

  bool operator<(const DataStruct& lhs, const DataStruct& rhs)
  {
    if (lhs.key1 != rhs.key1) {
      return lhs.key1 < rhs.key1;
    } else if (lhs.key2 != rhs.key2) {
      return lhs.key2 < rhs.key2;
    }
    return lhs.key3.length() < rhs.key3.length();
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    DataStruct input;
    in >> DelimiterIO{'('};

    int readKeysMask = 0;
    for (size_t i = 0; i < 3; ++i) {
      in >> DelimiterIO{':'};
      std::string label;
      in >> label;

      if (label == "key1" && !(readKeysMask & 1)) {
        in >> DoubleLiteralIO{input.key1};
        readKeysMask |= 1;
      } else if (label == "key2" && !(readKeysMask & 2)) {
        in >> UllLiteralIO{input.key2};
        readKeysMask |= 2;
      } else if (label == "key3" && !(readKeysMask & 4)) {
        in >> StringLiteralIO{input.key3};
        readKeysMask |= 4;
      } else {
        in.setstate(std::ios::failbit);
      }
    }

    in >> DelimiterIO{':'} >> DelimiterIO{')'};

    if (in) {
      input.isValid = true;
      dest = input;
    } else {
      in.clear();
      char discard = '0';
      while (in.get(discard) && discard != ')') {
      }
      dest.isValid = false;
    }

    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }

    FormatGuard guard(out);
    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d"
        << ":key2 " << src.key2 << "ull"
        << ":key3 \"" << src.key3 << "\":)";
    return out;
  }
}
