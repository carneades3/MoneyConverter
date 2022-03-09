#include "json_downloader.hpp"

Json_downloader::Json_downloader(const Json_downloader& other) {
   cerr << " COPY Constructor " << __func__ << '\n';
   if (nullptr != other.downloader)
      downloader = new Asio_downloader(*(other.downloader));
   doc = other.doc;
}

Json_downloader& Json_downloader::operator=(const Json_downloader& other) {
   cerr << " COPY " << __func__ << '\n';
   if (&other != this) {
      delete downloader;
      if (nullptr == other.downloader)
         downloader = nullptr;
      else
         downloader = new Asio_downloader(*(other.downloader));
      doc = other.doc;
   }
   return *this;
}

Json_downloader::Json_downloader(Json_downloader&& other) noexcept
   : downloader(other.downloader), doc(other.doc) {
   cerr << " MOVE Constructor " << __func__ << '\n';
   other.downloader = nullptr;
   other.doc = "";
}

Json_downloader& Json_downloader::operator=(Json_downloader&& other) noexcept {
   cerr << " MOVE " << __func__ << '\n';
   if (&other != this) {
      delete downloader;
      downloader = other.downloader;
      other.downloader = nullptr;
      doc = other.doc;
      other.doc = "";
   }
   return *this;
}
