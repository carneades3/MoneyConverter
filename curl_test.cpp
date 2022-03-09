#include "xml_nbp.hpp"
#include "curl_manager.hpp"
#include "curl_downloader.hpp"

using namespace std;
using namespace xml_NBP;

int main() {
   Curl_interface * downloader = nullptr;
   try {
      downloader = new Curl_downloader  { File_format::XML };
      int result = Xml_NBP_processing::download(downloader) ? 0 : 1;
      delete downloader;
      return result;
   }
   catch (const exception & e) {
      cerr << __func__ << " Exception: " << typeid(e).name() << " : " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " Unrecognized Exception: " <<  endl;
   }
   delete downloader;
   return 1;
}
