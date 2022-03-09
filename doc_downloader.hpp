#ifndef DOC_DOWNLOADER_HPP
#define DOC_DOWNLOADER_HPP

class Doc_downloader {
public:
   virtual void download () = 0;
   virtual ~ Doc_downloader() { };
   
   //Doc_downloader(Doc_downloader const &) = delete;
   //Doc_downloader& operator=(Doc_downloader const &) = delete;
protected:
   Doc_downloader() { }
};

#endif
