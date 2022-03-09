#ifndef INTEGER_EXTREMUMS_TESTS_HPP
#define INTEGER_EXTREMUMS_TESTS_HPP

#include <limits>
#include <string>
#include <utility>
#include <type_traits>
#include <iostream>

#include "integer.hpp"
#include "money_init_test.hpp"

using std::pair;
using std::string;
using std::numeric_limits;
using money::Money;
using std::cerr;
using integer_space::Integer;

namespace money_init_test {
   template <typename T>
   extern
   Money<T> construct(const string & DOLLARS, bool creating, const string & expected = "", const string & currency = "PLN", const bool NOT_assert_currency = true);

   template <typename T>
   class Integers_extremums_test {
      static_assert(numeric_limits<T>::is_integer && "integer required.");
      //static const T MAX;// = numeric_limits<T>::max();
      //static const T MIN;// = numeric_limits<T>::lowest();
      
      static void test(const T & number) {
         pair<string, string> parameters = as_string(number);
         cerr << __func__ << " " << parameters.first << " " << parameters.second << '\n';
         construct<T>(parameters.first, true, parameters.second);
      }
      
      static pair<string, string> as_string(const T & number) {
         cerr << __func__ << " " << number << '\n';
         string s = std::to_string(number);
         string expected  = s;
         cerr << __func__ << " " << s << '\n';
         if (s.size() > 0 && s[0] == '+')
            s = s.substr(1);
         if (s.size() > 2) {
            expected = s.insert(s.size() - 2, ".");
            expected.replace(s.size() - 3, 1, ",");
         }
         else 
            expected.append(",00");
         pair<string, string> result {s, expected};
         cerr << __func__ << " " << result.first << " " << result.second << '\n';
         return result;
      }
   public:
      static void test() {
         static const T MAX = numeric_limits<T>::max();
         static const T MIN = numeric_limits<T>::lowest();
         test(MAX);
         test(MIN);
      }
   };
}
/*
template <typename T> 
const T Integers_extremums_test<T>::MAX = numeric_limits<T>::max();
template <typename T> 
const T Integers_extremums_test<T>::MIN = numeric_limits<T>::lowest();
*/

#endif
