#ifndef XML_PROCESSING_HPP
#define XML_PROCESSING_HPP

#include <iostream>
#include <array>
#include <string>

#include "pugixml.hpp"

using pugi::xml_document;
using pugi::xml_parse_result;
//using pugi::xpath_node_set;

using std::cerr;
//using std::cout;
using std::string;

namespace xml_processing {
   
class Download_Interface {
   std::string document;
public:
   virtual void download (const char * const) = 0;
   virtual ~ Download_Interface() { cerr << __func__ << '\n'; };
   
   Download_Interface(Download_Interface const &) = delete;
   Download_Interface& operator=(Download_Interface const &) = delete;
   
   std::string get_doc() const { return document; }
protected:
   Download_Interface() { cerr << __func__ << '\n'; }
   
   void set_doc(const std::string & s) { document = s; }
};

class Xml_processing final {
public:
   class Exception : public std::exception { 
      static string msg;
   public:
      Exception() {}
      Exception(const string& message) { msg += message; }
      const char* what() const noexcept {
         return msg.c_str();
      }
   };
   
   static inline bool load_validate_xml(xml_document& xml_doc, const char * XML_STRING) {
      xml_parse_result result = xml_doc.load_string(XML_STRING);
      if (result.status != pugi::xml_parse_status::status_ok)  {
         cerr << "XML [" << XML_STRING << "] parsed with errors, attr value: [" 
            << xml_doc.child("node").attribute("attr").value() << "]\n";
         cerr << "Error description: " << result.description() << "\n";
         cerr << "Error offset: " << result.offset << " (error at [..." << XML_STRING[result.offset] << "]\n\n";
      }
      return result;
   }
   
   static inline bool load_validate_xml(xml_document& xml_doc, const string & XML_STRING) {
      return load_validate_xml(xml_doc, XML_STRING.c_str());
   }
   
   static inline void load_validate_xml_throw(xml_document& xml_doc, const char * XML_STRING) {
      xml_parse_result result = xml_doc.load_string(XML_STRING);
      if (result.status != pugi::xml_parse_status::status_ok)  {
         throw Exception(
            "XML [" + string(XML_STRING) + "] parsed with errors, attr value: [" 
            + xml_doc.child("node").attribute("attr").value() + "]\n"
            + " Error description: " + result.description() + "\nError offset: " + std::to_string(result.offset)
            + " (error at [..." + XML_STRING[result.offset] + "]\n\n");
      }
   }
   
   static inline void load_validate_xml_throw(xml_document& xml_doc, const string & XML_STRING) {
      load_validate_xml_throw(xml_doc, XML_STRING.c_str());
   }
};
   
}

#endif
