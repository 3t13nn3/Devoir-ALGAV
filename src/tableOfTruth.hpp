#ifndef __TABLEOFTRUTH_HPP__
#define __TABLEOFTRUTH_HPP__

#include <string>

// Handling big int
#include <gmpxx.h>

namespace ex1 {
class TableOfTruth {
   public:
    TableOfTruth();

    std::string Table(const mpz_class &x, int n);

   private:
    std::string _number;

    void Decomposition(const mpz_class &number);

    void Completion(int finalSize);
};
}  // namespace ex1

#endif
