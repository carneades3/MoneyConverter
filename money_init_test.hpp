#ifndef MONEY_INIT_TEST_HPP
#define MONEY_INIT_TEST_HPP

#include "money.hpp"

using money::Money;

namespace money_init_test {
   
   constexpr long double CONVERTER = (long double)(money::CENTS_PER_DOLLAR);

   template <typename T>
   struct Creation {
      template<typename... Args>
      Money<T> operator()(Args...args) const {
         return Money<T>::create(std::forward<Args>(args)...);
      }
   };
   
   template <typename T, typename Function, typename... Args>  
   void construct_incorrect(Function && f, Args&&... args ) { 
      try {
         f(std::forward<Args>(args)...);
         T obj = f(std::forward<Args>(args)...);
         cerr << __func__ << " !!!!!!!!!!!!!!!!!!" << obj << std::endl;
         assert(0);
      } catch (const std::invalid_argument& e) {
         cerr << __func__ << " " << typeid(e).name() << " " << e.what() << std::endl;
      } catch (const std::out_of_range& e) {
         cerr << __func__ << " " << typeid(e).name() << " " << e.what() << std::endl;
      } catch (const std::bad_cast& e) {
         cerr << __func__ << " " << typeid(e).name() << " " << e.what() << std::endl;
      }
   }

   void perform();
}

#endif
