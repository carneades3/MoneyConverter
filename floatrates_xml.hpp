#ifndef FLOAT_RATES_XML_HPP
#define FLOAT_RATES_XML_HPP

#include "floatrates.hpp"
#include "xml_processing.hpp"

class Float_rates_xml : public Float_rates {
public:   
   static constexpr char * const CODE_NODE = "/channel/item/targetCurrency";
   static constexpr char * const RATE_NODE = "/channel/item/exchangeRate";
   static constexpr char * const INVERSE_RATE_NODE = "/channel/item/inverseRate";
   
   using Float_rates::Float_rates;
   
   void set_rates_from_doc() override;
private:
   map<string, float_rates_info> get_data() const override;
   
public:
   ~ Float_rates_xml() = default;
   Float_rates_xml(const Float_rates_xml &) = default;
   Float_rates_xml(Float_rates_xml &&) = default;
   Float_rates_xml & operator=(const Float_rates_xml &) = default;
   Float_rates_xml & operator=(Float_rates_xml &&) = default;
   
   Float_rates_xml * clone() const override { return new Float_rates_xml(*this); }
};

#endif
