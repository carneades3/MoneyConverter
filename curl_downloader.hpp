#ifndef CURL_DOWNLOADER_HPP
#define CURL_DOWNLOADER_HPP

#include "curl_interface.hpp"
#include "curl_manager.hpp"

class Curl_downloader  : public Curl_interface {
   static size_t writer(char *data, const size_t size, const size_t nmemb, string * writer_data) {
      if(writer_data == nullptr)
         throw invalid_argument("writer_data is nullptr");
      
      writer_data->append(data, size * nmemb);
      return size * nmemb;
   } 
   
   string get_document(const char* const URL, const curl_slist * const HEADER) {
      Curl_Manager manager;
      manager.set_option(CURLOPT_URL, URL);
      manager.set_option(CURLOPT_FOLLOWLOCATION, 1L);
      manager.set_option(CURLOPT_WRITEFUNCTION, &Curl_downloader::writer);
      manager.set_option(CURLOPT_HTTPHEADER, HEADER);
      manager.set_option(CURLOPT_TIMEOUT, 10L);
      manager.set_option(CURLOPT_DNS_CACHE_TIMEOUT, 0L);
      string buffer;
      manager.set_option(CURLOPT_WRITEDATA, &buffer);
      manager.perform();
      return buffer;
   }

public:
   void download(const char* const URL) override {
      if (URL == nullptr)
         throw invalid_argument("URL is nullptr");
      const string accept_content = get_accept_content();
      struct curl_slist * header = nullptr;
      header = curl_slist_append(header, accept_content.c_str());
      try {
         const string result = get_document(URL, header);
         set_doc(result);
         curl_slist_free_all(header);
      } catch (const std::exception & e) {
         cerr << __func__ << " Exception: " << typeid(e).name() << " : " << e.what() << '\n';
         curl_slist_free_all(header);
         throw;
      }
      catch (...) {
         cerr << __func__ << " Unrecognized Exception: " <<  '\n';
         curl_slist_free_all(header);
         throw;
      }
   }
   
   using Curl_interface::Curl_interface;
   
   Curl_downloader(const Curl_downloader &) = default;
   Curl_downloader(Curl_downloader &&) = default;
   Curl_downloader & operator=(const Curl_downloader &) = default;
   Curl_downloader & operator=(Curl_downloader &&) = default;
};

#endif
