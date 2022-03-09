#include "money.hpp"
#include "integer_parsing.hpp"

#include <cmath>
#include <limits>
#include <sstream>

using std::numeric_limits;
using std::enable_if_t;
//using std::isfinite;
using std::istringstream;
//using std::bad_cast;
//using std::trunc;

namespace money {
   
template <typename T>
const string Money<T>::TYPE_NAME = typeid(T).name();

template <typename T>
const T Money<T>::TYPE_DEFAULT_OBJECT = T{};

const string LONG_DOUBLE_NAME = typeid(0.0L).name();
const string DOUBLE_NAME = typeid(0.0).name();
const string FLOAT_NAME = typeid(0.0f).name();
const string CHAR_NAME = typeid('A').name();
const string INT_FAST_8_T_NAME = typeid(((int_fast8_t)0)).name();
const string SHORT_NAME = typeid(((short)0)).name();
const string UNSIGNED_SHORT_NAME = typeid(((unsigned short)0)).name();
const string INT_NAME = typeid(0).name();
const string LONG_NAME = typeid(0L).name();
const string LONG_LONG_NAME = typeid(numeric_limits<long long>::max()).name();
const string UNSIGNED_INT_NAME = typeid(0u).name();
const string UNSIGNED_LONG_NAME = typeid(0uL).name();
const string UNSIGNED_LONG_LONG_NAME = typeid(numeric_limits<unsigned long long>::max()).name();
const Integer INTEGER_OBJECT = Integer{};
const string INTEGER_OBJECT_NAME = typeid(INTEGER_OBJECT).name();

inline void validate_cents(const long double & cents) {
   if (0 > cents || cents >= CENTS_PER_DOLLAR) {
      static const string ERROR = "cents must be 0 - " + to_string(CENTS_PER_DOLLAR);
      throw invalid_argument(ERROR);
   }
}

//extern template <>
template <>
Integer Money<Integer>::get_amount(const string & STR);

#if defined(__clang__)
template <typename T>
T Money<T>::get_amount(const string & STR) {
   cerr << __func__ << " = " << STR << '\n';  
   if (is_same<T, long double>::value)   
      return stold(STR);
   else if (is_same<T, double>::value)  
      return stod(STR);
   else if (is_same<T, float>::value)   
      return stof(STR);
   else if (is_same<T, unsigned long long>::value) 
      return stoull(STR);
   else if (is_same<T, long long>::value)   
      return stoll(STR);
   else if (is_same<T, unsigned long>::value) 
      return stoul(STR);
   else if (is_same<T, long>::value)
      return stol(STR);
   else if (is_same<T, int>::value)   
      return stoi(STR);
   else if (is_same<T, unsigned int>::value 
         || is_same<T, unsigned short>::value) {
      unsigned long amount = stoul(STR);
      if (is_overflow<T, decltype(amount)>(amount))
         throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
      return static_cast<T>(amount);
   }
   else if (is_same<T, short>::value
         || is_same<T, char>::value
         || is_same<T, int_fast8_t>::value) {
      int amount = stoi(STR);
      if (is_overflow<T, decltype(amount)>(amount))
         throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
      return static_cast<T>(amount);
   }
   else
      throw invalid_argument("No implementation for type '" + TYPE_NAME + "'");  
}
#elif defined(__GNUG__)
template <typename T>
T Money<T>::get_amount(const string & STR) {
   cerr << __func__ << " = " << STR << '\n';
   if (is_floating_point<T>::value ) {  
      if (TYPE_NAME == LONG_DOUBLE_NAME)   
         return stold(STR);
      else if (TYPE_NAME == DOUBLE_NAME)  
         return stod(STR);
      else if (TYPE_NAME == FLOAT_NAME)   
         return stof(STR);
   } 
   else if (numeric_limits<T>::is_integer) { 
      if (TYPE_NAME == UNSIGNED_LONG_LONG_NAME) 
         return stoull(STR);
      else if (TYPE_NAME == LONG_LONG_NAME)   
          return stoll(STR);
      else if (TYPE_NAME == UNSIGNED_LONG_NAME) 
         return stoul(STR);
      else if (TYPE_NAME == LONG_NAME)
         return stol(STR);
      else if (TYPE_NAME == INT_NAME)   
         return stoi(STR);
      else if (TYPE_NAME == UNSIGNED_INT_NAME 
            || TYPE_NAME == UNSIGNED_SHORT_NAME) {
         unsigned long amount = stoul(STR);
         if (is_overflow<T, decltype(amount)>(amount))
            throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
         return static_cast<T>(amount);
      }
      else if (TYPE_NAME == SHORT_NAME
            || TYPE_NAME == CHAR_NAME
            || TYPE_NAME == INT_FAST_8_T_NAME) {
         int amount = stoi(STR);
         if (is_overflow<T, decltype(amount)>(amount))
            throw out_of_range("amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
         return static_cast<T>(amount);
      }
      //else if (TYPE_NAME == INTEGER_OBJECT_NAME) 
      //   return Integer::parse_create(STR);
      else
         throw invalid_argument("No implementation for type '" + TYPE_NAME + "'");  
   }
}
#endif

template <>
Integer Money<Integer>::convert(const Money & MONEY) const;

template<typename T>
T Money<T>::convert(const Money & MONEY) const {
   //validate_currency(CURRENCY);
   if (MONEY.get_currency() == get_currency())
      return MONEY.amount_in_cents;
   
   const long double ratio = rates_per_USD[MONEY.get_currency()] / rates_per_USD[get_currency()];
   long double amount = ratio * static_cast<long double>(MONEY.amount_in_cents);
   amount = Money<long double>::round(amount);
   cerr << __func__ << " amount = " << amount << '\n';
   if (is_overflow<T, long double>(amount))
      throw out_of_range(string(__func__) + " amount = " + std::to_string(amount) + " is overflow for type " + TYPE_NAME);
   const T result = static_cast<T>(amount) ;
   return result; 
}

template <typename T>
template<typename Greater>
T Money<T>::calculate(const T & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   static_assert(((numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) &&
                        ! is_same<Greater, Integer>::value) && "(numeric_limits<Greater>::is_integer || is_floating_point<Greater>::value) && \
! is_same<Greater, Integer>::value" );
   Greater amount_in_cents = Greater(dollars) * Greater(CENTS_PER_DOLLAR);
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS == cents)
      amount_in_cents = Money<Greater>::round(amount_in_cents);
   else {
      Greater cents_round = dollars >= 0 ? Money<Greater>::round(cents) : -Money<Greater>::round(cents);
      amount_in_cents += Greater(cents_round);
   }
   
   if (is_overflow<T, Greater>(amount_in_cents))
      throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   T result = T { static_cast<T> (amount_in_cents) };
   //cerr << __func__ << " result = " << static_cast<long long int>(result) << '\n';
   cerr << __func__ << " result = " << std::to_string(result) << '\n';
   return result;
}

template <typename T>
//template<typename Integer>
T Money<T>::calculate_by_Integer(const T & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   cerr << __func__ << '\n';
   const Integer dollars_as_Integer = Integer::create_Integer(dollars);
   Integer amount_in_cents = dollars_as_Integer * CENTS_PER_DOLLAR_INTEGER;
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS != cents) {
      cerr << __func__ << " cents = " << cents << '\n';
      Integer cents_round = dollars_as_Integer >= Integer::ZERO ? Money<Integer>::round(cents) : -Money<Integer>::round(cents);
      amount_in_cents += cents_round;
   }
   if (Integer::is_overflow<T>(amount_in_cents))
      throw out_of_range("amount_in_cents = " + std::to_string(amount_in_cents) + " is overflow for type " + TYPE_NAME);
   
   T result = amount_in_cents.operator T();
   cerr << __func__ << " result = " << Integer::create_Integer(result) << '\n';
   return result;
}

template <>
//template<typename Integer>
Integer Money<Integer>::calculate_by_Integer(const Integer & dollars, const long double cents /*  = INCORRECT_CENTS */) const;

template<>
Money<Integer>::Money(const string & dollars, const long double cents, const string & currency/* = "PLN" */);

template <typename T>
Money<T>::Money(const string & dollars, const long double cents, const string & currency /* = "PLN" */) {
   cerr << __func__ << " TYPE_NAME = '" << TYPE_NAME << "' " << dollars << '\n';
   set_currency(currency);
   validate_cents(cents);
   if (! regex_match(dollars, INTEGER_REGEX)) 
      throw invalid_argument("Regex: dollars must be integer number ");
   string dollars_string = dollars;
   T amount = Money<T>::get_amount(dollars_string);
   if (is_floating_point<T>::value) 
      this->amount_in_cents = calculate<long double>(amount, cents);
   else if (numeric_limits<T>::is_integer) 
      this->amount_in_cents = calculate_by_Integer(amount, cents);

   cerr << __func__ << " amount = '" << amount << "' std::signbit = '" << std::boolalpha << std::signbit(amount) << "' \n";  

   if (dollars[0] == '-' && 0 == amount)   
      this->amount_in_cents = -this->amount_in_cents;
   
   if (is_same<T, char>::value || is_same<T, int_fast8_t>::value)
      cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << static_cast<int>(this->amount_in_cents) << '\n';
   else
      cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << this->amount_in_cents << '\n';   
}

template <typename Type, enable_if_t<numeric_limits<Type>::is_integer, bool> = true>
Type get_amount_by_Integer(const string & STR) {
   Integer integer = Integer::parse_create(STR);
   if (! is_same<Type, Integer>::value)
      return Type(integer);
   return integer;
}

template <typename T>
Money<T>::Money(const string & dollars, const string & currency /* = "PLN" */) {   // accept floating-point arguments
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   set_currency(currency);
   if (is_floating_point<T>::value) {
      if (! regex_match(dollars, E_FLOAT_POINT_REGEX)) 
         throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not floating-point format ");
   }
   else if (numeric_limits<T>::is_integer) { 
      if (! regex_match(dollars, FLOAT_POINT_REGEX))
         throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not non-exponent floating-point format "); 
   }
   this->amount_in_cents = calculate_amount_in_cents(dollars);
}

template <typename T>
Money<T> Money<T>::create(const string & dollars, const long double cents, const string & currency /* = "PLN" */) {  
   if (! equal_integer<long double>(cents))
      throw invalid_argument("Not exact value cents = " + to_string(cents));
   Money<T> money = Money<T>(dollars, cents, currency);
   return money;
}

template <typename Number, enable_if_t< is_floating_point<Number>::value, bool> = true> 
inline Number negate_minus_zero (const Number & n) {
   return n == 0 && signbit(n) ? -n : n;
}

template <typename Number, enable_if_t< is_floating_point<Number>::value, bool> = true>
string formatted_string(Number & dollars, Number & cents) {
   cerr << __func__ << " dollars = " << dollars <<  '\n';
   cerr << __func__ << " cents = " << cents <<  '\n';
   dollars = negate_minus_zero(dollars);
   if (signbit(cents)) 
      cents = -cents;
   cerr << __func__ << " dollars = " << dollars <<  '\n';
   cerr << __func__ << " cents = " << cents <<  '\n';
   ostringstream stream;
   stream << fixed << setprecision(0) << setw(0) << dollars << ",";
   stream.fill('0');
   stream << setw(2) << cents;
   string out = stream.str();
   return out;
}

template <typename Number, enable_if_t< is_integral<Number>::value, bool> = true>
string formatted_string(const Number dollars, const Number cents) {
   string dollars_string;
   string cents_string;
   if (is_same<Number, char>::value || is_same<Number, int_fast8_t>::value) {
      dollars_string = std::to_string(static_cast<int>(dollars));
      cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
      cents_string = std::to_string(static_cast<int>(cents));
      cerr << __func__ << " cents_string = " << cents_string <<  '\n';
   }
   else {
      dollars_string = std::to_string(dollars);
      cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
      cents_string = std::to_string(cents);
      cerr << __func__ << " cents_string = " << cents_string <<  '\n';
   }
   cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
   //string out = (dollars == 0 && get_amount_in_cents() < static_cast<T>(0)) ? "-" : "";
   string out = dollars_string + "," + cents_string;
   return out;
}

extern string formatted_string(const Integer & dollars, const Integer & cents);

template<>
Money<Integer>::operator string() const;

template <typename T>
Money<T>::operator string() const {
   T dollars = get_dollars(TYPE_DEFAULT_OBJECT);
   T cents = get_cents(TYPE_DEFAULT_OBJECT);
   cerr << __func__ << " get_cents " << cents << '\n';
   if (is_integral<T>::value && signbit(cents)) {
      cerr << __func__ << "\n --------------------------- cents < static_cast<T>(0)) " << '\n';
      cents = -cents;
   }
   cerr << __func__ << " cents " << cents << '\n';
   cerr << __func__ << " get_amount_in_cents() " << get_amount_in_cents() << '\n';
   string out = (dollars == 0 && get_amount_in_cents() < static_cast<T>(0)) ? "-" : "";
   out += formatted_string(dollars, cents);
   out +=  " " + get_currency();
   return out;
}

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> = true>
Money<Smaller> calculate_money(const Integer& AMOUNT, const string & CURRENCY) {
   static const string TYPE_NAME = typeid(Smaller).name();
   cerr << __func__ << " AMOUNT = " << AMOUNT << '\n';
   if (Integer::is_overflow<Smaller>(AMOUNT))
      throw out_of_range(string(__func__) + " amount = " + std::to_string(AMOUNT) + " is overflow for type " + TYPE_NAME);
   const Constructor_Args args {AMOUNT};
   Money<Smaller> result = Money<Smaller>(args.DOLLARS, args.CENTS, CURRENCY);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> = true>
Money<Smaller> calculate_money(const Greater& AMOUNT, const string & CURRENCY) {
   static_assert(is_NOT_smaller<Greater, Smaller>() && "is_NOT_smaller<Greater, Smaller> required");
   static const string TYPE_NAME = typeid(Smaller).name();
   cerr << __func__ << " AMOUNT = " << AMOUNT << '\n';
   if (is_overflow<Smaller, Greater>(AMOUNT))
      throw out_of_range(string(__func__) + " amount = " + std::to_string(AMOUNT) + " is overflow for type " + TYPE_NAME);
   const string dollars = std::to_string(AMOUNT / static_cast<long double>(CENTS_PER_DOLLAR));
   Money<Smaller> result = Money<Smaller>(dollars, CURRENCY);
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool>
#ifdef __clang__
   = true
#endif
>
Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b) {
   //static_assert(is_NOT_smaller<Greater, Smaller>() && "is_NOT_smaller<Greater, Smaller> required");
   Smaller conversion = /*Money<Smaller>::*/a.convert(b);
   Integer sum = Integer::create_Integer(a.amount_in_cents) + Integer::create_Integer(conversion);
   return calculate_money<Greater, Smaller>(sum, a.get_currency());
}

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool>
#ifdef __clang__
   = true
#endif
>
Money<Smaller> operator+(const Money<Smaller>& A, const Money<Smaller>& B) {
   static_assert(is_NOT_smaller<Greater, Smaller>() && "is_NOT_smaller<Greater, Smaller> required");
   Smaller conversion = /*Money<Smaller>::*/A.convert(B);
   Greater sum = Greater(A.amount_in_cents) + /*Money<Greater>::*/Greater(conversion);
   sum = Money<Greater>::round(sum);
   return calculate_money<Greater, Smaller>(sum, A.get_currency());
}

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool>>
Money<Smaller> operator-(const Money<Smaller>& a) {
   Integer minus = -Integer::create_Integer(a.amount_in_cents);
   return calculate_money<Greater, Smaller>(minus, a.get_currency());
}

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool>>
Money<Smaller> operator-(const Money<Smaller>& A) {
   static_assert(is_NOT_smaller<Greater, Smaller>() && "is_NOT_smaller<Greater, Smaller> required");
   Greater minus = - Greater(A.amount_in_cents);
   return calculate_money<Greater, Smaller>(minus, A.get_currency());
}

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> >
Money<Smaller> operator*(const Money<Smaller>& MONEY, const Smaller FACTOR) {
   const Integer product = Integer::create_Integer(MONEY.amount_in_cents) * Integer::create_Integer(FACTOR);
   return calculate_money<Greater, Smaller>(product, MONEY.get_currency());
}

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> >
Money<Smaller> operator*(const Money<Smaller>& MONEY, const Smaller FACTOR) {
   static_assert(is_NOT_smaller<Greater, Smaller>() && "is_NOT_smaller<Greater, Smaller> required");
   Greater product = Greater(MONEY.amount_in_cents) * Greater(FACTOR);
   product = Money<Greater>::round(product);
   return calculate_money<Greater, Smaller>(product, MONEY.get_currency());
}

template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> >
void operator*=(Money<Smaller>& MONEY, const Smaller FACTOR) {
   static_assert(is_NOT_smaller<Greater, Smaller>() && "is_NOT_smaller<Greater, U> required");
   static const string TYPE_NAME = typeid(Smaller).name();
   Greater product = Greater(MONEY.amount_in_cents) * Greater(FACTOR);
   product = Money<Greater>::round(product);
   cerr << __func__ << " product = " << product << '\n';
   if (is_overflow<Smaller, Greater>(product))
      throw out_of_range(string(__func__) + " amount = " + std::to_string(product) + " is overflow for type " + TYPE_NAME);
   MONEY.amount_in_cents = static_cast<Smaller>(product);
   cerr << __func__ << " MONEY.amount_in_cents = " << MONEY.amount_in_cents << '\n';
}

template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> >
void operator*=(Money<Smaller>& MONEY, const Smaller FACTOR) {
   static const string TYPE_NAME = typeid(Smaller).name();
   Integer product = Integer::create_Integer(MONEY.amount_in_cents) * Integer::create_Integer(FACTOR);
   cerr << __func__ << " product = " << product << '\n';
   if (Integer::is_overflow<Smaller>(product))
      throw out_of_range(string(__func__) + " amount = " + std::to_string(product) + " is overflow for type " + TYPE_NAME);
   MONEY.amount_in_cents = product.operator Smaller();
   cerr << __func__ << " MONEY.amount_in_cents = " << MONEY.amount_in_cents << '\n';
}

template<typename Smaller, enable_if_t<is_integral<Smaller>::value, bool>
#ifdef __clang__
#elif defined(__GNUG__)
   = true
#endif
>
inline Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b) {
#ifdef __clang__
   return operator+<Integer, Smaller>(a, b);
#elif defined(__GNUG__)
   return operator+<Integer, Smaller, true>(a, b);
#endif
}

template<typename Smaller, enable_if_t<is_floating_point<Smaller>::value && ! is_same<Smaller, long double>::value, bool >
#ifdef __clang__
#elif defined(__GNUG__)
   = true
#endif
>
inline Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b) {
#ifdef __clang__
   return operator+<long double, Smaller>(a, b);
#elif defined(__GNUG__)
   return operator+<long double, Smaller, true>(a, b);
#endif
}

template<typename Smaller, enable_if_t<is_integral<Smaller>::value, bool> = true >
inline Money<Smaller> operator*(const Smaller & factor, const Money<Smaller>& money) {
   return operator*<Integer, Smaller>(money, factor);
}

template<typename Smaller, enable_if_t<is_floating_point<Smaller>::value && ! is_same<Smaller, long double>::value, bool > = true >
inline Money<Smaller> operator*(const Smaller & factor, const Money<Smaller>& money) {
   return operator*<long double, Smaller>(money, factor);
}

inline Money<Integer> operator*(const Integer& FACTOR, const Money<Integer>& MONEY) {
   return MONEY * FACTOR;
}

inline Money<long double> operator*(const long double& FACTOR, const Money<long double>& MONEY) {
   return MONEY.operator*(FACTOR);
}

#ifdef DEBUG_OSTREAM
   template <class Number, enable_if_t<numeric_limits<Number>::is_integer || is_floating_point<Number>::value, bool> = true>
   inline ostringstream& start_settings(ostringstream * os, const Money<Number>& money) {
      validate_pointer(os);
      if (money.get_amount_in_cents() < static_cast<Number>(0))
         *os << '-';
      os->fill('0');
      return *os;
   }

   template <class Number, template<typename> class Money_Template, enable_if_t<
                     is_same<Number, char>::value || is_same<Number, int_fast8_t>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
      validate_pointer(os);
      int dollars = static_cast<int>(money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
      int cents = static_cast<int>(money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
      start_settings(os, money);
      *os << abs(dollars) << ",";
      *os << setw(2) << abs(cents);
      *os << ' ' << money.get_currency();
      return *os;
   }

   template <class Number, template<typename> class Money_Template, enable_if_t<is_integral<Number>::value &&
                        ! is_same<Number, char>::value && ! is_same<Number, int_fast8_t>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
      validate_pointer(os);
      Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      *os << (dollars < 0 ? -dollars : dollars) << ",";
      *os << setw(2) << (cents < 0 ? -cents : cents);
      *os << ' ' << money.get_currency();
      return *os;
   }
   
   template <class Number, template<typename> class Money_Template, enable_if_t<is_same<Number, Integer>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
      validate_pointer(os);
      Integer dollars = money.get_dollars(Money<Integer>::TYPE_DEFAULT_OBJECT);
      Integer cents = money.get_cents(Money<Integer>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      *os << dollars.string_without_signum() << ",";
      *os << setw(2) << cents.string_without_signum();
      *os << ' ' << money.get_currency();
      return *os;
   }

   template <class Number, template<typename> class Money_Template, enable_if_t<is_floating_point<Number>::value, bool> = true>
   ostringstream& operator<<(ostringstream * os, const Money_Template<Number>& money) {
      validate_pointer(os);
      std::streamsize os_precision = os->precision();
      Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      *os << fixed << setprecision(0) << setw(0) << (signbit(dollars) ? -dollars : dollars) << ",";
      *os << setw(2) << (signbit(cents) ? -cents : cents);
      *os << ' ' << money.get_currency();
      os->precision(os_precision);
      return *os;
   }

   template <class Number, enable_if_t<is_floating_point<Number>::value || numeric_limits<Number>::is_integer, bool> = true>
   ostream& operator<<(ostream& os, const Money<Number>& money) {
      ostringstream ostrs;
      string output = operator<<(&ostrs, money).str();
      return os << output;
   }
#else

template <class Number, enable_if_t<is_floating_point<Number>::value || numeric_limits<Number>::is_integer, bool> = true>
inline ostream& start_settings(ostream& os, const Money<Number>& money) {
   if (money.get_amount_in_cents() < static_cast<Number>(0))
      os << '-';
   os.fill('0');
   return os;
}

template <class Number, template<typename> class Money_Template, enable_if_t<
                  is_same<Number, char>::value || is_same<Number, int_fast8_t>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   int dollars = static_cast<int>(money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
   int cents = static_cast<int>(money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT));
   start_settings(os, money);
   os << abs(dollars) << ",";
   os << setw(2) << abs(cents);
   os << ' ' << money.get_currency();
   return os;
}

template <class Number, template<typename> class Money_Template, enable_if_t<is_integral<Number>::value &&
                       ! is_same<Number, char>::value && ! is_same<Number, int_fast8_t>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   start_settings(os, money);
   os << (dollars < 0 ? -dollars : dollars) << ",";
   os << setw(2) << (cents < 0 ? -cents : cents);
   os << ' ' << money.get_currency();
   return os;
}

ostream& operator<<(ostream& os, const Money<Integer>& money);

template <class Number, template<typename> class Money_Template, enable_if_t<is_floating_point<Number>::value, bool> = true>
ostream& operator<<(ostream& os, const Money_Template<Number>& money) {
   std::streamsize os_precision = os.precision();
   Number dollars = money.get_dollars(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   Number cents = money.get_cents(Money_Template<Number>::TYPE_DEFAULT_OBJECT);
   start_settings(os, money);
   os << fixed << setprecision(0) << setw(0) << (signbit(dollars) ? -dollars : dollars) << ",";
   os << setw(2) << (signbit(cents) ? -cents : cents);
   os << ' ' << money.get_currency();
   os.precision(os_precision);
   return os;
}
#endif

template <class Number, template<typename> class Money_Template>
istream& operator>>(istream& is, Money_Template<Number>& money) {
   string money_string;
   is >> money_string;
   cerr << money_string << '\n';
   if (!is) {
      cerr << "Error has occurred on the associated stream\n";
      return is;
   }
   if (money_string.size() < 4) {
      cerr << "Error: money string must be minimum 4 chars\n";
      return is;
   }
   const string currency = money_string.substr(0, 3);
   const string value = money_string.substr(3);
   money = Money_Template<Number>(value, currency);
   return is;
}

}
