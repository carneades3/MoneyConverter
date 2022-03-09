#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <iostream>
#include <utility>
//#define NDEBUG
#include <cassert>
#include <tuple>

using std::ostream;
using std::tuple_size;

namespace network {
   
enum class Network_library { NONE, ASIO, CURL };
enum class File_format { NONE, JSON, XML };

template<class Type>
inline bool is_valid(const int INDEX, const Type& object) {
   static constexpr size_t SIZE = tuple_size<Type>{};
   if (INDEX < 0 || INDEX >= SIZE) {
      assert(0 && "Unavailable index ");
      return false;
   }
   return true;
}

enum class Cache_control { no_store, no_cache };
ostream& operator<<(ostream& os, const Cache_control& x);

enum class Connection { keep_alive, close };
ostream& operator<<(ostream& os, const Connection& x);

enum class Method { get, options, post };
ostream& operator<<(ostream& os, const Method& x);

}

#endif
