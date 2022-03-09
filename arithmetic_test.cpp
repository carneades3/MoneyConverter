//#define TEST_INDEX_OPERATOR
#include "integer.hpp"
#include "integer_parsing.hpp"
#include "parsing_test.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>

using namespace std;
using namespace integer_space;

namespace arithmetic_test {
   
string get_number_string(const char character, const size_t length);

class Adding_Subtracting_Test {
private:
   static Integer first_static;
   static Integer second_static;
public:
   static void run() {
      cout << "\nTESTS OF ADDING NON-negative INTEGERS : \n";
      char signum = Integer::PLUS;
      parse_assert_adding_identical_signum(signum);

      cout << "\nTESTS OF ADDING NON-positive INTEGERS : \n";
      signum = Integer::MINUS;
      parse_assert_adding_identical_signum(signum);

      cout << "\nTESTS OF ADDING INTEGERS : \n";
      bool adding = true;
      parse_assert_adding_subtracting(adding);

      cout << "\nTESTS OF SUBTRACTING INTEGERS : \n";
      adding = false;
      parse_assert_adding_subtracting(adding);

      test_arithmetic_overflow();
   }

   static void parse_assert_adding_identical_signum(const char signum) {
      string signum_string = string(1, signum);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "4");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "2");
      assert_adding_subtracting(first_static, second_static, signum_string + "6", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "6", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "7");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "8");
      assert_adding_subtracting(first_static, second_static, signum_string + "15", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "15", true);

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "0");
      assert_adding_subtracting(first_static, second_static, "0", true);
      assert_adding_subtracting(second_static, first_static, "0", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "45");
      Parsing_Test::parse_string("second_static", second_static, "0");
      assert_adding_subtracting(first_static, second_static, signum_string + "45", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "45", true);

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "485");
      assert_adding_subtracting(first_static, second_static, signum_string + "485", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "485", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "145");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "197");
      assert_adding_subtracting(first_static, second_static, signum_string + "342", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "342", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "79");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "5");
      assert_adding_subtracting(first_static, second_static, signum_string + "84", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "84", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "59");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "35");
      assert_adding_subtracting(first_static, second_static, signum_string + "94", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "94", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "8900");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "4500");
      assert_adding_subtracting(first_static, second_static, signum_string + "13400", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "13400", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "139");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "60");
      assert_adding_subtracting(first_static, second_static, signum_string + "199", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "199", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "189");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "234");
      assert_adding_subtracting(first_static, second_static, signum_string + "423", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "423", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "1895678");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "234");
      assert_adding_subtracting(first_static, second_static, signum_string + "1895912", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "1895912", true);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "4567767189");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "34599876456");
      assert_adding_subtracting(first_static, second_static, signum_string + "39167643645", true);
      assert_adding_subtracting(second_static, first_static, signum_string + "39167643645", true);
   }

   static void parse_assert_adding_subtracting(const bool adding) {
      Parsing_Test::parse_string("first_static", first_static, "+4");
      Parsing_Test::parse_string("second_static", second_static, "0");
      string result = adding == true ? "+4" : "+4";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "+4" : "-4";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "-123");
      result = adding == true ? "-123" : "+123";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "-123" : "-123";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "+4");
      Parsing_Test::parse_string("second_static", second_static, "-2");
      result = adding == true ? "+2" : "+6";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "+2" : "-6";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "-7");
      Parsing_Test::parse_string("second_static", second_static, "+8");
      result = adding == true ? "+1" : "-15";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "+1" : "+15";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "+79");
      Parsing_Test::parse_string("second_static", second_static, "-5");
      result = adding == true ? "+74" : "+84";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "+74" : "-84";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "-59");
      Parsing_Test::parse_string("second_static", second_static, "-35");
      result = adding == true ? "-94" : "-24";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "-94" : "+24";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "+89");
      Parsing_Test::parse_string("second_static", second_static, "+45");
      result = adding == true ? "+134" : "+44";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "+134" : "-44";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "-139");
      Parsing_Test::parse_string("second_static", second_static, "+60");
      result = adding == true ? "-79" : "-199";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "-79" : "+199";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "-189");
      Parsing_Test::parse_string("second_static", second_static, "+234");
      result = adding == true ? "+45" : "-423";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "+45" : "+423";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "-18900");
      Parsing_Test::parse_string("second_static", second_static, "+234000");
      result = adding == true ? "+215100" : "-252900";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "+215100" : "+252900";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "-1894567323");
      Parsing_Test::parse_string("second_static", second_static, "+234123455");
      result = adding == true ? "-1660443868" : "-2128690778";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "-1660443868" : "+2128690778";
      assert_adding_subtracting(second_static, first_static, result, adding);

      Parsing_Test::parse_string("first_static", first_static, "+4567767");
      Parsing_Test::parse_string("second_static", second_static, "-34599876");
      result = adding == true ? "-30032109" : "+39167643";
      assert_adding_subtracting(first_static, second_static, result, adding);
      result = adding == true ? "-30032109" : "-39167643";
      assert_adding_subtracting(second_static, first_static, result, adding);
   }

   static inline void assert_adding_subtracting(const Integer & first, const Integer & second, const string & expectedResult,
                                         const bool adding) {
      Integer result = (adding == true) ?
                     add_assignment_print(first, second) : subtract_assignment_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
      result = (adding == true) ?
                     add_print(first, second) : subtract_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
   }

   static inline Integer add_print(const Integer &  first, const Integer &  second) {
      Integer sum = first + second;
      cout << first << " + " << second << " = " << sum << " \n";
      return sum;
   }
   
   static inline Integer add_assignment_print(const Integer &  first, const Integer &  second) {
      Integer third = first;
      cout << third << " += " << second << " is:\n ";
      third += second;
      cout <<  "third = " << third << " \n";
      return third;
   }

   static inline Integer subtract_print(const Integer & first, const Integer & second) {
      Integer difference = first - second;
      cout << first << " - " << second << " = " << difference << " \n";
      return difference;
   }
   
   static inline Integer subtract_assignment_print(const Integer &  first, const Integer &  second) {
      Integer third = first;
      cout << third << " -= " << second << " is:\n ";
      third -= second;
      cout <<  "third = " << third << " \n";
      return third;
   }

   static void test_arithmetic_overflow() {
      string large_number_string = arithmetic_test::get_number_string('5', Integer::MAX_ARRAY_LENGTH);
      Parsing_Test::parse_string("first_static", first_static, large_number_string);

      large_number_string = arithmetic_test::get_number_string('8', Integer::MAX_ARRAY_LENGTH);
      Parsing_Test::parse_string("second_static", second_static, large_number_string);
      try {
         add_print(first_static, second_static);
         assert(false);
      } catch (const out_of_range & e) {
         cerr << __func__ <<  " : Exception while adding: " << e.what() << "\n";
      }

      char signum = Integer::MINUS;
      second_static.set_signum(signum);
      try {
         subtract_print(first_static, second_static);
         assert(false);
      } catch (const out_of_range & e) {
         cerr << __func__ <<  " : Exception while subtracting: " << e.what() << "\n";
      }
   }
};
Integer Adding_Subtracting_Test::first_static;
Integer Adding_Subtracting_Test::second_static;


class Multiplying_Test {
private:
   static Integer first_static;
   static Integer second_static;
public:
    static void run() {
      cout << "\nTESTS OF MULTIPLYING NON-negative INTEGERS : \n";
      char signum = Integer::PLUS;
      parse_assert_multiplying_identical_signum(signum);

      cout << "\nTESTS OF MULTIPLYING NON-positive INTEGERS : \n";
      signum = Integer::MINUS;
      parse_assert_multiplying_identical_signum(signum);

      cout << "\nTESTS OF MULTIPLYING INTEGERS : \n";
      parse_assert_multiplying();

      test_arithmetic_overflow();
   }

   static void parse_assert_multiplying_identical_signum(const char signum) {
      string signum_string = string(1, signum);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "4");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "2");
      assert_multiplying(first_static, second_static, "+8");
      assert_multiplying(second_static, first_static, "+8");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "7");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "8");
      assert_multiplying(first_static, second_static, "+56");
      assert_multiplying(second_static, first_static, "+56");

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "0");
      assert_multiplying(first_static, second_static, "0");
      assert_multiplying(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "145");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "197");
      assert_multiplying(first_static, second_static, "+28565");
      assert_multiplying(second_static, first_static, "+28565");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "79");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "5");
      assert_multiplying(first_static, second_static, "+395");
      assert_multiplying(second_static, first_static, "+395");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "59");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "35");
      assert_multiplying(first_static, second_static, "+2065");
      assert_multiplying(second_static, first_static, "+2065");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "89");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "45");
      assert_multiplying(first_static, second_static, "+4005");
      assert_multiplying(second_static, first_static, "+4005");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "139");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "60");
      assert_multiplying(first_static, second_static, "+8340");
      assert_multiplying(second_static, first_static, "+8340");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "189");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "234");
      assert_multiplying(first_static, second_static, "+44226");
      assert_multiplying(second_static, first_static, "+44226");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "8900");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "45000");
      assert_multiplying(first_static, second_static, "+400500000");
      assert_multiplying(second_static, first_static, "+400500000");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "13999");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "6556077");
      assert_multiplying(first_static, second_static, "+91778521923");
      assert_multiplying(second_static, first_static, "+91778521923");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "76189");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "4234");
      assert_multiplying(first_static, second_static, "+322584226");
      assert_multiplying(second_static, first_static, "+322584226");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "45671000");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "34590");
      assert_multiplying(first_static, second_static, "+1579759890000");
      assert_multiplying(second_static, first_static, "+1579759890000");
   }

    static void parse_assert_multiplying() {
      Parsing_Test::parse_string("first_static", first_static, "+4");
      Parsing_Test::parse_string("second_static", second_static, "0");
      assert_multiplying(first_static, second_static, "0");
      assert_multiplying(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, "+45");
      Parsing_Test::parse_string("second_static", second_static, "0");
      assert_multiplying(first_static, second_static, "0");
      assert_multiplying(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "+485");
      assert_multiplying(first_static, second_static, "0");
      assert_multiplying(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "-123");
      assert_multiplying(first_static, second_static, "0");
      assert_multiplying(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, "+4");
      Parsing_Test::parse_string("second_static", second_static, "-2");
      assert_multiplying(first_static, second_static, "-8");
      assert_multiplying(second_static, first_static, "-8");

      Parsing_Test::parse_string("first_static", first_static, "-7");
      Parsing_Test::parse_string("second_static", second_static, "+8");
      assert_multiplying(first_static, second_static, "-56");
      assert_multiplying(second_static, first_static, "-56");

      Parsing_Test::parse_string("first_static", first_static, "+189");
      Parsing_Test::parse_string("second_static", second_static, "-234");
      assert_multiplying(first_static, second_static, "-44226");
      assert_multiplying(second_static, first_static, "-44226");

      Parsing_Test::parse_string("first_static", first_static, "+79678");
      Parsing_Test::parse_string("second_static", second_static, "-554356");
      assert_multiplying(first_static, second_static, "-44169977368");
      assert_multiplying(second_static, first_static, "-44169977368");

      Parsing_Test::parse_string("first_static", first_static, "-59897324");
      Parsing_Test::parse_string("second_static", second_static, "+35");
      assert_multiplying(first_static, second_static, "-2096406340");
      assert_multiplying(second_static, first_static, "-2096406340");

      Parsing_Test::parse_string("first_static", first_static, "+8900");
      Parsing_Test::parse_string("second_static", second_static, "-45000");
      assert_multiplying(first_static, second_static, "-400500000");
      assert_multiplying(second_static, first_static, "-400500000");

      Parsing_Test::parse_string("first_static", first_static, "-13999");
      Parsing_Test::parse_string("second_static", second_static, "+6556077");
      assert_multiplying(first_static, second_static, "-91778521923");
      assert_multiplying(second_static, first_static, "-91778521923");

      Parsing_Test::parse_string("first_static", first_static, "-76189");
      Parsing_Test::parse_string("second_static", second_static, "+4234");
      assert_multiplying(first_static, second_static, "-322584226");
      assert_multiplying(second_static, first_static, "-322584226");

      Parsing_Test::parse_string("first_static", first_static, "+45671000");
      Parsing_Test::parse_string("second_static", second_static, "-34590");
      assert_multiplying(first_static, second_static, "-1579759890000");
      assert_multiplying(second_static, first_static, "-1579759890000");
   }

   inline static void assert_multiplying(const Integer & first, const Integer & second, const string & expectedResult) {
      Integer result = multiply_assignment_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
      result = multiply_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
   }

   inline static Integer multiply_print(const Integer & first, const Integer & second) {
      Integer product = first * second;
      cout << first << " * " << second << " = " << product << " \n";
      return product;
   }
   
   static inline Integer multiply_assignment_print(const Integer &  first, const Integer &  second) {
      Integer third = first;
      cout << third << " *= " << second << " is:\n ";
      third *= second;
      cout <<  "third = " << third << " \n";
      return third;
   }

   static void test_arithmetic_overflow() {
      string large_number_string = arithmetic_test::get_number_string('8', Integer::MAX_ARRAY_LENGTH);
      Parsing_Test::parse_string("first_static", first_static, large_number_string);

      Parsing_Test::parse_string("second_static", second_static, "1234");
      try {
         multiply_print(first_static, second_static);
         assert(false);
      } catch (const out_of_range & e) {
         cerr << __func__ <<  " : Exception while multiplying: " << e.what() << "\n";
      }

      char signum = Integer::MINUS;
      second_static.set_signum(signum);
      try {
         multiply_print(second_static, first_static);
         assert(false);
      } catch (const out_of_range & e) {
         cerr << __func__ <<  " : Exception while multiplying: " << e.what() << "\n";
      }

      large_number_string = arithmetic_test::get_number_string('1', Integer::MAX_ARRAY_LENGTH / 2);
      Parsing_Test::parse_string("first_static", first_static, large_number_string);

      string other_number_string = arithmetic_test::get_number_string('1', Integer::MAX_ARRAY_LENGTH / 2);
      Parsing_Test::parse_string("second_static", second_static, other_number_string);
      signum = Integer::MINUS;
      second_static.set_signum(signum);

      assert_multiplying(first_static, second_static, "-123456790123456790120987654320987654321");
      assert_multiplying(second_static, first_static, "-123456790123456790120987654320987654321");

      other_number_string = arithmetic_test::get_number_string('1', Integer::MAX_ARRAY_LENGTH / 2 + 1);
      Parsing_Test::parse_string("second_static", second_static, other_number_string);

      assert_multiplying(first_static, second_static, "+1234567901234567901220987654320987654321");
      assert_multiplying(second_static, first_static, "+1234567901234567901220987654320987654321");

      large_number_string = arithmetic_test::get_number_string('1', Integer::MAX_ARRAY_LENGTH / 2 + 1);
      Parsing_Test::parse_string("first_static", first_static, large_number_string);

      try {
         multiply_print(first_static, second_static);
         assert(false);
      } catch (const out_of_range & e) {
         cerr << __func__ <<  " : Exception while multiplying: " << e.what() << "\n";
      }
   }
};
Integer Multiplying_Test::first_static;
Integer Multiplying_Test::second_static;

class Dividing_Remainder_Test {
private:
   static Integer first_static;
   static Integer second_static;
   
public:
   static void run()  {
      cout << "\nTESTS OF DIVIDING NON-negative INTEGERS : \n";
      char signum = Integer::PLUS;
      parse_assert_dividing_identical_signum(signum);

      cout << "\nTESTS OF DIVIDING NON-positive INTEGERS : \n";
      signum = Integer::MINUS;
      parse_assert_dividing_identical_signum(signum);

      cout << "\nTESTS OF DIVIDING INTEGERS : \n";
      parse_assert_dividing();
   }

   static void parse_assert_dividing_identical_signum(const char signum)  {
      string signum_string = string(1, signum);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "4");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "2");
      assert_dividing(first_static, second_static, "+2");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, "0");
      assert_remainder(second_static, first_static, signum_string + "2");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "7");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "8");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "+1");
      assert_remainder(first_static, second_static, signum_string + "7");
      assert_remainder(second_static, first_static, signum_string + "1");
      
      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "0");
      test_dividing_zero(first_static, second_static);
      test_dividing_zero(second_static, first_static);
      remainder_test_dividing_zero(first_static, second_static);
      remainder_test_dividing_zero(second_static, first_static);

      Parsing_Test::parse_string("first_static", first_static, signum_string + "145");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "197");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "+1");
      assert_remainder(first_static, second_static, signum_string + "145");
      assert_remainder(second_static, first_static, signum_string + "52");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "79");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "5");
      assert_dividing(first_static, second_static, "+15");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, signum_string + "4");
      assert_remainder(second_static, first_static, signum_string + "5");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "590");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "35");
      assert_dividing(first_static, second_static, "+16");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, signum_string + "30");
      assert_remainder(second_static, first_static, signum_string + "35");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "90");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "45");
      assert_dividing(first_static, second_static, "+2");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, "0");
      assert_remainder(second_static, first_static, signum_string + "45");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "139");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "60");
      assert_dividing(first_static, second_static, "+2");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, signum_string + "19");
      assert_remainder(second_static, first_static, signum_string + "60");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "189564");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "234");
      assert_dividing(first_static, second_static, "+810");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, signum_string + "24");
      assert_remainder(second_static, first_static, signum_string + "234");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "9000");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "4500000");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "+500");
      assert_remainder(first_static, second_static, signum_string + "9000");
      assert_remainder(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "149");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "6556077");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "+44000");
      assert_remainder(first_static, second_static, signum_string + "149");
      assert_remainder(second_static, first_static, signum_string + "77");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "76189");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "42");
      assert_dividing(first_static, second_static, "+1814");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, signum_string + "1");
      assert_remainder(second_static, first_static, signum_string + "42");

      Parsing_Test::parse_string("first_static", first_static, signum_string + "45671000");
      Parsing_Test::parse_string("second_static", second_static, signum_string + "34590");
      assert_dividing(first_static, second_static, "+1320");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, signum_string + "12200");
      assert_remainder(second_static, first_static, signum_string + "34590");
   }

   static void parse_assert_dividing()  {
      Parsing_Test::parse_string("first_static", first_static, "+4");
      Parsing_Test::parse_string("second_static", second_static, "0");
      test_dividing_zero(first_static, second_static);
      assert_dividing(second_static, first_static, "0");
      remainder_test_dividing_zero(first_static, second_static);
      assert_remainder(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, "+45");
      Parsing_Test::parse_string("second_static", second_static, "0");
      test_dividing_zero(first_static, second_static);
      assert_dividing(second_static, first_static, "0");
      remainder_test_dividing_zero(first_static, second_static);
      assert_remainder(second_static, first_static, "0");

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "+485");
      assert_dividing(first_static, second_static, "0");
      test_dividing_zero(second_static, first_static);
      assert_remainder(first_static, second_static, "0");
      remainder_test_dividing_zero(second_static, first_static);

      Parsing_Test::parse_string("first_static", first_static, "0");
      Parsing_Test::parse_string("second_static", second_static, "-123");
      assert_dividing(first_static, second_static, "0");
      test_dividing_zero(second_static, first_static);
      assert_remainder(first_static, second_static, "0");
      remainder_test_dividing_zero(second_static, first_static);

      Parsing_Test::parse_string("first_static", first_static, "+4");
      Parsing_Test::parse_string("second_static", second_static, "-2");
      assert_dividing(first_static, second_static, "-2");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, "0");
      assert_remainder(second_static, first_static, "-2");

      Parsing_Test::parse_string("first_static", first_static, "-7");
      Parsing_Test::parse_string("second_static", second_static, "+8");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "-1");
      assert_remainder(first_static, second_static, "-7");
      assert_remainder(second_static, first_static, "+1");

      Parsing_Test::parse_string("first_static", first_static, "+189");
      Parsing_Test::parse_string("second_static", second_static, "-234");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "-1");
      assert_remainder(first_static, second_static, "+189");
      assert_remainder(second_static, first_static, "-45");

      Parsing_Test::parse_string("first_static", first_static, "+79678");
      Parsing_Test::parse_string("second_static", second_static, "-554356");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "-6");
      assert_remainder(first_static, second_static, "+79678");
      assert_remainder(second_static, first_static, "-76288");

      Parsing_Test::parse_string("first_static", first_static, "-59897324");
      Parsing_Test::parse_string("second_static", second_static, "+35");
      assert_dividing(first_static, second_static, "-1711352");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, "-4");
      assert_remainder(second_static, first_static, "+35");

      Parsing_Test::parse_string("first_static", first_static, "+8900");
      Parsing_Test::parse_string("second_static", second_static, "-45000");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "-5");
      assert_remainder(first_static, second_static, "+8900");
      assert_remainder(second_static, first_static, "-500");

      Parsing_Test::parse_string("first_static", first_static, "-13999");
      Parsing_Test::parse_string("second_static", second_static, "+6556077");
      assert_dividing(first_static, second_static, "0");
      assert_dividing(second_static, first_static, "-468");
      assert_remainder(first_static, second_static, "-13999");
      assert_remainder(second_static, first_static, "+4545");

      Parsing_Test::parse_string("first_static", first_static, "-76189789");
      Parsing_Test::parse_string("second_static", second_static, "+4234");
      assert_dividing(first_static, second_static, "-17994");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, "-3193");
      assert_remainder(second_static, first_static, "+4234");

      Parsing_Test::parse_string("first_static", first_static, "+45671000");
      Parsing_Test::parse_string("second_static", second_static, "-34590");
      assert_dividing(first_static, second_static, "-1320");
      assert_dividing(second_static, first_static, "0");
      assert_remainder(first_static, second_static, "+12200");
      assert_remainder(second_static, first_static, "-34590");
   }

   inline static void assert_dividing(const Integer & first, const Integer & second, const string & expectedResult)  {
      Integer result = divide_assignment_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
      result = divide_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
   }
   
   inline static void assert_remainder(const Integer &  first, const Integer &  second, const string & expectedResult)  {
      Integer result = remainder_assignment_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
      result = remainder_print(first, second);
      assert(static_cast<string> (result) == (expectedResult));
   }

   inline  static Integer divide_print(const Integer &  first, const Integer &  second)  {
      Integer quotient = first / second;
      cout << first << " / " << second << " = " << quotient << " \n";
      return quotient;
   }
   
   static inline Integer divide_assignment_print(const Integer &  first, const Integer &  second) {
      Integer third = first;
      cout << third << " /= " << second << " is:\n ";
      third /= second;
      cout <<  "third = " << third << " \n";
      return third;
   }
   
   inline  static Integer remainder_print(const Integer &  first, const Integer &  second)  {
      Integer remainder = first % second;
      cout << first << " % " << second << " = " << remainder << " \n";
      return remainder;
   }
   
   static inline Integer remainder_assignment_print(const Integer &  first, const Integer &  second) {
      Integer third = first;
      cout << third << " %= " << second << " is:\n ";
      third %= second;
      cout <<  "third = " << third << " \n";
      return third;
   }
   
private:
   static void test_dividing_zero(const Integer &  first, const Integer &  second)  {
      try {
         divide_print(first, second);
         assert(false);
      } catch (const Arithmetic_Error & e) {
         cerr << __func__ <<  " : Exception while dividing: " << e.what() << "\n";
      }
      try {
         divide_assignment_print(first, second);
         assert(false);
      } catch (const Arithmetic_Error & e) {
         cerr << __func__ <<  " : Exception while dividing: " << e.what() << "\n";
      }
   }
   
   static void remainder_test_dividing_zero(const Integer &  first, const Integer &  second)  {
      try {
         remainder_print(first, second);
         assert(false);
      } catch (const Arithmetic_Error & e) {
         cerr << __func__ <<  " : Exception while remainder: " << e.what() << "\n";
      }
      try {
         remainder_assignment_print(first, second);
         assert(false);
      } catch (const Arithmetic_Error & e) {
         cerr << __func__ <<  " : Exception while remainder: " << e.what() << "\n";
      }
   }
};
Integer Dividing_Remainder_Test::first_static;
Integer Dividing_Remainder_Test::second_static;

void run() {
   Adding_Subtracting_Test::run();
   Multiplying_Test::run();
   Dividing_Remainder_Test::run();
}

string get_number_string(const char character, const size_t length) {
   if (character < '1' || character > '9') 
      throw  invalid_argument("Only digit from 1 to 9 is allowed");
   string digit_string = string(1, character);
   string large_number_string = "";
   for (short counter = 1; counter <= length; counter++) {
      large_number_string += digit_string;
   }

   return large_number_string;
}

}

int main() {
   try {
      arithmetic_test::run();
      cerr << "\n END of ALL TESTS\n";
      return 0;
   }
   catch (const Arithmetic_Error & e) {
      cerr << __func__ << " exception : " << e.what() << endl;
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (const runtime_error& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (const exception& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
