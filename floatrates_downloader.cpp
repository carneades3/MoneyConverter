#include "floatrates_downloader.hpp"

using namespace network;
using namespace std;

// How do I prevent the “static initialization order problem” for my static data members?
// replace of const string Floatrates_downloader::HOST = "www.floatrates.com";
const string & static_host() {
  static const string host = "www.floatrates.com";
  return host;
}

//const string Floatrates_downloader::HOST = "www.floatrates.com";
const Method Floatrates_downloader::ST_METHOD = Method::get;
const Cache_control Floatrates_downloader::ST_CACHE_CONTROL = Cache_control::no_store;
const Connection Floatrates_downloader::ST_CONNECTION = Connection::close;

template <const bool FLAG>
string Floatrates_downloader::get_by_asio() {
   using Downloader = std::conditional_t<FLAG, Json_downloader, Xml_downloader>;
   cerr << __func__ << " currency = " << currency << '\n';
   const string DIRECTORY = "/daily/" + currency + (FLAG ? ".json" : ".xml");
   //const string DIRECTORY = "/";            // "/" is root (main page of host) and "" has result 400 Bad Request
   static const string & HOST = static_host();
   Downloader downloader(HOST, ST_METHOD, DIRECTORY, ST_CACHE_CONTROL, ST_CONNECTION);
   downloader.download();
   const string DOC = downloader.get();
   return DOC;
}

string Floatrates_downloader::get_by_curl() {
   cerr << __func__ << " currency = " << currency << '\n';
   static const string & HOST = static_host();
   const string URL = HOST + "/daily/" + currency + (format == File_format::JSON ? ".json" : ".xml");
#ifdef __clang__
   Curl_downloader downloader = Curl_downloader {format};
#elif defined(__GNUG__)
   Curlcpp_downloader downloader = Curlcpp_downloader {format};
#endif
   Curl_interface & curl = downloader;
   curl.download(URL.data());
   const string DOC = curl.get_doc();
   return DOC;
}

void Floatrates_downloader::set_format(const File_format & FORMAT) { 
   if (FORMAT == format)
      return;
   switch (FORMAT) {
      case File_format::JSON :
         asio_getter = &Floatrates_downloader::get_by_asio<true>; 
         set_float_rates<true>();
         break;
      case File_format::XML :
         asio_getter = &Floatrates_downloader::get_by_asio<false>;
         set_float_rates<false>();
         break;
      default:
         throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)));
   }
   format = FORMAT; 
}

string Floatrates_downloader::format_currency(const string & CURRENCY) {
   if (CURRENCY.size() != 3)
      throw invalid_argument (" currency must be exactly 3 characters: " + CURRENCY);
   string result = CURRENCY;
   std::transform(result.begin(), result.end(), result.begin(),
      [](unsigned char c){ return std::tolower(c); });
   return result;
}

void Floatrates_downloader::download() {
   const string DOC = get_by_library();
   float_rates->set_document(DOC);
   float_rates->set_rates_from_doc();
   map <string, float_rates_info> rates = float_rates->float_rates();
   cout << '\n' << __func__ << '\n';
   for (const pair<string, float_rates_info> &p : rates) 
      cout << " 1 " << currency << " = " << p.second.rate << " " << p.second.code << " and "
         << " 1 " << p.second.code << " = " << p.second.inverse_rate << " " << currency << endl;
}

Floatrates_downloader::Floatrates_downloader(const Floatrates_downloader& other) 
   : currency(other.currency), format(other.format), library(other.library), asio_getter(other.asio_getter) {
   cerr << " COPY Constructor " << __func__ << '\n';
   if (nullptr != other.float_rates) {
      if (typeid(*(other.float_rates)) == typeid(Float_rates_json) || typeid(*(other.float_rates)) == typeid(Float_rates_xml))
         float_rates = other.float_rates->clone();
      else
         throw invalid_argument(string(__func__) + " unsupported type " + typeid(*(other.float_rates)).name());
   }
}

Floatrates_downloader& Floatrates_downloader::operator=(const Floatrates_downloader& other) {
   cerr << " COPY " << __func__ << '\n';
   if (&other != this) {
      delete float_rates;
      if (nullptr == other.float_rates)
         float_rates = nullptr;
      else if (Float_rates_json * ptr = dynamic_cast< Float_rates_json *> (other.float_rates))
         float_rates = new Float_rates_json(*ptr);
      else if (Float_rates_xml * ptr = dynamic_cast< Float_rates_xml *> (other.float_rates))
         float_rates = new Float_rates_xml(*ptr);
      else
         throw invalid_argument(string(__func__) + " unsupported type " + typeid(*(other.float_rates)).name());
      currency = other.currency;
      format = other.format;
      library = other.library;
      asio_getter = other.asio_getter;
   }
   return *this;
}

Floatrates_downloader::Floatrates_downloader(Floatrates_downloader&& other) noexcept
   : currency(other.currency), format(other.format), library(other.library), 
      float_rates(other.float_rates), asio_getter(other.asio_getter) {
   cerr << " MOVE Constructor " << __func__ << '\n';
   other.float_rates = nullptr;
   other.currency = "";
   other.asio_getter = nullptr;
}

Floatrates_downloader& Floatrates_downloader::operator=(Floatrates_downloader&& other) noexcept {
   cerr << " MOVE " << __func__ << '\n';
   if (&other != this) {
      delete float_rates;
      float_rates = other.float_rates;
      other.float_rates = nullptr;
      currency = other.currency;
      other.currency = "";
      format = other.format;
      library = other.library;
      asio_getter = other.asio_getter;
      other.asio_getter = nullptr;
   }
   return *this;
}

string Floatrates_downloader::get_by_library() { 
   switch (library) {
      case Network_library::ASIO :
         return get_by_function(asio_getter); 
      case Network_library::CURL :
         return get_by_function(&Floatrates_downloader::get_by_curl);
      case Network_library::NONE :
         throw invalid_argument(__func__ + string(" network library NONE is not allowed"));
      default:
         throw invalid_argument(__func__ + string(" Invalid network library ") + std::to_string(static_cast<int>(library)));
   }
} 

void Floatrates_downloader::set_library(const Network_library & LIBRARY) { 
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
