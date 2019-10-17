
// Copyright (c) 2018 Jean-Louis Leroy
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <yorel/yomm2.hpp>
#include <yorel/perfect_integer_hash.hpp>

#include <thread>
#include <typeinfo>

#define BOOST_TEST_MODULE yomm2
#include <boost/test/included/unit_test.hpp>

using namespace yorel::yomm2::detail;

struct Animal {
    virtual ~Animal() {}
};

BOOST_AUTO_TEST_CASE(find_hash) {
    using hash_t = hash<std::uintptr_t>;
    using finder_t = hash_finder<std::vector<std::uintptr_t>::const_iterator>;
    BOOST_TEST(finder_t::initial_bits(2) == 1);
    BOOST_TEST(finder_t::initial_bits(3) == 2);
}
