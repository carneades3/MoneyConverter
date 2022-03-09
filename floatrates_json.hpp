#ifndef FLOAT_RATES_JSON_HPP
#define FLOAT_RATES_JSON_HPP

#include "floatrates.hpp"

#include "nlohmann/json.hpp"

class Float_rates_json : public Float_rates {
public:   
   //using Float_rates::Float_rates;
#ifdef __clang__
   Float_rates_json(const string & DOCUMENT, bool setting_rates = true) : Float_rates(DOCUMENT, setting_rates) { 
      document = DOCUMENT;
      if (setting_rates)
         set_rates_from_doc();   // in g++ undefined reference to `Float_rates::set_rates_from_doc()'
   }
#elif defined(__GNUG__)
      Float_rates_json(const string & DOCUMENT) : Float_rates(DOCUMENT) { document = DOCUMENT; }
#endif
   
   void set_rates_from_doc() override;
private:
   map<string, float_rates_info> get_data() const override;
   
public:
   ~ Float_rates_json() = default;
   //Float_rates_json(const Float_rates_json & a) : Float_rates(a) { }
   Float_rates_json(const Float_rates_json & a) = default;
   Float_rates_json(Float_rates_json && a) = default;
   Float_rates_json & operator=(const Float_rates_json &) = default;
   Float_rates_json & operator=(Float_rates_json &&) = default;
   
   Float_rates * clone() const override { return new Float_rates_json(*this); }
};

#endif
