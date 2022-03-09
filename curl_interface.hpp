#ifndef CURL_INTERFACE_HPP
#define CURL_INTERFACE_HPP

#include <string>
#include <iostream>

#include "network.hpp"

using network::File_format;
using std::invalid_argument;

class Curl_interface {
   std::string document;
   File_format format = File_format::NONE;
public:
   virtual void download (const char * const) = 0;
   virtual ~ Curl_interface() { std::cerr << __func__ << '\n'; };
   
   //Curl_interface(Curl_interface const &) = delete;
   //Curl_interface& operator=(Curl_interface const &) = delete;
   
   std::string get_doc() const { return document; }
   
   void set_format(const File_format & FORMAT) { 
      if (FORMAT == format)
         return;
      switch (FORMAT) {
         case File_format::JSON :
         case File_format::XML :
            format = FORMAT; 
            break;
         default:
            throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)));
      } 
   }
   
   File_format get_format() const { return format; }
   
   Curl_interface(const File_format & FORMAT) { // public, not protected
      cerr << __func__ << '\n';
      set_format(FORMAT); 
   }
   
protected:
   
   void set_doc(const std::string & s) { document = s; }
   
   const string get_accept_content() const { 
      //static string result =  "Accept: application/";
      switch (format) {
         case File_format::JSON :
            //return result += "json";
            return "Accept: application/json";
         case File_format::XML :
            //return result.append("xml");
            return "Accept: application/xml";
         default:
            throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(format)));
      }
   }
   
   Curl_interface(const Curl_interface &) = default;
   Curl_interface(Curl_interface &&) = default;
   Curl_interface & operator=(const Curl_interface &) = default;
   Curl_interface & operator=(Curl_interface &&) = default;
};

#endif
