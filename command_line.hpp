#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace command_line {
   vector<string> to_vector_string(const int argc, const char * argv[]);
   unsigned long long examine_command_line(const int argc, const char * argv[]);
}

#endif
