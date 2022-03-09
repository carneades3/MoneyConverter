#define DEBUG_OSTREAM

#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"
#include "money_init_test.hpp"
#include "money_operations_test.hpp"
#include "money_currency_test.hpp"

#include <functional>

using namespace std;
using namespace integer_space;
using namespace money;

//static std::ios_base::Init toEnsureInitialization;

template <typename Function, typename... Args>  
bool execute_function(Function && func, Args&&... args ) {
   try {
      func(forward<Args>(args)...);
      
      return true;
   } catch (const money::bad_from_string& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const integer_space::Arithmetic_Error & e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::bad_alloc & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }catch (const std::runtime_error& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const std::exception& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   
   return false;
}
/*
#ifdef __clang__
   inline void initialize_static_map() {
      Money<char>::initialize_rates();
      Money<Integer>::initialize_rates();
      Money<int_fast8_t>::initialize_rates();
      Money<short>::initialize_rates();
      Money<unsigned short>::initialize_rates();
      Money<int>::initialize_rates();
      Money<unsigned>::initialize_rates();
      Money<long>::initialize_rates();
      Money<unsigned long>::initialize_rates();
      Money<long long>::initialize_rates();
      Money<unsigned long long>::initialize_rates();
      Money<float>::initialize_rates();
      Money<double>::initialize_rates();
      Money<long double>::initialize_rates();
   }
#endif
*/
template <typename Function, typename... Args>  
static inline  bool call_function(Function && func, Args&&... args) {
   bool result = execute_function(func, forward<Args>(args)...);
   assert_many(result == true, "result == ", result);
   return result;
}

namespace downloaders_test {
   extern void downloaders_test();
}

bool perform_tests() {
#ifdef __clang__
   //initialize_static_map();
#endif
   initializer_list < function <void()> > vec = { bind(money_init_test::perform), 
      bind(money_operations_test::perform),
      bind(money_currency_test::perform),
      //bind(downloaders_test::downloaders_test)
      
   };
   bool result = false;
   for (auto fun : vec) {
      result = call_function(fun);
      if (false == result)
         break;
   }
   return result;   
}

int main() {
   const bool result = perform_tests();
   cerr << "\n" << __func__ << " END of ALL TESTS\n";
   if (result)
      return EXIT_SUCCESS;
   else
      return EXIT_FAILURE;
}
