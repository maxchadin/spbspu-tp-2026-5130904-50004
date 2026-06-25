#ifndef IO_FORMATS_HPP
#define IO_FORMATS_HPP

#include <iostream>
#include <string>

namespace chadin {
  struct DelimiterIO {
    char expected;
  };

  struct DoubleLiteralIO {
    double& value;
  };

  struct UllLiteralIO {
    unsigned long long& value;
  };

  struct StringLiteralIO {
    std::string& value;
  };

  class FormatGuard {
  public:
    explicit FormatGuard(std::basic_ios<char>& stream);
    ~FormatGuard();
  private:
    std::basic_ios<char>& stream_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags flags_;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleLiteralIO&& dest);
  std::istream& operator>>(std::istream& in, UllLiteralIO&& dest);
  std::istream& operator>>(std::istream& in, StringLiteralIO&& dest);
}

#endif
