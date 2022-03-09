#define DEBUG_OSTREAM

#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

#include <cmath>

#include "integers_extremums_tests.hpp"
#include "money_utility_test.hpp"
#include "money_init_test.hpp"

using namespace std;
using namespace integer_space;
using namespace money;

namespace money_init_test {

template <typename T> 
Money<T> construct_cents(const string & DOLLARS, const long double CENTS, bool creating, const string & expected = "", const string & currency = "PLN", const bool NOT_assert_currency = true) { 
   Money<T> money(DOLLARS, CENTS, currency);
   print_assert(money, expected, NOT_assert_currency);
   if (creating) {
      Money<T> money_creating = Money<T>::create(DOLLARS, CENTS, currency);
      print_assert(money_creating, expected, NOT_assert_currency);
      assert(money == money_creating);
   }
   else
      construct_incorrect<Money<T>>(Creation<T>(), DOLLARS, CENTS, currency);
   return money;
}


template <typename T> 
Money<T> construct(const string & DOLLARS, bool creating, const string & expected/* = ""*/, const string & currency/* = "PLN"*/, const bool NOT_assert_currency/* = true*/) { 
   Money<T> money(DOLLARS, currency);
   print_assert(money, expected, NOT_assert_currency);
   if (creating) {
      Money<T> money_creating = Money<T>::create(DOLLARS, currency);
      print_assert(money_creating, expected, NOT_assert_currency);
      assert(money == money_creating);
   }
   else
      construct_incorrect<Money<T>>(Creation<T>(), DOLLARS, currency);
   return money;
}

template <typename T> 
void incorrect() { 
   try {
      Money<T> money("6", -34);
      cerr << __func__ << "money = " << money << '\n';
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
   try {
      Money<T> money("6", 100);
      cout << __func__ << "money = " << money << '\n';
      assert(0);
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

//constexpr long double CONVERTER = (long double)(CENTS_PER_DOLLAR);

template <typename T> 
void construct() {
   cerr << __func__ << '\n';
   construct_cents<T>("0", 99, true, "0,99");
   construct_cents<T>("1", 1, true, "1,01");
   construct_cents<T>("1", 0, true, "1,00");
   construct_cents<T>("0", 99.9, false, "1,00");
   construct_cents<T>("-0", 0, true, "0,00");
   construct_cents<T>("0", 0, true, "0,00");
   construct<T>("0.99999", false, "1,00");
   construct<T>("-0", true, "0,00");
   construct<T>("0.5", true, "0,50");
   construct<T>("0.50", true, "0,50");
   construct<T>("0.500", true, "0,50");
   construct_cents<T>("1", 2.5, false, "1,03");
   construct_cents<T>("0", 2.4, false, "0,02");
   if (numeric_limits<T>::is_signed) {
      construct_cents<T>("-1", 2.5, false, "-1,03");
      construct_cents<T>("-0", 2.4, false, "-0,02");
      construct_cents<T>("-1", 0, true, "-1,00");
      construct_cents<T>("-1", 1, true, "-1,01");
      construct_cents<T>("-1", 3, true, "-1,03");
      construct_cents<T>("-0", 99.9, false, "-1,00");
      construct_cents<T>("-0", 1, true, "-0,01");
      construct_cents<T>("-0", 99, true, "-0,99");
      construct<T>("-0.99999", false, "-1,00");
      construct<T>("-0.5", true, "-0,50");
      construct<T>("-0.50", true, "-0,50");
      construct<T>("-0.500", true, "-0,50");
      construct_cents<T>("-0", 5.5, false, "-0,06");
   }
   if (! is_same<T, char>::value && ! is_same<T, int_fast8_t>::value) {
      construct<T>("1.99999", false, "2,00");
      
      construct_cents<T>("20", 55, true, "20,55");
      if (! is_same<T, short>::value)
         construct_cents<T>("577", 45.7, false, "577,46");
      if (numeric_limits<T>::is_signed) {
         if (! is_same<T, short>::value)
            construct_cents<T>("-8577", 45.79, false, "-8577,46");   
         construct<T>("-1.99999", false, "-2,00");
         construct_cents<T>("-1", 99.9, false, "-2,00");
         construct_cents<T>("-1", 99, true, "-1,99");
         
         construct<T>("-10.67", true, "-10,67");
         construct<T>("-10.679", false, "-10,68");
         construct<T>("-10.6435", false, "-10,64");
         construct<T>("-10.099", false, "-10,10");
      }
      construct<T>("10", true, "10,00");
      
      if (is_same<T, Integer>::value) {
         construct_cents<T>("34028234663852885981782907897774079", 99,    true, "34028234663852885981782907897774079,99" );
         construct_cents<T>("-34028234663852885981782907897774079", 99,    true, "-34028234663852885981782907897774079,99" );
         construct<T>("34028234663852885981782907897774079.99",    true, "34028234663852885981782907897774079,99" );
         construct<T>("-34028234663852885981782907897774079.99",    true, "-34028234663852885981782907897774079,99" );
         construct<T>("34028234663852885981782907897774079.994",    false, "34028234663852885981782907897774079,99" );
         construct<T>("-34028234663852885981782907897774079.994",    false, "-34028234663852885981782907897774079,99" );
         construct<T>("34028234663852885981782907897774079.995",    false, "34028234663852885981782907897774080,00" );
         construct<T>("-34028234663852885981782907897774079.995",    false, "-34028234663852885981782907897774080,00" );
         construct<T>("34028234663852885981782907897774080.01",    true, "34028234663852885981782907897774080,01" );
         construct<T>("-34028234663852885981782907897774080.01",    true, "-34028234663852885981782907897774080,01" );
         construct<T>("34028234663852885981782907897774080.00",    true, "34028234663852885981782907897774080,00" );
         construct<T>("-34028234663852885981782907897774080.00",    true, "-34028234663852885981782907897774080,00" );
         construct<T>("34028234663852885981782907897774080.797",    false, "34028234663852885981782907897774080,80" );
         construct<T>("-34028234663852885981782907897774080.797",    false, "-34028234663852885981782907897774080,80" );
         construct<T>("34028234663852885981782907897774080.793333333", false, "34028234663852885981782907897774080,79" );
         construct<T>("-34028234663852885981782907897774080.793333333", false, "-34028234663852885981782907897774080,79" );
         construct<T>("99999999999999999999999999999999999999.99", true, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.99", true, "-99999999999999999999999999999999999999,99" );
         construct<T>("99999999999999999999999999999999999999.990", true, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.990", true, "-99999999999999999999999999999999999999,99" );
         construct<T>("99999999999999999999999999999999999999.991", false, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.991", false, "-99999999999999999999999999999999999999,99" );
         construct<T>("99999999999999999999999999999999999999.994", false, "99999999999999999999999999999999999999,99" );
         construct<T>("-99999999999999999999999999999999999999.994", false, "-99999999999999999999999999999999999999,99" );

         construct<T>(std::to_string( numeric_limits<float>::max() / CONVERTER / 100 ),    true, "34028234663852885981782907897774080,00" );
         construct<T>(std::to_string( numeric_limits<float>::lowest() / CONVERTER / 100 ), true, "-34028234663852885981782907897774080,00");
         construct<T>(std::to_string( numeric_limits<float>::max() / CONVERTER ),    true, "3402823466385288598232333985305853952,00" );
         construct<T>(std::to_string( numeric_limits<float>::lowest() / CONVERTER ), true, "-3402823466385288598232333985305853952,00");
      }
      if (is_floating_point<T>::value) {
         construct<T>("8.577444e+02", false, "857,74");
         construct<T>("8.577555e+02", false, "857,76");
         construct<T>(".50", true, "0,50");
         construct<T>(".5", true, "0,50");
         construct<T>(".500", true, "0,50");
         construct<T>(".501", false, "0,50");
         construct<T>("+.501", false, "0,50");
         construct<T>("50.", true, "50,00");
         construct<T>("5.", true, "5,00");
         construct<T>("05.", true, "5,00");
         construct<T>("-8.577444e+02", false, "-857,74");
         construct<T>("-8.577555e+02", false, "-857,76");
         construct<T>("-.50", true, "-0,50");
         construct<T>("-.5", true, "-0,50");
         construct<T>("-.500", true, "-0,50");
         construct<T>("-.501", false, "-0,50");
         construct<T>("-50.", true, "-50,00");
         construct<T>("-5.", true, "-5,00");
         construct<T>("-05.", true, "-5,00");
         if (is_same<T, float>::value) {
            construct<T>(std::to_string( numeric_limits<float>::max() / CONVERTER / 100 ),    true, "34028235832468283066569397853224960,92" );
            construct<T>(std::to_string( numeric_limits<float>::lowest() /CONVERTER / 100 ), true, "-34028235832468283066569397853224960,92");
            //assert(0);
            construct<T>("-8577e+03", true, "-8577000,68");
         }
         if (is_same<T, double>::value || is_same<T, long double>::value) {
            construct<T>("-8577e+03", true, "-8577000,00");
         }
      }
      construct_cents<T>("20", 5.5, false, "20,06");
      construct<T>("10.67", true, "10,67");
      construct<T>("10.679", false, "10,68");
      construct<T>("10.6435", false, "10,64");
      construct<T>("10.099", false, "10,10");
   }
   cerr << "END OF " << __func__ << '\n';
}

template <typename T> 
void incorrect_construct() {
   cerr << __func__ << '\n';
   incorrect<T>();
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", 5.0L);
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", 5);
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "20t", 7);
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-1.99999");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "1.99999");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "189.99999");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(1 + numeric_limits<long double>::max() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(-1 + numeric_limits<long double>::lowest() / CONVERTER)));
   }
   else if (is_same<T, Integer>::value || is_same<T, float>::value) {
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<long double>::max() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<long double>::lowest() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<double>::max() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<double>::lowest() / CONVERTER)));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<float>::max())));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<float>::lowest())));
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "34028234663852885981782907897774080793333.333");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-34028234663852885981782907897774080793333.333");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "99999999999999999999999999999999999999.999" );
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-99999999999999999999999999999999999999.999" );
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "99999999999999999999999999999999999999.995" );
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-99999999999999999999999999999999999999.995" );
      if (is_same<T, float>::value) {
         construct_incorrect<Money<float>>(Constructor<float, Money>(), "99999999999999999999999999999999999999.99" );
         construct_incorrect<Money<float>>(Constructor<float, Money>(), "-99999999999999999999999999999999999999.99" );
      }
      //assert(0);
   }
   
   if (is_unsigned<T>::value)
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-1", 4);
   //construct_incorrect<Money<T>><T>("+0", 4);
   if (numeric_limits<T>::is_integer) {
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-8577e+03");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-8.577444e+02");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "8.577555e+02");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), ".50");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "50.");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-.50");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-50.");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), ".5");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "05.");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-.5");
      construct_incorrect<Money<T>>(Constructor<T, Money>(), "-50.");
   }
   
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "inf", 8);
   construct_incorrect<Money<T>>(Constructor<T, Money>(), "inf");
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(std::nan(""))));
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<float>::infinity())));
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<double>::infinity())));
   construct_incorrect<Money<T>>(Constructor<T, Money>(), (std::to_string(numeric_limits<long double>::infinity())));
   
   //construct_incorrect<Money<T>>(Constructor<T, Money>(), "20.8", "VVV");
   //construct_incorrect<Money<T>>(Constructor<T, Money>(), "20", 5.0L, "WWW");
   //construct_incorrect<Money<T>><T>("57.7");
   cerr << "END OF " << __func__ << '\n';
}

template <typename T>
inline void test() {
   construct<T>();
   incorrect_construct<T>();
}

template <typename T>
void test_max_8bits_value() {
   constexpr T MAX_C = numeric_limits<T>::max();
   constexpr T MIN_C = numeric_limits<T>::lowest();
   construct<T>((std::to_string(MAX_C / CONVERTER)), true, "1,27");
   construct<T>((std::to_string(MIN_C / CONVERTER)), true, "-1,28");
   //assert(false);
}

void test_extremums() {
   Integers_extremums_test<Integer>::test();
   Integers_extremums_test<char>::test();
   Integers_extremums_test<int_fast8_t>::test();
   Integers_extremums_test<short>::test();
   Integers_extremums_test<unsigned short>::test();
   Integers_extremums_test<int>::test();
   Integers_extremums_test<unsigned>::test();
   Integers_extremums_test<long>::test();
   Integers_extremums_test<unsigned long>::test();
   Integers_extremums_test<long long>::test();
   Integers_extremums_test<unsigned long long>::test();
}

void test_Integer_overflow() {
   const array <Integer, 2> EXTREMUMS = { numeric_limits<Integer>::max(), numeric_limits<Integer>::lowest() };
   const Integer FACTOR = Integer(15);
   const Integer Integer_CONVERTER = Integer(CONVERTER);
   for (size_t i = 0; i < EXTREMUMS.size(); i++)
      try {
         const string MAX_STR = std::to_string((FACTOR * EXTREMUMS[i]) / Integer_CONVERTER);
         cout << __func__ << " MAX_STR for type = " << typeid(Integer).name() << " " << MAX_STR << '\n';
         assert(0);
      } catch (const out_of_range& e) {
         cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
      }
}

void perform() {
   test<char>();
   test<int_fast8_t>();
   test<short>();
   test<unsigned short>();
   test<int>();
   test<unsigned int>();
   test<long>();
   test<unsigned long>();
   test<long long>();
   test<unsigned long long>();
   test<long double>();
   test<double>();
   test<float>();
   test<Integer>();
   
   test_extremums();
   test_max_8bits_value<char>();
   test_max_8bits_value<int_fast8_t>();
   test_Integer_overflow();
   
   cerr << '\n' << __func__ << ": END of ALL TESTS\n";
   //test<string>();
}

}
