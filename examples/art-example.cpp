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
   ARTree<std::string, std::string> tree;

   tree.insert("A", "A");
   tree.insert("a", "a");
   tree.insert("aa", "aa");

   auto min = tree.minimum();
   if (min != tree.cend())
      std::cout << "Minimum value: " << *min << "\n";

   auto max = tree.maximum();
   if (max != tree.cend())
      std::cout << "Maximum value: " << *max << "\n\n\n";
   
   tree.dump(std::cout);

   return EXIT_SUCCESS;
}
