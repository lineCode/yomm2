
// Copyright (c) 2018 Jean-Louis Leroy
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <thread>
#include <typeinfo>

#include <yorel/perfect_integer_hash.hpp>

struct Animal {
    virtual ~Animal() {}
};

int main() {
    using std::cout;
    using std::setw;
    using std::setprecision;

    std::vector<std::uintptr_t> values = { reinterpret_cast<std::uintptr_t>(&typeid(Animal)) };
    // 1, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 43, 47, 50, 60, 100

    for (int i = 0; i < 5000; ++i) {
        values.push_back(values.back() + (1 + rand() % 3) * sizeof(std::type_info));
    }

    cout << setw(10) << "values" << setw(10) << "range" << setw(10) << "p"
         << setw(3) << "S" << setw(24) << "M"
         << "\n";

    std::size_t range = 1;

    for (int i = 1; i < values.size(); ++i) {
        hash_finder<std::vector<std::uintptr_t>::const_iterator> finder(
            values.begin(), values.begin() + i, 130863);

        while (finder.hash().range() < range) {
            finder.expand_range();
        }

        int found = 0;
        int attempts = 0;
        const int sub_attempts = 1000;

        while (!found) {
            hash<std::uintptr_t> first(finder.hash());

            for (int i = 0; i < sub_attempts; ++i) {
                ++attempts;
                if (finder.test()) {
                    if (!found++ ) {
                        first = finder.hash();
                    }
                }
                finder.next();
            }

            cout << setw(10) << i
                 << setw(10) << finder.hash().range()
                 << setw(10) << double(found) / attempts
                 << setw(3) << finder.hash().shift()
                 << std::flush;

            if (found)
                cout << setw(24) << first.multiply();

            cout << "\n";

            if (found) {
                break;
            }

            finder.expand_range();
            range = finder.hash().range();
        }
    }
}
