#include <iostream>
#include <map>
#include <cstdlib>
//#include <gmp.h>
#include <gmpxx.h>

#include "assertions.hpp"
#include "command_line.hpp"

#include "integer.hpp"
#include "operations.hpp"

using namespace std;
using namespace integer_space;

inline void print(const string & func_name, const exception& e) {
   cerr << func_name << " " << typeid(e).name() << " : " << e.what() << endl;
}

namespace gmp_random_tests {
   
using operations::Arithmetic;
using Base = operations::Operations<true>;
struct Operation_test : private Base {
   static void test_by_position(const Arithmetic & arithmetic, const mpz_class & N_1, const mpz_class & N_2, const Integer & O_1, const Integer & O_2) {
      cerr << " Operation_test::test_by_position " << '\n';
      vector<Operation_Ptr> vec = OPERATIONS.at(arithmetic);
      vec.push_back(compare);
      for (auto operation : vec) {
         operation(N_1, N_2, O_1, O_2);
         operation(-N_1, -N_2, -O_1, -O_2);
         operation(N_1, -N_2, O_1, -O_2);
         operation(-N_1, N_2, -O_1, O_2);
      }
   }
   
   static inline void check_unary(const Number & number, const Integer & object) {
      cerr << " Operation_test::check_unary " << '\n';
      Base::check_unary(number, object);
   }
   
private:
   static const map<Arithmetic, vector<Operation_Ptr>> OPERATIONS;
   //using Operation_Ptr = Operations<true>::Operation_Ptr;
};

using Operation_Ptr = Base::Operation_Ptr; 
const map<Arithmetic, vector<Operation_Ptr>> Operation_test::OPERATIONS = { { Arithmetic::ADD, { add } },
                                                                        { Arithmetic::SUBTRACT, { subtract } },
                                                                        { Arithmetic::MULTIPLY, { multiply } },
                                                                        { Arithmetic::DIVIDE, { divide, modulo } } };
                                 /*                                       
const map<Arithmetic, vector<Operation_Ptr>> Operation_test::OPERATIONS = { { Arithmetic::ADD, { Operation_test::add } },
                                                                        { Arithmetic::SUBTRACT, { Operation_test::subtract } },
                                                                        { Arithmetic::MULTIPLY, { Operation_test::multiply } },
                                                                        { Arithmetic::DIVIDE, { Operation_test::divide, Operation_test::modulo } } };*/
   
class Gmp_test {
   const unsigned long long M_SIZE;
   vector<mpz_class> rand_mpzs;
   vector<Integer> rand_integers;
   
   mpz_class random_mpz(const Arithmetic & arithmetic, gmp_randstate_t & state, const int operand);
   int upper_exponent_2(const Arithmetic & arithmetic, const int operand);
   
   Integer get_integer(const mpz_class & mpz) {
      string s = mpz.get_str();
      Integer result = Integer::parse_create(s);
      return result;
   }
   
   void generate(const Arithmetic & arithmetic);
   void operate(const Arithmetic & arithmetic) {
      generate(arithmetic);
      test(arithmetic);
      rand_mpzs.clear();
      rand_integers.clear();
   }
   void test (const Arithmetic & arithmetic);
public:
   Gmp_test(const unsigned long long s) : M_SIZE(s) {  
      if (s % 2 == 1)
         throw invalid_argument("size must be even number - is: " + to_string(s));
   }
   void run();
};

int Gmp_test::upper_exponent_2(const Arithmetic & arithmetic, const int operand) {
   switch (arithmetic) {
      case Arithmetic::ADD:
      case Arithmetic::SUBTRACT:
         return 128 ;
      case Arithmetic::MULTIPLY:
         return 64 ;
      case Arithmetic::DIVIDE:
         switch (operand) {
            case 1 :
               return 128 ;
            case 2 :
               return rand() % 32 ;
            default:
               throw invalid_argument("operand must be 1 or 2 - is: " + to_string(operand));
         }
      default:
         throw invalid_argument("unsupported arithmetic enum class - is: " + to_string(static_cast<int>(arithmetic)));      
         
   }
}

mpz_class Gmp_test::random_mpz(const Arithmetic & arithmetic, gmp_randstate_t & state, const int operand) {
   mpz_class signum, result;
   mpz_urandomb(signum.get_mpz_t(), state, 1);
   mpz_urandomb(result.get_mpz_t(), state, upper_exponent_2(arithmetic, operand));
   if (signum < 0.5)
      result *= -1;
   //gmp_printf("%Zd --------\n", result);
   return result;
}

void Gmp_test::test (const Arithmetic & arithmetic) {
   mpz_class number_T;
   mpz_class number_U;
   Integer object_T;
   Integer object_U;
   for (unsigned long long i = 0; i < M_SIZE ; i += 2) {
      cerr << "--- REPETITION " << i / 2 << '\n';
      number_T = rand_mpzs[i];
      number_U = rand_mpzs[i + 1];
      object_T = rand_integers[i];
      object_U = rand_integers[i + 1];
      Operation_test::check_unary(number_T, object_T);
      Operation_test::check_unary(number_U, object_U);
      Operation_test::test_by_position(arithmetic, number_T, number_U, object_T, object_U);
   }
}
   
void Gmp_test::generate(const Arithmetic & arithmetic)  { 
   vector<mpz_class> vec_mpzs(M_SIZE);
   vector<Integer> vec_integers(M_SIZE);
   mpz_class random;
   cout << __func__ << '\n';
   gmp_randstate_t state;
   gmp_randinit_mt(state);
   for(int i = 0 ; i < M_SIZE;){
      random = random_mpz(arithmetic, state, 1);
      vec_mpzs[i] = random;
      vec_integers[i++] = get_integer(random);
      if (Arithmetic::DIVIDE == arithmetic) {
         random = random_mpz(arithmetic, state, 2);
         vec_mpzs[i] = random;
         vec_integers[i++] = get_integer(random);
      }
   }
   rand_mpzs = std::move(vec_mpzs);
   rand_integers = std::move(vec_integers);
}
   
void Gmp_test::run () {
   operate(Arithmetic::ADD);
   operate(Arithmetic::SUBTRACT);
   operate(Arithmetic::MULTIPLY);
   operate(Arithmetic::DIVIDE);
}

}

int main(const int argc, const char * argv[]) {
   try {
      const unsigned long long M_SIZE = command_line::examine_command_line(argc, argv);
      gmp_random_tests::Gmp_test test(M_SIZE);
      test.run();
      
      cerr << "\n END of ALL TESTS\n";
      return 0;
   }
   catch (const Arithmetic_Error & e) {
      print(__func__, e);
   }
   catch (const out_of_range & e) {
      print(__func__, e);
   }
   catch (const invalid_argument& e) {
      print(__func__, e);
   }
   catch (const runtime_error& e) {
      print(__func__, e);
   }
   catch (const exception& e) {
      print(__func__, e);
   }
   catch (...) {
      cerr << __func__ << " unrecognized exception\n";
   }
   return 1;
}
