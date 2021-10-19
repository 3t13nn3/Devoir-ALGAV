#include "head.h"

/*
arg: 0 initial number
Return a decimal number to a binary number under a vector<bool>
*/
std::string decomposition(const mpz_class &number) {

  // lib provide a conversion to base 2, way more faster
  //.get_str(2) to get the number under a base 2 format string
  // care, this giving us the invert of what we want, need to swap 0 and 1
  std::string s = number.get_str(2);

  std::string toReturn(s.size(), false);

  for (int i = toReturn.size() - 1; i >= 0; --i) {
    toReturn[toReturn.size() - 1 - i] = s[i];
  }

  return toReturn;
}

/*
arg: 0 binary number under a vector<bool>, 1 final size of the vector
Return the vector with the new size
*/
std::string completion(std::string &number, int finalSize) {

  int nSize = number.size();

  if (finalSize <= nSize) {
    number = {number.begin(), number.end() + (finalSize - nSize)};
  } else {
    number.insert(number.end(), finalSize - nSize, '0');
  }

  return number;
}

/*
arg: 0 number to convert to a binary number under a slice,
1 size of the slice (need to be a power of 2, 1<<n as arg avoid pb)
Return a X as base 10 number under a slice of size N (base 2 number)
Just using the methods defined above
*/
std::string table(const mpz_class &x, int n) {

  std::string vec = decomposition(x);
  return completion(vec, n);
}
