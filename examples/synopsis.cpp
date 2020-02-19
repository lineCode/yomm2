// Copyright (c) 2018-2020 Jean-Louis Leroy
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

class Animal {
  public:
    virtual ~Animal() {}
};

class Dog     : public Animal {};
class Bulldog : public Dog {};
class Cat     : public Animal {};
class Dolphin : public Animal {};

// =============================================================================
// add behavior to existing classes, without changing them

#include <yorel/yomm2/cute.hpp>

using yorel::yomm2::virtual_;

register_class(Animal);
register_class(Dog, Animal);
register_class(Bulldog, Dog);
register_class(Cat, Animal);
register_class(Dolphin, Animal);

// open method with single virtual argument <=> virtual function "from outside"
declare_method(std::string, kick, (virtual_<Animal&>));

// implement 'kick' for dogs
define_method(std::string, kick, (Dog& dog)) {
  return "bark";
}

// implement 'kick' for bulldogs
define_method(std::string, kick, (Bulldog& dog)) {
    return next(dog) + " and bite";
}

// multi-method
declare_method(std::string, meet, (virtual_<Animal&>, virtual_<Animal&>));

// 'meet' catch-all implementation
define_method(std::string, meet, (Animal&, Animal&)) {
  return "ignore";
}

define_method(std::string, meet, (Dog& dog1, Dog& dog2)) {
  return "wag tail";
}

define_method(std::string, meet, (Dog& dog, Cat& cat)) {
  return "chase";
}

define_method(std::string, meet, (Cat& cat, Dog& dog)) {
  return "run";
}

// -----------------------------------------------------------------------------
// main

#include <iostream>
#include <memory>
#include <string>

int main()
{
    yorel::yomm2::update_methods();

    std::unique_ptr<Animal>
        hector = std::make_unique<Bulldog>(),
        snoopy = std::make_unique<Dog>();

    std::cout << "kick snoopy: " << kick(*snoopy) << "\n"; // bark
    std::cout << "kick hector: " << kick(*hector) << "\n"; // bark and bite

    std::unique_ptr<Animal>
        sylvester = std::make_unique<Cat>(),
        flipper = std::make_unique<Dolphin>();

  std::cout << "hector meets sylvester: " << meet(*hector, *sylvester) << "\n"; // chase
  std::cout << "sylvester meets hector: " << meet(*sylvester, *hector) << "\n"; // run
  std::cout << "hector meets snoopy: " << meet(*hector, *snoopy) << "\n"; // wag tail
  std::cout << "hector meets flipper: " << meet(*hector, *flipper) << "\n"; // ignore
}
