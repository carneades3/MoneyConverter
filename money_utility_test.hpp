#ifndef MONEY_UTILITY_TEST_HPP
#define MONEY_UTILITY_TEST_HPP

#define DEBUG_OSTREAM

//#include "integer.hpp"
#include "assertions.hpp"
//#include "money.hpp"

//using namespace std;
//using namespace integer_space;
//using namespace money;

using std::cout;
  
template <class Number, template<typename> class Template>
static void print_assert(const Template<Number> & object, const string & expected = "", const bool NOT_assert_currency = true) {
   static const string TYPE_NAME = typeid(Template<Number>).name();
   cout << "                                       object of type " << TYPE_NAME << " = '" << object << "'\n";
   string object_string = static_cast<string>(object);
   if (NOT_assert_currency)
      object_string = object_string.substr(0, object_string.size() - 4);
   auto t = std::make_tuple(object_string, " != ", expected);
#ifdef DEBUG_OSTREAM
   cout << "\n------------------               DEBUG_OSTREAM\n";
   cout << "                                       object of type " << TYPE_NAME << " = '" << object << "'\n";
   ostringstream * os = reinterpret_cast<ostringstream *>(& cout);
   os << object;
   cout << "'\n";
   ostringstream ostrs;
   string output = operator<<(&ostrs, object).str();
   if (NOT_assert_currency)
      output = output.substr(0, output.size() - 4);
   assert_many(output == expected, t);
#endif
   assert_many(object_string == expected, t);
}

template <class Type, template<typename> class Template>
struct Constructor {
	template<typename... Args>
	Template<Type> operator()(Args...args) const {
		return Template<Type>(std::forward<Args>(args)...);
	}
};

#endif
