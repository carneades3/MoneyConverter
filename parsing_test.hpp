#ifndef PARSING_TEST_HPP
#define PARSING_TEST_HPP

#include<string>

#include "integer.hpp"
#include "parsing_test.hpp"

using std::string;
using integer_space::Integer;

class Parsing_Test {
public :
   static void parse_incorrect_string(const string & integer_name, Integer & integer, const string & incorrect_string);
   static void parse_string(const string & integer_name, Integer & integer, const string & STR);
   static void test_parsing(const string & integer_name, Integer & integer);
};

#endif
