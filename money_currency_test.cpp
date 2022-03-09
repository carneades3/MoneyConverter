
#include "money_currency_test.hpp"
#include "money_utility_test.hpp"
#include "integers_extremums_tests.hpp"
//#include "money_init_test.hpp"
#include "money_init_test.hpp"

#include <thread>

using namespace std;
using namespace integer_space;
using namespace money;
using namespace money_init_test;


/*
template <typename T, typename Function, typename... Args>  
extern
void money_init_test::construct_incorrect(Function && f, Args&&... args );
*/

namespace money_init_test {

template <typename T> 
extern
Money<T> construct_cents(const string & DOLLARS, const long double CENTS, bool creating, const string & expected/* = ""*/, const string & currency/* = "PLN"*/, const bool NOT_assert_currency/* = true*/);

template <typename T> 
//extern
Money<T> construct(const string & DOLLARS, bool creating, const string & expected/* = ""*/, const string & currency/* = "PLN"*/, const bool NOT_assert_currency/* = true*/);

template <typename T, typename Function, typename... Args>  
//extern
void construct_incorrect(Function && f, Args&&... args );

}
/*
namespace money_currency_test {

template <typename T>
class Currency_test {
public:
   static void construct() {
      cerr << "\start of Currency_test::" << __func__ << '\n';
      money_init_test::construct_cents<T>("0", 99, true, "0,99 USD", "USD", false);
      money_init_test::construct_cents<T>("1", 1, true, "1,01 EUR", "EUR", false);
      money_init_test::construct_cents<T>("1", 0, true, "1,00 PLN", "PLN", false);
      money_init_test::construct_cents<T>("0", 99.9, false, "1,00 GBP", "GBP", false);
      money_init_test::construct_cents<T>("-0", 0, true, "0,00 CHF", "CHF", false);
      money_init_test::construct_cents<T>("0", 0, true, "0,00 JPY", "JPY", false);
      money_init_test::construct<T>("0.99999", false, "1,00 CAD", "CAD", false);
      money_init_test::construct<T>("-0", true, "0,00 JPY", "JPY", false);
      money_init_test::construct<T>("0.5", true, "0,50 JPY", "JPY", false);
      money_init_test::construct<T>("0.50", true, "0,50 JPY", "JPY", false);
      money_init_test::construct<T>("0.500", true, "0,50 JPY", "JPY", false);
      money_init_test::construct_cents<T>("1", 2.5, false, "1,03 GBP", "GBP", false);
      money_init_test::construct_cents<T>("0", 2.4, false, "0,02 GBP", "GBP", false);
      cerr << "\nEND of Currency_test::" << __func__ << '\n';
   }
};
}
*/

namespace money_currency_test {
   
#define EQUALITY(money_A, money_B) \
   assert(static_cast<string>(money_A) == static_cast<string>(money_B)); \
   assert(money_A.get_amount_in_cents() == money_B.get_amount_in_cents());
   
template <typename T>
class Currency_test {
   static const string TYPE_NAME;

   static void add() {
      cerr << "\nstart of money_currency_test::" << __func__ << '\n';
      Money<T> A("0", 20, "USD");
      Money<T> B("0.5", "EUR");
      Money<T> result = A + B;
      cerr << A << " + " << B << " = " << result << '\n';
      A += B;
      cerr << A << " += " << B << " == " << result << '\n';
      assert(static_cast<string>(A) == static_cast<string>(result));
      assert(A.get_amount_in_cents() == result.get_amount_in_cents());
      cerr << "\nEND of money_currency_test::" << __func__ << '\n';
   }

   static void subtract() {
      cerr << "\nstart of money_currency_test::" << __func__ << '\n';
      Money<T> A("1.2", "GBP");
      Money<T> B("0", 66, "CHF");
      Money<T> result = A - B;
      cerr << A << " - " << B << " = " << result << '\n';
      A -= B;
      cerr << A << " -= " << B << " == " << result << '\n';
      assert(static_cast<string>(A) == static_cast<string>(result));
      assert(A.get_amount_in_cents() == result.get_amount_in_cents());
      cerr << "\nEND of money_currency_test::" << __func__ << '\n';
   }

   static void multiply() {
      cerr << "\nstart of money_currency_test::" << __func__ << '\n';
      Money<T> A("0", 2, "JPY");
      const T FACTOR = T(2);
      Money<T> result = A * FACTOR;
      cerr << A << " * " << FACTOR << " = " << result << '\n';
      A *= FACTOR;
      cerr << A << " *= " << FACTOR << " == " << result << '\n';
      assert(static_cast<string>(A) == static_cast<string>(result));
      assert(A.get_amount_in_cents() == result.get_amount_in_cents());
      cerr << "\nEND of money_currency_test::" << __func__ << '\n';
   }

#undef EQUALITY

   static void operate() {
      add();
      subtract();
      multiply();
   }
   
   static void construct() {
      cerr << "\nstart of money_currency_test::" << __func__ << '\n';
      money_init_test::construct_cents<T>("0", 99, true, "0,99 USD", "USD", false);
      money_init_test::construct_cents<T>("1", 1, true, "1,01 EUR", "EUR", false);
      money_init_test::construct_cents<T>("1", 0, true, "1,00 PLN", "PLN", false);
      money_init_test::construct_cents<T>("0", 99.9, false, "1,00 GBP", "GBP", false);
      money_init_test::construct_cents<T>("-0", 0, true, "0,00 CHF", "CHF", false);
      money_init_test::construct_cents<T>("0", 0, true, "0,00 JPY", "JPY", false);
      money_init_test::construct<T>("0.99999", false, "1,00 CAD", "CAD", false);
      money_init_test::construct<T>("-0", true, "0,00 JPY", "JPY", false);
      money_init_test::construct<T>("0.5", true, "0,50 JPY", "JPY", false);
      money_init_test::construct<T>("0.50", true, "0,50 JPY", "JPY", false);
      money_init_test::construct<T>("0.500", true, "0,50 JPY", "JPY", false);
      money_init_test::construct_cents<T>("1", 2.5, false, "1,03 GBP", "GBP", false);
      money_init_test::construct_cents<T>("0", 2.4, false, "0,02 GBP", "GBP", false);
      cerr << "\nEND of money_currency_test::" << __func__ << '\n';
   }

   static void incorrect_construct() {
      cerr << "\nstart of money_currency_test::" << __func__ << '\n';
      money_init_test::construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", "VVV");
      money_init_test::construct_incorrect<Money<T>>(Constructor<T, Money>(), "20", 5.0L, "WWW");
      cerr << "\nEND of money_currency_test::" << __func__ << '\n';
   }

   static void test_input() {
      Money<T> money ( std::to_string (T{}), "PLN" );
      assert(static_cast<string>(money) == "0,00 PLN");
      assert(static_cast<int>(money.get_amount_in_cents()) == 0);
      cout << "\nEnter money in example formats: USD5 (USD is currency name and 5 is value), EUR8.78\n";
      cout << "Enter value of money as type of " + TYPE_NAME + ": ";
      cin >> money;
      cout << "\nEntered money is " << money << '\n';
      std::this_thread::sleep_for(1000ms);
   }
   
   inline static void replace_char(string & S, const char remove, const char insert) {
      const size_t pos = S.find(remove);
      if (pos != string::npos)
         S.replace(pos, 1, string(1, insert));
   }
   
   inline static void remove_char(string & S, const char remove) {
      const size_t pos = S.find(remove);
      if (pos != string::npos)
         S.erase(pos, 1);
   }
   
   template<typename U = T, enable_if_t<is_same<U, char>::value || is_same<U, int_fast8_t>::value, bool>  = true>
   static inline T output_amount(const string & STR) {
      T result = static_cast<T>(from_string<int>(STR, true)); 
      return result;
   }
   
   template<typename U = T, enable_if_t<! is_same<U, char>::value && ! is_same<U, int_fast8_t>::value, bool>  = true>
   static inline T output_amount(const string & STR) {
      return from_string<T>(STR, true); 
   }
   
   static pair<string, T> input_to_output(const string & STR) {
      constexpr size_t CURRENCY_LENGTH = 3;
      if (STR.size() <= CURRENCY_LENGTH)
         throw invalid_argument("Error: money string must be more than " + std::to_string(CURRENCY_LENGTH) + " chars");
      const string currency = STR.substr(0, CURRENCY_LENGTH);
      string amount = STR.substr(CURRENCY_LENGTH);
      string expected_string = amount + " " + currency;
      replace_char(expected_string, '.', ',');
      remove_char(amount, '.');
      T expected_amount = output_amount(amount);
      return { expected_string, expected_amount};
   }

   static void test_istringstream(const string & STR, Money<T> & money) {
      istringstream stream(STR);
      stream >> money;
      cerr << "\nEntered money (number type: " << TYPE_NAME << " ) is " << money << '\n';
      std::this_thread::sleep_for(1000ms);
      const pair<string, T> output = input_to_output(STR);
      assert(static_cast<string>(money) == output.first);
      cerr << "\nmoney.get_amount_in_cents() " << output.second << '\n';
      assert(money.get_amount_in_cents() == output.second);
   }
   
   static const array<string, 5>  INPUT;
   static void test_istringstream() {
      cerr << __func__ << " TEST OF " << TYPE_NAME << '\n';
      Money<T> money ( std::to_string (T{}), "PLN" );
      assert(static_cast<string>(money) == "0,00 PLN");
      assert(static_cast<int>(money.get_amount_in_cents()) == 0);
      for (string m : INPUT)
         if (numeric_limits<T>::is_signed || (! numeric_limits<T>::is_signed && m.find('-') == string::npos))
            test_istringstream(m, money);
   }
public:
   static void perform() {
      //////Currency_test<T>::construct();
      construct();
      incorrect_construct();
      operate();
      test_istringstream();
      //test_input();
   }
};

template <typename T>
const string Currency_test<T>::TYPE_NAME = typeid(T).name();

template <typename T>
const array<string, 5> Currency_test<T>::INPUT = { "CHF0.23", "CHF-0.23", "PLN0.00", "PLN1.23", "GBP-1.04" };

void perform() {
   Currency_test<Integer>::perform();
   Currency_test<short>::perform();
   Currency_test<char>::perform();
   Currency_test<int_fast8_t>::perform();
   Currency_test<unsigned short>::perform();
   Currency_test<int>::perform();
   Currency_test<unsigned>::perform();
   Currency_test<long>::perform();
   Currency_test<unsigned long>::perform();
   Currency_test<long long>::perform();
   Currency_test<unsigned long long>::perform();
   Currency_test<float>::perform();
   Currency_test<double>::perform();
   Currency_test<long double>::perform();
   
   cerr << '\n' << __func__ << ": END of ALL TESTS\n";
}

}
