#include "integer.hpp"
#include "integer_parsing.hpp"
#include "assertions.hpp"
#include "parsing_test.hpp"
#include "random_numbers.hpp"
#include "command_line.hpp"

#include "operations.hpp"

#include<iostream>
#include<limits>
#include<climits>
#include<array>
//#define NDEBUG
#include <cassert>

using namespace std;
using namespace integer_space;

namespace random_tests {

using operations::Arithmetic;
bool is_overflow(const long long & N_1, const long long & N_2, const Arithmetic & arithmetic) {
   switch (arithmetic) {
      case Arithmetic::ADD:
         return ( N_1 > 0 && N_2 > 0 && LLONG_MAX - N_1 < N_2 )   
            ||  ( N_1 < 0 && N_2 < 0 && LLONG_MIN - N_1 > N_2 ) ;
      case Arithmetic::SUBTRACT:
         return ( N_1 < 0 && N_2 > 0 && LLONG_MIN + N_2 > N_1 )   
            ||  ( N_1 > 0 && N_2 < 0 && LLONG_MAX + N_2 < N_1 ) 
            ||  ( N_1 == 0 && N_2 == LLONG_MIN ) ;
      case Arithmetic::MULTIPLY:
         return ( N_1 > +1 && N_2 > +1 && LLONG_MAX / N_2 < N_1 )  
            ||  ( N_1 < -1 && N_2 < -1 && LLONG_MAX / N_2 > N_1 ) 
            ||  ( N_1 > +1 && N_2 < -1 && LLONG_MIN / N_2 < N_1 )   
            ||  ( N_1 < -1 && N_2 > +1 && LLONG_MIN / N_2 > N_1 ) 
            ||  ( N_1 == -1 && N_2 == LLONG_MIN ) 
            ||  ( N_1 == LLONG_MIN && N_2 == -1 ) ;
      case Arithmetic::DIVIDE:
         return N_1 == LLONG_MIN && N_2 == -1 ;
   }
}

using Base = operations::Operations<false>;
struct Operation_test : 
#ifdef __clang__
   private
#elif defined(__GNUG__)
   public
#endif
                        Base {
   static void add(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Operation_test::add " << '\n';
      if (is_overflow(N_1, N_2, Arithmetic::ADD))
         return;
      Base::add(N_1, N_2, O_1, O_2);
   }
   
   static void subtract(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Operation_test::subtract " << '\n';
      if (is_overflow(N_1, N_2, Arithmetic::SUBTRACT))
         return;
      Base::subtract(N_1, N_2, O_1, O_2);
   }
   
   static void multiply(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Operation_test::multiply " << '\n';
      if (is_overflow(N_1, N_2, Arithmetic::MULTIPLY))
         return;
      Base::multiply(N_1, N_2, O_1, O_2);
   }
   
   static void divide(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Operation_test::divide " << '\n';
      if (is_overflow(N_1, N_2, Arithmetic::DIVIDE))
         return;
      Base::divide(N_1, N_2, O_1, O_2);
   }
   
   static inline void check_unary(const long long & number, const Integer & object) {
      cerr << " Operation_test::check_unary " << '\n';
      assert_number_Integer(number, object);
      assert_number_Integer(+number, +object);
      if (number != numeric_limits<long long>::min()) // llabs(numeric_limits<long long>::min()) - 1 == llabs(numeric_limits<long long>::max())
         assert_number_Integer(-number, -object);
   }
   
   static void test_by_position(const long long & N_1, const long long & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Operation_test::test_by_position " << '\n';
      for (auto operation : OPERATIONS) {
         operation(N_1, N_2, O_1, O_2);
         if (N_1 != numeric_limits<long long>::min() && N_2 != numeric_limits<long long>::min())
            operation(-N_1, -N_2, -O_1, -O_2);
         if (N_2 != numeric_limits<long long>::min())
            operation(N_1, -N_2, O_1, -O_2);
         if (N_1 != numeric_limits<long long>::min())
            operation(-N_1, N_2, -O_1, O_2);
      }
   }
   
#ifdef __clang__
   template <typename Number, template<typename> class Generator_Type>
   class Random_Operations;
   template <typename Number, template<typename> class Generator_Type>
   friend void Random_Operations<Number, Generator_Type>::run_one_random (const unsigned long long REPETITIONS);
#elif defined(__GNUG__)
   
#endif
   
   static constexpr array<Operation_Ptr, 6> OPERATIONS = { add, subtract, multiply, divide, modulo, compare };
};

using Operation_Ptr = Base::Operation_Ptr;
constexpr array<Operation_Ptr, 6> Operation_test::OPERATIONS;

template <typename T>
Integer construct_Integer (const T& NUMBER) { 
   static_assert(is_integral<T>::value && "Integral required.");
   const string STR = to_string(NUMBER);
   Integer i = Integer::parse_create(STR);
   assert_number_Integer(NUMBER, i);
   return i;
}

template <typename Number, template<typename> class Generator_Type>
class Random_Operations {
   static_assert(is_integral<Number>::value && "Integral required.");
private:
   Generator_Type<Number> generator;
   bool overflow = false;
public:
   explicit Random_Operations(const Generator_Type<Number> & gen) : generator(gen) { }
   
   ~ Random_Operations() = default;
   Random_Operations(const Random_Operations &) = default;
   Random_Operations(Random_Operations &&) = default;
   Random_Operations & operator=(const Random_Operations &) = default;
   Random_Operations & operator=(Random_Operations &&) = default;
   
   const Number get_random_number() { return generator(); }
   Generator_Type<Number> get_generator() { return generator(); }
   void set_generator(const Generator_Type<Number> &  gen) { generator = gen; }
   
   void run_one_random (const unsigned long long REPETITIONS) {
      cout << "\n\n ----------- Test run for integer type: " << typeid(Number).name() 
           << " and generator type: " << typeid(Generator_Type<Number>).name() << '\n';
      Number number;
      Integer object;
      static const Integer ZERO = construct_Integer(0);
      assert_number_Integer(0, ZERO);
      assert_number_Integer(-0, construct_Integer(-0));
      assert_number_Integer(+0, construct_Integer(+0));
      Operation_test::compare(0, 0, ZERO, ZERO);
      for (unsigned long long i = 0; i < REPETITIONS; i++) {
         cerr << "--- REPETITION " << i << '\n';
         number = generator();
         object = construct_Integer(number);
         Operation_test::check_unary(number, object);
         Operation_test::test_by_position(number, number, object, object);
         Operation_test::test_by_position(number, 0, object, ZERO);
      }
   }
};

void run_two_random (const Generator_Interface * const gen_1, const Generator_Interface * const gen_2, const unsigned long long REPETITIONS) {
   if (gen_1 == nullptr || gen_2 == nullptr )
      throw invalid_argument("Can not accept null pointer");
   cout << "\n\n ----------- Test run for types: " << typeid(*gen_1).name() << " and " << typeid(*gen_2).name() << '\n';
   long long number_T;
   long long number_U;
   Integer object_T;
   Integer object_U;
   for (unsigned long long i = 0; i < REPETITIONS; i++) {
      cerr << "--- REPETITION " << i << '\n';
      number_T = gen_1->operator()('A');
      number_U = gen_2->operator()('A');
      object_T = construct_Integer(number_T);
      object_U = construct_Integer(number_U);
      Operation_test::check_unary(number_T, object_T);
      Operation_test::check_unary(number_U, object_U);
      Operation_test::test_by_position(number_T, number_U, object_T, object_U);
   }
}

template <typename Number, template<typename> class Generator_Type>
void random_operations(const Generator_Type<Number> & gen, const unsigned long long REPETITIONS) {
   Random_Operations<Number, Generator_Type> operations (gen); 
   operations.run_one_random(REPETITIONS);
   run_two_random(&gen, &gen, REPETITIONS);
}

template <typename Number>
void run_by_one_type(const Base_Generator<Number> * const gen, const unsigned long long REPETITIONS) {
   Base_Generator<Number> * const ptr_base = const_cast<Base_Generator<Number> *> (gen);
   
   if (const Generator<Number> * const ptr_generator = dynamic_cast< Generator<Number> *> (ptr_base))
      random_operations(*ptr_generator, REPETITIONS);
   else if (const Generator_64<Number> * const ptr_generator = dynamic_cast<Generator_64<Number> *> (ptr_base))
      random_operations(*ptr_generator, REPETITIONS);
   else
      throw invalid_argument("Pointer to Base_Generator<Number> is unacceptable - pointer value: "
         + to_string(reinterpret_cast<unsigned long long> (gen)));
      
   cerr << hex << "Pointer to Base_Generator<Number> pointer value: "
         << reinterpret_cast<unsigned long long> (gen) << '\n';
   cerr << dec;
}

void run_by_one_type(const unsigned long long REPETITIONS) {
   run_by_one_type<long long>(& Generator_set::generators.gen_long.GENERATOR, REPETITIONS);
   run_by_one_type<int>(& Generator_set::generators.gen_int.GENERATOR, REPETITIONS);
   run_by_one_type<short>(& Generator_set::generators.gen_short.GENERATOR, REPETITIONS);
   run_by_one_type<char>(& Generator_set::generators.gen_char.GENERATOR, REPETITIONS);
   
   run_by_one_type<int>(& Generator_set::generators.gen_int.GENERATOR_64, REPETITIONS);
   run_by_one_type<short>(& Generator_set::generators.gen_short.GENERATOR_64, REPETITIONS);
   run_by_one_type<char>(& Generator_set::generators.gen_char.GENERATOR_64, REPETITIONS);
   run_by_one_type<long long>(& Generator_set::generators.gen_long.GENERATOR_64, REPETITIONS);
} 

void run_different_types(const unsigned long long REPETITIONS) {  
   static constexpr array<const Generator_Interface*, 8> GENERATORS = {
      & Generator_set::generators.gen_int.GENERATOR,
      & Generator_set::generators.gen_long.GENERATOR,
      & Generator_set::generators.gen_short.GENERATOR,
      & Generator_set::generators.gen_char.GENERATOR,
      
      & Generator_set::generators.gen_int.GENERATOR_64,
      & Generator_set::generators.gen_long.GENERATOR_64,
      & Generator_set::generators.gen_short.GENERATOR_64,
      & Generator_set::generators.gen_char.GENERATOR_64
   };
   /*
   vector<int> hi;
   hi.push_back(2);
   const Base_Generator<char>& ij = Generator_set::generators.gen_char.GENERATOR;
   vector<Base_Generator<char>> h;
   h.push_back(ij);*/
     /*
   static constexpr array<const Generator_Interface*, 8> GENERATORS = {
      & Generator_set::generators->gen_int.GENERATOR,
      & Generator_set::generators->gen_long.GENERATOR,
      & Generator_set::generators->gen_short.GENERATOR,
      & Generator_set::generators->gen_char.GENERATOR,
      & Generator_set::generators->gen_int.GENERATOR_64,
      & Generator_set::generators->gen_long.GENERATOR_64,
      & Generator_set::generators->gen_short.GENERATOR_64,
      & Generator_set::generators->gen_char.GENERATOR_64
   }; */

   for (int_fast8_t first = 0; first < GENERATORS.size() - 1; first++) 
      for (int_fast8_t second = first + 1; second < GENERATORS.size(); second++)
         run_two_random(GENERATORS[first], GENERATORS[second], REPETITIONS);
}

} // end of namespace random_tests

using namespace command_line;

int main(const int argc, const char * argv[]) {
   try {
      const unsigned long long REPETITIONS = examine_command_line(argc, argv);
      random_tests::run_by_one_type(REPETITIONS);
      random_tests::run_different_types(REPETITIONS);
      
      cerr << "\n END of ALL TESTS\n";
      return 0;
   }
   catch (const Arithmetic_Error & e) {
      cerr << __func__ << " " << typeid(Arithmetic_Error).name() << " : " << e.what() << endl;
   }
   catch (const invalid_argument& e) {
      cerr << __func__ << " " << typeid(invalid_argument).name() << " : " << e.what() << endl;
   }
   catch (const runtime_error& e) {
      cerr << __func__ << " " << typeid(runtime_error).name() << " : " << e.what() << endl;
   }
   catch (const exception& e) {
      cerr << __func__ << " " << typeid(exception).name() << " : " << e.what() << endl;
   }
   catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
