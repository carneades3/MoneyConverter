#ifndef XML_NBP_HPP
#define XML_NBP_HPP

#include <array>

#include "xml_processing.hpp"
#include "curl_interface.hpp"

using pugi::xml_document;
using pugi::xpath_node_set;
using pugi::xpath_node;
using pugi::xml_node;
using pugi::xml_text;

using std::cout;

using xml_processing::Xml_processing;

namespace xml_NBP {

class Xml_NBP_processing final {
   static constexpr char* CURRENCY_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Currency";
   static constexpr char* CODE_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Code";
   static constexpr char* MID_RATE_NODE = "/ArrayOfExchangeRatesTable/ExchangeRatesTable/Rates/Rate/Mid";
   
   static const std::array<const char * const, 2> URL_ARRAY ;
   
   static xml_document doc;
   
   static bool print(const xpath_node_set & CURRENCIES, const xpath_node_set & CODES, const xpath_node_set & RATES) {
      const size_t SIZE = CURRENCIES.size();
      if (SIZE != CODES.size() || SIZE != RATES.size()) {
         cerr << "Different size of xpath node sets:\n";
         cerr << "CURRENCIES = " << SIZE << "\n";
         cerr << "CODES = " << CODES.size() << "\n";
         cerr << "RATES = " << RATES.size() << "\n";
         return false;
      }
      for (size_t i = 0; i < SIZE; i++) {
         xpath_node  code_xpathnode = CODES[i];
         xml_node    code_xmlnode   = code_xpathnode.node();
         xml_text    code_xmltext   = code_xmlnode.text();
         xpath_node  rate_xpathnode = RATES[i];
         xml_node    rate_xmlnode   = rate_xpathnode.node();
         xml_text    rate_xmltext   = rate_xmlnode.text();
         double rate                = std::stod(rate_xmltext.as_string());
         double inverse_rate        = 1.0 / rate;
         cout << " 1 PLN = " << inverse_rate << " " << code_xmltext.get() << " and"
            << " 1 " << code_xmltext.as_string() << " = " << rate << " PLN\n";
      }
      return true;
   }
public:
   static bool process_xml_document(const char * const STR) {
      if (! Xml_processing::load_validate_xml(doc, STR))
         return false;
      try {
         const xpath_node_set CURRENCIES = doc.select_nodes(CURRENCY_NODE);
         const xpath_node_set CODES = doc.select_nodes(CODE_NODE);
         const xpath_node_set RATES = doc.select_nodes(MID_RATE_NODE);
         return print(CURRENCIES, CODES, RATES);
      }
      catch (pugi::xpath_exception const & e) {
         cerr << e.result().description() << '\n';
         return false;
      }
   }
   
   static bool download(Curl_interface * downloader) {
      if (downloader == nullptr)
         throw std::invalid_argument("downloader can not be nullptr");
      std::string xml_doc;
      for (const char* URL :  URL_ARRAY) {
         downloader->download(URL);
         xml_doc = downloader->get_doc();
         if (xml_doc.empty()) {
            cerr << "Error: Xml document can not be empty\n";
            return false;
         }
         if ( ! Xml_NBP_processing::process_xml_document(xml_doc.c_str()))
            return false;
      }
      return true;
   }
   
};

const std::array<const char * const, 2> Xml_NBP_processing::URL_ARRAY = { "http://api.nbp.pl/api/exchangerates/tables/a/",
                                                                  "http://api.nbp.pl/api/exchangerates/tables/b/" };

xml_document Xml_NBP_processing::doc;
   
}

#endif
