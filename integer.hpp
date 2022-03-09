#ifndef INTEGER_HPP
#define INTEGER_HPP

#include<stdexcept>
#include<string>
#include<array>
#include<vector>
#include<iostream>
#include <limits>
#include <cmath>
#include <sstream>
#include <cmath>

using std::array;
using std::vector;
using std::invalid_argument;
using std::string;
using std::to_string;
using std::size_t;
using std::is_integral;
using std::is_floating_point;
using std::numeric_limits;
using std::out_of_range;
using std::ostream;
using std::istream;
using std::cerr;
using std::enable_if_t;
using std::isfinite;
using std::is_same;
using std::trunc;

//typedef int_fast8_t digit_type;
using digit_type = short;

namespace integer_space {
   
class Arithmetic_Error : public std::exception { 
   string msg {"!!! Arithmetic error: "};
public:
   Arithmetic_Error(const string& message) { msg += message; }
   const char* what() const noexcept {
      return msg.c_str();
   }
};

template <typename Number, enable_if_t<
              is_floating_point<Number>::value || is_integral<Number>::value, bool> = true>
inline bool equal_integer(const Number x) {
   return isfinite(x) && x == trunc(x);
}

class Integer;

Integer operator-(const Integer& first, const Integer& second);
Integer operator+(const Integer& first, const Integer& second);
bool operator<(const Integer& first, const Integer& second);
bool operator>(const Integer& first, const Integer& second);
bool operator==(const Integer& first, const Integer& second);

inline Integer operator+(const Integer& integer);

inline Integer& operator+=(Integer& first, const Integer& second);

inline Integer& operator-=(Integer& first, const Integer& second);
   
class Integer {
public:
   static constexpr short MAX_ARRAY_LENGTH = 40;
   static constexpr char  NEUTRAL   = ' ';
   static constexpr char  PLUS      = '+';
   static constexpr char  MINUS     = '-';
   static const string SIZE_INCORRECT;
   static const string SIGNUM_INCORRECT;
   static const Integer MIN;
   static const Integer MAX;
   static const long double FLOAT_MIN;
   static const long double FLOAT_MAX;
   static constexpr int_fast8_t BASE = 10;
   static const Integer ZERO;
   static const Integer ONE;
private:
   array<digit_type, MAX_ARRAY_LENGTH> integer_array { 0 };
   char signum { NEUTRAL };   // less than 0 for integers < 0, more than 0 for integers > 0, 0 for 0   
   explicit Integer(const array<digit_type, MAX_ARRAY_LENGTH> & ARRAY);
public:
   //Integer() { }
   
   Integer(const array<digit_type, MAX_ARRAY_LENGTH> & table, const char signum) {
      validate_init(table, signum);
   }
   
   explicit Integer(const long long int x) { 
      *this = create_Integer(x);
      //const string STR = to_string(x);
      //this->parse(STR); 
   }
   
   static Integer create_Integer(const Integer & other) { 
      Integer integer = other; 
      return integer;
   }
   /*
   explicit Integer(const long double x) { 
      const string STR = to_string(x);
      this->parse(STR); 
   }
   */
   template <typename Number, enable_if_t<is_floating_point<Number>::value, bool> = true>
   static Integer create_Integer(const Number x) { 
      if (! isfinite(x))
         throw invalid_argument(string(__func__) + ": Argument " + to_string(x) + " is not finite");
      cerr << __func__ << " FLOAT_MIN = " << FLOAT_MIN << " FLOAT_MAX = " << FLOAT_MAX << '\n';
      if (x < FLOAT_MIN || x > FLOAT_MAX)
         throw out_of_range(__func__ + string(" type: ") + typeid(Number).name() + " number: " + to_string(x) +
                                    " is out of range for Integer class ");
      long double truncated = trunc(x);
      long double abs_truncated = 
#if defined(__clang__)
                                    std::abs(truncated);
#elif defined(__GNUG__)
                                    std::abs(truncated);
                                    //std::fabs(truncated);
#endif
      //digit_type digit = 0;
      //if (*this < numeric_limits<Number>::lowest() || *this > numeric_limits<Number>::max())
      //   throw out_of_range(" Integer " + string(*this) + " is out of range for type " + typeid(Number).name());
      array<digit_type, MAX_ARRAY_LENGTH> table { 0 };
      for (int_fast8_t i = MAX_ARRAY_LENGTH - 1; abs_truncated > 0 && i >= 0; i--) {
         table[i] = fmod(abs_truncated, BASE); 
         abs_truncated /= 10;
         abs_truncated = trunc(abs_truncated);
      }
      //if (i < 0 && truncated > 0)
      //   throw out_of_range(" type: " + typeid(Number).name() + " number: " + to_string(x) + " is out of range for Integer class ");
      char signum = NEUTRAL;
      if (truncated < 0)
         signum = MINUS;
      else if(truncated > 0)
         signum = PLUS;
      Integer integer = Integer{ table, signum };  
      return integer;
   }
   
   template <typename Number, enable_if_t<is_integral<Number>::value, bool> = true>
   static Integer create_Integer(const Number x) { 
      const string STR = to_string(x);
      Integer integer = parse_create(STR);  
      return integer;
   }
   
   template <const unsigned int N>
   static Integer create_Integer(const array<digit_type, N> & table, const char signum) {
      static_assert( N <= MAX_ARRAY_LENGTH && N > 0 && "create_Integer requires 0 < N <= MAX_ARRAY_LENGTH");
      Integer integer;
      integer.validate_init(table, signum);
      return integer;
   }
   
   static Integer parse_create(const string& STR) {
      Integer integer;
      integer.parse(STR);
      return integer;
   }
   
   static Integer parse_create_signed_zero(const string& STR) {
      if (STR.size() < 2)
         throw invalid_argument("Size must be >= 2");
      Integer integer;
      if (PLUS == STR[0] || MINUS == STR[0])
         integer.parse(STR.substr(1));
      else
         integer.parse(STR);
      return integer;
   }
   
   Integer(const vector<digit_type> & vec, const char signum) {
      validate_size(vec);
      validate_init(vec, signum);
   }
   
   Integer() = default;
   ~ Integer() = default;
   Integer(const Integer &) = default;
   Integer(Integer &&) = default;
   Integer & operator=(const Integer &) = default;
   Integer & operator=(Integer &&) = default;
private:
   template <typename Container>
   static void validate_size(const Container& obj) {
      if (MAX_ARRAY_LENGTH < obj.size() || obj.size() <= 0)
         throw invalid_argument(Integer::SIZE_INCORRECT);
   }
   array<digit_type, MAX_ARRAY_LENGTH> get_integer_array() const { return integer_array; }
   
   template <typename Container>
   void validate_init(const Container& obj, char signum);
   
   template <typename Container>
   void validate_set(const Container & TABLE);
   
   static void assign_remainder(const Integer & DIVIDEND, const Integer & MULTIPLE_OF_DIVISOR, 
                                       vector<digit_type> & dividend_array);
   static void multiply_divisor(array<digit_type, MAX_ARRAY_LENGTH> & result_array, const short current_dividend_index,
       const Integer & DIVIDEND, vector<digit_type> & dividend_array, const Integer & DIVISOR);
   static array<digit_type, MAX_ARRAY_LENGTH> calculate_dividing(const array<digit_type, MAX_ARRAY_LENGTH> & DIVIDEND_ARRAY, 
               short current_dividend_index, const Integer & DIVISOR);
   static void validate(const char signum) {
      if (Integer::PLUS != signum && Integer::MINUS != signum && Integer::NEUTRAL != signum )
         throw invalid_argument(Integer::SIGNUM_INCORRECT);
   }
   
   friend void set_signum_adding_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, const int_fast8_t comparing_signum_result);
   friend void set_opposite_signum(Integer & result, const char signum);
   friend void set_signum_subtracting_Non_Zero_result(const Integer& first, const Integer& second,
                                       Integer & result, const int_fast8_t comparing_signum_result);
#ifdef TEST_INDEX_OPERATOR
public:
#endif
   digit_type & operator[](const size_t i) { return integer_array[i]; }
   
public:
   operator string() const;
   void set_integer_array(const vector<digit_type> & vec) {
      validate_size(vec);
      validate_set(vec);
   }
   
   string string_without_signum() const {
      string str = string(*this);
      switch (signum) {
         case NEUTRAL:
            return str;
         case MINUS:
         case PLUS:
            return str.substr(1);
      }
   }
   
   template <const unsigned int N>
   void set_integer_array(const array<digit_type, N> & table) {
      static_assert( N <= MAX_ARRAY_LENGTH && N > 0 && "set_integer_array requires 0 < N <= MAX_ARRAY_LENGTH");
      validate_set(table);
   }
   
      template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                 || is_integral<Number>::value), bool> = true>
   //template <typename Number, enable_if_t<is_integral<Number>::value, bool> = true>
   static bool is_overflow(const Integer & integer) {
      static const Integer lowest = create_Integer(numeric_limits<Number>::lowest());
      static const Integer max = create_Integer(numeric_limits<Number>::max());
      return integer < lowest || integer > max;
   }
   
   template <typename Number, enable_if_t<(is_floating_point<Number>::value 
                                 || is_integral<Number>::value), bool> = true>
   explicit operator Number() const { 
      //static_assert(! is_same<float, Number>);
      //cerr << " numeric_limits<Number>::lowest() = " << numeric_limits<Number>::lowest();
      //cerr << " \nnumeric_limits<Number>::max() = " << numeric_limits<Number>::max() << '\n';
      if (is_integral<Number>::value || is_same<Number, float>::value) {
         static const Integer lowest = create_Integer(numeric_limits<Number>::lowest());
         static const Integer max = create_Integer(numeric_limits<Number>::max());
         if (*this < lowest || *this > max)
            throw out_of_range(string(__func__) + " : Integer " + string(*this) + " is out of range for type " + typeid(Number).name());
      }
      Number number = (*this)[0];
      for (int_fast8_t i = 1; i < MAX_ARRAY_LENGTH; i++) 
         number = (*this)[i] + BASE * number;
      if (this->signum == MINUS)
         number = -number;
      return number;  
   }

   void reset_number_to_zero() {
      signum = NEUTRAL;
      integer_array.fill(0);
   }
   array<digit_type, MAX_ARRAY_LENGTH> get_integer_array_copy() const {
      array<digit_type, MAX_ARRAY_LENGTH> copy(integer_array);
      return copy;
   }
   
   void validate_signum(const char signum) const {
      if (signum == NEUTRAL && false == is_zero()) 
         throw invalid_argument("Requirement: can not set signum to neutral for nonzero integer");
      else if (signum != NEUTRAL && true == is_zero()) {
         throw invalid_argument("Requirement: can not set signum to other than neutral for zero integer");
      }
   }

   void set_signum(const char signum) {
      validate_signum(signum);
      this->signum = signum;
   }
   char get_signum() const { return signum; }
   size_t size() const { return integer_array.size(); }
   
   void parse(const string& STR);
   void parse_unsigned(const string& STR);
   bool is_zero() const { return is_zero(*this); }
   static bool is_zero(const Integer& integer);
   int_fast8_t compare_signum(const Integer& integer) {
      return Integer::compare_signum(*this, integer);
   }
   static int_fast8_t compare_signum(const Integer& first, const Integer& second);
   static bool is_absolute_value_greater(const Integer& first, const Integer& second);
   static bool is_absolute_value_less(const Integer& first, const Integer& second);
   static Integer divide_absolute_values(const Integer& DIVIDEND, const Integer& DIVISOR);
   static Integer add_absolute_values(const Integer & first, const Integer & second);
   static Integer subtract_absolute_values(const Integer & first, const Integer & second);
   static Integer multiply_absolute_values(const Integer & first, const Integer & second);
   static void detect_multiplication_overflow(const Integer & first, const Integer & second);
   
   const digit_type & operator[](const size_t i) const { return integer_array[i]; }
   
   friend Integer operator-(const Integer& integer);
   friend Integer operator*(const Integer& first, const Integer& second);
   friend Integer operator%(const Integer& DIVIDEND, const Integer& DIVISOR);
   friend Integer operator/(const Integer& dividend, const Integer& divisor);
   
      // prefix increment
   Integer& operator++() {
      operator+=(*this, ONE);
      return *this; // return new value by reference
   }

   // postfix increment
   Integer operator++(int) {
      Integer old = *this; // copy old value
      operator++();  // prefix increment
      return old;    // return old value
   }

   // prefix decrement
   Integer& operator--() {
      operator-=(*this, ONE);
      return *this; // return new value by reference
   }

   // postfix decrement
   Integer operator--(int) {
      Integer old = *this; // copy old value
      operator--();  // prefix decrement
      return old;    // return old value
   }
};

inline Integer operator+(const Integer& integer) {
   return integer;
}

inline Integer& operator+=(Integer& first, const Integer& second) {
   first = first + second;
   return first;
}

inline Integer& operator-=(Integer& first, const Integer& second) {
   first = first - second;
   return first;
}

inline Integer& operator*=(Integer& first, const Integer& second) {
   first = first * second;
   return first;
}

inline Integer& operator/=(Integer& first, const Integer& second) {
   first = first / second;
   return first;
}

inline Integer& operator%=(Integer& first, const Integer& second) {
   first = first % second;
   return first;
}

inline bool operator<=(const Integer& first, const Integer& second) {
   return ! (first > second);
}

inline bool operator>=(const Integer& first, const Integer& second) {
   return ! (first < second);
}

inline bool operator!=(const Integer& first, const Integer& second) {
   return !(first == second);
}

inline ostream& operator<<(ostream& os, const Integer & integer) {
   return os << string(integer);
}

inline istream& operator>>(istream& is, Integer & integer) {
   string integer_string;
   is >> integer_string;
   if (! is)
      throw std::runtime_error("Istream error when reading Integer");
   
   integer.parse(integer_string);
   return is;
}

template <typename Container>   // private
void Integer::validate_init(const Container& TABLE, const char signum) {
   //validate_size(TABLE);
   const size_t SIZE = TABLE.size();
   validate(signum);
   digit_type number;
   unsigned short zeros = 0;
   for (short source_index = SIZE - 1; source_index >= 0 ; source_index--) {
      number = TABLE[source_index];
      if (number == 0) 
         zeros++;
      else if (number < 0 || number > 9)
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9");
   }
   if (zeros == SIZE && signum != NEUTRAL) 
      throw invalid_argument("Requirement: signum must be neutral for array with only zeros");
   if (zeros < SIZE && signum == NEUTRAL) 
      throw invalid_argument("Requirement: signum can not be neutral for array with element other than zero");
   short dest_index = MAX_ARRAY_LENGTH - 1;
   for (short source_index = SIZE - 1; source_index >= 0 ; source_index--) {
      number = TABLE[source_index];
      (*this).integer_array[dest_index] = number;
      dest_index--;
   }
   this->signum = signum;
}

template <typename Container>  // private
void Integer::validate_set(const Container & TABLE) {  // assume that integer_array is positive number, change of signum is allowed by set_signum()
   this->integer_array.fill(0);  // not call reset_number_to_zero() due to change signum (in set_integer_array() save previous signum)
   digit_type number;
   short dest_index = MAX_ARRAY_LENGTH - 1;
   if (TABLE.size() == 0)
      std::cerr << "\n                          TABLE.size() = " << TABLE.size() << '\n';
   for (short source_index = TABLE.size() - 1; source_index >= 0 ; source_index--) {
      number = TABLE[source_index];
      if (number < 0 || number > 9) {
         reset_number_to_zero();    // similar to constructor, reset to zero
         throw invalid_argument("Requirement: in array must be only integers from 0 to 9");
      }
      this->integer_array[dest_index] = number;
      dest_index--;
   }
   if (this->signum == NEUTRAL && false == is_zero())
      this->signum = PLUS;
   else if (this->signum != NEUTRAL && is_zero()) 
      this->signum = NEUTRAL;
}
   
}

namespace std {
using Integer = integer_space::Integer;

string to_string(const Integer& integer);
bool signbit(const Integer & integer);
Integer abs(const Integer & integer);

template<> class numeric_limits<Integer> {
public:
   static constexpr bool is_specialized = true;
   static constexpr bool is_signed = true;
   static constexpr bool is_integer = true;
   static constexpr bool is_exact = true;
   static constexpr bool has_infinity = false;
   static constexpr bool has_quiet_NaN = false;
   static constexpr bool has_signaling_NaN = false;
   static constexpr bool is_modulo = false;
   static constexpr bool traps = true;
   static constexpr bool tinyness_before = false;
   
   static Integer lowest() { return Integer::MIN; };
   static Integer min() { return Integer::MIN; };
   static Integer max() { return Integer::MAX; };
};
}

#endif
