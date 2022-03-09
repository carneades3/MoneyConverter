#ifndef RANDOM_NUMBERS_HPP
#define RANDOM_NUMBERS_HPP

#include <random>
#include <type_traits>
#include <functional>
#include <chrono>
#include <limits>

using std::is_integral;
using std::uniform_int_distribution;
using std::function;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::default_random_engine;
using std::mt19937_64;
using std::random_device;

class Generator_Interface {
public:
   virtual long long operator()(char &&) const = 0;   // argument of char to save memory
   virtual ~ Generator_Interface() = default;
   
   Generator_Interface(Generator_Interface const &) = delete;
   Generator_Interface& operator=(Generator_Interface const &) = delete;
protected:
   Generator_Interface() { }
};

template <class T> 
class Base_Generator : public Generator_Interface {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   virtual ~ Base_Generator() = default;
   virtual T operator()() const  = 0;
   
   static long long generate_seed() {
      time_point<system_clock> point = system_clock::now();
      duration<long long> since_epoch = duration_cast<seconds>(point.time_since_epoch());
      long long seed = since_epoch.count();
      return seed;
   }
   
   Base_Generator& operator=(Base_Generator const &) = delete;
protected:
   Base_Generator(const T & MIN, const T & MAX) : min(MIN) , max(MAX) {
      if (MIN > MAX)
         throw invalid_argument("Requirement: MIN <= MAX");
   }
   Base_Generator(const Base_Generator & other) : min(other.min), max(other.max) { }
   
public:
   const T min;
   const T max;
};

template <typename T> 
class Generator : public Base_Generator<T> {
public:
   explicit Generator(const T & MIN, const T & MAX) : Base_Generator<T>(MIN, MAX) {
      const long long seed = Base_Generator<T>::generate_seed();
      default_random_engine engine (seed);
      uniform_int_distribution<>::param_type param { static_cast<int>(MIN, MAX) };
      this->generator = bind(uniform_int_distribution<>{param}, engine);
   }
   T operator()() const override final { return generator(); }
   long long operator()(char &&) const override final { return generator(); }
   
private:
   function<T()> generator;
};

template <typename T> 
class Generator_64 : public Base_Generator<T> {
public:
   Generator_64(const T MIN, const T MAX) : Base_Generator<T>(MIN, MAX) {
      mt19937_64 engine;
      uniform_int_distribution<long long>::param_type param {MIN, MAX};
      dist.param (param);
   }
   
   Generator_64(const Generator_64 & other) : Base_Generator<T>(other) {
      dist = other.dist;
   }
   
   T operator()() const override final { return dist(rd); }
   long long operator()(char &&) const override final { return dist(rd); }
   
   uniform_int_distribution<long long> get_distribution() const { return dist; }
   
private:
   mutable uniform_int_distribution<long long> dist;
   static random_device rd;
};

template <typename T> 
random_device Generator_64<T>::rd;

using std::numeric_limits;

template <typename T> 
struct Generator_MINMAX {
   static_assert(is_integral<T>::value && "Integral required.");
public:
   static const Generator<T> GENERATOR;
   static const Generator_64<T> GENERATOR_64;
};

template <typename T> 
const Generator<T> Generator_MINMAX<T>::GENERATOR = Generator<T> { numeric_limits<T>::min(), numeric_limits<T>::max() }  ;

template <typename T> 
const Generator_64<T> Generator_MINMAX<T>::GENERATOR_64 = Generator_64<T> { numeric_limits<T>::min(), numeric_limits<T>::max()  } ;

struct Generator_set final { 
   static const Generator_MINMAX<long long> gen_long;
   static const Generator_MINMAX<int> gen_int;
   static const Generator_MINMAX<short>  gen_short;
   static const Generator_MINMAX<char> gen_char;
   
   Generator_set(const Generator_set &other) = delete;
   Generator_set & operator=(const Generator_set &) = delete;
   Generator_set(Generator_set&&) = delete;
   Generator_set& operator=(Generator_set&&) = delete;
   //static const Generator_set * const generators;
   static const Generator_set  generators;
private:
   Generator_set() { }
};

//const Generator_set * const Generator_set::generators = new Generator_set;
const Generator_set Generator_set::generators;

#endif
