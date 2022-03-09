#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>
#include <climits>
#include <cmath>

using namespace std;
using namespace integer_space;

namespace main_integer_test {

template <const size_t SIZE>
static Integer set(const array<digit_type, SIZE> & table, Integer & integer) {
   cout << __func__ << " tests set_integer_array <" << SIZE <<  "> \n";
   integer.set_integer_array<SIZE>(table);
   cout << integer << '\n';
   return integer;
}

static Integer set(const vector<digit_type> & vec, Integer & integer) {
   cout << __func__ << " tests set_integer_array <vector> \n";
   integer.set_integer_array(vec);
   cout << integer << '\n';
   return integer;
}

template <typename T,
              std::enable_if_t<std::is_integral<T>::value, bool> = true>
inline Integer construct_Integer (const T& NUMBER) { 
   cerr << __func__ << " construct by Integral " << NUMBER << " : ";
   Integer i = Integer::create_Integer(NUMBER);
   cout << i << '\n';
   assert_number_Integer(NUMBER, i);
   return i;
}

template <typename Floating,
              std::enable_if_t<std::is_floating_point<Floating>::value, bool> = true>
inline Integer construct_Integer (const Floating& NUMBER) { 
   cerr << __func__ << " construct by Float " << NUMBER << " : ";
   Integer i = Integer::create_Integer(NUMBER);
   cout << i << '\n';
   assert_number_Integer(NUMBER, i);
   return i;
}

static Integer constructor_test(const array<digit_type, Integer::MAX_ARRAY_LENGTH> & table, const char signum) {
   cout << __func__ << " tests construct Integer by array<digit_type, Integer::MAX_ARRAY_LENGTH> \n";
   Integer i = Integer(table, signum);
   cout << i << '\n';
   return i;
}

template <const unsigned int N>
static Integer creation_test(const array<digit_type, N> & table, const char signum) {
   cout << __func__ << " tests Integer::create_Integer <" << N <<  "> \n";
   Integer i =  Integer::create_Integer<N>(table, signum);
   cout << i << '\n';
   return i;
}

static Integer constructor_test(const vector<digit_type> & vec, const char signum) {
   cout << __func__ << " tests construct Integer by <vector> \n";
   Integer i (vec, signum);
   cout << i << '\n';
   return i;
}

static Integer default_constructor_test() {
   cout << __func__ << " tests construct Integer by default constructor \n";
   Integer i ;
   cout << i << '\n';
   assert( static_cast<string>(i) == "0");
   return i;
}

void set_integer_array_test(Integer & integer) {
   cout << __func__ << "\n";
   char sign = integer.get_signum();
   
   //if (Integer::NEUTRAL == sign)
   //   sign = Integer::PLUS;
   constexpr array<digit_type, 5> a = { 7, 2, 7, 0};
   set(a, integer);
   integer_assert(integer, sign, "72700");
   
   constexpr array<digit_type, Integer::MAX_ARRAY_LENGTH> i40 = { 1, 2, 5};
   set(i40, integer);
   integer_assert(integer, sign, "1250000000000000000000000000000000000000");
   
   vector<digit_type> vec(Integer::MAX_ARRAY_LENGTH);
   set(vec, integer);
   integer_assert(integer, Integer::NEUTRAL, "0");
   
   vec = { 0, 4 , 5};
   //constexpr array<digit_type, Integer::MAX_ARRAY_LENGTH> nul  = reinterpret_cast<array<digit_type, Integer::MAX_ARRAY_LENGTH>>(nullptr);
   set(vec, integer);
   integer_assert(integer, Integer::PLUS, "45");
   
   vec = { 0 };
   set(vec, integer);
   integer_assert(integer, Integer::NEUTRAL, "0");
}

void fail_set_integer_array_test(Integer & integer) {
   const string STR = static_cast<string>(integer);
   try {
      cout << __func__ << " tests FAIL of set_integer_array <vector> \n";
      vector<digit_type> vec = { 0, 4 , 5};
      vec.resize(45, 9);
      set(vec, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      assert( static_cast<string>(integer) == STR);
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   
   try {
      constexpr array<digit_type, 2> integer_array = { 55, 0};
      cout << __func__ << " tests FAIL of set_integer_array <" << integer_array.size() <<  "> \n";
      set(integer_array, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      assert( static_cast<string>(integer) == "0");
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   
   try {
      constexpr array<digit_type, 2> integer_array = { -5, 0};
      cout << __func__ << " tests FAIL of set_integer_array <" << integer_array.size() <<  "> \n";
      set(integer_array, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      assert( static_cast<string>(integer) == "0");
      cerr << __func__ << " exception: " << e.what() << endl;
   }
   
   try {
      const vector<digit_type> vec = { };
      set(vec, integer);
      cout << __func__ << " tests FAIL of set_integer_array <vector> \n";
      set(vec, integer);
      assert(false);
   }
   catch (const invalid_argument& e) {
      assert( static_cast<string>(integer) == "0");
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

static void incorrect_parsing_test(const string & STR) {
   try {
      cout << __func__ << " tests FAIL of Integer::parse_create by: '" << STR << "' \n";
      Integer::parse_create(STR);
      assert(false);
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " exception: " << e.what() << endl;
   }
}

inline void test_set_and_fail_set(Integer & integer) {
   set_integer_array_test(integer);
   fail_set_integer_array_test(integer);
}

static Integer parsing_test() {
   cout << __func__ << " tests Integer::parse_create \n";
   Integer integer = Integer::parse_create("999");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("+999"));
   
   integer = Integer::parse_create("-78999");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("-78999"));
   
   integer = Integer::parse_create("0");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("0"));
   
   integer = Integer::parse_create("7");
   cout << integer << '\n'; 
   assert(static_cast<string>(integer) == ("+7"));
   
   incorrect_parsing_test("888888888888888888888888888888888888888888888888888888888888888");
   incorrect_parsing_test("-99999eee");
   incorrect_parsing_test("-99999edf");
   incorrect_parsing_test("99999 ");
   incorrect_parsing_test("t");
   incorrect_parsing_test(" 5");
   
   assert(static_cast<string>(integer) == ("+7"));
   return integer;
}

void constructor_test() {
   default_constructor_test();
   constexpr array<digit_type, Integer::MAX_ARRAY_LENGTH> integer_array = { 7, 2};
   Integer ia = constructor_test(integer_array, Integer::PLUS);
   assert(string(ia) == "+7200000000000000000000000000000000000000");
   test_set_and_fail_set(ia);
   
   vector<digit_type> vec = { 7, 5, 8, 0};
   Integer iv = constructor_test(vec, Integer::PLUS);
   assert(string(iv) == "+7580");
   test_set_and_fail_set(iv);
   
   constexpr array<digit_type, 1> a2 = { 2 };
   Integer i2 = creation_test<1>(a2, Integer::MINUS);
   assert(string(i2) == "-2");
   test_set_and_fail_set(i2);
   
   parsing_test();
   Parsing_Test::test_parsing("i2", i2);
   /*
   Integer id(3.4);
   assert(string(id) == "+3");
   id = 6;
   assert(string(id) == "+6");
   */
   construct_Integer(3.4);
   construct_Integer(+6.4);
   construct_Integer(63.9);
   construct_Integer(-7.1);
   construct_Integer(0.);
   construct_Integer(1.0);
   construct_Integer(.7);
   
   construct_Integer(39);
   construct_Integer(+69);
   construct_Integer(-69);
   construct_Integer(0);
}

Integer& move_test (Integer&& param) {
   static const vector<digit_type> VEC = { 1, 1, 1};
   param = Integer {VEC, Integer::PLUS};
   cout << __func__ << " after param = Integer {VEC, Integer::PLUS} = " << param << '\n';
   assert(static_cast<string>(param) == ("+111"));
   return param;
}

Integer& move_test_void (Integer&& param) {
   return param;
}

void assignment_test() {
   static constexpr array<digit_type, Integer::MAX_ARRAY_LENGTH> a1 = { 7, 2, 8, 5};
   static constexpr array<digit_type, Integer::MAX_ARRAY_LENGTH> a2 = a1;
   assert(a1 == a2);
   assert(&a1 != &a2);
   const vector<digit_type> v1 = { 7, 2, 8, 5};
   const vector<digit_type> v2 = v1;
   assert(v1 == v2);
   assert(&v1 != &v2);
   Integer i1 = { v1, Integer::MINUS };
   cout << " i1 = " << i1 << '\n';
   assert(static_cast<string>(i1) == ("-7285"));
   Integer i2 = i1;
   cout << " after i2 = i1 => i1 = " << i1 << '\n';
   cout << " after i2 = i1 => i2 = " << i2 << '\n';
   assert(static_cast<string>(i1) == ("-7285"));
   assert(static_cast<string>(i2) == ("-7285"));
   Integer i3(i2);
   cout << " after i3(i2) => i1 = " << i1 << '\n';
   cout << " after i3(i2) => i2 = " << i2 << '\n';
   cout << " after i3(i2) => i3 = " << i3 << '\n';
   assert(static_cast<string>(i1) == ("-7285"));
   assert(static_cast<string>(i2) == ("-7285"));
   assert(static_cast<string>(i3) == ("-7285"));
   
   move_test_void({vector<digit_type> { 4, 7, 9, 2}, Integer::MINUS });
   cout << " after call move_test_void({ vector<digit_type>{ 4, 7, 9, 2}, Integer::MINUS }) => i2 = " << i2 << '\n';
   i2 = move_test_void({vector<digit_type> { 4, 7, 9, 2}, Integer::MINUS });
   cout << " after return from i2 = move_test_void({vector<digit_type>{ 4, 7, 9, 2}, Integer::MINUS }) => i2 = " << i2 << '\n';
   assert(static_cast<string>(i2) == ("-4792"));
   
   move_test({ });
   cout << " after call move_test({ }) => i3 = " << i3 << '\n';
   i3 = move_test({ });
   cout << " after return from i3 = move_test({ }) => i3 = " << i3 << '\n';
   assert(static_cast<string>(i3) == ("+111"));
}

void increment_decrement_test() {
   Integer i = default_constructor_test();
   assert(static_cast<string>(i) == ("0"));
   assert(static_cast<string>(i++) == ("0"));
   assert(static_cast<string>(i) == ("+1"));
   assert(static_cast<string>(++i) == ("+2"));
   assert(static_cast<string>(i) == ("+2"));
   assert(static_cast<string>(--i) == ("+1"));
   assert(static_cast<string>(i) == ("+1"));
   assert(static_cast<string>(i--) == ("+1"));
   assert(static_cast<string>(i) == ("0"));
   assert(static_cast<string>(--i) == ("-1"));
   assert(static_cast<string>(i) == ("-1"));
   assert(static_cast<string>(i--) == ("-1"));
   assert(static_cast<string>(i) == ("-2"));
   assert(static_cast<string>(i++) == ("-2"));
   assert(static_cast<string>(i) == ("-1"));
   assert(static_cast<string>(++i) == ("0"));
   assert(static_cast<string>(i) == ("0"));
}

void input_test() {
   Integer i;
   cout << "\nEnter Integer object: ";
   cin >> i;
   cout << "Entered Integer object is " << i << '\n';
}

}

using namespace main_integer_test;

int main() {
   try {
      constructor_test();
      assignment_test();
      increment_decrement_test();
      cerr << " numeric_limits<Integer>::max() = " << numeric_limits<Integer>::max() << '\n';
      cerr << " numeric_limits<Integer>::min() = " << numeric_limits<Integer>::min() << '\n';
      printf("trunc(NAN) = %+f    isfinite(NAN) = %d\n",   trunc(NAN), isfinite(NAN));
      input_test();
      cerr << " 5 / 0 = " << Integer::create_Integer(5) / Integer::ZERO << '\n';
      
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
