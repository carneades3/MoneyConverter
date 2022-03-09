#ifndef CURLCPP_DOWNLOADER_HPP
#define CURLCPP_DOWNLOADER_HPP

#include "curl_interface.hpp"

#include "curl_header.h"
#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"

using curl::curl_header;
using curl::curl_ios;
using curl::curl_easy;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

using std::stringstream;

class Curlcpp_downloader  : public Curl_interface {
   stringstream get_document(const char* const URL, const curl_header & HEADER) {
      stringstream stream;
      try {
         curl_ios<stringstream> writer(stream);
         curl_easy easy(writer);
         
         easy.add<CURLOPT_HTTPHEADER>(HEADER.get());
         easy.add<CURLOPT_URL>(URL);
         easy.add<CURLOPT_TIMEOUT>(10);
         easy.add<CURLOPT_DNS_CACHE_TIMEOUT>(0);
         easy.add<CURLOPT_FOLLOWLOCATION>(1);

         easy.perform();
      }
      catch (const curl_easy_exception & e) {
         curlcpp_traceback errors = e.get_traceback();
         e.print_traceback();
         throw;
      }
      return stream;
   }
public:
   void download(const char* const URL) override {
      if (URL == nullptr)
         throw invalid_argument("URL is nullptr");
      const string accept_content = get_accept_content();
      curl_header header;
      header.add(accept_content.c_str());
      stringstream stream = get_document(URL, header);
      set_doc(stream.str());
   }
   
   using Curl_interface::Curl_interface;
   
   Curlcpp_downloader(const Curlcpp_downloader &) = default;
   Curlcpp_downloader(Curlcpp_downloader &&) = default;
   Curlcpp_downloader & operator=(const Curlcpp_downloader &) = default;
   Curlcpp_downloader & operator=(Curlcpp_downloader &&) = default;
};

#endif
