#include "system.hpp"

namespace system_utility {

string add_dir_to_environment(const string & directory, const string & env_name) {
   const string env = get_environment(env_name);
   const string new_env = env + ":" + directory;
   cerr << " new_env = " << new_env << '\n';
   int result = setenv(env_name.data(), new_env.c_str(), 1);
   cerr << " getenv(env_name) = " << getenv(env_name.data()) << '\n';
   if (result) 
      throw runtime_error(" result of setenv = " + std::to_string(result) + " " + strerror(errno));
   return new_env;
}
   
}
