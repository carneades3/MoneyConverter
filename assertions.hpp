#ifndef ASSERTIONS_HPP
#define ASSERTIONS_HPP

//#define NDEBUG
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <tuple>

#include "integer.hpp"

using std::cerr;
using std::string;
using std::ostringstream;
using std::ios_base;
using std::tuple;
using std::enable_if;
using std::get;
using std::abort;
using std::tie;

using integer_space::Integer;

template <class Pointer, std::enable_if_t<std::is_pointer<Pointer>::value
                                          || std::is_array<Pointer>::value, bool> = true> 
inline void validate_pointer(Pointer pointer) {
   if (nullptr == pointer)
      throw invalid_argument(string(typeid(Pointer).name()) + " pointer is null");   
}

inline void validate_pointer(std::nullptr_t) {
   throw invalid_argument("type: nullptr_t - pointer is null");   
}

#define assert_many(EX,...) \
  ((EX) || (assert_tuples (tie(__VA_ARGS__), #EX, __FILE__, __LINE__, __DATE__, __TIME__)))
  
#define ASSERT(expr) \
   if (expr) \
      {} \
   else \
      GENERAL_ASSERT(#expr, expr, __FILE__, __LINE__, __DATE__, __TIME__, "")

#ifndef NDEBUG
#  define general_assert(Expr, Msg) \
   GENERAL_ASSERT(#Expr, Expr, __FILE__, __LINE__, __DATE__, __TIME__,  Msg)
#else
#   define general_assert(Expr, Msg) ;
#endif
    
#ifndef NDEBUG
#  define integer_assert(Integer, Signum_Char, Number_String) \
   INTEGER_ASSERT(Integer, Signum_Char, Number_String, __FILE__, __LINE__, __DATE__, __TIME__)
#else
#   define integer_assert(Integer, Signum_Char, String) ;
#endif
   
#ifndef NDEBUG
#  define assert_Integer(Integer, Number_String) \
   ASSERT_INTEGER(Integer, Number_String, __FILE__, __LINE__, __DATE__, __TIME__)
#else
#   define assert_Integer(Integer, String) ;
#endif
   
#ifndef NDEBUG
#  define assert_number_Integer(LongLong, Integer) \
   ASSERT_NUMBER_INTEGER<long long>(LongLong, Integer, __FILE__, __LINE__, __DATE__, __TIME__)
#else
#   define assert_number_Integer(LongLong, Integer) ;
#endif

inline void GENERAL_ASSERT(const char* expr_str, const bool expr, const char* file, const int line, 
                const char* date, const char* time, const string& msg) {
    if (! expr) {
      cerr << "\nAssert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n"
            << "Date:\t\t"   << date  << "\n"
            << "Time:\t\t"   << time  << "\n";
      //exit(EXIT_FAILURE);
      std::terminate();
   }
}

inline void INTEGER_ASSERT(const Integer& x, const char expected_signum, const string& expected_number,
                  const char* file, const int line,
                  const char* date, const char* time) {
   const string signum_str = expected_signum == Integer::NEUTRAL ? "" : string(1, expected_signum);
   if (static_cast<string>(x) != signum_str + expected_number) {
      const string EXPECTED = signum_str + expected_number;
      const string message = " " + static_cast<string>(x) +
                        " != expected " + EXPECTED;
      GENERAL_ASSERT(EXPECTED.c_str(), static_cast<string>(x) == EXPECTED, file, line, date, time, message);
   }
}

inline void ASSERT_INTEGER(const Integer& x, const string& expected_number,
                  const char* file, const int line,
                  const char* date, const char* time) {
   const string signum_str = expected_number == "0" ? "" : string(1, x.get_signum());
   if (static_cast<string>(x) != signum_str + expected_number) {
      const string EXPECTED = signum_str + expected_number;
      const string message = " " + static_cast<string>(x) +
                        " != expected " + EXPECTED;
      GENERAL_ASSERT(EXPECTED.c_str(), static_cast<string>(x) == EXPECTED, file, line, date, time, message);
   }
}

template <typename T>
inline void ASSERT_NUMBER_INTEGER(const T & NUMBER, const Integer & OBJECT,
                     const char* file, const int line,
                     const char* date, const char* time) {
   ostringstream stream;
   if (0 != NUMBER)
      stream.setf(ios_base::showpos);
   stream << NUMBER;
   const string number_string = stream.str();
   const string object_string = string(OBJECT);
   if (number_string != object_string) {
      const string message = "\n number = " + number_string + "\n object = " + object_string;
      GENERAL_ASSERT(object_string.c_str(), number_string == object_string, file, line, date, time, message);
   }
} 
      /*  
template <typename ...Args>
string print_tuple(tuple<Args...> tp) {
  return ""; //print the tuple...
}
        
void AsserT(const string & info, const char * msg, const char* file, const int line,
                     const char * date, const char * time) {
   cerr << "\nAssertion failed:\n"
            << "Expression:\t" << msg << "\n"
            << "Source file:\t\t" << file << ", line " << line << "\n"
            << "Date:\t\t"   << date  << "\n"
            << "Time:\t\t"   << time  << "\n"
            << "  Info: " << info << '\n' ;
   std::abort();
}
*/
template<const unsigned int N>
struct print_tuple {
   template<typename... T>
   static typename enable_if<(N < sizeof...(T))>::type
   print(ostream& os, const tuple<T...>& t) {
      os << " " << get<N>(t);
      print_tuple<N + 1>::print(os, t);
   }
   
   template<typename... T>
   //static typename enable_if<! (N<sizeof...(T))>::type
   static typename enable_if<(N >= sizeof...(T))>::type
   //static typename enable_if_t<! (N<sizeof...(T))>
   print(ostream& os, const tuple<T...>&) { }    // for empty tuple
};

inline ostream& operator<< (ostream & os, const tuple<> &) {
   return os << "";
}

template<typename S, typename... T>
inline ostream& operator<< (ostream & os, const tuple<S, T...> & t) {
   os << get<0>(t);
   print_tuple<1>::print(os, t);
   return os;
}
/*
template <typename ...Args>
string print_tuple(tuple<Args...> tp) {
  return ""; //print the tuple...
}
*/
template <typename ...Args>
bool assert_tuples(tuple<Args...> info, const char *msg, const char *file, int line,
                   const char * date, const char * time) {
   cerr << "\nAssertion failed:\n"
            << "Expression:\t" << msg << "\n"
            << "Source file:\t\t" << file << ", line " << line << "\n"
            << "Date:\t\t"   << date  << "\n"
            << "Time:\t\t"   << time  << "\n"
            << "  Info: " << info << '\n' ;
   abort();
   //assert(false);
   //return false;
}

/*
#define assert_many(EX,...) \
  (void)((EX) || (AsserT(print(__VA_ARGS__),#EX,__FILE__, __LINE__, __DATE__, __TIME__),0))
  */
/*
template <typename T>
void ASSERT_MONEY(const Money<T>& money, const string& expected_money,
                  const char* file, const int line,
                  const char* date, const char* time) {
   //const string signum_str = expected_number == "0" ? "" : string(1, x.get_signum());
   if (static_cast<string>(money) != expected_money) {
      //const string EXPECTED = signum_str + expected_number;
      const string message = " " + static_cast<string>(x) +
                        " != expected " + EXPECTED;
      GENERAL_ASSERT(expected_money.c_str(), static_cast<string>(money) == expected_money,
                     file, line, date, time, message);
   }
}
*/
#endif
