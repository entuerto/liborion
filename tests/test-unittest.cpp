
#include <orion/TestUtils.h>
#include <iostream>

int main(int argc, char** argv)
{
   // Help with uncaught exceptions in g++
   //std::set_terminate(__gnu_cxx::__verbose_terminate_handler);

   using namespace orion::unittest;

   return run_all_tests(TestStdOutput::create(std::cout));
}
