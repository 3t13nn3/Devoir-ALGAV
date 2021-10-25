#include "tableOfTruth.hpp"

using namespace Devoir;

TableOfTruth::TableOfTruth() {}

/*
arg: 0 initial number
Fill a decimal number to a binary number under a string member of the class
_number
*/
void TableOfTruth::Decomposition(const mpz_class &number) {

  // lib provide a conversion to base 2, way more faster
  //.get_str(2) to get the number under a base 2 format string
  // care, this giving us the invert of what we want, need to swap 0 and 1
  std::string s = number.get_str(2);

  // we can assign because they have the same size
  _number = s;

  for (int i = _number.size() - 1; i >= 0; --i) {
    _number[_number.size() - 1 - i] = s[i];
  }

}

/*
arg: 0  final size of the vector
Resize the _number string
*/
void TableOfTruth::Completion(int finalSize) {

  int nSize = _number.size();

  if (finalSize <= nSize) {
    _number = {_number.begin(), _number.end() + (finalSize - nSize)};
  } else {
    _number.insert(_number.end(), finalSize - nSize, '0');
  }
}

/*
arg: 0 number to convert to a binary number under a slice,
1 size of the slice (need to be a power of 2, 1<<n as arg avoid pb)
Return a X as base 10 number under a slice of size N (base 2 number)
Just using the methods defined above
*/
std::string TableOfTruth::Table(const mpz_class &x, int n) {
  Decomposition(x);
  Completion(n);
  return _number;
}
