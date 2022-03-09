#ifndef MONEY_HPP
#define MONEY_HPP

#include "money_utility.hpp"

namespace money {

   
template <typename T>
class Money {
static_assert((numeric_limits<T>::is_integer || is_floating_point<T>::value) && "Number required.");
public:
   static const string TYPE_NAME;
   static const T TYPE_DEFAULT_OBJECT;
   
   static inline T round(long double x) {
      return static_cast<T>(floor(x + 0.5));
   }
   
   static inline void validate_currency(const string & CURRENCY) {
      if (rates_per_USD.end() == rates_per_USD.find(CURRENCY)) 
         throw invalid_argument("Currency " + CURRENCY + " is not available");
   }

   Money(const string & dollars, const long double cents, const string & currency = "PLN");   
   Money(const string & dollars, const string & currency = "PLN");            // constructors allow rounding of cents 
   // create methods disallow rounding of cents and accept only cents without fraction
   static Money create(const string & dollars, const long double cents, const string & currency = "PLN");
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value, bool>  = true>
   static Money<U> create(const string & dollars, const string & currency = "PLN") {
      const long double amount = from_string<long double>(dollars, true) * CENTS_PER_DOLLAR;
      if (! equal_integer<long double>(amount))
         throw invalid_argument("Not exact value dollars = " + to_string(amount));
      
      Money<U> money = Money<U>(dollars, currency);
      return money;
   }
   
   template<typename U = T, enable_if_t<numeric_limits<U>::is_integer, bool>  = true>
   static Money<U> create(const string & dollars, const string & currency = "PLN") {
      const size_t dot_position = dollars.find('.');
      if (dot_position != string::npos) {
         static const regex EXACT = regex { R"(^[+-]?(\d+).\d[\d]?[0]*$)" } ;
         if (! regex_match(dollars, EXACT))
            throw invalid_argument(string(__func__) +  " Regex: entered string '"
                  + dollars + "' is not exact format ");
      }
      Money<U> money = Money<U>(dollars, currency);
      return money;
   }
   
   ~ Money() = default;
   Money(const Money &) = default;
   Money(Money &&) = default;
   Money & operator=(const Money &) = default;
   Money & operator=(Money &&) = default;
   
   T convert(const Money &) const;
   
   Money operator+() const { return *this; }
   
   template<typename U = T, enable_if_t<! numeric_limits<U>::is_signed, bool>   = true>
   Money operator-() const = delete;
   template<typename U = T, enable_if_t<false == numeric_limits<U>::is_signed, bool>   = true >
   Money operator-() = delete;
   template<typename U = T, enable_if_t<! numeric_limits<U>::is_signed, bool>  = true >
   Money operator-() = delete;
   
   template<typename U = T, enable_if_t<is_same<U, Integer>::value, bool>  = true>
   Money<Integer> operator-() const {
      const Constructor_Args args { -this->amount_in_cents };
      Money<Integer> result = Money<Integer>(args.DOLLARS, args.CENTS, get_currency());
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
   
   template<typename U = T, enable_if_t<is_same<U, long double>::value, bool>  = true>
   Money<long double> operator-() const {
      const string dollars_string = dollars_from_amount(-this->amount_in_cents);
      Money<long double> result = Money<long double>(dollars_string, get_currency());
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
   
   template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool>>
   friend Money<Smaller> operator-(const Money<Smaller>& a);

   template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
               (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> >
   friend Money<Smaller> operator-(const Money<Smaller>& A);
   
   template<typename U = T, enable_if_t<is_integral<U>::value, bool>  = true>
   Money operator-() const { 
      return money::operator-<Integer, U>(*this);
   }
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value && ! is_same<U, long double>::value, bool>  = true>
   Money operator-() const { 
      return money::operator-<long double, U>(*this);
   }
   
   //Money operator+(const Money& other) const;

   template<typename U = T, enable_if_t<numeric_limits<U>::is_signed, bool>  = true>
   Money operator-(const Money& other) const { 
      try {
         return money::operator+(*this, -other); 
      } catch (const out_of_range &) {
         Money result = other + (-*this);
         result = -result; 
         return result;
      }
   }
   
   template<typename U = T, enable_if_t< ! numeric_limits<U>::is_signed, bool>  = true>
   Money operator-(const Money& other) const { 
      if (other > *this)
         throw out_of_range(string(__func__) + " other amount is " + std::to_string(other.amount_in_cents) + " > this->amount is "
            + std::to_string(this->amount_in_cents) + " This is overflow for type " + TYPE_NAME);
      
      T difference = this->amount_in_cents - convert(other);
      cerr << __func__ << " difference = " << difference << '\n';
      const Constructor_Args args { difference };
      Money result = Money(args.DOLLARS, args.CENTS, get_currency());
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
   
   template<typename U = T, enable_if_t<numeric_limits<U>::is_signed, bool>  = true>
   Money& operator-=(const Money& other) {
      try {
         return operator+=(-other); 
      } catch (const out_of_range &) {
         Money result = other + (-*this);
         *this = -result; 
         return *this;
      }
   }
   
   template<typename U = T, enable_if_t< ! numeric_limits<U>::is_signed, bool>  = true>
   Money& operator-=(const Money& other) {
      if (other > *this)
         throw out_of_range(string(__func__) + " other amount is " + std::to_string(other.amount_in_cents) + " > this->amount is "
            + std::to_string(this->amount_in_cents) + " This is overflow for type " + TYPE_NAME);
      this->amount_in_cents -= convert(other);
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
      return *this;
   }
   
   template<typename U = T, enable_if_t<is_same<U, Integer>::value, bool>  = true>
   Money& operator+=(const Money& other) {
      this->amount_in_cents += convert(other);
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
      return *this;
   }
   
   template<typename U = T, enable_if_t<is_same<U, long double>::value, bool>  = true>
   Money& operator+=(const Money& other) {
      this->amount_in_cents += /*Money::*/convert(other);
      this->amount_in_cents = Money<long double>::round(amount_in_cents);
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
      return *this;
   }
   
   template<typename U = T, enable_if_t<is_integral<U>::value, bool>  = true>
   Money<U>& operator+=(const Money<U>& other) {
      return operator+=<Integer, U>(other);
   }
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value && ! is_same<U, long double>::value, bool>  = true>
   Money<U>& operator+=(const Money<U>& other) {
      return operator+=<long double, U>(other);
   }
   
   template<typename U = T, enable_if_t<is_same<U, Integer>::value, bool>  = true>
   Money& operator*=(const Integer& factor) {
      this->amount_in_cents *= factor;
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
      return *this;
   }
   
   template<typename U = T, enable_if_t<is_same<U, long double>::value, bool>  = true>
   Money& operator*=(const long double& factor) {
      this->amount_in_cents *= factor;
      this->amount_in_cents = Money<long double>::round(amount_in_cents);
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
      return *this;
   }
   
   template<typename U = T, enable_if_t<is_integral<U>::value, bool>  = true>
   Money<T>& operator*=(const T& factor) {
      money::operator*=<Integer, T>(*this, factor);
      return *this;
   }
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value && ! is_same<U, long double>::value, bool>  = true>
   Money<T>& operator*=(const U& factor) {
      money::operator*=<long double, T>(*this, factor);
      return *this;
   }
   
   template<typename U = T, enable_if_t<is_same<U, Integer>::value, bool>  = true>
   Money operator*(const Integer& factor) const {
      const Integer product = this->amount_in_cents * factor;
      cerr << __func__ << " FACTOR = " << factor << " product = " << product << '\n';
      const Constructor_Args args {product};
      Money<Integer> result = Money<Integer>(args.DOLLARS, args.CENTS, get_currency());//::create
      cerr << __func__ << " result = " << result << '\n';
      return result;
   }
   
   template<typename U = T, enable_if_t<is_same<U, long double>::value, bool>  = true>
   Money operator*(const long double& factor) const {
      long double product = this->amount_in_cents * factor;
      product = Money<long double>::round(product);
      cerr << __func__ << " factor = " << factor << " product = " << product << '\n';
      const string dollars_string = dollars_from_amount(product);
      Money<long double> result = Money<long double>(dollars_string, get_currency());//::create
      return result;
   }
   
   template<typename U = T, enable_if_t<is_integral<U>::value, bool>  = true>
   Money operator*(const U& factor) const {
      return money::operator*<Integer, U>(*this, factor);
   }
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value && ! is_same<U, long double>::value, bool>  = true>
   Money operator*(const U& factor) const {
      return money::operator*<long double, U>(*this, factor);
   }
   
   bool operator==(const Money& other) const { return amount_in_cents == convert(other); }
   bool operator!=(const Money& other) const { return !(*this == other); } ;
   
   bool operator>(const Money& other) const { return amount_in_cents > convert(other); }
   bool operator<=(const Money& other) const { return !operator>(other); };
   bool operator<(const Money& other) const { return amount_in_cents < convert(other); }
   bool operator>=(const Money& other) const { return !operator<(other); };
   
   template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
   Type get_dollars(Type &) const { 
      return amount_in_cents / CENTS_PER_DOLLAR;   
   }
   
   Integer get_dollars(const Integer &) const { 
      return amount_in_cents / CENTS_PER_DOLLAR_INTEGER;   
   }

   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_dollars(Type &) const { return trunc(amount_in_cents / CENTS_PER_DOLLAR); }
   
   template <typename Type, enable_if_t<is_integral<Type>::value, bool> = true>
   Type get_cents(Type &) const { 
      return amount_in_cents % CENTS_PER_DOLLAR;
   }
   
   Integer get_cents(const Integer &) const { 
      return amount_in_cents % CENTS_PER_DOLLAR_INTEGER;
   }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_cents(Type &) const { return trunc(fmod(amount_in_cents, CENTS_PER_DOLLAR)); }
   
   template <typename Type, enable_if_t<is_floating_point<Type>::value, bool> = true>
   Type get_floating_amount() const { return amount_in_cents / CENTS_PER_DOLLAR; }
      
   operator string() const;
   
   T get_amount_in_cents() const { return amount_in_cents; }
   
   string get_currency() const { return currency; }
   
   void set_currency(const string & CURRENCY) {
      validate_currency(CURRENCY);
      currency = CURRENCY;
   }

   static T get_amount(const string & STR);
   
   Money operator*(const Money & ) = delete;
   Money operator*(Money) = delete;
   Money operator*(Money&&) = delete;
   Money operator*(Money&) = delete;
   Money operator*(const Money & ) const = delete;
   Money operator*(Money) const = delete;
   Money operator*(Money&&) const = delete;
   Money operator*(Money&) const = delete;
   
   Money operator/(Money) = delete;
   Money operator/(Money&&) = delete;
   Money operator/(Money&) = delete;
   Money operator/(const Money & ) = delete;
   Money operator/(Money) const = delete;
   Money operator/(Money&&) const = delete;
   Money operator/(Money&) const = delete;
   Money operator/(const Money & ) const = delete;
   
   Money operator%(const Money & ) = delete;
   Money operator%(Money) = delete;
   Money operator%(Money&&) = delete;
   Money operator%(Money&) = delete;
   Money operator%(const Money & ) const = delete;
   Money operator%(Money) const = delete;
   Money operator%(Money&&) const = delete;
   Money operator%(Money&) const = delete;
   
   friend Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b);
   friend Money<long double> operator+(const Money<long double>& a, const Money<long double>& b);
   
   template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> >
   friend Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);
   
   template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> >
   friend Money<Smaller> operator+(const Money<Smaller>& a, const Money<Smaller>& b);
   
   template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> >
   friend Money<Smaller> operator*(const Money<Smaller>&, const Smaller );
   
   template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> >
   friend Money<Smaller> operator*(const Money<Smaller>&, const Smaller);
   
   template<typename Greater, typename Smaller, enable_if_t<is_floating_point<Smaller>::value ||
            (is_integral<Smaller>::value && ! is_same<Greater, Integer>::value), bool> >
   friend void operator*=( Money<Smaller>& MONEY, const Smaller FACTOR);
   
   template<typename Greater, typename Smaller, enable_if_t<is_integral<Smaller>::value && is_same<Greater, Integer>::value, bool> >
   friend void operator*=( Money<Smaller>& MONEY, const Smaller FACTOR);
private:
   template <typename Greater>
   T calculate(const T & dollars, const long double cents = INCORRECT_CENTS   ) const;
   
   T calculate_by_Integer(const T & dollars, const long double cents = INCORRECT_CENTS) const;
   
   template<typename Greater, typename U = T, enable_if_t<is_floating_point<U>::value ||
            (is_integral<U>::value && ! is_same<Greater, Integer>::value), bool>  = true>
   Money& operator+=(const Money<U>& other) {
      static_assert(is_NOT_smaller<Greater, U>() && "is_NOT_smaller<Greater, U> required");
      U conversion = convert(other);
      Greater sum = Greater(this->amount_in_cents) + Greater(conversion);
      sum = Money<Greater>::round(sum);
      cerr << __func__ << " sum = " << sum << '\n';
      if (is_overflow<T, Greater>(sum))
         throw out_of_range(string(__func__) + " amount = " + std::to_string(sum) + " is overflow for type " + TYPE_NAME);
      this->amount_in_cents = static_cast<U>(sum);
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
      return *this;
   }
   
   template<typename Greater, typename U = T, enable_if_t<is_integral<U>::value && is_same<Greater, Integer>::value, bool>  = true>
   Money& operator+=(const Money<U>& other) {
      T conversion = convert(other);
      Integer sum = Integer::create_Integer(this->amount_in_cents) + Integer::create_Integer(conversion);
      cerr << __func__ << " sum = " << sum << '\n';
      if (Integer::is_overflow<T>(sum))
         throw out_of_range(string(__func__) + " amount = " + std::to_string(sum) + " is overflow for type " + TYPE_NAME);
      this->amount_in_cents = sum.operator U();
      cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents << '\n';
      return *this;
   }

   template<typename U = T, enable_if_t<is_same<U, Integer>::value, bool> = true>
   Integer calculate_amount_in_cents(const string & dollars) const {
      Integer result;
      size_t dot_position = dollars.find('.');
      cerr << __func__ << " dot_position = " << dot_position <<  '\n';
      string dollars_string = dollars.substr(0, dot_position);
      if (regex_match(dollars, MINUS_ZERO_REGEX)) {
         cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
         dollars_string = dollars_string.substr(1);
         cerr << __func__ << " dollars_string = " << dollars_string <<  '\n';
      }
      Integer dollars_part = Money<T>::get_amount(dollars_string);
      
      if (dot_position == string::npos) {
         cerr << __func__ << " dot_position = string::npos " << dot_position <<  '\n';
         result = calculate_by_Integer(dollars_part);
         cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents <<  '\n';
      }
      else {
         string cents_string = dollars.substr(dot_position + 1);
         if (cents_string.size() > 2)
            cents_string.insert(2, ".");
         cerr << __func__ << " cents_string = " << cents_string <<  '\n';
         long double cents_part = stold(cents_string);
         if (cents_string.size() == 1)
            cents_part *= 10;
         result = calculate_by_Integer(dollars_part, cents_part);
         if (dollars[0] == '-' && result > Integer::ZERO && result <= Integer::create_Integer(100))
            result = -result;
      }
      return result;
   }
   
   template<typename U = T, enable_if_t<is_floating_point<U>::value, bool>  = true>
   T calculate_amount_in_cents(const string & dollars) const {
      U dollars_part = Money<T>::get_amount(dollars);
      U result  = calculate<long double>(dollars_part);
      return result;
   }
   
   template<typename U = T, enable_if_t<is_integral<U>::value, bool>  = true>
   U calculate_amount_in_cents(const string & dollars) const {
      U result;
      size_t dot_position = dollars.find('.');
      cerr << __func__ << " dot_position = " << dot_position <<  '\n';
      string dollars_string = dollars.substr(0, dot_position);
      U dollars_part = Money<T>::get_amount(dollars_string);
      
      if (dot_position == string::npos) {
         cerr << __func__ << " dot_position = string::npos " << dot_position <<  '\n';
         result = calculate_by_Integer(dollars_part);
         cerr << __func__ << " this->amount_in_cents = " << this->amount_in_cents <<  '\n';
      }
      else {
         string cents_string = dollars.substr(dot_position + 1);
         if (cents_string.size() > 2)
            cents_string.insert(2, ".");
         cerr << __func__ << " cents_string = " << cents_string <<  '\n';
         long double cents_part = stold(cents_string);
         if (cents_string.size() == 1)
            cents_part *= 10;
         
         result = calculate_by_Integer(dollars_part, cents_part);
         if (dollars[0] == '-' && result > 0 && result <= 100)
            result = -result;
      }
      return result;
   }

   T amount_in_cents { };
   string currency = "PLN";
   static map <string, long double> rates_per_USD;
/*#ifdef __clang__
public:
   static void initialize_rates() {
      rates_per_USD = set_rates_per_USD(Network_library::ASIO, File_format::JSON);
   }
#endif  */ 
};

template <typename T>
map <string, long double> Money<T>::rates_per_USD = set_rates_per_USD(Network_library::CURL, File_format::XML);

}

#include "money.cpp"

#endif
