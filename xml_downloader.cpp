#include "xml_downloader.hpp"

Xml_downloader::Xml_downloader(const Xml_downloader& other) {
   cerr << " COPY Constructor " << __func__ << '\n';
   if (nullptr != other.downloader.get())
      downloader.reset(new Asio_downloader(*(other.downloader)));
   doc = other.doc;
}

Xml_downloader& Xml_downloader::operator=(const Xml_downloader& other) {
   cerr << " COPY " << __func__ << '\n';
   if (&other != this) {
      if (nullptr == other.downloader.get())
         downloader.reset(nullptr);
      else
         downloader.reset(new Asio_downloader(*(other.downloader)));
      doc = other.doc;
   }
   return *this;
}

Xml_downloader::Xml_downloader(Xml_downloader&& other) noexcept
   : doc(other.doc) {
   cerr << " MOVE Constructor " << __func__ << '\n';
   //other.downloader = nullptr;
   downloader.reset(other.downloader.release());
   other.doc = "";
   assert(other.downloader.get() == nullptr);
   assert(other.downloader == nullptr);
}

Xml_downloader& Xml_downloader::operator=(Xml_downloader&& other) noexcept {
   cerr << " MOVE " << __func__ << '\n';
   if (&other != this) {
      downloader.reset(other.downloader.release());
      assert(other.downloader.get() == nullptr);
      assert(other.downloader == nullptr);
      doc = other.doc;
      other.doc = "";
   }
   return *this;
}
