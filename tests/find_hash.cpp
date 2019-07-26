
// Copyright (c) 2018 Jean-Louis Leroy
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <yorel/yomm2.hpp>
#include <yorel/yomm2/runtime.hpp>

#include <thread>
#include <typeinfo>

#define BOOST_TEST_MODULE yomm2
#include <boost/test/included/unit_test.hpp>

using namespace yorel::yomm2::detail;

struct Animal {
    virtual ~Animal() {}
};

BOOST_AUTO_TEST_CASE(find_hash) {
    log_on(&std::cerr);

    // std::vector<std::uintptr_t> values;

    // std::uintptr_t val = reinterpret_cast<std::uintptr_t>(&typeid(Animal));

    // YOMM2_TRACE(log() << "values start at " << val);

    // for (int j = 0; j < 1000; ++j) {
    //     values.push_back(val += sizeof(std::type_info));
    // }

    std::vector<std::uintptr_t> values = {
        6608296, 6608352, 6608416, 6608480, 6608536, 6608576, 6608624, 6608672, 6608720, 6608768
    };

    hash_function hash;
    runtime::metrics_t metrics;
    runtime::find_hash_function(values, hash, metrics);

    log_off();
}
