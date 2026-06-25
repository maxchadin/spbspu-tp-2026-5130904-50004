#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "data_struct.hpp"

int main()
{
  using namespace chadin;

  std::vector<DataStruct> rawData;
  std::copy(
    std::istream_iterator<DataStruct>(std::cin),
    std::istream_iterator<DataStruct>(),
    std::back_inserter(rawData)
  );

  std::vector<DataStruct> filteredData;
  std::copy_if(
    rawData.begin(),
    rawData.end(),
    std::back_inserter(filteredData),
    [](const DataStruct& ds) { return ds.isValid; }
  );

  std::sort(filteredData.begin(), filteredData.end());

  std::copy(
    filteredData.begin(),
    filteredData.end(),
    std::ostream_iterator<DataStruct>(std::cout, "\n")
  );

  return 0;
}
