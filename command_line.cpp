#include "command_line.hpp"
#include <stdexcept>

using namespace std;

namespace command_line {
   
vector<string> to_vector_string(const int argc, const char * argv[]) {
   vector<string> vec;
   for (int i = 0; i < argc; i++)
      vec.push_back(argv[i]);
   if (argc != vec.size())
      throw runtime_error("argc " + to_string(argc) + " must be equal to vec.size() = " + to_string(vec.size()));
   return vec;
}

unsigned long long examine_command_line(const int argc, const char * argv[]) {
   long long number = 100;
   switch (argc) {
      case 1:
         return number;
      case 2: {
         string number_string = to_vector_string(argc, argv)[1];
         number = stoull(number_string);
         return number;
      }
      default :
         throw invalid_argument ("Number of command line arguments can be 1 or 2");
   }
}

}
