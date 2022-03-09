#include "system.hpp"

using namespace std;
using namespace system_utility;

int main() {
   string exec_name = "floatrates_test";
   string exec_dir = get_current_working_dir();
   string abs_exec = get_absolute_path(exec_dir, "floatrates_test");
   const string & env_name = "PATH";
   add_dir_to_environment(exec_dir, env_name);
   return execute("valgrind", "valgrind", "--leak-check=full", "--show-leak-kinds=all", 
         //"--exit-on-first-error=yes", "--error-exitcode=1",
         abs_exec.c_str());
}
