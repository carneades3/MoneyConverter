#ifndef FLOAT_RATES_HPP
#define FLOAT_RATES_HPP

#include <string>
#include <map>
#include <iostream>

using std::string;
using std::map;
using std::pair;

struct float_rates_info {
   string code;
   long double rate;
   long double inverse_rate;
   
   bool operator==(const float_rates_info & other) const {
      return code == other.code && rate == other.rate && inverse_rate == other.inverse_rate; }
   
   bool operator!=(const float_rates_info & other) const {
      return ! operator==(other); }
};

class Float_rates {
public:   
#ifdef __clang__
   Float_rates(const string & DOCUMENT, bool setting_rates = false) { 
      document = DOCUMENT;
      if (setting_rates)
         set_rates_from_doc();   // in g++ undefined reference to `Float_rates::set_rates_from_doc()'
   }
#elif defined(__GNUG__)
   Float_rates(const string & DOCUMENT) { document = DOCUMENT; }
#endif
   
   virtual void set_rates_from_doc() = 0;
   void set_document(const string & DOCUMENT) {
      if (DOCUMENT != document) {
         document = DOCUMENT;
         rates_valid = false;
         inverse_valid = false;
      }
   }
   
   string get_document() const { return document; }
   
   map<string, long double> rates() const {  
      return get_rates(RATE, rates_map, rates_valid); }
      
   map<string, long double> inverse_rates() const { 
      map<string, long double> res = get_rates(INVERSE_RATE, inverse_rates_map, inverse_valid);
      //std::cout << "\n" << __func__ << '\n';
      //for (const auto& [code, rate] : res)
      //  std::cerr << "   " << code << " | " << rate << "\n";
      return res; }
      //return get_rates(INVERSE_RATE, inverse_rates_map, inverse_valid); }
      
   map<string, float_rates_info> float_rates() const { return float_rates_map; }
protected:
   string document;
   map<string, float_rates_info> float_rates_map;
private:
   mutable map<string, long double> rates_map;
   mutable map<string, long double> inverse_rates_map;
   mutable bool rates_valid = false;
   mutable bool inverse_valid = false;
   
   enum Rate_kind{ RATE, INVERSE_RATE };
   
   map<string, long double> get_rates(Rate_kind kind, map<string, long double>& a_rates, bool & valid ) const { 
      return valid ? a_rates : a_rates = rates(kind, valid); 
   }
     
   virtual map<string, float_rates_info> get_data() const = 0;
   
   map<string, long double> rates(Rate_kind, bool & valid) const;
   
public:
   virtual ~Float_rates() { };
   virtual Float_rates * clone() const = 0;
protected:
   Float_rates(const Float_rates &) = default;
   Float_rates(Float_rates &&) = default;
   Float_rates & operator=(const Float_rates &) = default;
   Float_rates & operator=(Float_rates &&) = default;
};

inline static bool assert_floatrates(const Float_rates & a1, const Float_rates & a2) {
   return (a1.get_document() == a2.get_document()) && (a1.float_rates() == a2.float_rates()) &&
   (a1.inverse_rates() == a2.inverse_rates()) && (a1.rates() == a2.rates());
}

#endif
