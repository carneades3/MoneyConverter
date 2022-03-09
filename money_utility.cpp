#include <iostream>
#include "integer.hpp"
#include "money.hpp"
//#include "asio_downloader.hpp"
//#include "json_downloader.hpp"
#include "floatrates_downloader.hpp"

//using std::ios_base;

//static std::ios_base::Init toEnsureInitialization;

namespace money {
   
const Integer CENTS_PER_DOLLAR_INTEGER = Integer::create_Integer(CENTS_PER_DOLLAR);

const long double CONVERT_FACTOR = 100;

template<>
Integer Money<Integer>::convert  (const Money<Integer> & MONEY) const {
   if (MONEY.get_currency() == get_currency())
      return MONEY.amount_in_cents; 
   
   const long double ratio = rates_per_USD[MONEY.get_currency()] / rates_per_USD[get_currency()];
   long double amount = ratio * static_cast<long double>(MONEY.amount_in_cents);
   amount = Money<long double>::round(amount);
   const Integer result = Integer::create_Integer(amount) ;
   return result; 
}
/*
template<>
Integer Money<Integer>::convert(const Money & MONEY, const string & CURRENCY) {
   validate_currency(CURRENCY);
   if (MONEY.get_currency() == CURRENCY)
      return MONEY.amount_in_cents;
   
   const long double ratio = rates_per_USD[MONEY.get_currency()] / rates_per_USD[CURRENCY];
   const long double amount = ratio * static_cast<long double>(MONEY.amount_in_cents);
   const Integer result = Integer::create_Integer(amount) ;
   return result; 
}
*/
template<>
Integer Money<Integer>::get_amount(const string & STR) {
   return Integer::parse_create(STR);
}

template <>
//template<typename Integer>
Integer Money<Integer>::calculate_by_Integer(const Integer & dollars, const long double cents /*  = INCORRECT_CENTS */) const {
   cerr << __func__ << '\n';
   Integer amount_in_cents = dollars * CENTS_PER_DOLLAR_INTEGER;
   cerr << __func__ << " amount_in_cents = " << amount_in_cents << '\n';
   if (INCORRECT_CENTS != cents) {
      cerr << __func__ << " cents = " << cents << '\n';
      Integer cents_round = dollars >= Integer::ZERO ? Money<Integer>::round(cents) : -Money<Integer>::round(cents);
      amount_in_cents += cents_round;
   }
   cerr << __func__ << " result = " << amount_in_cents << '\n';
   return amount_in_cents;
}

template<>
Money<Integer>::Money(const string & dollars, const long double cents, const string & currency /* = "PLN" */) {
   cerr << __func__ << " TYPE_NAME = '" << TYPE_NAME << "' " << dollars << '\n';
   validate_cents(cents);
   set_currency(currency);
   if (! regex_match(dollars, INTEGER_REGEX)) 
      throw invalid_argument("Regex: dollars must be integer number ");
   string dollars_string = dollars;
   if (regex_match(dollars, MINUS_ZERO_REGEX))
      dollars_string = dollars.substr(1);
   Integer amount = Money<Integer>::get_amount(dollars_string);
   this->amount_in_cents = calculate_by_Integer(amount, cents);
   cerr << __func__ << " amount = '" << amount << "' std::signbit = '" << std::boolalpha << std::signbit(amount) << "' \n";  
   if (dollars[0] == '-' && Integer::ZERO == amount)
      this->amount_in_cents = -this->amount_in_cents;
   
   cerr << __func__ << " this->amount_in_cents = '" << TYPE_NAME << "' " << this->amount_in_cents << '\n';   
}

template<>
Money<Integer>::Money(const string & dollars, const string & currency /* = "PLN" */) {   // accept floating-point arguments
   cerr << __func__ << " TYPE_NAME = " << TYPE_NAME << ' ' << dollars << '\n';
   set_currency(currency);
   if (! regex_match(dollars, FLOAT_POINT_REGEX))
      throw invalid_argument(string(__func__) +  " Regex: entered string '"
               + dollars + "' is not non-exponent floating-point format "); 
   this->amount_in_cents = calculate_amount_in_cents(dollars);
}

template<>
Money<Integer>::operator string() const {
   Integer dollars = get_dollars(TYPE_DEFAULT_OBJECT);
   Integer cents = get_cents(TYPE_DEFAULT_OBJECT);
   if (cents < Integer::ZERO) 
      cents = -cents;
   string out = (dollars == Integer::ZERO && signbit(get_amount_in_cents())) ? "-" : "";
   out += formatted_string(dollars, cents);
   out +=  " " + get_currency();
   return out;
}

Money<Integer> operator+(const Money<Integer>& a, const Money<Integer>& b) {
   const Integer sum = a.amount_in_cents + a.convert(b);
   cerr << __func__ << " sum = " << sum << '\n';
   const Constructor_Args args {sum};
   Money<Integer> result = Money<Integer>(args.DOLLARS, args.CENTS, a.get_currency());//::create
   cerr << __func__ << " result = " << result << '\n';
   return result;
}

Money<long double> operator+(const Money<long double>& a, const Money<long double>& b) {
   long double sum = a.amount_in_cents + a.convert(b);
   sum = Money<long double>::round(sum);
   cerr << __func__ << " sum = " << sum << '\n';
   const string dollars_string = dollars_from_amount(sum);
   Money<long double> result = Money<long double>(dollars_string, a.get_currency());//::create
   cerr << __func__ << " result = " << result << '\n';
   return result;
}
   
string formatted_string(const Integer & dollars, const Integer & cents) {
   ostringstream stream;
   stream << dollars;
   string dollars_string = stream.str();
   if (dollars > Integer::ZERO)
      dollars_string = dollars_string.substr(1);
   reset(stream);
   stream << cents;
   string cents_string = stream.str();
   if (! cents.is_zero())
      cents_string = cents_string.substr(1);
   cents_string = cents_string.length() == 1 ? ("0" + cents_string) : cents_string;
   string out = dollars_string + "," + cents_string;
   return out;
}

map <string, long double> download(const Network_library & library, const File_format & format, const string & CURRENCY = "USD") {
#ifdef __clang__
   static std::ios_base::Init toEnsureInitialization;
#endif
   cerr << __func__ << '\n';
   Floatrates_downloader downloader(library, format, CURRENCY);
   downloader.download();
   //////const Float_rates floatrates = downloader.get_float_rates();
   //floatrates.set_rates_from_json();
   map <string, long double> rates = downloader.inverse_rates();
   //std::cerr << "   " << __func__ << " | " << "\n";
   //for (const auto [code, rate] : rates)
   //     std::cerr << "   " << code << " | " << rate << "\n";
   //rates.insert({{"PLN", 1}});
   
   
   return rates;
}
/*
map <string, long double> get_by_curl(const File_format & format, const string & CURRENCY = "USD") {
   string currency = format_currency(CURRENCY);
#ifdef __clang__
   static std::ios_base::Init toEnsureInitialization;
#endif
   cerr << __func__ << " currency = " << currency << '\n';
   const string HOST = "www.floatrates.com";
   const Method METHOD = Method::get;
   const string DIRECTORY = "/daily/" + currency + ".json";
   //const string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
   const Cache_control CACHE_CONTROL = Cache_control::no_store;
   const Connection CONNECTION = Connection::close;
   Json_downloader downloader(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
   downloader.download();
   const string JSON_DOC = downloader.get();// = get_document(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION);
   
   Float_rates floatrates = { JSON_DOC };
   map <string, long double> rates = floatrates.inverse_rates();
   floatrates.set_rates_from_json();
   assert(rates == floatrates.inverse_rates());
   return rates;
}
*/
map <string, long double> & set_rates_per_USD(const Network_library & library, const File_format & format) {
   //test();
   static map <string, long double> result;
   const string USD = "USD";
   switch (library) {
      case Network_library::ASIO :
         if (format == File_format::JSON || format == File_format::XML) 
            result = download(library, format, USD);
         else
            throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)) + " for ASIO ");
         break; 
      case Network_library::CURL :
         if (format == File_format::JSON || format == File_format::XML)
            result = download(library, format, USD);
         else
            throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)) + " for CURL ");
         break; 
      case Network_library::NONE :
         throw invalid_argument(__func__ + string(" network library NONE is not allowed"));
      default:
         throw invalid_argument(__func__ + string(" Invalid network library ") + std::to_string(static_cast<int>(library)));
   }
   result.insert({{USD, 1}});
   //test();
   return result;
}

#ifndef DEBUG_OSTREAM
   ostream& operator<<(ostream& os, const Money<Integer>& money) {
      Integer dollars = money.get_dollars(Money<Integer>::TYPE_DEFAULT_OBJECT);
      Integer cents = money.get_cents(Money<Integer>::TYPE_DEFAULT_OBJECT);
      start_settings(os, money);
      os << dollars.string_without_signum() << ",";
      os << setw(2) << cents.string_without_signum();
      os << ' ' << money.get_currency();
      return os;
   }
#endif

}
