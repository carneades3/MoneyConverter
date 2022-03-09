#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "floatrates.hpp"
#include "floatrates_json.hpp"
#include "floatrates_xml.hpp"

struct Abstract_factory { 
   virtual Float_rates * create() = 0; };
   
template <typename T>
struct Float_rates_factory : Abstract_factory {
   static_assert((std::is_base_of<Float_rates, T>::value) && "Float_rates derived class required.");
   Float_rates * create() { 
#ifdef __clang__
      return  new T {"", false };
#elif defined(__GNUG__)
      return  new T {""};
#endif
   }
};

#endif
