#ifndef ASIO_DOC_DOWNLOADER_HPP
#define ASIO_DOC_DOWNLOADER_HPP

class Asio_doc_downloader : public Doc_downloader {
public:
   virtual void download () = 0;
   virtual ~ Doc_downloader() { };
   
   //Doc_downloader(Doc_downloader const &) = delete;
   //Doc_downloader& operator=(Doc_downloader const &) = delete;
protected:
   Asio_doc_downloader() { }
};

#endif
