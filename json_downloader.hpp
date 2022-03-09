#ifndef JSON_DOWNLOADER_HPP
#define JSON_DOWNLOADER_HPP

#include "asio_downloader.hpp"
#include "network.hpp"
#include "doc_downloader.hpp"

using std::cerr;

class Json_downloader : Doc_downloader {
   Asio_downloader * downloader = nullptr;
   string doc;
public:
   Json_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      cerr << " Constructor " << __func__ << '\n';
      downloader = new Asio_downloader(HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION); }
   
   Json_downloader(const Json_downloader& other);

	Json_downloader& operator=(const Json_downloader& other);
	
	Json_downloader(Json_downloader&& other) noexcept;
	
	Json_downloader& operator=(Json_downloader&& other) noexcept;

   ~Json_downloader() { cerr << __func__ << '\n'; delete downloader; }
   
   void download() override {
      doc = downloader->download();
      Asio_downloader::modify_document(doc, "{", "}");
   }
   
   string get() const { return doc; };   
   Asio_downloader * get_asio_downloader() const { return downloader; }; 
};

#endif
