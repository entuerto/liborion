//
// art-example.cpp
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/ART.h>

#include <iostream>
#include <string>

using namespace orion;

int main()
{
   ARTree<std::string> tree;

   tree.insert("A", "A");
   tree.insert("a", "a");
   tree.insert("aa", "aa");

   std::cout << "Minimum value: " << tree.minimum() << "\n";
   std::cout << "Maximum value: " << tree.maximum() << "\n";
   
   tree.dump(std::cout);

   return EXIT_SUCCESS;
}
