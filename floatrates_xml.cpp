#include "floatrates_xml.hpp"

#include <iostream>
#include <sstream>

using std::cerr;
using std::pair;
using std::exception;

using namespace pugi;
using namespace xml_processing;

static inline size_t validate_sizes(const xpath_node_set & INVERSE_RATES, const xpath_node_set & CODES, const xpath_node_set & RATES) {
   const size_t SIZE = INVERSE_RATES.size();
   if (SIZE != CODES.size() || SIZE != RATES.size()) {
      throw Xml_processing::Exception(
         "Different size of xpath node sets:\nINVERSE_RATES = " + std::to_string(SIZE)
         + "\nCODES = " + std::to_string(CODES.size()) + "\nRATES = " + std::to_string(RATES.size()));
   }
   return SIZE;
}

static inline string get_node_string (const xpath_node & NODE) {
   xml_node    node   = NODE.node();
   xml_text    text   = node.text();
   string result = text.as_string(); // code_xmltext.get()
   return result;
}

static inline double get_node_double (const xpath_node & NODE) {
   const string node_text = get_node_string(NODE);
   const double result = std::stod(node_text);
   return result; 
}

static map<string, float_rates_info> get_data(const xpath_node_set & INVERSE_RATES, const xpath_node_set & CODES, const xpath_node_set & RATES) {
   const size_t SIZE = validate_sizes(INVERSE_RATES, CODES, RATES);
   map<string, float_rates_info> result; 
   for (size_t i = 0; i < SIZE; i++) { 
      xpath_node NODE = CODES[i];
      const string code = get_node_string(NODE);
      NODE = RATES[i];
      const double rate = get_node_double(NODE);
      NODE = INVERSE_RATES[i];
      const double inverse_rate = get_node_double(NODE);
      result[code] = float_rates_info { code, rate, inverse_rate };
   }
   return result;
}

map<string, float_rates_info> Float_rates_xml::get_data() const {
   xml_document xml_doc;
   Xml_processing::load_validate_xml_throw(xml_doc, document);
   try {
      const xpath_node_set INVERSE_RATES = xml_doc.select_nodes(INVERSE_RATE_NODE);
      const xpath_node_set CODES = xml_doc.select_nodes(CODE_NODE);
      const xpath_node_set RATES = xml_doc.select_nodes(RATE_NODE);
      
      return ::get_data(INVERSE_RATES, CODES, RATES);
   }
   catch (pugi::xpath_exception const & e) {
      cerr << "!!! Error xpath_exception exception: "  << typeid(e).name() << e.result().description() << '\n';
      throw;
   }
}

void Float_rates_xml::set_rates_from_doc() {
   try {
      float_rates_map = get_data();
   }
   catch (const Xml_processing::Exception & e) {
      cerr << "!!! Error xml exception: "  << typeid(e).name() << " : " << e.what() << '\n';
      throw;
   } catch (const exception & e) {
      cerr << "Exception: " << e.what() << std::endl;
      throw;
   }
}
