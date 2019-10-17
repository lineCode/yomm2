
// Copyright (c) 2018 Jean-Louis Leroy
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <vector>

using std::cout;
using std::fixed;
using std::flush;
using std::setw;
using std::setprecision;
using std::vector;

#include <yorel/perfect_integer_hash.hpp>

struct Animal {
    virtual ~Animal() {}
};

using hash_t = hash<std::uintptr_t>;
using finder_t = hash_finder<vector<std::uintptr_t>::const_iterator>;

struct solution_t {
    std::uintptr_t M;
    vector<int> check;
};

struct column_t {
    vector<solution_t> solutions;
};

int main() {
    const char* scenario_title[] = { "one range", "split range" };

    for (int ranges = 1; ranges <= 3; ++ranges) {
        cout << "\n## " << ranges << " range(s)\n\n";

        vector<std::uintptr_t> values = { reinterpret_cast<std::uintptr_t>(&typeid(Animal)) };
        // 1, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 43, 47, 50, 60, 100

        const int TESTS = 1 * 1000;
        const int VALUES = 1 * 1000 + 1;

        for (int i = 0; i < VALUES; ++i) {
            if (ranges && i == VALUES / ranges) {
                values.push_back(values.back() + 0x4000 * sizeof(std::type_info));
            } else {
                values.push_back(values.back() + (1 + rand() % 3) * sizeof(std::type_info));
            }
        }

        const int nbits = finder_t::initial_bits(values.size()) + 2;

        cout << "| " << setw(8) << "";

        for (int i = 1; i <= nbits; ++i) {
            cout << " | " << setw(8) << i;
        }

        cout << " |\n|";

        for (int i = 0; i <= nbits; ++i) {
            cout << "----------|";
        }

        cout << "\n";

        vector<column_t> columns(nbits);
        finder_t finder(values.begin(), values.begin() + 2);

        cout << "| " << setw(8) << 2 << " | " << flush;

        for (auto& column : columns) {
            for (int i = 0; i < TESTS; ++i) {
                if (finder.test()) {
                    column.solutions.push_back({ finder.hash().multiply(), finder.hits() });
                }
                finder.next();
            }
            cout << setw(8) << setprecision(1) << fixed << 100. * column.solutions.size() / TESTS
                 << " | ";
            finder.expand_range();
        }

        cout << "\n";

        for (int size = 3; size < VALUES; ++size) {
            bool display =
                size < 100 || (size < 1000 && (size % 100 == 0)) || size % 1000 == 0;

            if (display)
                cout << "| " << setw(8) << size << flush;

            for (int col = 0; col < nbits; ++col) {
                auto& column = columns[col];
                const int bits = col + 1;
                auto impossible = bits < finder_t::initial_bits(size);

                for (auto iter = column.solutions.begin(); iter != column.solutions.end(); ) {
                    if (iter->check[hash_t(iter->M, bits)(values[size])]++) {
                        iter = column.solutions.erase(iter);
                    } else {
                        ++iter;
                    }
                }

                if (display) {
                    cout << " | " << setw(8);

                    if (impossible) {
                        cout << "";
                    } else {
                        cout << setprecision(1) << fixed << 100. * column.solutions.size() / TESTS;
                    }

                    cout << flush;
                }
            }

            if (display)
                cout << " |\n";
        }
    }
}
