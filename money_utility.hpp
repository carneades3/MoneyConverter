#ifndef MONEY_UTILITY_HPP
#define MONEY_UTILITY_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <iomanip>
#include <regex>
#include <map> 

#include <cmath>

#include "integer.hpp"
#include "network.hpp"
#include "floatrates.hpp"

using std::ostream;
using std::istream;
using std::string;
using std::is_integral;
using std::is_floating_point;
using std::enable_if_t;
using std::is_same;
using std::ostringstream;
using std::setprecision;
using std::fixed;
using std::setw;
using std::setfill;
using std::stringstream;
using std::istringstream;
using std::is_convertible;
using std::signbit;
using std::regex;
using std::trunc;
using std::floor;
using std::map;

using integer_space::Integer;

namespace money {
   
constexpr int_fast8_t CENTS_PER_DOLLAR = 100;
constexpr int_fast8_t INCORRECT_CENTS = -112;

const regex E_FLOAT_POINT_REGEX = regex { R"(^[+-]?(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)" } ;
const regex FLOAT_POINT_REGEX   = regex { R"(^[+-]?(\d+([.]\d*)?|[.]\d+)$)" } ;
const regex INTEGER_REGEX       = regex { R"(^[+-]?(\d+)$)" } ;
const regex MINUS_ZERO_REGEX    = regex { R"(^-0([.][0-9]+)?$)" } ;

extern const Integer CENTS_PER_DOLLAR_INTEGER;

inline string dollars_from_amount(const long double AMOUNT) {
   const long double dollars = AMOUNT / CENTS_PER_DOLLAR;
   string dollars_string = std::to_string(dollars);
   return dollars_string;
}

template<typename Greater>
bool is_overflow_for_Integer(const Greater & x) {
   try {
      Integer::create_Integer(x);
      return false;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
      return true;
   }
} 

template<typename Smaller, typename Greater
#if defined(__clang__)
   , enable_if_t<(is_floating_point<Greater>::value || is_integral<Greater>::value), bool> = true
#endif
>
inline bool is_overflow(const Greater & x) {
   return x < numeric_limits<Smaller>::lowest() || x > numeric_limits<Smaller>::max();
}

template <typename Number, enable_if_t<
              is_floating_point<Number>::value || is_integral<Number>::value, bool> = true>
inline bool equal_integer(const Number x) {
   cerr << __func__ << " x = " << std::to_string(x) << '\n';
   cerr << " trunc(x) = " << std::to_string(trunc(x)) << '\n';
   cerr << std::boolalpha << " isfinite(x) = " << isfinite(x) << '\n';
   cerr << std::boolalpha << " x == trunc(x) = " << (x == trunc(x)) << '\n';
   return isfinite(x) && x == trunc(x);
}

struct bad_from_string : public std::bad_cast {
   const char * what() const noexcept override {
      return "Bad cast from string";
   }
};

template <typename Type>
Type from_string(const string & STR, bool eof_checking) {
   istringstream stream { STR };
   Type result;
   stream >> result;
   if (!stream)
      throw bad_from_string {};
   if (eof_checking && ! stream.eof()) {
      cerr << " Type = " << typeid(Type).name() << '\n';
      throw invalid_argument(__func__ + string(" entered string '" + STR + "' is not accepted format "));
   }
   return result;
}

template<typename T>
constexpr bool is_resetting_stream() {
   return is_convertible<T, stringstream>::value || is_convertible<T, istringstream>::value
               || is_convertible<T, ostringstream>::value;
}

template<typename Greater, typename Smaller >
constexpr bool is_NOT_smaller() {
   return (false == is_same<Greater, Smaller>::value) && 
   (numeric_limits<Greater>::lowest() <= numeric_limits<Smaller>::lowest()
     || numeric_limits<Greater>::max() >= numeric_limits<Smaller>::max());
}

template<typename T>
void reset(T & stream) {
   static_assert(is_resetting_stream<T>());
   const static stringstream initial;
   stream.str("");
   stream.clear();
   stream.copyfmt(initial);
}

struct Constructor_Args {
   const string DOLLARS;
   const long double CENTS;
   
   template<typename U, enable_if_t< ! numeric_limits<U>::is_signed || is_same<U, Integer>::value, bool> = true>
   Constructor_Args(const U& AMOUNT) : DOLLARS (dollars(AMOUNT)), CENTS (cents(AMOUNT)) { }
   
private:
   template<typename U, enable_if_t< ! numeric_limits<U>::is_signed, bool>  = true>
   inline string dollars (const U& AMOUNT) {
      const U dollars = AMOUNT / CENTS_PER_DOLLAR;
      return std::to_string(dollars);
   }
   
   template<typename U, enable_if_t< ! numeric_limits<U>::is_signed, bool>  = true>
   inline long double cents (const U& AMOUNT) {
      U cents = AMOUNT % CENTS_PER_DOLLAR;
      return static_cast<long double>(cents);
   }
   
   inline string dollars (const Integer& AMOUNT) {
      const Integer dollars = AMOUNT / CENTS_PER_DOLLAR_INTEGER;
      string dollars_string = std::to_string(dollars);
      if (dollars.is_zero() && AMOUNT < Integer::ZERO)
         dollars_string.insert(0, 1, Integer::MINUS);
      return dollars_string;
   }
   
   inline long double cents (const Integer& AMOUNT) {
      Integer cents = AMOUNT % CENTS_PER_DOLLAR_INTEGER;
      cents = std::move(std::abs(cents));
      return static_cast<long double>(cents);
   }
};

using namespace network;

map <string, long double> & set_rates_per_USD(const Network_library & library, const File_format & format);

template <typename T>
class Money;

template<typename T, typename Result = Money<T>>
Result operator!(const Money<T>& a) = delete;
template<typename T, typename Result = Money<T>>
Result operator!(const Money<T> a) = delete;
template<typename T, typename Result = Money<T>>
Result operator!(Money<T>& a) = delete;
template<typename T, typename Result = Money<T>>
Result operator!(Money<T>&& a) = delete;

template<typename U, enable_if_t<! numeric_limits<U>::is_signed, bool>   = true, typename Result = Money<U>>
Result operator-(const Money<U>& a) = delete;
template<typename U, enable_if_t<false == numeric_limits<U>::is_signed, bool> = true, typename Result = Money<U>>
Result operator-(Money<U> a) = delete;
template<typename U, enable_if_t<! numeric_limits<U>::is_signed, bool> = true, typename Result = Money<U>>
Result operator-(Money<U>& a) = delete;
template<typename U, enable_if_t<! numeric_limits<U>::is_signed, bool> = true, typename Result>
Result operator-(Money<U>&& a) = delete;

Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b);
Money<long double> operator+(const Money<long double>& a, const Money<long double>& b);

Money<Integer> operator*(const Integer& FACTOR, const Money<Integer>& MONEY);
Money<long double> operator*(const long double& FACTOR, const Money<long double>& MONEY);

template<typename Smaller, enable_if_t<is_integral<Smaller>::value, bool> = true>
Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);

template<typename Smaller, enable_if_t<is_floating_point<Smaller>::value && ! is_same<Smaller, long double>::value, bool > = true>
Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> = true>
Money<Smaller> operator-(const Money<Smaller>& A);

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> = true>
Money<Smaller> operator-(const Money<Smaller>& a);

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool>  = true>
Money<Smaller> operator*(const Money<Smaller>& MONEY, const Smaller FACTOR);

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> = true>
Money<Smaller> operator*(const Money<Smaller>& MONEY, const Smaller FACTOR);

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool>  = true>
void operator*=(Money<Smaller>& MONEY, const Smaller FACTOR);

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> = true >
void operator*=(Money<Smaller>& MONEY, const Smaller FACTOR);

}

#endif
