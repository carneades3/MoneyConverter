#ifndef FLOATRATES_DOWNLOADER_HPP
#define FLOATRATES_DOWNLOADER_HPP

#include "network.hpp"
#include "floatrates.hpp"
#include "floatrates_json.hpp"
#include "floatrates_xml.hpp"
#include "factory.hpp"
#include "asio_downloader.hpp"
#include "json_downloader.hpp"
#include "xml_downloader.hpp"
#include "curl_downloader.hpp"

#ifdef __clang__
#elif defined(__GNUG__)
   #include "curlcpp_downloader.hpp"
#endif

#include <functional>

using network::File_format;
using network::Network_library;

class Floatrates_downloader {
// due to "static initialization order fiasco":
// replace of const string Floatrates_downloader::HOST = "www.floatrates.com" to function const string & static_host()
   static const Method ST_METHOD;
   static const Cache_control ST_CACHE_CONTROL;
   static const Connection ST_CONNECTION;
   
   string currency;
   File_format format = File_format::NONE;
   Network_library library = Network_library::NONE;
   Float_rates * float_rates = nullptr;
   
   typedef  string (Floatrates_downloader::*Asio_getter)();
   //string (Floatrates_downloader::*)<const bool>() getter = &Floatrates_downloader::get_by_asio<true>();
   Asio_getter asio_getter = &Floatrates_downloader::get_by_asio<true>;
   
   template <const bool FLAG>
   inline void set_float_rates() {
      using Downloader = std::conditional_t<FLAG, Float_rates_json, Float_rates_xml>;
      delete float_rates;
#ifdef __clang__
      float_rates = new Downloader { "", false };
#elif defined(__GNUG__)
      float_rates = new Downloader {""};
#endif
   }
   
   template <typename Function, typename... Args>
   inline string get_by_function(Function && func, Args&&... args) { 
      auto function = std::mem_fn(func);
      string result = function(*this, std::forward<Args>(args)...); 
      return result; 
   } 
   
   string get_by_curl();
   template <const bool FLAG>
   string get_by_asio(); 
public:
   Floatrates_downloader(const Network_library & LIBRARY, const File_format & FORMAT, const string & CURRENCY = "usd") 
            : currency(format_currency(CURRENCY)) { 
      if (FORMAT == File_format::NONE || LIBRARY == Network_library::NONE)
         throw std::invalid_argument(__func__ + string(" file format NONE or Network_library NONE are not allowed"));
      set_format(FORMAT); 
      set_library(LIBRARY); 
   }

   static string format_currency(const string & CURRENCY);
   
   //string get_by_asio(const File_format & format);
   string get_by_library();
   
   string get_currency() const { return currency; }
   void set_currency(const string & CURRENCY) { currency = format_currency(CURRENCY); }
   File_format get_format() const { return format; }
   void set_format(const File_format & FORMAT);
   Network_library get_library() const { return library; }
   void set_library(const Network_library & LIBRARY);
   
   map<string, long double> rates() const {  
      return (float_rates == nullptr) ? map<string, long double> {} : float_rates->rates(); }
   
   map<string, long double> inverse_rates() const {  
      return (float_rates == nullptr) ? map<string, long double> {} : float_rates->inverse_rates(); }
   
   void download();
   
   bool operator==(const Floatrates_downloader & other) const {
      bool result = currency == other.currency && format == other.format 
         && library == other.library && asio_getter == other.asio_getter;
      if (result) {
         if (float_rates == nullptr && other.float_rates == nullptr)
            return true;
         if (float_rates == nullptr || other.float_rates == nullptr)
            return false;
         if (typeid(*(other.float_rates)) != typeid(*float_rates))
            return false;
         
         return assert_floatrates(*float_rates, *(other.float_rates));
      }
      return false;
   }
   
   bool operator!=(const Floatrates_downloader & other) const {
      return ! operator==(other); }
      
   const Float_rates * const get_float_rates() const { return float_rates; }
   
   Floatrates_downloader(const Floatrates_downloader &);
   Floatrates_downloader(Floatrates_downloader &&) noexcept;
   Floatrates_downloader & operator=(const Floatrates_downloader &);
   Floatrates_downloader & operator=(Floatrates_downloader &&) noexcept;
   ~Floatrates_downloader() { delete float_rates; }
};

#endif
