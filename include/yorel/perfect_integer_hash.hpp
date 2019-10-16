// Copyright (c) 2019 Jean-Louis Leroy
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef YOREL_PERFECT_INTEGER_HASH_INCLUDED
#define YOREL_PERFECT_INTEGER_HASH_INCLUDED

#include <limits>
#include <random>

template<typename INTEGER>
class hash {
  public:
    hash(INTEGER mult, std::size_t shift) : M(mult), S(shift) {
    }

    std::size_t operator ()(INTEGER value) const {
        return static_cast<std::size_t>(M * value >> S);
    }

    int range() const {
        return 1 << (std::numeric_limits<INTEGER>::digits - S);
    }

    INTEGER multiply() const {
        return M;
    }

    void multiply(INTEGER new_m) {
        M = new_m;
    }

    std::size_t shift() const {
        return S;
    }

    // private:
    INTEGER M;
    std::size_t S;
};

struct hash_finder_config {

};

namespace detail {

template<typename ITER>
using element_t = std::remove_cv_t<
    std::remove_reference_t<
        decltype(*std::declval<ITER>())>>;

template<typename INTEGER>
struct hash_finder_base {
    using HASH = hash<INTEGER>;

    static std::size_t initial_bits(std::size_t range) {
        std::size_t S = 0;

        while (range) {
            range >>= 1;
            S += 1;
        }

        return S;
    }
};

}

template<typename ITER>
class hash_finder : public detail::hash_finder_base<detail::element_t<ITER>> {
    using INTEGER = detail::element_t<ITER>;
    using HASH = hash<INTEGER>;

  public:
    hash_finder(ITER first, ITER last, INTEGER seed)
    : first(first), last(last),
      count(std::distance(first, last)),
      bits(this->initial_bits(count)),
      rnd(seed),
      hf(uniform_dist(rnd), std::numeric_limits<INTEGER>::digits - bits),
      check(hf.range()) {
    }

    void expand_range() {
        ++bits;
        check.resize(1 << bits);
        std::fill(check.begin(), check.end(), 0);
        hf = HASH(uniform_dist(rnd), std::numeric_limits<INTEGER>::digits - bits);
    }

    bool test() {
        for (ITER iter = first; iter != last; ++iter) {
            auto h = hf(*iter);
            if (check[h]++) {
                return false;
            }
        }

        return true;
    }

    void next() {
        std::fill(check.begin(), check.end(), 0);
        hf.multiply(uniform_dist(rnd));
    }

    HASH hash() const {
        return hf;
    }

  private:
    ITER first, last;
    std::size_t count, bits;
    std::default_random_engine rnd;
    std::uniform_int_distribution<INTEGER> uniform_dist;
    HASH hf;
    std::vector<int> check;
};

#endif
