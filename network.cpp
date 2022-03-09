#include "network.hpp"

#include <array> 
#include <string>

using namespace std;

namespace network {
   
ostream& operator<<(ostream& os, const Cache_control& x) {
   static const array<string, 2> AVAILABLE_CACHE_CONTROLS = { "no-store", "no-cache" };
   const int INDEX = static_cast<int>(x);
   if (is_valid(INDEX, AVAILABLE_CACHE_CONTROLS))
      return os << AVAILABLE_CACHE_CONTROLS[INDEX];
   return os << AVAILABLE_CACHE_CONTROLS[0];
}

ostream& operator<<(ostream& os, const Connection& x) {
   static const array<string, 2> AVAILABLE_CONNECTIONS = { "keep-alive", "close" };
   const int INDEX = static_cast<int>(x);
   if (is_valid(INDEX, AVAILABLE_CONNECTIONS))
      return os << AVAILABLE_CONNECTIONS[INDEX];
   return os << AVAILABLE_CONNECTIONS[1];
}

ostream& operator<<(ostream& os, const Method& x) {
   static const array<string, 3> AVAILABLE_METHODS = { "GET", "OPTIONS", "POST" };
   const int INDEX = static_cast<int>(x);
   if (is_valid(INDEX, AVAILABLE_METHODS))
      return os << AVAILABLE_METHODS[INDEX];
   return os << AVAILABLE_METHODS[0];
}
   
}
