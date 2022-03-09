#define DEBUG_OSTREAM

#include "integer.hpp"
#include "assertions.hpp"
#include "money.hpp"

#include <cmath>

#include "integers_extremums_tests.hpp"
#include "money_operations_test.hpp"
#include "money_init_test.hpp"
#include "money_utility_test.hpp"

#include <functional>

using namespace std;
using namespace integer_space;
using namespace money;
using namespace money_init_test;

using std::is_member_function_pointer;
using std::mem_fn;
/*
template <typename T> 
extern
Money<T> construct_cents(const string & DOLLARS, const long double CENTS, bool creating, const string & expected = "", const string & currency = "PLN");

template <typename T> 
extern
Money<T> construct(const string & DOLLARS, bool creating, const string & expected);
*/
namespace money_operations_test {
   
template <typename Function, typename... Args>  
void operation_failed(Function && f, Args&&... args ) { 
   try {
      f(std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

template <typename Type> 
void binary_operation(Type& (Type::*func)(const Type&), const string & A_DOLLARS, const string & B_DOLLARS, 
                      const string & expected = "") { 
   Type       A(A_DOLLARS);
   const Type B(B_DOLLARS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

template <typename Type> 
void binary_operation(Type& (Type::*func)(const Type&), const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") { 
   Type       A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

template <typename Type> 
void binary_operation(Type (Type::*func)(const Type&) const, const string & A_DOLLARS, const string & B_DOLLARS, 
                      const string & expected = "") { 
   const Type       A(A_DOLLARS);
   const Type B(B_DOLLARS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
}

template <typename Type> 
void binary_operation(Type (Type::*func)(const Type&) const, const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") {
   const Type       A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
}

template <typename Type>
string get_operator_name(bool (Type::*func)(const Type&) const) {
   if (func == &Type::operator==)
      return "operator==";
   if (func == &Type::operator!=)
      return "operator!=";
   if (func == &Type::operator>=)
      return "operator>=";
   if (func == &Type::operator<=)
      return "operator<=";
   if (func == &Type::operator>)
      return "operator>";
   if (func == &Type::operator<)
      return "operator<";
   
   throw std::invalid_argument(string(__func__) + ": not matched operator  ");
}

/*
template <typename Type> 
inline void print_assert_bool(const bool RESULT, const bool expected, bool (Type::*func)(const Type&) const, const Type& A, const Type& B) {
   cerr << std::boolalpha << " RESULT IS " << (RESULT) << '\n'; 
   if((RESULT) != (expected)) { 
      cerr << " " << A << " " << get_operator_name<Type>(func) << " " << B << " returns unexpected result "  << '\n';
      assert(false); 
   }
}
*/
#define print_assert_bool(RESULT, expected, func, A, B) \
   cerr << std::boolalpha << " RESULT IS " << (RESULT) << '\n'; \
   if((RESULT) != (expected)) { \
      cerr << " " << A << " " << get_operator_name<Type>(func) << " " << B << " returns unexpected result "  << '\n'; \
      assert(false); \
   }

template <typename Type> 
void binary_operation(bool (Type::*func)(const Type&) const, const string & A_DOLLARS, const string & B_DOLLARS, 
                      const bool expected) { 
   const Type       A(A_DOLLARS);
   const Type B(B_DOLLARS);
   bool RESULT = (A.*func)(B);
   print_assert_bool(RESULT, expected, func, A, B);
}

template <typename Type> 
void binary_operation(bool (Type::*func)(const Type&) const, const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const bool expected) { 
   const Type       A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   bool RESULT = (A.*func)(B);
   print_assert_bool(RESULT, expected, func, A, B);
}

template <typename Type, typename Function> 
void binary_operation(Function && func, const string & A_DOLLARS, const string & B_DOLLARS, const string & expected = "") { 
   const Type A(A_DOLLARS);
   const Type B(B_DOLLARS);
   const Type RESULT = func(A, B);
   print_assert(RESULT, expected);
}

template <typename Type, typename Function> 
void binary_operation(Function && func, const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") { 
   const Type A(A_DOLLARS, A_CENTS);
   const Type B(B_DOLLARS, B_CENTS);
   const Type RESULT = func(A, B);
   print_assert(RESULT, expected);
}

template <typename Type, template<typename> class Template> 
void binary_operation(Template<Type>& (Template<Type>::*func)(const Type&), const string & A_DOLLARS, const Type & FACTOR, 
                      const string & expected = "") { 
   Template<Type>       A(A_DOLLARS);
   Template<Type> RESULT = (A.*func)(FACTOR);
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

template <typename Type, template<typename> class Template> 
void binary_operation(Template<Type>& (Template<Type>::*func)(const Type&), const string & A_DOLLARS, const long double A_CENTS, 
                      const Type & FACTOR,  const string & expected = "") { 
   Template<Type>       A(A_DOLLARS, A_CENTS);
   Template<Type> RESULT = (A.*func)(FACTOR);
   print_assert(RESULT, expected);
   print_assert(A, expected);
}

#define MULTIPLY_TEST(A) \
   RESULT = A * FACTOR; \
   Template<Type> RESULT_2 = FACTOR * A; \
   cerr << " A * FACTOR " << RESULT << '\n'; \
   cerr << " FACTOR * A " << RESULT_2 << '\n'; \
   assert(RESULT == RESULT_2 && "A * FACTOR != FACTOR * A")

template <typename Type, template<typename> class Template > 
void binary_operation(Template<Type> (Template<Type>::*func)(const Type&) const, const string & A_DOLLARS, const Type & FACTOR,
                      const string & expected = "") { 
   const Template<Type>       A(A_DOLLARS);
   Template<Type> RESULT = (A.*func)(FACTOR);
   print_assert(RESULT, expected);
   MULTIPLY_TEST(A);
}

template <typename Type, template<typename> class Template> 
void binary_operation(Template<Type> (Template<Type>::*func)(const Type&) const, const string & A_DOLLARS, const long double A_CENTS, 
                      const Type & FACTOR, const string & expected = "") {
   const Template<Type>       A(A_DOLLARS, A_CENTS);
   Template<Type> RESULT = (A.*func)(FACTOR);
   print_assert(RESULT, expected);
   MULTIPLY_TEST(A);
}

#undef MULTIPLY_TEST

template <typename Type, template<typename> class Template, typename Func_1, typename Func_2 >
void binary_operation(Func_1 && func_1, Func_2 && func_2, const Template<Type> & OBJECT, const Type & FACTOR, 
                      const string & expected = "") { 
   Template<Type> RESULT_1 = func_1(OBJECT, FACTOR);
   print_assert(RESULT_1, expected);
   Template<Type> RESULT_2 = func_2(FACTOR, OBJECT);
   assert(RESULT_1 == RESULT_2 && " RESULT_1 != RESULT_2 ");
   print_assert(RESULT_2, expected);
}

template <typename Type, template<typename> class Template, typename Func_1, typename Func_2
   , enable_if_t<is_member_function_pointer<Func_1>::value, bool> = true >
inline void binary_operation(Func_1 && func_1, Func_2 && func_2, const string & A_DOLLARS, const Type & FACTOR, 
                      const string & expected = "") { 
   const Template<Type> OBJECT(A_DOLLARS);
   auto member_func = std::mem_fn(func_1);
   binary_operation(member_func, func_2, OBJECT, FACTOR, expected);
}

template <typename Type, template<typename> class Template, typename Func_1, typename Func_2,
   enable_if_t<is_member_function_pointer<Func_1>::value, bool> = true > 
inline void binary_operation(Func_1 && func_1, Func_2 && func_2, const string & A_DOLLARS, const long double A_CENTS, 
                      const Type & FACTOR, const string & expected = "") { 
   const Template<Type> OBJECT(A_DOLLARS, A_CENTS);
   auto member_func = std::mem_fn(func_1);
   binary_operation(member_func, func_2, OBJECT, FACTOR, expected);
}
/*
template <typename Type> 
void unary_operation(Type& (Type::*func)(const Type&), const string & A_DOLLARS, const string & expected = "") { 
   Type       A(A_DOLLARS);
   Type RESULT = (A.*func)();
   print_assert(RESULT, expected);
   print_assert(A, expected);
}
*/

template <typename Type>
string get_operator_name(Type (Type::*func)() const) {
   /* Technical details: pointers to member functions and pointers to data are not necessarily represented in the same way. A pointer to a member function might be a data structure rather than a single pointer. Think about it: if it’s pointing at a virtual function, it might not actually be pointing at a statically resolvable pile of code, so it might not even be a normal address — it might be a different data structure of some sort.  */
   typedef Type (Type::*Member_func)() const;
   Member_func plus = &Type::operator+;   /* member function pointer    */
   Member_func minus = &Type::operator-;   /* member function pointer    */
   if (func == plus)
      return "operator+";
   if (func == minus)
      return "operator-";
   
   throw std::invalid_argument(string(__func__) + ": not matched operator  ");
}

template <typename Type> 
void unary_operation(Type (Type::*func)() const, const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") { 
   const Type A(A_DOLLARS, A_CENTS);
   const Type RESULT = (A.*func)();
   cerr << __func__ << " Tested of " << get_operator_name(func)   << '\n';
   print_assert(RESULT, expected);
}

template <typename Type> 
void unary_operation(Type (Type::*func)() const, const string & A_DOLLARS, const string & expected = "") { 
   const Type A(A_DOLLARS);
   const Type RESULT = (A.*func)();
   cerr << __func__ << " Tested of " << get_operator_name(func)   << '\n';
   print_assert(RESULT, expected);
}

template <typename Binary_Oper_Type, typename Function, typename... Args>  
void binary_operation_failed(Function && f, Args&&... args ) { 
   try {
      cerr << __func__ <<  endl;
      binary_operation<Binary_Oper_Type>(f, std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}

template <typename Unary_Oper_Type, typename Function, typename... Args>  
void unary_operation_failed(Function && f, Args&&... args ) { 
   try {
      unary_operation<Unary_Oper_Type>(f, std::forward<Args>(args)...);
      assert(0 && "---------------------- operation should be failed !!!");
   } catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const out_of_range& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } catch (const bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
}
/*
template <typename Type> 
void unary_operation(Type (Type::*func)(const Type&) const, const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") { 
   Type       A(A_DOLLARS, A_CENTS);
   Type RESULT = (A.*func)(B);
   print_assert(RESULT, expected);
}
*/

const Integer HUNDRED = Integer::create_Integer(100);
const Integer TEN = Integer::create_Integer(10);
      
template <typename Type, template<typename> class Template = Money>
struct Dollars_string {
public:
   const string a_dollars;
   const string b_dollars;
   
   Dollars_string (const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS) : a_dollars(dollars_string(A_DOLLARS, A_CENTS)),
                                                   b_dollars(dollars_string(B_DOLLARS, B_CENTS))
                                                   { }
                                                   
   Dollars_string (const string & A_DOLLARS, const long double A_CENTS) // FOR operator*
                  : a_dollars(dollars_string(A_DOLLARS, A_CENTS)) 
                           { }
private:
   template <typename U = Type, enable_if_t<false == is_same<U, Integer>::value, bool>  = true>
   string dollars_string(const string & DOLLARS, const long double CENTS) {
      string dollars;
      string cents;
      Type cents_rounded = Template<Type>::round(CENTS);  
      if (cents_rounded < 100) 
         dollars = DOLLARS;
      else {
         Type dollars_number =  Template<Type>::get_amount(DOLLARS);
         if (dollars_number <= 0 && DOLLARS.find(Integer::MINUS) != string::npos)
            dollars_number -= 1;
         else
            dollars_number += 1;
         dollars = std::to_string(static_cast<int>(dollars_number));
         cents_rounded -= 100;
      } 
      cents = std::to_string(static_cast<int>(cents_rounded));
      if (cents_rounded < 10)
         cents.insert(0, "0");
      const string RESULT = dollars + "." + cents;
      cerr << '\n' << __func__ << " RESULT = " << RESULT << '\n';
      return RESULT;
   }
   
   template <typename U = Type, enable_if_t<is_same<U, Integer>::value, bool>  = true>
   string dollars_string(const string & DOLLARS, const long double CENTS) {
      string dollars;
      string cents;
      Integer cents_rounded = Template<Integer>::round(CENTS);  
      if (cents_rounded < HUNDRED)
         dollars = DOLLARS;
      else {
         Integer dollars_number; 
         try {
            dollars_number = Integer::parse_create(DOLLARS);
            ++dollars_number;
         } catch (const invalid_argument & e) {
            if (dollars_number <= Integer::ZERO && DOLLARS.find(Integer::MINUS) != string::npos) {
               dollars_number = Integer::parse_create_signed_zero(DOLLARS);
               --dollars_number;
            }
            else
               throw;
         } 
         dollars = dollars_number.operator string();
         cents_rounded -= HUNDRED;
      }
      cents = cents_rounded.string_without_signum();
      if (cents_rounded < TEN)
         cents.insert(0, "0");
      const string RESULT = dollars + "." + cents;
      cerr << '\n' << __func__ << " RESULT = " << RESULT << '\n';
      return RESULT;
   }
   /*
   template <typename U = Type, enable_if_t<is_same<U, Integer>::value, bool>  = true>
   string cents_string(const Type cents_number) { 
      return cents_number.string_without_signum();
   }
   
   template <typename U = Type, enable_if_t<false == is_same<U, Integer>::value, bool>  = true>
   string cents_string(const Type cents_number) { 
      return std::to_string(cents_number);
   }*/
};

/*
template <>
string Dollars_string<Integer, Money>::cents_string(const long double CENTS) {
   Integer cents_number = Money<Integer>::round(CENTS);
   string cents = cents_number.string_without_signum();
   return cents;
}

template <typename Type, template<typename> class Template>
string Dollars_string<Type, Template>::cents_string(const long double CENTS) {
   Type cents_number = Template<Type>::round(CENTS);  
   string cents = std::to_string(cents_number);
   return cents;
}
*/
string replace_string(const string & S, const string remove, const string insert) {
   const size_t pos = S.find(remove);
   string result = S;
   if (pos != string::npos)
      result.replace(pos, remove.size(), insert);
   return result;
}

template <typename Type, template<typename> class Template = Money>
class Test_unary : public Dollars_string<Type, Template> { 
   static_assert(numeric_limits<Type>::is_signed && "Type signed required");
public:
   Test_unary(const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") : 
                      Dollars_string<Type, Template>(A_DOLLARS, A_CENTS, "", 0.0L) {
      cerr << "\n\n#########################" << __func__ << '\n';
      unary_operation<Template<Type>> (&Template<Type>::operator+, A_DOLLARS, A_CENTS, replace_dot_by_comma());
      unary_operation<Template<Type>> (&Template<Type>::operator-, A_DOLLARS, A_CENTS, expected);
      unary_operation<Template<Type>> (&Template<Type>::operator+, Dollars_string<Type, Template>::a_dollars, replace_dot_by_comma());
      unary_operation<Template<Type>> (&Template<Type>::operator-, Dollars_string<Type, Template>::a_dollars, expected);
   }
private:
   string replace_dot_by_comma() {
      string expected_plus = replace_string(Dollars_string<Type, Template>::a_dollars, ".", ",");
      if (expected_plus == "-0,00")
         expected_plus.erase(0, 1);
      cerr << "\n\n#" << __func__ << " expected_plus = " << expected_plus << '\n';
      return expected_plus;
   }
};

template <typename Type, template<typename> class Template = Money>
class Failed_test_unary : public Dollars_string<Type, Template> { 
public:
static_assert(numeric_limits<Type>::is_signed && "Type signed required");
   Failed_test_unary(const string & A_DOLLARS, const long double A_CENTS, const string & expected = "") : 
                      Dollars_string<Type, Template>(A_DOLLARS, A_CENTS, "", 0.0L) {
      cerr << "\n\n#########################" << __func__ << '\n';
      unary_operation_failed<Template<Type>, Template<Type> (Template<Type>::*)() const> (&Template<Type>::operator-, A_DOLLARS, A_CENTS, expected);
   }
};

template <typename Type, template<typename> class Template = Money>
class Test_adding : public Dollars_string<Type, Template> { 
   using S = Dollars_string<Type, Template>;
public:
   Test_adding(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") : 
                      S(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) {
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation<Template<Type>, Template<Type>(const Template<Type>&, const Template<Type>&)> (operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator+=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>, Template<Type>(const Template<Type>&, const Template<Type>&)> (operator+, S::a_dollars, S::b_dollars, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator+=, S::a_dollars, S::b_dollars, expected);
   }
};

template <typename Type, template<typename> class Template = Money>
class Test_subtracting : public Dollars_string <Type, Template> { 
public:
   using S = Dollars_string<Type, Template>;
   
   Test_subtracting(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const string & expected = "") :
                      S(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) { 
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation<Template<Type>> (&Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator-, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator-=, S::a_dollars, S::b_dollars, expected);
      binary_operation<Template<Type>> (&Template<Type>::operator-, S::a_dollars, S::b_dollars, expected);
   }
};

template <typename Type, template<typename> class Template = Money>
struct Test_multiplying : public Dollars_string <Type, Template> { 
public:
   using S = Dollars_string<Type, Template>;
   
   Test_multiplying(const string & A_DOLLARS, const long double A_CENTS, const string & FACTOR, const string & expected = "")
                      : Test_multiplying(A_DOLLARS, A_CENTS, Template<Type>::get_amount(FACTOR), expected) { 
   }
   
   Test_multiplying(const string & A_DOLLARS, const long double A_CENTS, const Type & FACTOR, const string & expected = "")
                      : S(A_DOLLARS, A_CENTS) { 
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation<Type, Template, Template<Type> (Template<Type>::*)(const Type&) const, Template<Type> (const Type&, const Template<Type>&)> (&Template<Type>::operator*, operator*, A_DOLLARS, A_CENTS, FACTOR, expected);
      binary_operation<Type, Template, Template<Type> (Template<Type>::*)(const Type&) const, Template<Type> (const Type&, const Template<Type>&)> (&Template<Type>::operator*, operator*, S::a_dollars, FACTOR, expected);
      
      binary_operation<Type, Template> (&Template<Type>::operator*=, A_DOLLARS, A_CENTS, FACTOR, expected);
      binary_operation<Type, Template> (&Template<Type>::operator*, A_DOLLARS, A_CENTS, FACTOR, expected);
      binary_operation<Type, Template> (&Template<Type>::operator*=, S::a_dollars, FACTOR, expected);
      binary_operation<Type, Template> (&Template<Type>::operator*, S::a_dollars, FACTOR, expected);
   }
};

template <typename Type, template<typename> class Template = Money>
class Test_relations : public Dollars_string <Type, Template> { 
public:
   using S = Dollars_string<Type, Template>;
   Test_relations(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, 
                      const long double B_CENTS, const bool equality, const bool smaller) :
                      S (A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) { 
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation<Template<Type>> (&Template<Type>::operator==, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, equality);
      binary_operation<Template<Type>> (&Template<Type>::operator!=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, ! equality);
      binary_operation<Template<Type>> (&Template<Type>::operator==, S::a_dollars, S::b_dollars, equality);
      binary_operation<Template<Type>> (&Template<Type>::operator!=, S::a_dollars, S::b_dollars, ! equality);
      binary_operation<Template<Type>> (&Template<Type>::operator<=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, equality || smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, equality || ! smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator<=, S::a_dollars, S::b_dollars, equality || smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>=, S::a_dollars, S::b_dollars, equality || ! smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator<, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS, ! smaller && ! equality);
      binary_operation<Template<Type>> (&Template<Type>::operator<, S::a_dollars, S::b_dollars, smaller);
      binary_operation<Template<Type>> (&Template<Type>::operator>, S::a_dollars, S::b_dollars, ! smaller && ! equality);
   }
};

template <typename Type, template<typename> class Template = Money>
class Failed_test_adding : public Dollars_string<Type, Template> { 
public:
   using S = Dollars_string<Type, Template>;
   Failed_test_adding(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, const long double B_CENTS) :
                      S(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) { 
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator+=,
                     A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator+=, S::a_dollars, S::b_dollars);
      using Alias = Template<Type>;
      binary_operation_failed< Alias, Alias(const Alias&, const Alias&)>(operator+, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed< Alias, Alias(const Alias&, const Alias&)>(operator+, S::a_dollars, S::b_dollars);
   }
};

template <typename Type, template<typename> class Template = Money>
class Failed_test_subtracting : public Dollars_string<Type, Template> { 
public:
   using S = Dollars_string<Type, Template>;
   Failed_test_subtracting(const string & A_DOLLARS, const long double A_CENTS, const string & B_DOLLARS, const long double B_CENTS) :
                      S(A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS) {
      cerr << "\n\n#########################" << __func__ << '\n';
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator-=, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed<Template<Type>, Template<Type>& (Template<Type>::*)(const Template<Type>&)>(&Template<Type>::operator-=, S::a_dollars, S::b_dollars);
      using Alias = Template<Type>;
      binary_operation_failed<Alias, Alias (Alias::*)(const Alias&) const>(&Alias::operator-, A_DOLLARS, A_CENTS, B_DOLLARS, B_CENTS);
      binary_operation_failed<Alias, Alias (Alias::*)(const Alias&) const>(&Alias::operator-, S::a_dollars, S::b_dollars);
   }
};

template <typename T, template<typename> class Template = Money>
void correct_adding() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         Test_adding<T>("-1", 28, "-0", 2, "-1,30");
         if (! is_same<T, short>::value)
            Test_adding<T>("100", 23, "-450", 34, "-350,11");
      }
      Test_adding<T, Template>("-0", 23, "-0", 34, "-0,57");
      Test_adding<T, Template>("-0", 23, "0", 34, "0,11");
      Test_adding<T, Template>("0", 23, "-0", 34, "-0,11");
      Test_adding<T, Template>("-0", 99.9, "0", 99.4, "-0,01");
      Test_adding<T, Template>("-0", 99.4, "0", 99.9, "0,01");
      Test_adding<T, Template>("0", 99.9, "-0", 99.5, "0,00");
      Test_adding<T, Template>("-0", 99.9, "0", 99.5, "0,00");
      Test_adding<T, Template>("-0", 99.9, "0", 19.4, "-0,81");
      Test_adding<T, Template>("-0", 99.9, "0", 19.5, "-0,80");
      Test_adding<T>("-1", 28, "0", 2, "-1,26");
   }
   Test_adding<T>("0", 23, "0", 34, "0,57");
   Test_adding<T, Template>("0", 9.4, "0", 9.9, "0,19");
   Test_adding<T, Template>("0", 9.5, "0", 9.9, "0,20");
}

template <typename T, template<typename> class Template = Money>
void correct_subtracting() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         Test_subtracting<T>("-1", 28, "0", 2, "-1,30");
         if (! is_same<T, short>::value)
            Test_subtracting<T>("100", 23, "-450", 34, "550,57");
      }
      Test_subtracting<T>("-0", 2, "-1", 28, "1,26");
      Test_subtracting<T>("-1", 28, "-0", 2, "-1,26");
      Test_subtracting<T, Template>("-0", 99.9, "0", 19.4, "-1,19");
      Test_subtracting<T, Template>("-0", 99.9, "0", 19.5, "-1,20");
      Test_subtracting<T, Template>("-0", 99.4, "-0", 99.9, "0,01");
      Test_subtracting<T, Template>("0", 99.9, "0", 99.5, "0,00");
      Test_subtracting<T, Template>("-0", 99.9, "-0", 99.4, "-0,01");
   }
   Test_subtracting<T>("0", 99, "0", 99, "0,00");
}

template <typename T, template<typename> class Template = Money>
void correct_multiplying() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         Test_multiplying<T, Template>("-1", 28, "2", "-2,56");
         if (! is_same<T, short>::value) {
            Test_multiplying<T, Template>("-100", 23, std::to_string(-34), "3407,82");
            Test_multiplying<T, Template>("-100", 23, std::to_string(34), "-3407,82");
            Test_multiplying<T, Template>("100", 23, "-34", "-3407,82");
         }
      }
      Test_multiplying<T, Template>("-0", 28, "-2", "0,56");
   }
   if (! is_same<T, char>::value && ! is_same<T, int_fast8_t>::value) {
      if (! is_same<T, short>::value && ! is_same<T, unsigned short>::value)
         Test_multiplying<T, Template>("100", 23, "34", "3407,82");
   }
   Test_multiplying<T, Template>("0", 28, "2", "0,56");
}

template <typename T, template<typename> class Template = Money>
void correct_relations() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
         Test_relations<T>("-1", 28, "0", 2, false, true);
         if (! is_same<T, short>::value)
            Test_relations<T>("100", 23, "-450", 34, false, false);
      }
      Test_relations<T, Template>("-0", 23, "-0", 34, false, false);
      Test_relations<T, Template>("-0", 23, "0", 34, false, true);
      Test_relations<T, Template>("0", 23, "-0", 34, false, false);
   }
   Test_relations<T>("0", 23, "0", 34, false, true);
   Test_relations<T>("0", 99, "0", 99, true, false);
   Test_relations<T>("0", 0, "0", 0, true, false);
}

template <typename T, template<typename> class Template = Money, enable_if_t<numeric_limits<T>::is_signed, bool>  = true>
void correct_unary() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (! is_same<T, char>::value &&  ! is_same<T, int_fast8_t>::value) {
      Test_unary<T>("-1", 28, "1,28");
      if (! is_same<T, short>::value) {
         Test_unary<T>("100", 23, "-100,23");
         Test_unary<T, Template>("99", 0, "-99,00");
         Test_unary<T>("-99", 99, "99,99");
      }
   }
   Test_unary<T, Template>("-0", 23, "0,23");
   Test_unary<T, Template>("0", 23, "-0,23");
   Test_unary<T>("0", 0, "0,00");
   Test_unary<T>("-0", 0, "0,00");
}

template <typename T, template<typename> class Template = Money, enable_if_t<! is_same<T, Integer>::value && ! is_same<T, long double>::value && ! is_same<T, double>::value && numeric_limits<T>::is_signed, bool>  = true>
void failed_unary() {
   cerr << "\n\n#########################" << __func__ << '\n';
   const T dollars = numeric_limits<T>::lowest() / CENTS_PER_DOLLAR;
   T cents = fmod(numeric_limits<T>::lowest(), CENTS_PER_DOLLAR);
   string dollars_string = std::to_string(dollars);
   if (is_same<T, float>::value) {
      const size_t dot_pos = dollars_string.find(".");
      if (dot_pos != string::npos)
         dollars_string = dollars_string.substr(0, dot_pos);
   }
   Failed_test_unary<T>(dollars_string, std::abs(cents));
}

const string Integer_MAX = static_cast<string>(Integer::MAX);
template <typename T, template<typename> class Template = Money>
void failed_adding() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
         Failed_test_adding<T>("-1", 28, "-0", 2);
      }
      if (is_same<T, short>::value)
         Failed_test_adding<T>("100", 23, "310", 34);
   }
   if (! is_same<T, double>::value && ! is_same<T, long double>::value)
      Failed_test_adding<T>(Integer_MAX, 28, "0", 2);
}

const string Integer_MIN = static_cast<string>(Integer::MIN);
template <typename T, template<typename> class Template = Money>
void failed_subtracting() {
   cerr << "\n\n#########################" << __func__ << '\n';
   if (numeric_limits<T>::is_signed) {
      if (is_same<T, char>::value || is_same<T, int_fast8_t>::value) {
         Failed_test_subtracting<T>("-1", 28, "0", 2);
      }
      if (is_same<T, short>::value)
         Failed_test_subtracting<T>("100", 23, "-310", 34);
      if (! is_same<T, double>::value && ! is_same<T, long double>::value)
         Failed_test_subtracting<T>(Integer_MIN, 28, "0", 2);
   }
}

template <typename T, template<typename> class Template = Money>
inline void correct() {
   correct_adding<T>();
   correct_subtracting<T>();
   correct_relations<T>();
   correct_multiplying<T, Money>();
}

template <typename T, template<typename> class Template = Money>
inline void failed() {
   failed_adding<T>();
   failed_subtracting<T>();
}

template <typename T, template<typename> class Template = Money, enable_if_t<! is_same<T, Integer>::value && ! is_same<T, long double>::value && ! is_same<T, double>::value && numeric_limits<T>::is_signed, bool>  = true>
inline void perform() {
   correct<T>();
   correct_unary<T>();
   failed<T>();
   failed_unary<T>();
}
template <typename T, template<typename> class Template = Money, enable_if_t<is_same<T, Integer>::value 
      || is_same<T, long double>::value || is_same<T, double>::value, bool>  = true>
inline void perform() {
   correct<T>();
   correct_unary<T>();
   failed<T>();
}

template <typename T, template<typename> class Template = Money, enable_if_t<false == numeric_limits<T>::is_signed, bool>  = true>
inline void perform() {
   correct<T>();
   failed<T>();
}

void perform() {
   perform<char, Money>();
   perform<Integer, Money>();
   perform<int_fast8_t>();
   perform<short>();
   perform<unsigned short>();
   perform<int>();
   perform<unsigned>();
   perform<long>();
   perform<unsigned long>();
   perform<long long>();
   perform<unsigned long long>();
   perform<float>();
   perform<double>();
   perform<long double>();
   
   cerr << '\n' << __func__ << ": END of ALL TESTS\n";
}

}
