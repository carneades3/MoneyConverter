#ifndef CURL_MANAGER_HPP
#define CURL_MANAGER_HPP

#include <string>
#include <exception>

#include <curl/curl.h>

using std::string;

struct Curl_Error : public std::exception {
private:
   string msg = {"!!! error with curl library: "};
public:
   Curl_Error() { }
   Curl_Error(const string& message) {  msg += message; }
   const char* what() const noexcept {
      return msg.data();
   }
};
//inline string Curl_Error::msg = {"!!! error with curl library: "};

class Curl_Manager {
   CURL * connection = nullptr;
   CURLcode code = CURLE_FAILED_INIT;
   
   string get_message(const CURLoption & option) {
      switch (option) {
         case CURLOPT_URL:
            return "Failed to set URL";
         case CURLOPT_FOLLOWLOCATION:
            return "Failed to set redirect option";
         case CURLOPT_WRITEFUNCTION:
            return "Failed to set write callback function";
         case CURLOPT_WRITEDATA:
            return "Failed to set write data";
         case CURLOPT_HTTPHEADER:
            return "Failed to set http header";
         case CURLOPT_TIMEOUT:
            return "Failed to set timeout";
         case CURLOPT_DNS_CACHE_TIMEOUT:
            return "Failed to set DNS_CACHE timeout";
         default:
            return "";
      }
   }
public:
   Curl_Manager() {
      code = curl_global_init(CURL_GLOBAL_DEFAULT);
      check_error("Failed to curl_global_init");
      connection = curl_easy_init();
      if (connection == nullptr) 
         throw Curl_Error("Failed to create CURL connection - connection is null pointer");
   }
   
   template <class T>
   inline void set_option(const CURLoption & option, const T & value) {
      code = curl_easy_setopt(connection, option, value);
      check_error(get_message(option));
   }
   
   inline void check_error(const string& message) {
      if(code != CURLE_OK)
         throw Curl_Error(message + " " + curl_easy_strerror(code));
   }
   
   inline void perform() {
      code = curl_easy_perform(connection);
      if(code != CURLE_OK) {
         //curl_easy_cleanup(connection);
         throw Curl_Error(string("Failed to curl_easy_perform ") + curl_easy_strerror(code));
      }
   }
   
   ~Curl_Manager() { 
      cerr << __func__ << '\n';
      curl_easy_cleanup(connection);
      curl_global_cleanup();
      //check_error("Failed to curl_easy_perform"); 
   }
};

#endif
