#ifndef FLOATRATES_TEST_HPP
#define FLOATRATES_TEST_HPP

#include "floatrates_downloader.hpp"

using std::invalid_argument;
using std::cout;

namespace float_rates_test {
   
void test();
   
using network::File_format;

class Float_rates_test {
   string currency;
   File_format format = File_format::NONE;
   Float_rates * rates_ptr = nullptr;
   Network_library library = Network_library::NONE;
public:   
   Float_rates_test(const Network_library & LIBRARY, const File_format & f, const string & c) : currency(c) {
      if ( c.length() == 0)
         throw invalid_argument(string(__func__) + "empty string can not be accepted ");
      if (f == File_format::NONE || LIBRARY == Network_library::NONE)
         throw std::invalid_argument(__func__ + string(" file format NONE or Network_library NONE are not allowed"));
      set_format(f); 
      set_library(LIBRARY);
   }
   
   void run() {
      Floatrates_downloader downloder(library, format, currency);
      const string doc = downloder.get_by_library();
      cout << '\n' << __func__ << " ----------------- Downloaded string start: \n" << doc << '\n';
      rates_ptr->set_document(doc);
      rates_ptr->set_rates_from_doc();
      map <string, float_rates_info> rates = rates_ptr->float_rates();
      cout << '\n' << __func__ << " ----------------- Downloaded string end \n";
      for (const pair<string, float_rates_info> &p : rates) 
         cout << " 1 " << currency << " = " << p.second.rate << " " << p.second.code << " and "
            << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << currency << '\n';
   }
   
   void set_format(const File_format & FORMAT) { 
      if (FORMAT == format)
         return;
      switch (FORMAT) {
         case File_format::JSON :
            delete rates_ptr;
            rates_ptr = Float_rates_factory<Float_rates_json>().create(); 
            break;
         case File_format::XML :
            delete rates_ptr;
            rates_ptr = Float_rates_factory<Float_rates_xml>().create(); 
            break;
         default:
            throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)));
      }
      format = FORMAT; 
   }
   
   void set_library(const Network_library & LIBRARY) { 
      if (LIBRARY == library)
         return;
      switch (LIBRARY) {
         case Network_library::ASIO :
         case Network_library::CURL :
            library = LIBRARY;
            break;
         default:
            throw invalid_argument(__func__ + string(" Invalid network library ") + std::to_string(static_cast<int>(library)));
      }
   } 
   
   Float_rates_test(Float_rates_test&& other) noexcept
		: currency(other.currency), format(other.format), rates_ptr(other.rates_ptr), library(other.library) {
      cerr << " MOVE Constructor " << __func__ << '\n';
		other.rates_ptr = nullptr;
      other.currency = "";
	}
	
	Float_rates_test& operator=(Float_rates_test&& other) noexcept {
      cerr << " MOVE " << __func__ << '\n';
		if (&other != this) {
         delete rates_ptr;
         rates_ptr = other.rates_ptr;
         other.rates_ptr = nullptr;
         currency = other.currency;
         other.currency = "";
         format = other.format;
         library = other.library;
      }
		return *this;
	}
	
	Float_rates * const& get_float_rates() const { return rates_ptr; }
	string get_currency() const { return currency; }
	File_format get_format() const { return format; }
   
   Float_rates_test(Float_rates_test const &) = delete;
   Float_rates_test& operator=(Float_rates_test const &) = delete;
   ~Float_rates_test() { delete rates_ptr; }
   
   bool operator==(const Float_rates_test & other) const {
      bool result = currency == other.currency && format == other.format && library == other.library;
      if (! result)
         return result;
      if (rates_ptr == nullptr && other.rates_ptr == nullptr)
         return true;
      if (rates_ptr != nullptr && other.rates_ptr != nullptr && typeid(*(other.rates_ptr)) == typeid(*rates_ptr)
            && assert_floatrates(*rates_ptr, *(other.rates_ptr)))
         return true;
      return false;
   }
   
   bool operator!=(const Float_rates_test & other) const {
      return ! operator==(other); }
};

}

#endif
