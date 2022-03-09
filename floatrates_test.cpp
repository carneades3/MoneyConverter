#include "floatrates_test.hpp"

using namespace std;
using namespace xml_processing;

namespace downloaders_test {
   extern void downloaders_test();
}

namespace float_rates_test {
/*
static void validate_format(const File_format & FORMAT) { 
   switch (FORMAT) {
      case File_format::JSON :
      case File_format::XML :
         break;
      default:
         throw invalid_argument(__func__ + string(" Invalid file format ") + std::to_string(static_cast<int>(FORMAT)));
   }
}
*/
template <class T> 
class Document_test {
   static_assert(is_base_of<Float_rates, T>::value && "is_base_of<Float_rates, T>::value");
   
   static T * get_check_rates_ptr(Float_rates * rates_ptr) {
      if (rates_ptr == nullptr)
         throw invalid_argument(string(__func__) + " Type of rates_ptr can not be null ");
      T * document_ptr = dynamic_cast<T *>(rates_ptr);
      if (document_ptr == nullptr)
         throw invalid_argument(string(__func__) + " Type of rates_ptr is " + typeid(document_ptr).name()
            + " Required type: " + typeid(T).name());
      return document_ptr;
   }
   
   static T create_rates(const Network_library & LIBRARY, const File_format & format, const string & currency) {
      Float_rates_test test { LIBRARY, format, currency };
      test.run();
      Float_rates * rates_ptr = test.get_float_rates();
      T * doc_ptr = get_check_rates_ptr(rates_ptr);
      T & doc_ref = *doc_ptr;
      assert(assert_floatrates(doc_ref, *doc_ptr));
      T result(doc_ref);
      assert(assert_floatrates(doc_ref, result));
      assert(assert_floatrates(*doc_ptr, result));
      return result;
   }
   
   static constexpr inline File_format get_format() {
      /*
      if (is_same<T, Float_rates_json>::value)
         return File_format::JSON;
      if (is_same<T, Float_rates_xml>::value)
         return File_format::XML;
      */
      return is_same<T, Float_rates_json>::value ? File_format::JSON : File_format::XML; 
      //static_assert(false && "Not supported type");
   }
public:   
   static void run() {
      array<Network_library, 2> LIBRARIES = { Network_library::CURL, Network_library::ASIO };
      for (Network_library item : LIBRARIES) {
         constructors_test(item);
         //test_floatrates_downloader(item);
      }
   }
   
   static void constructors_test(const Network_library & LIBRARY) {
      //const File_format format = is_same<T, Float_rates_json>::value ? File_format::JSON : File_format::XML; 
      constexpr File_format format = get_format();
      //string currency {  };
      T object = create_rates(LIBRARY, format, "eur");
      T & obj_ref = object;
      assert(assert_floatrates(obj_ref, object));
      
      T obj1(obj_ref);
      assert(assert_floatrates(obj_ref, obj1));
      assert(assert_floatrates(object, obj1));
      T obj2 = obj1;
      assert(assert_floatrates(obj2, obj1));
      T obj3(move(obj2));
      assert( ! assert_floatrates(obj3, obj2));
      assert(assert_floatrates(obj3, obj1));
      assert(assert_floatrates(obj3, obj_ref));
      assert(assert_floatrates(obj3, object));
      //currency = {  };
      T other = create_rates(LIBRARY, format, "usd");
      assert( ! assert_floatrates(other, obj2));
      assert( ! assert_floatrates(other, obj1));
      assert( ! assert_floatrates(other, obj_ref));
      assert( ! assert_floatrates(other, object));
      assert( ! assert_floatrates(other, obj3));
      
      obj1 = other;
      assert( assert_floatrates(other, obj1));
      assert( ! assert_floatrates(obj1, obj2));
      assert( ! assert_floatrates(obj1, obj_ref));
      assert( ! assert_floatrates(obj1, object));
      assert( ! assert_floatrates(obj1, obj3));
      
      obj3 = move(other);
      assert( assert_floatrates(other, obj2));
      assert( assert_floatrates(obj3, obj1));
      assert( ! assert_floatrates(obj3, obj2));
      assert( ! assert_floatrates(obj3, obj_ref));
      assert( ! assert_floatrates(obj3, object));
      assert( ! assert_floatrates(obj3, other));
      
      obj3 = obj3;
      assert( assert_floatrates(obj3, obj1));
      assert( ! assert_floatrates(obj3, obj2));
      assert( ! assert_floatrates(obj3, obj_ref));
      assert( ! assert_floatrates(obj3, object));
      assert( ! assert_floatrates(obj3, other));
      
      const map<string, float_rates_info> empty_info;
      const map<string, long double> empty_rates {};
      obj3 = move(obj3);
      assert(obj3.get_document() == ""); 
      assert(obj3.float_rates() == empty_info);
      assert(obj3.inverse_rates() == empty_rates);
      assert(obj3.rates() == empty_rates);
      assert( ! assert_floatrates(obj3, obj1));
      assert( assert_floatrates(obj3, obj2));
      assert( ! assert_floatrates(obj3, obj_ref));
      assert( ! assert_floatrates(obj3, object));
      assert( assert_floatrates(obj3, other));
   }
};
/*
template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
Document * get_check_rates_ptr(Float_rates * rates_ptr) {
   if (rates_ptr == nullptr)
      throw invalid_argument(string(__func__) + " Type of rates_ptr can not be null ");
   Document * document_ptr = dynamic_cast<Document *>(rates_ptr);
   if (document_ptr == nullptr)
      throw invalid_argument(string(__func__) + " Type of rates_ptr is " + typeid(document_ptr).name()
         + " Required type: " + typeid(Document).name());
   return document_ptr;
}

template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
Document create_rates(const File_format & format, const string & currency) {
   Float_rates_test test { format, currency };
   test.run();
   Float_rates * rates_ptr = test.get_float_rates();
   Document * doc_ptr = get_check_rates_ptr<Document>(rates_ptr);
   Document & doc_ref = *doc_ptr;
   assert(assert_floatrates(doc_ref, *doc_ptr));
   Document result(doc_ref);
   assert(assert_floatrates(doc_ref, result));
   assert(assert_floatrates(*doc_ptr, result));
   return result;
}

template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
constexpr File_format get_format() {
   return is_same<Document, Float_rates_json>::value ? File_format::JSON : File_format::XML; 
}

template <class Document, enable_if_t<is_base_of<Float_rates, Document>::value, bool>  = true>
static void constructors_test() {
   //static const File_format format = is_same<Document, Float_rates_json>::value ? File_format::JSON : File_format::XML; 
   static constexpr File_format format = get_format<Document>();
   string currency { "pln" };
   Document object = create_rates<Document>(format, currency);
   //document_ptr = get_check_rates_ptr<Document>(rates_ptr);
   Document & obj_ref = object;
   assert(assert_floatrates(obj_ref, object));
   Document obj1(obj_ref);
   assert(assert_floatrates(obj_ref, obj1));
   assert(assert_floatrates(object, obj1));
   Document obj2 = obj1;
   assert(assert_floatrates(obj2, obj1));
   Document obj3(move(obj2));
   assert( ! assert_floatrates(obj3, obj2));
   assert(assert_floatrates(obj3, obj1));
   assert(assert_floatrates(obj3, obj_ref));
   assert(assert_floatrates(obj3, object));
   currency = { "usd" };
   Document other = create_rates<Document>(format, currency);
   assert( ! assert_floatrates(other, obj2));
   assert( ! assert_floatrates(other, obj1));
   assert( ! assert_floatrates(other, obj_ref));
   assert( ! assert_floatrates(other, object));
   assert( ! assert_floatrates(other, obj3));
   
   obj1 = other;
   assert( assert_floatrates(other, obj1));
   assert( ! assert_floatrates(obj1, obj2));
   assert( ! assert_floatrates(obj1, obj_ref));
   assert( ! assert_floatrates(obj1, object));
   assert( ! assert_floatrates(obj1, obj3));
   
   obj3 = move(other);
   assert( assert_floatrates(other, obj2));
   assert( assert_floatrates(obj3, obj1));
   assert( ! assert_floatrates(obj3, obj2));
   assert( ! assert_floatrates(obj3, obj_ref));
   assert( ! assert_floatrates(obj3, object));
   assert( ! assert_floatrates(obj3, other));
}
*/

static void test_Float_rates_test() {
   Float_rates_test obj { Network_library::ASIO, File_format::JSON, "pln" };
   // Float_rates_test obj1(obj); call to deleted constructor of 'float_rates_test::Float_rates_test'
   Float_rates_test obj1 { Network_library::ASIO, File_format::JSON, "pln" };
   assert(obj == obj1);
   assert( ! (obj != obj1));
   Float_rates_test obj2 { Network_library::ASIO, File_format::XML, "pln" };
   assert(obj != obj2);
   assert( ! (obj == obj2));
   // Float_rates_test obj3 = obj1; call to deleted constructor of 'float_rates_test::Float_rates_test'
   // obj2 = obj1; overload resolution selected deleted operator '=' 
   // candidate function has been explicitly deleted Float_rates_test& operator=(Float_rates_test const &) = delete;
   Float_rates_test obj3(move(obj1));
   assert(obj3 != obj1);
   assert(obj1.get_float_rates() == nullptr);
   assert(obj3 == obj);
   assert(obj3 != obj2);
   
   Float_rates_test obj4 { Network_library::ASIO, File_format::XML, "pln" };
   assert(obj4 == obj2);
   assert(obj3 != obj4);
   assert(obj != obj4);
   assert(obj4 != obj1);
   
   obj4 = move(obj);
   Float_rates * float_rates_ptr = obj.get_float_rates();
   assert(float_rates_ptr == nullptr);
   assert(obj4 != obj);
   assert(obj1 == obj);
   assert(obj4 != obj2);
   assert(obj3 == obj4);
   assert(obj4 != obj1);
   
   obj4 = move(obj4);
   assert(obj4 != obj);
   assert(obj1 == obj);
   assert(obj4 != obj2);
   assert(obj3 == obj4);
   assert(obj4 != obj1);
   
   Float_rates_test obj5 { Network_library::ASIO, File_format::JSON, "pln" };
   assert(obj4 == obj5);
   assert(obj5 != obj);
   assert(obj5 != obj2);
   assert(obj3 == obj5);
   assert(obj5 != obj1);
   
   obj5.run();
   assert(obj4 != obj5);
   assert(obj5 != obj);
   assert(obj5 != obj2);
   assert(obj3 != obj5);
   assert(obj5 != obj1);
   
   obj3.run();
   assert(obj4 != obj3);
   assert(obj3 != obj);
   assert(obj3 != obj2);
   assert(obj3 == obj5);
   assert(obj3 != obj1);
}

static void float_rates_test() {
   Float_rates_test test { Network_library::CURL, File_format::JSON, "GBP" };
   test.run();
   test.set_format(File_format::XML);
   test.run();
   test.set_library(Network_library::ASIO);
   test.run();
   test.set_format(File_format::JSON);
   test.run();
}
   
void test() {
   downloaders_test::downloaders_test();
   Document_test<Float_rates_json>::run();
   Document_test<Float_rates_xml>::run();
   test_Float_rates_test();
   float_rates_test();
}

}

int main() {
   try {
      float_rates_test::test();
      return 0;
   }
   catch (const nlohmann::json::exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (pugi::xpath_exception const & e) {
      cerr << "!!! Error xpath_exception exception: "  << typeid(e).name() << e.result().description() << '\n';
   }
   catch (const Xml_processing::Exception & e) {
      cerr << __func__  << typeid(e).name() << " : " << e.what() << '\n';
   }
   catch (const Asio_IO_Stream_Exception & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (const asio::system_error &e) {
      cerr << "!!! System Error ! Error code = " << e.code()
           << "\n Message: " << e.what();
      return e.code().value();
   } 
   catch (const std::invalid_argument& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
   catch (const std::bad_cast& e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   } 
   catch (const std::bad_alloc & e) {
      cerr  << __func__ << " " << typeid(e).name() << " " << e.what() << '\n';
   }
   catch (const exception & e) {
      cerr << __func__ << " " << typeid(e).name() << " " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " " << "Unrecognized Exception: " <<  endl;
   }
   return 1;
}
