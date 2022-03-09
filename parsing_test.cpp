#include "parsing_test.hpp"

#include<iostream>
//#define NDEBUG
#include <cassert>

using std::cout;
using std::cerr;

void Parsing_Test::parse_incorrect_string(const string & integer_name, Integer & integer, const string & incorrect_string) {
   try {
      integer.parse(incorrect_string);
      cerr << "???? " << integer_name << " integer after parsing of '" << incorrect_string << "': " << integer << "\n";
      assert(false);
   } catch (const invalid_argument & e) {
      cerr << __func__ << " exception: " << e.what() << "\n";
   }
}

void Parsing_Test::parse_string(const string & integer_name, Integer & integer, const string & STR) {
   integer.parse(STR);
   cout << integer_name << " integer after parsing of '" << STR << "': " << integer << "\n";
}

void Parsing_Test::test_parsing(const string & integer_name, Integer & integer) {
   cout << ("\n TESTS OF PARSING STRING :\n");
   parse_string(integer_name, integer, "12345");
   assert(string(integer) == "+12345");

   parse_string(integer_name, integer, "-1234565");
   assert(static_cast<string>(integer) == ("-1234565"));

   parse_string(integer_name, integer, "+45");
   assert(static_cast<string>(integer) == ("+45"));

   parse_string(integer_name, integer, "0");
   assert(static_cast<string>(integer) == ("0"));

   parse_incorrect_string(integer_name, integer, "E123");
   assert(static_cast<string>(integer) == ("0"));

   parse_incorrect_string(integer_name, integer, "p");
   assert(static_cast<string>(integer) == ("0"));

   parse_incorrect_string(integer_name, integer, "6p");
   assert(static_cast<string>(integer) == ("0"));

   parse_string(integer_name, integer, "10");
   assert(static_cast<string>(integer) == ("+10"));

   parse_string(integer_name, integer, "345");
   assert(static_cast<string>(integer) == ("+345"));

   parse_incorrect_string(integer_name, integer, "0p");
   assert(static_cast<string>(integer) == ("+345"));

   parse_incorrect_string(integer_name, integer, "++7");
   assert(static_cast<string>(integer) == ("+345"));

   parse_incorrect_string(integer_name, integer, "");
   assert(static_cast<string>(integer) == ("+345"));

   parse_incorrect_string(integer_name, integer, " ");
   assert(static_cast<string>(integer) == ("+345"));

   parse_incorrect_string(integer_name, integer, "8 ");
   assert(static_cast<string>(integer) == ("+345"));

   parse_incorrect_string(integer_name, integer, "-0");
   assert(static_cast<string>(integer) == ("+345"));
   
   parse_incorrect_string(integer_name, integer, "+0");
   assert(static_cast<string>(integer) == ("+345"));

   parse_string(integer_name, integer, "000");
   assert(static_cast<string>(integer) == ("0"));

   parse_string(integer_name, integer, "088");
   assert(static_cast<string>(integer) == ("+88"));

   parse_string(integer_name, integer, "-02");
   assert(static_cast<string>(integer) == ("-2"));

   parse_incorrect_string(integer_name, integer, "844444444444444444444444444444444444444444444444444444444444444444444444444");
   assert(static_cast<string>(integer) == ("-2"));

   cout << ("\n ------------------------\n");
}

