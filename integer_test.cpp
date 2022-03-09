#define TEST_INDEX_OPERATOR

#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>
#include <array>
#include <algorithm>
#include <cstring>
#include <random>

using namespace std;
using namespace integer_space;

namespace integer_test {
   const array<string, 2> ALLOWED = {"vector", "array"};  // must be static to exist in program all time running
   const array<string, 2>::const_iterator BEGIN = ALLOWED.begin();
   const array<string, 2>::const_iterator END = ALLOWED.end();
   constexpr int INCORRECT_INDEX = -44;
   
   string get_allowed() {
      string result = " Only acceptable type: ";
      for (auto type : ALLOWED) 
         result += " " + type;
      return result;
   }
   
   template <typename Container>
   inline int get_type_index(const Container &) {
      const string TYPE = typeid(Container).name();
      for (int i = 0; i < ALLOWED.size(); i++) 
         if (TYPE.find(ALLOWED[i]) != string::npos)
            return i;
      return INCORRECT_INDEX;
   }
   
   template <typename Container>
   void print_container(const string & title, const Container & C) {
      if (INCORRECT_INDEX == get_type_index(C))
         throw invalid_argument(__func__ + get_allowed());
      cout << title;
      for (auto elem : C)
         cout << " " << elem;
      cout << "\nEnd of container printing\n";
   }
   
   namespace conversions {
   
      template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                    || is_integral<Number>::value), bool> = true>
      void test_incorrect_conversion(const Integer & integer) {
         static const string TYPE_NAME = typeid(Number).name();
         try {
            Number n = integer.operator Number();
            //Number n = Number(integer);
            cerr << " !!! " << __func__ << " object of type '" << TYPE_NAME << "' = " << n << "\n";
            assert(false);
         } catch (const out_of_range & e) {
            cerr << __func__ << " object of type '" << TYPE_NAME << "' message: " <<  e.what() << "\n";
         }
      }
      
      template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                    || is_integral<Number>::value), bool> = true>
      inline void print_assert_conversion(const Integer & integer, const Number value) {
         cout << typeid(Number).name() << " Integer value = " << integer;
         cout << "\nstatic_cast to " << typeid(Number).name() << " = " << static_cast<Number>(integer) << " = "
            << integer.operator Number() << "\n";
         assert(static_cast<Number>(integer) == value);
         assert(integer.operator Number() == value);
      }
      
      template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                    || is_integral<Number>::value), bool> = true>
      void test_conversion(const bool condition, const Number value) {
         if (condition)
            print_assert_conversion(Integer::create_Integer(value),
                                    value);
         else {
            static const string TYPE_NAME = typeid(Number).name();
            if (is_integral<Number>::value) {
               try {
                  Integer integer = Integer::create_Integer(value);
                  cerr << " !!! " << __func__ << " Integer created from object of type '" << TYPE_NAME << "' = " << integer << "\n";
                  assert(false);
               } catch (const out_of_range & e) {
                  cerr << __func__ << " object of type '" << TYPE_NAME << "' message: " <<  e.what() << "\n";
               }
            }
            else {
               try {
                  Integer integer = Integer::create_Integer(value);
                  cerr << " !!! " << __func__ << " Integer created from object of type '" << TYPE_NAME << "' = " << integer << "\n";
                  assert(false);
               } catch (const out_of_range & e) {
                  cerr << __func__ << " object of type '" << TYPE_NAME << "' message: " <<  e.what() << "\n";
               } catch (const invalid_argument & e) {
                  cerr << __func__ << " object of type '" << TYPE_NAME << "' message: " <<  e.what() << "\n";
               }
            }
         }
      }
      
      template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                    || is_integral<Number>::value), bool> = true>
      inline void test_conversion(const bool lowest_ok, const bool max_ok) {
         cout << __func__ << "\nlowest of ";
         test_conversion(lowest_ok, numeric_limits<Number>::lowest());
         cout << "\nmax of ";
         test_conversion(max_ok, numeric_limits<Number>::max());
      }
      
      void test_conversions() {
         test_conversion<char>(true, true);
         test_conversion<int_fast8_t>(true, true);
         test_conversion<short>(true, true);
         test_conversion<unsigned short>(true, true);
         test_conversion<int>(true, true);
         test_conversion<unsigned>(true, true);
         test_conversion<long>(true, true);
         test_conversion<unsigned long>(true, true);
         test_conversion<long long>(true, true);
         test_conversion<unsigned long long>(true, true);
         
         test_conversion<float>(true, true);
         test_conversion<double>(false, false);
         test_conversion<long double>(false, false);
      }
      
      template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                    || is_integral<Number>::value), bool> = true>
      inline void test_operator_number(const bool condition, const Integer & integer) {
         if (condition)
            print_assert_conversion(integer, integer.operator Number());
         else 
            test_incorrect_conversion<Number>(integer);
      }
      
      template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                    || is_integral<Number>::value), bool> = true>
      inline void test_operator_number(const bool lowest_ok, const bool max_ok) {
         cout << __func__ << "\nlowest of ";
         test_operator_number<Number>(lowest_ok, numeric_limits<Integer>::lowest());
         cout << "\nmax of ";
         test_operator_number<Number>(max_ok, numeric_limits<Integer>::max());
      }
      
      void test_operator_number() {
         test_operator_number<char>(false, false);
         test_operator_number<int_fast8_t>(false, false);
         test_operator_number<short>(false, false);
         test_operator_number<unsigned short>(false, false);
         test_operator_number<int>(false, false);
         test_operator_number<unsigned>(false, false);
         test_operator_number<long>(false, false);
         test_operator_number<unsigned long>(false, false);
         test_operator_number<long long>(false, false);
         test_operator_number<unsigned long long>(false, false);
         
         test_operator_number<float>(false, false);
         test_operator_number<double>(true, true);
         test_operator_number<long double>(true, true);
      }
      
   }
      
#ifdef TEST_INDEX_OPERATOR
   namespace index_operator {
      void test_index_operator() {
         cout << "\n                              test_index_operator:\n";
         Integer A  = Integer::create_Integer<7>({7, 2, 3, 0, 6, 5, 0}, Integer::PLUS);
         cout << "\nInteger 'A' After construct by array and signum's byte of '" << Integer::PLUS << "' : " << A << "\n";
         assert(static_cast<string>(A)==("+7230650"));
         conversions::test_incorrect_conversion<char>(A);
         conversions::test_incorrect_conversion<int_fast8_t>(A);
         conversions::test_incorrect_conversion<short>(A);
         conversions::test_incorrect_conversion<unsigned short>(A);
         //test_incorrect_conversion<double>(A);
         assert(int(A) == 7230650);
         assert(unsigned(A) == 7230650);
         assert(long(A) == 7230650);
         assert((unsigned long)(A) == 7230650);
         assert((long long)(A) == 7230650);
         assert((unsigned long long)(A) == 7230650);
         assert((long double)(A) == 7230650);
         assert(double(A) == 7230650);
         assert(float(A) == 7230650);
         
         A[Integer::MAX_ARRAY_LENGTH - 1] = 3;
         cout << "Integer 'A' After change last index to 3:   "<< A << "\n";
         
         Integer B  = A;
         cout << "Integer 'B' After construct and assign to other integer (copy of integer 'A'): " << static_cast<string>(B) << "\n";
         assert(static_cast<string>(A)==("+7230653"));
         assert(static_cast<string>(B)== ("+7230653"));
         
         B[Integer::MAX_ARRAY_LENGTH - 1] = 2;
         cout << "Integer 'B' After change last index to 2:   " << B << "\n";
         cout << "Integer 'A'  should be unchanged:   " << A << "\n";
         
         assert(static_cast<string>(A)==("+7230653"));
         assert(static_cast<string>(B)== ("+7230652"));
         
         Integer C { A } ;
         cout << "Integer 'C' After construct by other integer (copy of integer 'A'): " << static_cast<string>(C) << "\n";
         assert(static_cast<string>(C)==("+7230653"));
         
         C = B;
         cout << "Integer 'C' After assign to other integer (integer 'B'): " << static_cast<string>(C) << "\n";
         assert(static_cast<string>(C)==("+7230652"));
         assert(static_cast<string>(B)==("+7230652"));
         
         Integer D { std::move(A) };
         cout << "Integer 'D' After construct by other integer (std::move of integer 'A'): " << static_cast<string>(D) << "\n";
         cout << "Integer 'A' should be unchanged:: " << static_cast<string>(A) << "\n";
         assert(static_cast<string>(D)==("+7230653"));
         assert(static_cast<string>(A)==("+7230653"));
         
         D = std::move (B);
         cout << "Integer 'D' After assign to other integer (std::move of integer 'B'): " << static_cast<string>(D) << "\n";
         cout << "Integer 'B' should be unchanged:: " << static_cast<string>(B) << "\n";
         assert(static_cast<string>(D)==("+7230652"));
         assert(static_cast<string>(B)==("+7230652"));
         
         A.set_signum(Integer::MINUS);
         assert(static_cast<string>(A)==("-7230653"));
         conversions::test_incorrect_conversion<char>(A);
         conversions::test_incorrect_conversion<int_fast8_t>(A);
         conversions::test_incorrect_conversion<short>(A);
         conversions::test_incorrect_conversion<unsigned short>(A);
         assert(int(A) == -7230653);
         conversions::test_incorrect_conversion<unsigned>(A);
         assert(long(A) == -7230653);
         conversions::test_incorrect_conversion<unsigned long>(A);
         assert((long long)(A) == -7230653);
         conversions::test_incorrect_conversion<unsigned long long>(A);
         assert((long double)(A) == -7230653);
         assert(double(A) == -7230653);
         assert(float(A) == -7230653);
         
         cout << "\n    END of                          test_index_operator:\n";
      }
   }
#endif
   
namespace constructor_test {
   template <typename Container, const unsigned int SIZE>
   void test_incorrect_construct(const string & exception_info, const Container & ARRAY, const char signum) {
      const string TYPE = typeid(Container).name();
      if (TYPE.find("array") == string::npos)
         throw invalid_argument(string(__func__) + " only std::array is acceptable argument");
      try {
            Integer::create_Integer<SIZE>(ARRAY, signum);
            assert(false);
      } catch (const invalid_argument & e) {
         cerr << __func__ << " Exception while create_Integer integer '" << exception_info << "' : " << e.what() << "\n";
      }
   }
   
   void test_incorrect_construct(const string & exception_info, const vector<digit_type> & vec, const char signum) {
      try {
         Integer(vec, signum);
         assert(false);
      } catch (const invalid_argument & e) {
         cerr << __func__ << " Exception while construct integer '" << exception_info << "' : " << e.what() << "\n";
      }
   }
   
   template <typename Container, const unsigned int SIZE>
   void array_to_vector_fail_construct(const string & exception_info, const Container & ARRAY, const char signum) {
      static_assert( SIZE <= Integer::MAX_ARRAY_LENGTH && SIZE > 0 && "create_Integer requires 0 < SIZE <= MAX_ARRAY_LENGTH");
      const string TYPE = typeid(Container).name();
      if (TYPE.find("array") == string::npos)
         throw invalid_argument(string(__func__) + " only std::array is acceptable argument");
      vector<digit_type> vec = { ARRAY.begin(), ARRAY.end() };
      test_incorrect_construct(exception_info, vec, signum);
   }
   
   void test_constructors() {
      cout << "\n CORRECT CONSTRUCT OF OBJECTS:\n";
      Integer first  = {};
      cout << "After construct by default constructor: " << static_cast<string>(first) << "\n";
      assert(static_cast<string>(first) == ("0"));
      assert(char(first) == 0);
      //test_incorrect_conversion<int>(first);

      array<digit_type, 5> numbers = {1, 2, 3, 0, 6};
      char signum = Integer::PLUS;
      Integer second  = Integer::create_Integer<5>(numbers, signum);
      cout << "After construct by array and signum's byte of '" << signum << "' : " << second << "\n";
      assert(static_cast<string>(second)==("+12306"));
      assert(short(second) == 12306);
      conversions::test_incorrect_conversion<char>(second);

      Integer third  = second;
      cout << "After construct by other integer: " << static_cast<string>(third) << "\n";
      assert(static_cast<string>(third)==("+12306"));
      assert((unsigned short)third == 12306);
      conversions::test_incorrect_conversion<int_fast8_t>(third);
      //test_incorrect_conversion<int>(third);
      
      cout << "\n INCORRECT ATTEMPTS TO CONSTRUCT OF OBJECTS:\n";

      vector<digit_type> too_large_vector_size(Integer::MAX_ARRAY_LENGTH + 3);
      assert(too_large_vector_size.size() == Integer::MAX_ARRAY_LENGTH + 3);
      test_incorrect_construct("with too large vector's size", too_large_vector_size, Integer::NEUTRAL);

      array<digit_type, 5> non_digits_array = {11, 2, 5, 0, 8};
      test_incorrect_construct<array<digit_type, 5>, 5>("with non digit element in array", non_digits_array, Integer::NEUTRAL);
      array_to_vector_fail_construct<array<digit_type, 5>, 5>("with non digit element in vector", non_digits_array, Integer::NEUTRAL);
      try {
         array_to_vector_fail_construct<vector<digit_type>, 5>("with non digit element in vector", too_large_vector_size, Integer::NEUTRAL);
      } catch (const invalid_argument & e) {
         cerr << __func__ << " : " << e.what() << "\n";
      }
      
      array<digit_type, 5> minus_array = {1, -2, 5, 0, 8};
      test_incorrect_construct<array<digit_type, 5>, 5>("with minus digit element in array", minus_array, Integer::NEUTRAL);
      array_to_vector_fail_construct<array<digit_type, 5>, 5>("with minus digit element in vector", minus_array, Integer::NEUTRAL);

      array<digit_type, 5> zero_array = {0, 0, 0, 0, 0};
      signum = Integer::PLUS;
      test_incorrect_construct<array<digit_type, 5>, 5>("with incorrect signum " + string(1, signum) + " for zero array", zero_array, signum);
      array_to_vector_fail_construct<array<digit_type, 5>, 5>("with incorrect signum " + string(1, signum) + " for zero vector", zero_array, signum);
      signum = Integer::MINUS;
      test_incorrect_construct<array<digit_type, 5>, 5>("with incorrect signum " + string(1, signum) + " for zero array", zero_array, signum);
      array_to_vector_fail_construct<array<digit_type, 5>, 5>("with incorrect signum " + string(1, signum) + " for zero vector", zero_array, signum);

      array<digit_type, 5> non_zero_array = {0, 0, 0, 0, 1};
      signum = Integer::NEUTRAL;
      test_incorrect_construct<array<digit_type, 5>, 5>("with incorrect signum '" + string(1, signum) + "' for non-zero array", non_zero_array, signum);
      array_to_vector_fail_construct<array<digit_type, 5>, 5>("with incorrect signum '" + string(1, signum) + "' for non-zero vector", non_zero_array, signum);

      cout << "\n ------------------------\n";
   }
}

constexpr size_t DIGIT_SIZE = sizeof(digit_type);
template <const unsigned int N>
Integer create_construct(const char SIGNUM, const digit_type * TABLE) {
   static_assert( N <= Integer::MAX_ARRAY_LENGTH && N > 0 && "create_construct requires 0 < N <= MAX_ARRAY_LENGTH");

   const vector<digit_type> VEC = { TABLE, TABLE + N };
   Integer integer_vec = {VEC, SIGNUM};
   cerr << "integer construct by vector: " << integer_vec;
   
   array<digit_type, N> ARRAY;
   static constexpr size_t COPY_SIZE =  N * DIGIT_SIZE;
   memcpy(ARRAY.data(), TABLE, COPY_SIZE);
   Integer integer_array = Integer::create_Integer<N>(ARRAY, SIGNUM);
   cerr << "\ninteger after construct by array: " << integer_array << '\n';
   
   assert(integer_array == integer_vec);
   return integer_vec;
}

template <const unsigned int N>
void set(const digit_type TABLE[], Integer & integer) {
   static_assert( N <= Integer::MAX_ARRAY_LENGTH && N > 0 && "set requires 0 < N <= MAX_ARRAY_LENGTH");
   validate_pointer(TABLE);

   const vector<digit_type> VEC = { TABLE, TABLE + N };
   integer.set_integer_array(VEC);
   cerr << " integer after set by vector: " << integer;
   const string integer_string = static_cast<string> (integer);
   
   array<digit_type, N> ARRAY;
   copy(TABLE, TABLE + N, ARRAY.begin());
   integer.set_integer_array<N>(ARRAY);
   cerr << "\ninteger after set by array: " << integer << '\n';
   assert(integer_string == static_cast<string> (integer));
}

void set_random_values(array<digit_type, Integer::MAX_ARRAY_LENGTH> & modified) {
   random_device rd;
   uniform_int_distribution<digit_type> dist(0, 9);
   for (int_fast8_t n = 0; n < Integer::MAX_ARRAY_LENGTH; ++n)
      modified[n] = dist(rd);
}

void test_get_copy_array(Integer & integer) {
   cout << " \nTEST of GETTING INTEGER ARRAY COPY\n";
   array<digit_type, Integer::MAX_ARRAY_LENGTH> copy = integer.get_integer_array_copy();
   print_container("Copy of integer array:", copy);
   set_random_values(copy);
   print_container("Copy of integer array after set_random_values:", copy);
   print_container("integer array:", integer.get_integer_array_copy());
   integer.set_integer_array<Integer::MAX_ARRAY_LENGTH>(copy);
   print_container("integer array after set modified previous copy:", integer.get_integer_array_copy());
   assert(copy == integer.get_integer_array_copy());
}

}

namespace setters_test {
   using namespace integer_test;
   void test_set_array(const string & integer_name, Integer integer) {
      cout << "\n TESTS OF SET ARRAY OR SIGNUM :\n";
      constexpr digit_type array1 [] = {4, 9};
      set<2>(array1, integer);
      cout << integer_name << " integer after set other table: " << integer << '\n';
      assert_Integer(integer, "49");

      constexpr digit_type array2[] = {4, 2, 3};
      set<3>(array2, integer);
      cout << integer_name << " integer after set other table: " << integer << '\n';
      assert_Integer(integer, "423");

      constexpr digit_type  array3[] = {4};
      set<1>(array3, integer);
      cout << integer_name << " integer after set other table: " << integer << '\n';
      assert_Integer(integer, "4");

      constexpr digit_type  array4[] = {0};
      set<1>(array4, integer);
      cout << integer_name << " integer after set other table: " << integer << '\n';
      assert_Integer(integer, "0");

      char signum = Integer::PLUS;
      try {
         integer.set_signum(signum);
         cerr << "??? " << integer_name << " integer after set signum to '" << signum << "' : " << integer;
         assert(false);
      } catch (const invalid_argument & e) {
         cerr << "\nException while set signum to '" << signum << "' : " << e.what() << "\n";
      }

      constexpr digit_type  array5[] = {3, 5, 7};
      set<3>(array5, integer);
      cout << integer_name << " integer after set other table: " << integer << '\n';
      assert("+357" == static_cast<string> (integer));

      signum = Integer::PLUS;
      integer.set_signum(signum);
      cout <<  integer_name << " integer after set signum to '" << signum << "' : " << integer << '\n';
      assert("+357" == static_cast<string> (integer));

      signum = Integer::MINUS;
      integer.set_signum(signum);
      cout <<  integer_name << " integer after set signum to '" << signum << "' : " << integer << '\n';
      assert("-357" == static_cast<string> (integer));
      
      signum = Integer::NEUTRAL;
      try {
         integer.set_signum(signum);
         cerr << "??? " << integer_name << " integer after set signum to '" << signum << "' : " << integer << '\n';
         assert(false);
      } catch (const invalid_argument & e) {
         cerr << "\nException while set signum to '" << signum << "' : " << e.what() << "\n";
         assert("-357" == static_cast<string> (integer));
      }

      set<1>(array4, integer);
      cout << integer_name << " integer after set other table: " << integer << '\n';
      assert_Integer(integer, "0");
      signum = Integer::NEUTRAL;
      integer.set_signum(signum);
      cout <<  integer_name << " integer after set signum to '" << signum << "' : " << integer << '\n';
      integer_assert(integer, Integer::NEUTRAL, "0");

      cout << "\n ------------------------\n";
   }
}

namespace comparing_test {
   
   extern "C" bool is_zero_C (const Integer * const integer) {
      return Integer::is_zero(*integer);
   }
   
   inline string bool_string_C(bool (* function) (const Integer * const ), const Integer& integer) {
      return function ( &integer) ? " TRUE " : " FALSE ";
   }
   
   inline string bool_string(bool (* function) (const Integer& ), const Integer& integer) {
      return function (integer) ? " TRUE " : " FALSE ";
   }
   inline string bool_string(bool (* function) (const Integer&, const Integer& ), const Integer& first, const Integer& second) {
      return function (first, second) ? " TRUE " : " FALSE ";
   }
   using namespace integer_test;
   
   void compare(const Integer &first, const Integer &second) {
      cout << "\n COMPARE OF INTEGERS:\n";
      cout << first << "\n";
      cout << second << "\n";

      cout << first << " is zero: " << bool_string(Integer::is_zero, first) << " \n";
      cout << second << " is zero: " << bool_string(Integer::is_zero, second) << " \n";
      
      cout << first << " is zero (C): " << bool_string_C(is_zero_C, first) << " \n";
      cout << second << "C is zero (C): " << bool_string_C(is_zero_C, second) << " \n";

      cout << first << " == " << second << " : " << bool_string(operator==, first, second) << " \n";
      cout << first << " != " << second << " : " << bool_string(operator!=, first, second) << " \n";
      cout << first << " > " << second << " : " << bool_string(operator>, first, second) << " \n";
      cout << first << " <= " << second << " : " << bool_string(operator<=, first, second) << " \n";
      cout << first << " < " << second << " : " << bool_string(operator<, first, second) << " \n";
      cout << first << " >= " << second << " : " << bool_string(operator>=, first, second) << " \n";
   }

   void assert_comparing_different_signum(Integer & first, Integer & second) {
      Parsing_Test::parse_string("first", first, "554");
      assert(static_cast<string> (first) == ("+554"));

      Parsing_Test::parse_string("second", second, "-8890");
      assert(static_cast<string> (second) == ("-8890"));

      compare(first, second);
      assert(first.is_zero() == false);
      assert(second.is_zero() == false);

      assert((first == second) == false);
      assert((first != second) == true);

      assert((first > second) == true);
      assert((first <= second) == false);

      assert((first <  second) == false);
      assert((first >=  second) == true);

      first = -first;
      assert(static_cast<string> (first) == ("-554"));

      second = -second;
      assert(static_cast<string> (second) == ("+8890"));

      compare(first, second);
      assert(first.is_zero() == false);
      assert(second.is_zero() == false);

      assert((first == second) == false);
      assert((first != second) == true);

      assert((first > second) == false);
      assert((first <= second) == true);

      assert((first <  second) == true);
      assert((first >=  second) == false);
   }

   void assert_comparing_different_signumZero(Integer & first, Integer & second) {
      Parsing_Test::parse_string("first", first, "+7");
      assert(static_cast<string> (first) == ("+7"));

      Parsing_Test::parse_string("second", second, "0");
      assert(static_cast<string> (second) == ("0"));

      compare(first, second);
      assert(first.is_zero() == false);
      assert(second.is_zero() == true);

      assert((first == second) == false);
      assert((first != second) == true);

      assert((first > second) == true);
      assert((first <= second) == false);

      assert((first <  second) == false);
      assert((first >=  second) == true);

      first = -first;
      assert(static_cast<string> (first) == ("-7"));

      second = - second;
      assert(static_cast<string> (second) == ("0"));

      compare(first, second);
      assert(first.is_zero() == false);
      assert(second.is_zero() == true);

      assert((first == second) == false);
      assert((first != second) == true);

      assert((first > second) == false);
      assert((first <= second) == true);

      assert((first <  second) == true);
      assert((first >=  second) == false);
   }

   void assert_comparing_identical_signum(Integer & first, Integer & second) {
      Parsing_Test::parse_string("first", first, "99787");
      assert(static_cast<string> (first) == ("+99787"));

      Parsing_Test::parse_string("second", second, "+87");
      assert(static_cast<string> (second) == ("+87"));

      compare(first, second);
      assert(first.is_zero() == false);
      assert(second.is_zero() == false);

      assert((first == second) == false);
      assert((first != second) == true);

      assert((first > second) == true);
      assert((first <= second) == false);

      assert((first <  second) == false);
      assert((first >=  second) == true);

      first = -first;
      assert(static_cast<string> (first) == ("-99787"));

      second = -second;
      assert(static_cast<string> (second) == ("-87"));

      compare(first, second);
      assert(first.is_zero() == false);
      assert(second.is_zero() == false);

      assert((first == second) == false);
      assert((first != second) == true);

      assert((first > second) == false);
      assert((first <= second) == true);

      assert((first <  second) == true);
      assert((first >=  second) == false);
   }
}

using namespace integer_test;
using namespace integer_test::conversions;

using Array_MAX = array<digit_type, Integer::MAX_ARRAY_LENGTH>; 

int main() {
   try {
      constructor_test::test_constructors();
      constexpr digit_type NUMBERS[] = {0, 2, 3, 0, 6};
      char signum = Integer::MINUS;
      Integer first  = create_construct<5>(signum, NUMBERS);
      cout << "First integer after construct by table and signum's '" << signum << "' : " << first << '\n';
      assert(static_cast<string>(first) == ("-2306"));

      constexpr digit_type TABLE[] = {4, 2, 3, 0, 7, 4};
      set<6>(TABLE, first);
      cout << "First integer after set other table: " << first << '\n';
      assert(static_cast<string>(first) == ("-423074"));
      
      try {
         set<6>(nullptr, first);
         cout << "??? First integer after set minus_number_array: " << first << '\n';
         assert(false);
      } catch (const invalid_argument & e) {
         cerr << __func__ <<  " : Exception while set null_array: " << e.what() << "\n";
         cout << "First integer after exception catch: " << first << '\n';
         assert(static_cast<string>(first) == ("-423074"));
      }
      test_get_copy_array(first);
      try {
         constexpr digit_type minus_number_array[] = {4, -2, 3, 0, 7, 4};
         set<6>(minus_number_array, first);
         cout << "??? First integer after set minus_number_array: " << first << '\n';
         assert(false);
      } catch (const invalid_argument & e) {
         cerr << __func__ <<  " : Exception while set minus_number_array: " << e.what() << "\n";
         cout << "First integer after exception catch: " << first << '\n';
         assert(static_cast<string>(first) == ("0"));
      }
      
      Array_MAX first_integer_array_copy = first.get_integer_array_copy();
      print_container("Copy of first integer array:", first_integer_array_copy);
      first_integer_array_copy[14] = first_integer_array_copy[11] = first_integer_array_copy[12] = 9;
      print_container("Copy of first integer array after change:", first_integer_array_copy);
      print_container("First integer array:", first.get_integer_array_copy());
      
      setters_test::test_set_array("first", first);

      Integer second  = create_construct<5>(Integer::MINUS, NUMBERS);
      assert(static_cast<string>(second) == ("-2306"));
      setters_test::test_set_array("second", second);

      Parsing_Test::test_parsing("first", first);
      Parsing_Test::test_parsing("second", second);

      comparing_test::compare(first, second);
      assert(first.is_zero() == false);
      assert(second.is_zero() == false);

      assert((first == second) == true);
      assert((first != second) == false);

      assert((first > second) == false);
      assert((first <= second) == true);

      assert((first <  second) == false);
      assert((first >=  second) == true);

      comparing_test::assert_comparing_different_signum(first, second);
      comparing_test::assert_comparing_identical_signum(first, second);
      comparing_test::assert_comparing_different_signumZero(first, second);
      
#ifdef TEST_INDEX_OPERATOR
      integer_test::index_operator::test_index_operator();
#endif
      test_conversions();
      test_operator_number();
      
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
