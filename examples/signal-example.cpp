//
// signal-example.h
//
// Copyright (c) 2013-2017 Tomas Palazuelos
//
// Distributed under the MIT Software License. (See accompanying file LICENSE.md)
//
#include <orion/Signal.h>
#include <iostream>

using namespace orion;

//----------------------------------------------------------
class Object
{
public:
   Object() {}

   void print(int i) { std::cout << "Printed number: " << i << "\n"; }
};

//----------------------------------------------------------
void die(int i)
{
   std::cout << "Die number: " << i << "\n";
}

//----------------------------------------------------------
class Object2
{
public:
   Object2() {}

   void print()
   {
      std::cout << "Hello world"
                << "\n";
   }
};

//----------------------------------------------------------
class Object3
{
public:
   Object3() {}

   void print(int i) { std::cout << "Ptr print number: " << i << "\n"; }
};

//----------------------------------------------------------
class Object4
{
public:
   typedef std::shared_ptr<Object4> SharedPtr;

   Object4() {}

   void print(int i) { std::cout << "Shared Ptr print number: " << i << "\n"; }
};

//----------------------------------------------------------
int main()
{
   Object o;
   Object2 o2;
   Object3* ptr = new Object3;
   Object4::SharedPtr shptr(new Object4);

   Signal<void(int)> signal;
   Signal<void()> signal2;

   signal.connect(die);
   signal.connect(std::bind(&Object::print, o, std::placeholders::_1));
   signal.connect(&o, &Object::print);
   signal.emit(0);

   signal2.connect(std::bind(&Object2::print, o2));
   signal2.connect(&o2, &Object2::print);
   signal2.emit();

   signal.connect(std::bind(&Object3::print, ptr, std::placeholders::_1));
   signal.connect(ptr, &Object3::print);
   signal.emit(3);

   signal.connect(std::bind(&Object4::print, shptr, std::placeholders::_1));
   signal.connect(shptr, &Object4::print);
   signal.emit(6);

   delete ptr;
}
