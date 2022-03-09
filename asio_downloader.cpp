#include "asio_downloader.hpp"
using namespace std;

//const string Asio_downloader::HTTP_VERSION = "HTTP/1.1";
string Asio_IO_Stream_Exception::msg = {" !!! error on the socket stream: "};

string Asio_downloader::get_document() const {
   asio::basic_socket_iostream<asio::ip::tcp> socket_iostream;
   socket_iostream.connect(host, "http");
   socket_iostream.expires_from_now(chrono::seconds(1));
   socket_iostream    << method << " " << directory << " " << HTTP_VERSION << "\r\n";
   socket_iostream    << "Host: " + host + "\r\n";
   socket_iostream    << "Cache-Control: " << cache_control << "\r\n";
   socket_iostream    << "Connection: " << connection  << "\r\n\r\n";
   socket_iostream.flush();
//#if defined(__clang__)
//#elif defined(__GNUG__)   
   process_response_headers(socket_iostream);
//#endif
   stringstream strstream;
   strstream << socket_iostream.rdbuf();
   
   socket_iostream.close();
   if (! socket_iostream)
      throw Asio_IO_Stream_Exception(socket_iostream.error().message());
   string result = strstream.str();
   return result;
}

string Asio_downloader::download() const {
   try {
      string result = get_document();
      return result;
   } 
   catch (const Asio_IO_Stream_Exception & e) {
      cerr << e.what() << endl;
      throw;
   }
   catch (const asio::system_error &e) {
      cerr << "!!! System Error ! Error code = " << e.code()
           << "\n Message: " << e.what();
      throw;
   }
   catch (const exception & e) {
      cerr << "Exception: " << e.what() << endl;
      throw;
   }
}

void Asio_downloader::process_response_headers(asio::ip::tcp::iostream & socket_iostream) const {
   string http_version;
   socket_iostream >> http_version;
   std::cout << " http_version = " << http_version;
   unsigned int status_code;
   socket_iostream >> status_code;
   cout << "\n status code of response = " << status_code;
   string status_message;
   getline(socket_iostream, status_message);
   cout << "\n status message = " << status_message;
   if (socket_iostream.fail() || http_version != HTTP_VERSION) 
      throw Asio_IO_Stream_Exception("Invalid response ");
   // Process the response headers, which are terminated by a blank line.
   string header;
   while (getline(socket_iostream, header) && header != "\r")
      cout << header << "\n";
   cout << "\n";
}

void Asio_downloader::erase(string & doc, const char C, const char A) {
   for (unsigned i = 0; i < doc.size(); i++) {
      if (C == doc[i] || A == doc[i]) {
         doc.erase(doc.begin() + i);
         if (A == doc[i]) {
            doc.erase(doc.begin() + i);
            while (i < doc.size() && doc[i] != A) 
               doc.erase(doc.begin() + i);
            if (i < doc.size() && A == doc[i]) 
               doc.erase(doc.begin() + i);
         }
      }
   }
}

void Asio_downloader::modify_document(string & doc, const string & begin, const string & end) {
   //cerr << __func__ << '\n';
   //cerr << doc << '\n';
   size_t first = doc.find(begin);
   doc = doc.substr(first);
   size_t last = doc.rfind(end);
   doc = doc.substr(0, last + 1);
   erase(doc, '\r', '\n');
}

//
