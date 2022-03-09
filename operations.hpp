#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include <iostream>
#include <iomanip>

#include "assertions.hpp"
#include <gmpxx.h>

//using std::ios_base;

namespace operations {

#ifndef NDEBUG
#  define ASSERT_CONDITIONAL(MpzOrLongLong, Integer, boolean) \
   (boolean ? ASSERT_NUMBER_INTEGER(reinterpret_cast<const mpz_class& >(MpzOrLongLong), Integer, __FILE__, __LINE__, __DATE__, __TIME__) \
   : ASSERT_NUMBER_INTEGER(reinterpret_cast<const long long &>(MpzOrLongLong), Integer, __FILE__, __LINE__, __DATE__, __TIME__));
#else
#   define ASSERT_CONDITIONAL(MpzOrLongLong, Integer) ;
#endif
   
enum class Arithmetic { ADD, SUBTRACT, MULTIPLY, DIVIDE };
   
template <const bool FLAG>
struct Operations {
   /*
   template <const bool flag>
   struct Foo {
      std::conditional_t<flag, mpz_class, long long> Number;
   };
   using Number = Foo<FLAG>;
   */
   using Number = std::conditional_t<FLAG, mpz_class, long long>;
   typedef void (* Operation_Ptr)(const Number & , const Number & , const Integer & , const Integer & );
      
   static void add(const Number & N_1, const Number & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Base::add " << '\n';
      Number number_result = N_1 + N_2;
      cerr << std::showpos << "\n number: " << N_1 << " + " << N_2 << " = " << number_result;
      Integer object_result   = O_1 + O_2;
      cerr << "\n object: " << O_1 << " + " << O_2 << " = " << object_result << '\n';
#ifndef NDEBUG
      ASSERT_CONDITIONAL(number_result, object_result, FLAG);
#endif
   }
   
   static void subtract(const Number & N_1, const Number & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Base::subtract " << '\n';
      Number number_result = N_1 - N_2;
      cerr << std::showpos << "\n number: " << N_1 << " - " << N_2 << " = " << number_result;
      Integer object_result   = O_1 - O_2;
      cerr << "\n object: " << O_1 << " - " << O_2 << " = " << object_result << '\n';
#ifndef NDEBUG
      ASSERT_CONDITIONAL(number_result, object_result, FLAG);
#endif
   }
   
   static void multiply(const Number & N_1, const Number & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Base::multiply " << '\n';
      Number number_result = N_1 * N_2;
      cerr << std::showpos << "\n number: " << N_1 << " * " << N_2 << " = " << number_result;
      Integer object_result   = O_1 * O_2;
      cerr << "\n object: " << O_1 << " * " << O_2 << " = " << object_result << '\n';
#ifndef NDEBUG
      ASSERT_CONDITIONAL(number_result, object_result, FLAG);
#endif
   }
   
   static void divide(const Number & N_1, const Number & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Base::divide " << '\n';
      if (N_2 == 0)
         return;
      Number number_result = N_1 / N_2;
      cerr << std::showpos << "\n number: " << N_1 << " / " << N_2 << " = " << number_result;
      Integer object_result   = O_1 / O_2;
      cerr << "\n object: " << O_1 << " / " << O_2 << " = " << object_result << '\n';
#ifndef NDEBUG
      ASSERT_CONDITIONAL(number_result, object_result, FLAG);
#endif
   }
   
   static void modulo(const Number & N_1, const Number & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Base::modulo " << '\n';
      if (N_2 == 0)
         return;
      Number number_result = N_1 % N_2;
      cerr << std::showpos << "\n number: " << N_1 << " % " << N_2 << " = " << number_result;
      Integer object_result   = O_1 % O_2;
      cerr << "\n object: " << O_1 << " % " << O_2 << " = " << object_result << '\n';
#ifndef NDEBUG
      ASSERT_CONDITIONAL(number_result, object_result, FLAG);
#endif
   }
   
   static void compare(const Number & N_1, const Number & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Base::compare " << '\n';
      assert(O_1.is_zero() == (N_1 == 0));
      
      assert((O_1 == O_2) == (N_1 == N_2));
      assert((O_1 != O_2) == (N_1 != N_2));

      assert((O_1 > O_2) == (N_1 > N_2));
      assert((O_1 <= O_2) == (N_1 <= N_2));

      assert((O_1 <  O_2) == (N_1 < N_2));
      assert((O_1 >=  O_2) == (N_1 >= N_2));
   }
   
   static inline void check_unary(const Number & number, const Integer & object) {
      cerr << " Base::check_unary " << '\n';
#ifndef NDEBUG
      ASSERT_CONDITIONAL(number, object, FLAG);
#endif
      Number lvalue_number = +number;
      Integer lvalue_object = +object;
#ifndef NDEBUG
      ASSERT_CONDITIONAL(lvalue_number, lvalue_object, FLAG);
#endif
      lvalue_number = -number;
      lvalue_object = -object;
#ifndef NDEBUG
      ASSERT_CONDITIONAL(lvalue_number, lvalue_object, FLAG);
#endif
   }
};

}

#endif
