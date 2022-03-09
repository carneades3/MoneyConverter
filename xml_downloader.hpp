#ifndef XML_DOWNLOADER_HPP
#define XML_DOWNLOADER_HPP

#include "asio_downloader.hpp"
#include "network.hpp"
#include "doc_downloader.hpp"

#include <memory>

using std::cerr;
using std::unique_ptr;

//template<class T> 
//inline T& unmove(T&& t) { return t; }

class Xml_downloader : Doc_downloader {
   unique_ptr<Asio_downloader> downloader;
   //Asio_downloader & downloader;
   //Asio_downloader && downloader;
   string doc;
public:
   Xml_downloader(const string & HOST, const Method & METHOD, const string & DIRECTORY,
                         const Cache_control & CACHE_CONTROL, const Connection & CONNECTION) {
      //Asio_downloader d = { HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION };
      downloader.reset(new Asio_downloader{ HOST, METHOD, DIRECTORY, CACHE_CONTROL, CONNECTION });
      cerr << " Constructor " << __func__ << '\n'; }
      
   //Xml_downloader(Asio_downloader & d) : downloader (d) {
   //   cerr << " Constructor " << __func__ << '\n'; }
   /*
   Xml_downloader(const Xml_downloader& other) = default;

	Xml_downloader& operator=(const Xml_downloader& other) = default; // explicitly defaulted function was implicitly deleted 
	
	Xml_downloader(Xml_downloader&& other)   = default;
	
	Xml_downloader& operator=(Xml_downloader&& other)  = default; // explicitly defaulted function was implicitly deleted 
	*/
   Xml_downloader(const Xml_downloader& other) ;

	Xml_downloader& operator=(const Xml_downloader& other);
	
	Xml_downloader(Xml_downloader&& other) noexcept;
	
	Xml_downloader& operator=(Xml_downloader&& other) noexcept;

   ~Xml_downloader() { cerr << __func__ << '\n'; }
   
   void download() override {
      doc = downloader->download();
      Asio_downloader::modify_document(doc, "<", ">");
   }
   
   string get() const { return doc; };   
   Asio_downloader * get_asio_downloader() const { return downloader.get(); }; 
};

#endif
