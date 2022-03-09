#include "floatrates.hpp"

#include <iostream>
#include <sstream>

using std::cerr;
using std::pair;
using std::exception;
/*
void to_json(nlohmann::json& j, float_rates_info& info) {
   j = nlohmann::json {{"code", info.code}, {"rate", info.rate}, {"inverseRate", info.inverse_rate}};
}
*/

map<string, long double> Float_rates::rates(Rate_kind kind, bool & valid) const { 
   map<string, long double> result;
   for (const pair<string, float_rates_info> &p : float_rates_map) {
      switch (kind) {
         case RATE:
            result.insert({ p.second.code, p.second.rate});
            break;
         case INVERSE_RATE:
            result.insert({ p.second.code, p.second.inverse_rate});
      }
   }
   valid = true;
   return result; 
}
