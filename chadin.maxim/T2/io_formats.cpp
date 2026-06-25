#include "io_formats.hpp"

namespace chadin {
  FormatGuard::FormatGuard(std::basic_ios<char>& stream):
    stream_(stream),
    fill_(stream.fill()),
    precision_(stream.precision()),
    flags_(stream.flags())
  {
  }

  FormatGuard::~FormatGuard()
  {
    stream_.fill(fill_);
    stream_.precision(precision_);
    stream_.flags(flags_);
  }

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char current = '0';
    in >> current;
    if (in && current != dest.expected) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleLiteralIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    if (!(in >> dest.value)) {
      return in;
    }
    char suffix = '0';
    in >> suffix;
    if (in && suffix != 'd' && suffix != 'D') {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, UllLiteralIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    if (!(in >> dest.value)) {
      return in;
    }
    char char1 = '0', char2 = '0', char3 = '0';
    in >> char1 >> char2 >> char3;
    if (in) {
      bool isUll = (char1 == 'u' || char1 == 'U') &&
                   (char2 == 'l' || char2 == 'L') &&
                   (char3 == 'l' || char3 == 'L');
      if (!isUll) {
        in.setstate(std::ios::failbit);
      }
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringLiteralIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> DelimiterIO{'"'};
    if (!in) {
      return in;
    }
    std::getline(in, dest.value, '"');
    return in;
  }
}
